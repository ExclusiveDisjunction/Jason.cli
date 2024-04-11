use std::fmt::Display;
use std::ops::{Add, Index, IndexMut, Sub};

use super::mt_base::VarComm;

#[derive(Clone)]
pub struct Vector {
    val: Vec<f64>,
    d: usize
}
impl VarComm for Vector {
    type StoredData = Vec<f64>;

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
                for _ in a..=self.d {
                    proper_data.push(0.00f64);
                }
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
    fn read_from_sterilize(&mut self, input_string: &str) -> Result<(), String> {
        if input_string.len() < 3 || &input_string[0..3] != "VEC" {
            return Err(String::from("Input string not long enough."));
        }

        let splits: Vec<&str> = input_string.split(' ').into_iter().collect();
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

        self.resize(d);

        for i in 0..d {
            if i + 2 >= splits.len() {
                return Err(String::from("There are more numbers expected, but not enough given."));
            }

            let target = splits[i+2];
            assert!(i < self.d);

            if let Ok(t) = target.parse::<f64>() {
                self.val[i] = t;
            }
            else {
                return Err(format!("{target} is not of proper format and could not be converted."));
            }
        }

        Ok(())
    }
}
impl Add for Vector {
    type Output = Result<Self, String>;

    fn add(self, rhs: Self) -> Self::Output {
        if self.d != rhs.d { Err("Dimension mismatch.".to_string()) }
        else {
            let mut result = Vector::new(self.d);

            for i in 0..self.d {
                result.val[i] = self.val[i] + rhs.val[i];
            }

            Ok(result)
        }   
    }
}
impl Sub for Vector {
    type Output = Result<Self, String>;

    fn sub(self, rhs: Self) -> Self::Output {
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
impl Display for Vector {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        let mut result = String::from("{ ");

        for i in &self.val {
            result += &format!("{i} ");
        }

        write!(f, "{}", format!("{result}}}"))
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
    fn ne(&self, other: &Self) -> bool {
        self.d != other.d || self.val != other.val
    }
}
impl Vector {
    pub fn new(d: usize) -> Self {
        let mut val = Vec::<f64>::new();
        for _ in 0..d {
            val.push(0f64);
        }

        Self {
            d,
            val
        }
    }
    pub fn from_list(vals: Vec<f64>) -> Self {
        Self {
            d: vals.len(),
            val: vals
        }
    }
    pub fn from_val(dim: usize, val: f64) -> Self {
        return if dim == 0 {
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

    fn resize(&mut self, new_d: usize)
    {
        self.d = new_d;
        self.val.clear();

        if new_d > 0 {
            for _ in 0..new_d {
                self.val.push(0f64);
            }
        }
    }
}
