use std::ops::{Add, Div, Index, IndexMut, Mul, Sub};

pub enum VariableType {
    Scalar(Scalar),
    Vector(Vector),
    Matrix(Matrix), //Functions will be defined later.
    None
}

pub trait VarComm {
    type StoredData;

    fn val_eq(&self, other: &Self) -> bool;
    fn get_val(&self) -> &Self::StoredData;
    fn set_val(&mut self, new_data: Self::StoredData);
}

pub struct Scalar {
    val: f64
}
impl VarComm for Scalar {
    type StoredData = f64;

    fn get_val(&self) -> &Self::StoredData {
        &self.val
    }
    fn set_val(&mut self, new_data: Self::StoredData) {
        self.val = new_data
    }
    fn val_eq(&self, other: &Self) -> bool {
        self.val == other.val
    }
}
impl Add for Scalar {
    type Output = Self;

    fn add(self, rhs: Self) -> Self::Output {
        Scalar { val: self.val + rhs.val }
    }
}
impl Sub for Scalar {
    type Output = Self;

    fn sub(self, rhs: Self) -> Self::Output {
        Scalar { val: self.val - rhs.val }
    }
}
impl Mul for Scalar {
    type Output = Self;

    fn mul(self, rhs: Self) -> Self::Output {
        Scalar { val: self.val * rhs.val }
    }
}
impl Div for Scalar {
    type Output = Self;

    fn div(self, rhs: Self) -> Self::Output {
        Scalar { val: self.val / rhs.val }
    }
}
impl ToString for Scalar {
    fn to_string(&self) -> String {
        self.val.to_string()
    }
}
impl Scalar {
    pub fn new(val: f64) -> Self {
        Self {
            val
        }
    }
}

pub struct Vector {
    val: Vec<f64>,
    d: usize
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
impl ToString for Vector {
    fn to_string(&self) -> String {
        let mut result = String::from("{ ");

        for i in self.val {
            result += &format!("{i} ");
        }

        result += "}";
        result
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
impl Vector {
    pub fn new(d: usize) -> Self {
        assert!(d > 0);
        Self {
            d,
            val: Vec::<f64>::new()
        }
    }
}

pub struct Matrix {
    val: Vec<Vector>,
    m: usize,
    n: usize
}
impl Add for Matrix {
    type Output = Result<Self, String>;

    fn add(self, rhs: Self) -> Self::Output {
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
}
impl Sub for Matrix {
    type Output = Result<Self, String>;

    fn sub(self, rhs: Self) -> Self::Output {

    }
}
impl Mul for Matrix {
    type Output = Result<Self, String>;

    fn mul(self, rhs: Self) -> Self::Output {

    }
}
impl ToString for Matrix {
    fn to_string(&self) -> String {
        match (self.m, self.n) {
            (1, _) => { //Only one row

            }
            (_, _) => { //Mutiple rows

            }
        }
    }
}
impl Matrix {
    pub fn new(m: usize, n: usize) -> Self{
        assert!(m > 0 && n > 0);

        let mut result = Vec::<Vector>::new();
        for _ in 0..m {
            result.push(Vector::new(n));
        }

        Self {
            val: result,
            m,
            n
        }
    }

    fn largest_num_digits(&self) -> i32 {
        let mut largest = 0;
        for row in 0..self.m {
            for col in 0..self.n {
                let result = self.val[row][col].checked_ilog10().unwrap_or(0) + 1;
            }
        }
    }
}

impl Add for VariableType {
    type Output = Result<VariableType, String>;

    fn add(self, rhs: Self) -> Self::Output {
        match (self, rhs) {
            (VariableType::Scalar(x), VariableType::Scalar(y)) => Ok(VariableType::Scalar(x + y)),
            (VariableType::Vector(x), VariableType::Vector(y)) => {
                let comp = x + y;
                if let Ok(z) = comp {
                    Ok(VariableType::Vector(z))
                }
                else {
                    Err(comp.err().unwrap())
                }
            }
            (VariableType::Matrix(x), VariableType::Matrix(y)) => {
                let comp = x + y;
                if let Ok(z) = comp {
                    Ok(VariableType::Matrix(z))
                }
                else {
                    Err(comp.err().unwrap())
                }
            }
            (VariableType::None, VariableType::None) => Err(String::from("Cannot add None")),
            (_, _) => Err("Mismatched types.".to_string())
        }
    }
}
impl Sub for VariableType {
    type Output = Result<VariableType, String>;

    fn sub(self, rhs: Self) -> Self::Output {
        match (self, rhs) {
            (VariableType::Scalar(x), VariableType::Scalar(y)) => Ok(VariableType::Scalar(x - y)),
            (VariableType::Vector(x), VariableType::Vector(y)) => {
                let comp = x - y;
                if let Ok(z) = comp {
                    Ok(VariableType::Vector(z))
                }
                else {
                    Err(comp.err().unwrap())
                }
            }
            (VariableType::Matrix(x), VariableType::Matrix(y)) => {
                let comp = x - y;
                if let Ok(z) = comp {
                    Ok(VariableType::Matrix(z))
                }
                else {
                    Err(comp.err().unwrap())
                }
            }
            (VariableType::None, VariableType::None) => Err(String::from("Cannot subtract None")),
            (_, _) => Err("Mismatched types.".to_string())
        }
    }
}
impl Mul for VariableType {
    type Output = Result<VariableType, String>;

    fn mul(self, rhs: Self) -> Self::Output {
        match (self, rhs) {
            (VariableType::Scalar(x), VariableType::Scalar(y)) => Ok(VariableType::Scalar(x * y)),
            (VariableType::Matrix(x), VariableType::Matrix(y)) => {
                let comp = x * y;
                if let Ok(z) = comp {
                    Ok(VariableType::Matrix(z))
                }
                else {
                    Err(comp.err().unwrap())
                }
            }
            (VariableType::None, VariableType::None) => Err(String::from("Cannot multiply None")),
            (_, _) => Err(String::from("Type does not support multiplication, or mismatched types."))
        }
    }
}
impl Div for VariableType {
    type Output = Result<VariableType, String>;

    fn div(self, rhs: Self) -> Self::Output {
        match (self, rhs) {
            (VariableType::Scalar(x), VariableType::Scalar(y)) => Ok(VariableType::Scalar(x * y)),
            (VariableType::None, VariableType::None) => Err(String::from("Cannot divide None")),
            (_, _) => Err("Type does not support division.".to_string())
        }
    }
}
impl ToString for VariableType {
    fn to_string(&self) -> String {
        match self {
            VariableType::Scalar(x) => x.to_string(),
            VariableType::Vector(x) => x.to_string(),
            VariableType::Matrix(x) => x.to_string(),
            VariableType::None => String::from("None")
        }
    }
}