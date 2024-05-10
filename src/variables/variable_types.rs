use std::fmt::{Debug, Display};
pub use crate::math_types::{Matrix, Vector, Scalar, VarComm};

#[derive(Debug, Clone)]
pub enum VariableType {
    Scalar(Scalar),
    Vector(Vector),
    Matrix(Matrix), //Functions will be defined later.
    None
}

impl PartialEq for VariableType{
    fn eq(&self, other: &Self) -> bool {
        match (self, other) {
            (VariableType::Scalar(a), VariableType::Scalar(b)) => a == b,
            (VariableType::Vector(a), VariableType::Vector(b)) => a == b,
            (VariableType::Matrix(a), VariableType::Matrix(b)) => a == b,
            (VariableType::None, VariableType::None) => true,
            (_, _) => false
        }
    }
}
impl Display for VariableType {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            VariableType::Scalar(x) => Display::fmt(&x, f),
            VariableType::Vector(x) => Display::fmt(&x, f),
            VariableType::Matrix(x) => Display::fmt(&x, f),
            VariableType::None => write!(f, "None")
        }
    }
}
impl VariableType {
    pub fn display_type(&self) -> String {
        match self {
            VariableType::Scalar(_) => String::from("(Scalar)"),
            VariableType::Vector(v) => format!("(Vector: {})", v.dim()),
            VariableType::Matrix(m) => format!("(Matrix: {}x{})", m.rows(), m.cols()),
            VariableType::None => String::from("(None)"),
        }
    }

    pub fn is_none(&self) -> bool {
        self == &VariableType::None
    }
    pub fn is_val(&self) -> bool {
        self != &VariableType::None
    }

    pub fn add(&self, rhs: &Self) -> Result<Self, String> {
        match (self, rhs) {
            (VariableType::Scalar(x), VariableType::Scalar(y)) => Ok(VariableType::Scalar(x.add(y))),
            (VariableType::Vector(x), VariableType::Vector(y)) => Ok(VariableType::Vector(x.add(y)?)),
            (VariableType::Matrix(x), VariableType::Matrix(y)) => Ok(VariableType::Matrix(x.add(y)?)),
            (VariableType::None, VariableType::None) | (VariableType::None, _) | (_, VariableType::None) => Err(String::from("Cannot add None")),
            (_, _) => Err("Mismatched types.".to_string())
        }
    }
    pub fn sub(&self, rhs: &Self) -> Result<Self, String> {
        match (self, rhs) {
            (VariableType::Scalar(x), VariableType::Scalar(y)) => Ok(VariableType::Scalar(x.sub(y))),
            (VariableType::Vector(x), VariableType::Vector(y)) => Ok(VariableType::Vector(x.sub(y)?)),
            (VariableType::Matrix(x), VariableType::Matrix(y)) => Ok(VariableType::Matrix(x.sub(y)?)),
            (VariableType::None, VariableType::None) | (VariableType::None, _) | (_, VariableType::None)  => Err(String::from("Cannot subtract None")),
            (_, _) => Err("Mismatched types.".to_string())
        }
    }
    pub fn mul(&self, rhs: &Self) -> Result<Self, String> {
        match (self, rhs) {
            (VariableType::Scalar(x), VariableType::Scalar(y)) => Ok(VariableType::Scalar(x.mul(y))),
            (VariableType::Matrix(x), VariableType::Matrix(y)) => Ok(VariableType::Matrix(x.mul(y)?)),
            (VariableType::None, VariableType::None) | (VariableType::None, _) | (_, VariableType::None)  => Err(String::from("Cannot multiply None")),
            (_, _) => Err(String::from("Type does not support multiplication, or mismatched types."))
        }
    }
    pub fn div(&self, rhs: &Self) -> Result<Self, String> {
        match (self, rhs) {
            (VariableType::Scalar(x), VariableType::Scalar(y)) => Ok(VariableType::Scalar(x.div(y))),
            (VariableType::None, VariableType::None) | (VariableType::None, _) | (_, VariableType::None)  => Err(String::from("Cannot divide None")),
            (_, _) => Err("Type does not support division.".to_string())
        }
    }
    pub fn modulo(&self, rhs: &Self) -> Result<Self, String> {
        match (self, rhs) {
            (VariableType::Scalar(x), VariableType::Scalar(y)) => Ok(VariableType::Scalar(x.modulo(y)) ),
            (VariableType::None, VariableType::None) | (VariableType::None, _) | (_, VariableType::None)  => Err(String::from("Cannot modulo None")),
            (_, _) => Err("Type does not support modulo.".to_string())
        }
    }
    pub fn powf(&self, rhs: &Self) -> Result<Self, String> {
        match (self, rhs) {
            (VariableType::Scalar(x), VariableType::Scalar(y)) => Ok(VariableType::Scalar(x.powf(y))),
            (VariableType::Vector(_), _) | (_, VariableType::Vector(_)) => Err(String::from("Vectors do not support power raising.")),
            (VariableType::Matrix(x), VariableType::Scalar(y)) => Ok(VariableType::Matrix(x.pow(y)?)),
            (VariableType::Matrix(_), _) => Err(String::from("Cannot raise a matrix to any object that is not a scalar.")),
            (VariableType::None, VariableType::None) | (VariableType::None, _) | (_, VariableType::None)  => Err(String::from("Cannot None to a power")),
            (_, _) => Err("Type does not support pow.".to_string())
        }
    }

    pub fn sterilize(&self) -> String {
        match self {
            VariableType::Scalar(s) => s.sterilize(),
            VariableType::Vector(v) => v.sterilize(),
            VariableType::Matrix(m) => m.sterilize(),
            VariableType::None => String::from("NAN")
        }
    }
    pub fn from_sterilize(input: &str) -> Result<Self, String> {
        if input.len() < 3 {
            return Err(String::from("Not enough characters."));
        }

        match &input[0..3] {
            "SCA" => Ok(VariableType::Scalar(Scalar::from_sterilize(input)?)),
            "VEC" => Ok(VariableType::Vector(Vector::from_sterilize(input)?)),
            "MAT" => Ok(VariableType::Matrix(Matrix::from_sterilize(input)?)),
            "NAN" => Ok(VariableType::None),
            a => Err(format!("Cannot deduce type name '{a}'"))
        }
    }
}