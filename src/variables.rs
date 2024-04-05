use std::ops::{Add, Div, Mul, Sub};
use crate::math_types::{matrix::Matrix as Matrix, vector::Vector as Vector, scalar::Scalar as Scalar};

pub enum VariableType {
    Scalar(Scalar),
    Vector(Vector),
    Matrix(Matrix), //Functions will be defined later.
    None
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