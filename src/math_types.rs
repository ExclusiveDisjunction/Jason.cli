use std::fmt::Display;
use std::ops::{Index, IndexMut, Range};
use std::cmp::max;
use rand::prelude::*;
use crate::ops::num_resolver::get_number_digits_count;

pub trait VarComm {
    type StoredData;
    type SterilizedType;

    fn val_eq(&self, other: &Self) -> bool;
    fn get_val(&self) -> &Self::StoredData;
    fn set_val(&mut self, new_data: Self::StoredData) -> Result<(), String>;

    fn sterilize(&self) -> String;
    fn from_sterilize(input_string: &str) -> Result<Self::SterilizedType, String>;
}

#[derive(PartialEq, PartialOrd, Debug)]
pub struct Scalar {
    val: f64
}
impl VarComm for Scalar {
    type StoredData = f64;
    type SterilizedType = Self;

    fn val_eq(&self, other: &Self) -> bool {
        self.val == other.val
    }
    fn get_val(&self) -> &Self::StoredData {
        &self.val
    }
    fn set_val(&mut self, new_data: Self::StoredData) -> Result<(), String>{
        self.val = new_data;
        Ok(())
    }

    fn sterilize(&self) -> String {
        String::from("SCA ") + &self.val.to_string()
    }
    fn from_sterilize(input_string: &str) -> Result<Self, String> {
        if input_string.len() < 3 {
            return Err(String::from("Cannot resolve input string as the type specifier was not found."));
        }

        if input_string.get(0..3).unwrap() != "SCA" {
            return Err(String::from("Cannot resolve input string as the type is not a scalar."));
        }

        /*
            The format is:

            SCA [VAL]

            So we need from index 4 to the end of the string, trimmed to convert to a f64.
        */

        let slice = input_string.get(4..input_string.len());
        match slice {
            Some(nums) => {
                let trimmed = String::from(nums.trim());
                let result = trimmed.parse::<f64>();

                match result {
                    Ok(a) => Ok(Scalar::new(a)),
                    Err(_) => Err(String::from("Value is not of proper numeric format."))
                }
            }
            _ => Ok(Scalar::new(0f64))
        }
    }
}
impl Display for Scalar {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}", self.val)
    }
}
impl Scalar {
    pub fn new(val: f64) -> Self {
        Self {
            val
        }
    }

    pub fn add(&self, obj: &Self) -> Self {
        Self {
            val: self.val + obj.val
        }
    }
    pub fn sub(&self, obj: &Self) -> Self {
        Self {
            val: self.val - obj.val
        }
    }
    pub fn mul(&self, obj: &Self) -> Self {
        Self {
            val: self.val * obj.val
        }
    }
    pub fn div(&self, obj: &Self) -> Self {
        Self {
            val: self.val / obj.val
        }
    }
    pub fn pow(&self, obj: &Self) -> Self {
        Self {
            val: self.val.powf(obj.val)
        }
    }
}


//VECTOR

#[derive(Clone, Debug)]
pub struct Vector {
    val: Vec<f64>,
    d: usize
}
impl VarComm for Vector {
    type StoredData = Vec<f64>;
    type SterilizedType = Self;

    fn val_eq(&self, other: &Self) -> bool {
        self.val == other.val
    }
    fn get_val(&self) -> &Self::StoredData {
        &self.val
    }
    fn set_val(&mut self, new_data: Self::StoredData) -> Result<(), String> {
        let mut proper_data: Self::StoredData;
        match new_data.len() {
            a if a < self.d => { //Less, we need to expand the values to match d. 
                proper_data = new_data;
                proper_data.resize(self.d, 0f64);
            }
            a if a == self.d => { //All fine.
                proper_data = new_data;
            }
            _ => { //a > self.d, greater. We need to shrink the values to match d.
                let result = new_data.get(0..self.d);
                match result {
                    Some(a) => proper_data = Vec::<f64>::from(a),
                    None => proper_data = Vec::<f64>::new()
                }
            }
        }

        assert_eq!(self.d, proper_data.len());
        self.val = proper_data;
        Ok(())
    }

    fn sterilize(&self) -> String {
        let mut result = format!("VEC {} ", self.d);

        for num in &self.val {
            result += &format!("{} ", num);
        }
        result
    }
    fn from_sterilize(input_string: &str) -> Result<Self, String> {
        if input_string.len() < 3 || &input_string[0..3] != "VEC" {
            return Err(String::from("Input string not long enough."));
        }

        let splits: Vec<&str> = input_string.split(' ').collect();
        if splits.len() < 3 {
            return Err(String::from("the input string does not contain enough information to extract"));
        }

        let d: usize;
        {
            let d_temp: Result<usize, _> = splits[1].parse();

            match d_temp {
                Ok(new_d) => d = new_d,
                _ => return Err(String::from("The dimensions could not be resolved."))
            }
        }

        let mut val = Vec::<f64>::new();

        for i in 0..d {
            if i + 2 >= splits.len() {
                return Err(String::from("There are more numbers expected, but not enough given."));
            }

            let target = splits[i+2];
            assert!(i < d);

            if let Ok(t) = target.parse::<f64>() {
               val.push(t);
            }
            else {
                return Err(format!("{target} is not of proper format and could not be converted."));
            }
        }

        Ok(Self {
            val,
            d
        })
    }
}
impl Display for Vector {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        let mut result = String::from("{ ");

        for i in &self.val {
            result += &format!("{i} ");
        }
        result += "}";

        write!(f, "{}", result)
    }
}
impl Index<usize> for Vector {
    type Output = f64;

    fn index(&self, index: usize) -> &Self::Output {
        assert!(index < self.d);

        &self.val[index]
    }
}
impl IndexMut<usize> for Vector {
    fn index_mut(&mut self, index: usize) -> &mut Self::Output {
        assert!(index < self.d);

        &mut self.val[index]
    }
}
impl PartialEq for Vector {
    fn eq(&self, other: &Self) -> bool {
        self.d == other.d && self.val == other.val
    }
}
impl Vector {
    pub fn new(d: usize) -> Self {
        Self {
            d,
            val: vec![0f64; d]
        }
    }
    pub fn from_list(vals: Vec<f64>) -> Self {
        Self {
            d: vals.len(),
            val: vals
        }
    }
    pub fn from_val(dim: usize, val: f64) -> Self {
        if dim == 0 {
            Self::error_vector()
        }
        else {
            let mut result = Vec::<f64>::new();
            for _ in 0..dim {
                result.push(val);
            }

            Self {
                val: result,
                d: dim
            }
        }
    }
    pub fn from_num(num: f64) -> Self {
        Self {
            val: vec!(num),
            d: 1
        }
    }

    pub fn error_vector() -> Self {
        Self {
            val: Vec::<f64>::new(),
            d: 0
        }
    }
    pub fn zero_vector(dim: usize) -> Self {
        Self::from_val(dim, 0f64)
    }

    pub fn dim(&self) -> usize {
        self.d
    }
    pub fn is_valid(&self) -> bool {
        self.d != 0
    }

    pub fn magnitude(&self) -> Result<f64, String> {
        if self.d == 0 {
            Err(String::from("Cannot get magnitude from an error vector."))
        }
        else if self.d == 1 {
            Ok(self.val[0])
        }
        else {
            let mut sum = 0f64;
            for e in &self.val {
                sum += e.powf(2f64);
            }

            Ok(sum.sqrt())
        }
    }
    pub fn angle(&self) -> Result<f64, String> {
        if self.d == 0 {
            Err(String::from("Cannot get angle from an error vector."))
        }
        else if self.d == 1 {
            Err(String::from("Cannot evaluate the angle of a scalar."))
        }
        else {
            Ok(self.magnitude()?.atan())
        }
    }

    pub fn cross_product(&self, obj: &Self) -> Result<Self, String> {
        if self.d != obj.d {
            return Err(String::from("Dimension mismatch."));
        }

        match (self.d, obj.d) {
            (1, 1) => Ok(Vector::from_num(self.val[0] * obj.val[0])),
            (2, 2) => {
                //Since we can just treat this 2d vector as a 3d vector by setting z to 0 for both, we can evaluate this.
                //x & y both evaluate to zero.
                let z = (self.val[0] * obj.val[1]) - (self.val[1] * obj.val[0]);

                Ok(Self::from_list(vec![0f64, 0f64, z]))
            }
            (3, 3) => {
                let x = (self.val[1] * obj.val[2]) - (self.val[2] * obj.val[1]);
                let y = (self.val[2] * obj.val[0]) - (self.val[0] * obj.val[2]);
                let z = (self.val[0] * obj.val[1]) - (self.val[1] * obj.val[0]);

                Ok(Self::from_list(vec![x, y, z]))
            }
            (_, _) => Err(format!("Cannot cross vectors of dimensions {} and {}", self.d, obj.d))
        }
    }
    pub fn dot_product(&self, obj: &Self) -> Result<f64, String> {
        if !self.is_valid() || !obj.is_valid() {
            Err(String::from("Empty vectors."))
        }
        else if self.d != obj.d {
            Err(String::from("Dimension mismatch."))
        }
        else {
            let mut result: f64 = 0f64;
            for i in 0..self.d {
                result += self.val[i] * obj.val[i];
            }

            Ok(result)
        }
    }

    pub fn add(&self, rhs: &Self) -> Result<Self, String> {
        if self.d != rhs.d { Err("Dimension mismatch.".to_string()) }
        else {
            let mut result = Vector::new(self.d);

            for i in 0..self.d {
                result.val[i] = self.val[i] + rhs.val[i];
            }

            Ok(result)
        }
    }
    pub fn sub(&self, rhs: &Self) -> Result<Self, String> {
        if self.d != rhs.d { Err("Dimension mismatch.".to_string()) }
        else {
            let mut result = Vector::new(self.d);

            for i in 0..self.d {
                result.val[i] = self.val[i] - rhs.val[i];
            }

            Ok(result)
        }
    }
}

//MATRIX

#[derive(Clone, Debug)]
pub struct Matrix {
    val: Vec<Vector>,
    m: usize,
    n: usize
}
impl Index<usize> for Matrix {
    type Output = Vector;

    fn index(&self, index: usize) -> &Self::Output {
        assert!(index < self.m);

        &self.val[index]
    }
}
impl IndexMut<usize> for Matrix {
    fn index_mut(&mut self, index: usize) -> &mut Self::Output {
        assert!(index < self.m);

        &mut self.val[index]
    }
}
impl PartialEq for Matrix {
    fn eq(&self, other: &Self) -> bool {
        self.m == other.m && self.n == other.n && self.val == other.val
    }
}
impl Display for Matrix {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        let mut result = String::new();
        let schema = self.get_column_width_schematic();
        match (self.m, self.n) {
            (1, _) => { //Only one row
                if let Some(a) = self.get_row_string(0, &schema, '[', ']') {
                    result = a;
                }
            }
            (_, _) => { //Multiple rows
                // ⌊ ⌋ ⌈ ⌉ |
                for i in 0..self.m {
                    let open;
                    let close;
                    if i == 0 {
                        open = '⌈';
                        close = '⌉';
                    } else if i == self.val.len() - 1 {
                        open = '⌊';
                        close = '⌋';
                    } else {
                        open = '|';
                        close = open;
                    }

                    let this_row_result = self.get_row_string(i, &schema, open, close);
                    if let Some(a) = this_row_result {
                        result += &format!("{a}\n");
                    }
                }
            }
        }

        write!(f, "{result}")
    }
}
impl VarComm for Matrix {
    type StoredData = Vec<Vector>;
    type SterilizedType = Self;

    fn val_eq(&self, obj: &Self) -> bool {
        self.val == obj.val
    }
    fn get_val(&self) -> &Self::StoredData {
        &self.val
    }
    fn set_val(&mut self, new_data: Self::StoredData) -> Result<(), String> {
        if new_data.is_empty() || !new_data[0].is_valid() {
            self.resize(0, 0);
        }

        for (i , item) in new_data.iter().enumerate().take(self.m) {
            if item.dim() != self.n {
                return Err(format!("At row {i}, there is a dimension mismatch. ({} != {})", item.dim(), self.n));
            }
        }

        self.m = new_data.len();
        self.n = new_data[0].dim();
        self.val = new_data;
        Ok(())
    }

    fn sterilize(&self) -> String {
        let mut result = format!("MAT {} {} ", self.m, self.n);

        for i in 0..self.m {
            for j in 0..self.n {
                result += &format!("{} ", self.val[i][j]);
            }
        }

        result
    }
    fn from_sterilize(input: &str) -> Result<Self, String> {
        if input.len() < 3 || &input[0..3] != "MAT" {
            return Err(String::from("Input string not long enough."));
        }

        let splits:Vec<&str> = input.split(' ').collect();
        if splits.len() < 3 {
            return Err(String::from("The input string does not contain enough information."));
        }

        let m: usize;
        let n: usize;
        {
            let m_temp: Result<usize, _> = splits[1].parse();
            let n_temp: Result<usize, _> = splits[2].parse();

            match (m_temp, n_temp) {
                (Ok(new_m), Ok(new_n)) => {
                    m = new_m;
                    n = new_n;
                }
                (_, _) => return Err(String::from("The dimensions could not be resolved."))
            }
        }

        let mut val = Vec::<Vector>::new();
        for _ in 0..m {
            val.push(Vector::new(n));
        }

        for k in 0..(m * n) {
            let i = k / m;
            let j = k % m;
            if k + 2 >= splits.len() {
                return Err(String::from("There are more numbers expected, but not enough were given."));
            }

            let target = splits[k+2];

            assert!(i < m && j < n);

            if let Ok(t) = target.parse::<f64>() {
                val[i][j] = t;
            }
            else {
                return Err(format!("{target} is not a proper number and could not be resolved, at index {i}, {j} (num {k})"));
            }
        }

        Ok(Self {
            val,
            m,
            n
        })
    }
}
impl Matrix {
    fn resize(&mut self, m: usize, n: usize) {
        self.m = m;
        self.n = n;
        self.val.clear();

        if m != 0 && n != 0 {
            for _ in 0..m {
                self.val.push(Vector::new(n));
            }
        }
    }

    pub fn new(m: usize, n: usize) -> Self {
        let mut result = Vec::<Vector>::new();

        if m != 0 && n != 0 {
            for _ in 0..m {
                result.push(Vector::new(n));
            }
        }
        
        Self {
            val: result,
            m,
            n
        }
    }
    pub fn error_matrix() -> Self {
        Self {
            val: Vec::<Vector>::new(),
            m: 0 ,
            n : 0
        }
    }
    pub fn identity_matrix(d: usize) -> Self {
        let mut result = Self::new(d, d);

        for i in 0..d {
            result[i][i] = 1f64;
        }

        result
    }
    pub fn random_matrix(m: usize, n: usize) -> Self{
        let mut result = Matrix::new(m, n);
        let upper = 10f64;
        let lower = -10f64;
        let mut rng = thread_rng();

        for i in 0..m {
            for j in 0..n {
                result.val[i][j] = rng.gen_range(lower..=upper);
            }
        }

        result
    }

    pub fn is_error(&self) -> bool {
        self.m != 0 && self.n != 0
    }
    pub fn rows(&self) -> usize {
        self.m
    }
    pub fn cols(&self) -> usize {
        self.n
    }

    pub fn extract(&self, rows: Range<usize>, cols: Range<usize>) -> Self {
        if rows.is_empty() || cols.is_empty() { Matrix::error_matrix(); }
        
        let mut result = Matrix::new(rows.len(), cols.len());
        let mut ip = 0;
        let mut jp = 0;

        for i in rows {
            for j in cols.clone() {
                result.val[ip][jp] = self.val[i][j];
                ip += 1;
                jp += 1;
            }
        }

        result
    }

    pub fn row_swap(&mut self, orig_row: usize, new_row: usize) {
        if orig_row == new_row || orig_row >= self.m || new_row >= self.m {
            return;
        }

        self.val.swap(orig_row, new_row);
    }
    pub fn row_add(&mut self, orig_row: usize, fac: f64, target_row: usize) {
        if fac == 0f64 || orig_row >= self.m || target_row >= self.m {
            return;
        }

        for j in 0..self.n {
            self.val[target_row][j] += self.val[orig_row][j] * fac;
        }
    }

    pub fn determinant(&self) -> Option<f64> {
        let mut mat = self.val.clone();
        let mut det = 1.0;

        for i in 0..self.n {
            // Find pivot row and swap if necessary
            let mut max_row = i;
            for j in (i + 1)..self.n {
                if mat[j][i].abs() > mat[max_row][i].abs() {
                    max_row = j;
                }
            }

            if max_row != i {
                mat.swap(i, max_row);
                // Change sign of determinant when swapping rows
                det = -det;
            }

            // Check if matrix is singular
            if mat[i][i].abs() < 1e-10 {
                return Some(0.0);
            }

            det *= mat[i][i];
            // Perform row operations to make lower triangular matrix
            for k in (i + 1)..self.n {
                let factor = mat[k][i] / mat[i][i];
                for j in i..self.n {
                    mat[k][j] -= factor * mat[i][j];
                }
            }
        }

        Some(det)
    }
    pub fn invert(&self) -> Option<Matrix> {
        if self.m != self.n {
            return None;
        }

        let id = Self::identity_matrix(self.m);
        let aug = id.augment(self);
        match aug {
            Ok(a) => {
                let left = a.extract(0..self.m, 0..self.m);
                let right = a.extract(0..self.m, self.m..(self.m * 2));

                if right != id {
                    None
                } else {
                    Some(left)
                }
            }
            Err(_) => None
        }
    }
    pub fn transpose(&self) -> Matrix {
        let mut result = Matrix::new(self.n, self.m); //Switched
        for i in 0..self.m {
            for j in 0..self.n {
                result.val[j][i] = self.val[i][j];
            }
        }

        result
    }
    pub fn augment(&self, obj: &Self) -> Result<Matrix, String> {
        if self.m != obj.m {
            return Err(String::from("The dimensions for the matrices are not compatible for augmentation."));
        }

        let rows = self.m;
        let cols1 = self.n;
        let cols2 = obj.n;
        let mut result = Matrix::new(rows, cols1 + cols2);
        for i in 0..rows {
            for j in 0..cols1 {
                result.val[i][j] = self.val[i][j];
            }
            for (jp,j) in (cols1..cols2).enumerate() {
                result.val[i][j] = obj.val[i][jp];
            }
        }

        Ok(result)
    }

    pub fn row_echelon_form(&mut self) {
        let rows = self.m;
        let cols = self.n;

        let mut current_row = 0;
        for current_col in 0..cols {
            let mut pivot_row = current_row;
            while pivot_row < rows && self.val[pivot_row][current_col] == 0f64 {
                pivot_row += 1;
            }

            if pivot_row < rows {
                self.row_swap(current_row, pivot_row);

                // Normalize the pivot row
                let pivot_value = self.val[current_row][current_col];
                for col in current_col..cols {
                    self.val[current_row][col] /= pivot_value;
                }

                // Eliminate entries below the pivot
                for row in (current_row + 1)..rows {
                    let multiplier = self.val[row][current_col];
                    for col in current_col..cols {
                        self.val[row][col] -= multiplier * self.val[current_row][col];
                    }
                }

                current_row += 1;
            }
            else {
                continue;
            }
        }
    }
    pub fn reduced_row_echelon_form(&mut self) {
        self.row_echelon_form();

        let rows = self.m;
        let cols = self.n;

        let mut lead = 0;
        for r in 0..rows {
            if lead >= cols {
                break;
            }

            let mut i = r;
            while self.val[i][lead] == 0f64 {
                i += 1;
                if i == rows {
                    i = r;
                    lead += 1;
                    if lead == cols {
                        break;
                    }
                }
            }

            if lead < cols {
                self.row_swap(i, r);
                let divisor = self.val[r][lead];
                if divisor != 0f64 {
                    for j in 0..cols {
                        self.val[r][j] /= divisor;
                    }
                }

                for k in 0..rows {
                    if k != r {
                        let factor = self.val[k][lead];
                        for j in 0..cols {
                            self.val[k][j] = factor * self.val[r][j];
                        }
                    }
                }
            }

            lead += 1;
        }
    }

    pub fn get_column_width_schematic(&self) -> Vec<(bool, u32)> {
        let mut result = Vec::<(bool, u32)>::new(); //We only care about columns.

        /*
            This algorithm will find the largest width of each column, and store the result in the result value.

            For instance:

            [4 3 1 2]
            [33 4 1 -2]
            [5 -44.3 2 1]

            Which evaluates to:
            [(false, 2), (true, 5), (false, 1), (true, 2)]

            Which can be used to re-format the matrix when printing to be:
            [4   3    1  2]
            [33  4    1 -2]
            [5  -44.3 2  1]
        */

        for j in 0..self.n {
            let mut negative_found = false;
            let mut largest_num:u32 = 0;
            for i in 0..self.m {
                let curr = self.val[i][j];
                if curr < 0f64 {
                    negative_found = true;
                }
                largest_num = max(get_number_digits_count(curr, 3), largest_num);
            }

            result.push((negative_found, largest_num));
        }

        result
    }
    pub fn get_row_string(&self, row: usize, schema: &[(bool, u32)], open_brace: char, close_brace: char) -> Option<String> {
        if row > self.m {
            return None;
        }

        /*
            Convert the number to a string
            If the length of that string is less than the maximum of the column, then add extra spaces to the end till it meets the maximum.
            If there is a negative in that column, and the current number is not negative, add one to the beginning of the string, and remove one from the end. If it is negative, do not add any extra spaces.
            Repeat this for each number for each column and row, and put a space between columns. 
        */

        let mut result =format!("{open_brace} ");
        let host = &self.val[row];
        for i in 0..self.n {
            let this_schema = &schema[i];
            let has_negative = this_schema.0;
            let width: u32 = this_schema.1;

            let curr = host[i];
            let mut curr_str = curr.to_string();
            if curr >= 0f64 && has_negative {
                curr_str = format!(" {curr_str}");
            }

            if (has_negative && (curr_str.len() as u32) < width + 1) || ((curr_str.len() as u32) < width) {
                let mut diff = width - (curr_str.len() as u32);
                let mut space_str = String::new();
                if has_negative {
                    diff += 1;
                }

                for _ in 0..diff {
                    space_str += " ";
                }

                curr_str += &space_str;
            }

            result += &format!("{curr_str} ");
        }

        Some(format!("{result}{close_brace}"))
    }

    pub fn add(&self, rhs: &Self) -> Result<Self, String> {
        if self.m == rhs.m && self.n == rhs.n {
            let mut result = Matrix::new(self.m, self.n);

            for row in 0..self.m {
                for col in 0..self.n {
                    result.val[row][col] = self.val[row][col] + rhs.val[row][col];
                }
            }

            Ok(result)
        }
        else {
            Err(String::from("Dimension Mismatch"))
        }
    }
    pub fn sub(&self, rhs: &Self) -> Result<Self, String> {
        if self.m == rhs.m && self.n == rhs.n {
            let mut result = Matrix::new(self.m, self.n);

            for row in 0..self.m {
                for col in 0..self.n {
                    result.val[row][col] = self.val[row][col] - rhs.val[row][col];
                }
            }

            Ok(result)
        }
        else {
            Err(String::from("Dimension Mismatch"))
        }
    }
    pub fn mul(&self, rhs: &Self) -> Result<Self, String> {
        if self.n != rhs.m || self.m == 0 || self.n == 0 || rhs.m == 0 || rhs.n == 0 {
            return Err(String::from("Dimensional mismatch, or one matrix is empty."));
        }

        let r = self.m;
        let c = rhs.n;
        let mut result = Matrix::new(r, c);

        for i in 0..r {
            for j in 0..c {
                result.val[i][j] = 0f64;

                for k in 0..rhs.m {
                    result.val[i][j] += self.val[i][k] * rhs.val[k][j];
                }
            }
        }

        Ok(result)
    }
}