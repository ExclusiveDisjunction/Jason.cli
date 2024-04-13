use std::fmt::Display;
use crate::math_types::{matrix::Matrix as Matrix, vector::Vector as Vector, scalar::Scalar as Scalar};
use crate::math_types::mt_base::VarComm;

pub enum VariableType {
    Scalar(Scalar),
    Vector(Vector),
    Matrix(Matrix), //Functions will be defined later.
    None
}

impl Display for VariableType {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        let str = match self {
            VariableType::Scalar(x) => x.to_string(),
            VariableType::Vector(x) => x.to_string(),
            VariableType::Matrix(x) => x.to_string(),
            VariableType::None => String::from("None")
        };
        write!(f, "{}", str)
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

    pub fn add(&self, rhs: Self) -> Result<Self, String> {
        match (self, rhs) {
            (VariableType::Scalar(x), VariableType::Scalar(y)) => Ok(VariableType::Scalar(x.add(&y))),
            (VariableType::Vector(x), VariableType::Vector(y)) => {
                let comp = x.add(&y);
                if let Ok(z) = comp {
                    Ok(VariableType::Vector(z))
                }
                else {
                    Err(comp.err().unwrap())
                }
            }
            (VariableType::Matrix(x), VariableType::Matrix(y)) => {
                let comp = x.add(&y);
                if let Ok(z) = comp {
                    Ok(VariableType::Matrix(z))
                }
                else {
                    Err(comp.err().unwrap())
                }
            }
            (VariableType::None, VariableType::None) | (VariableType::None, _) | (_, VariableType::None) => Err(String::from("Cannot add None")),
            (_, _) => Err("Mismatched types.".to_string())
        }
    }
    pub fn sub(&self, rhs: Self) -> Result<Self, String> {
        match (self, rhs) {
            (VariableType::Scalar(x), VariableType::Scalar(y)) => Ok(VariableType::Scalar(x.sub(&y))),
            (VariableType::Vector(x), VariableType::Vector(y)) => {
                let comp = x.sub(&y);
                if let Ok(z) = comp {
                    Ok(VariableType::Vector(z))
                }
                else {
                    Err(comp.err().unwrap())
                }
            }
            (VariableType::Matrix(x), VariableType::Matrix(y)) => {
                let comp = x.sub(&y);
                if let Ok(z) = comp {
                    Ok(VariableType::Matrix(z))
                }
                else {
                    Err(comp.err().unwrap())
                }
            }
            (VariableType::None, VariableType::None) | (VariableType::None, _) | (_, VariableType::None)  => Err(String::from("Cannot subtract None")),
            (_, _) => Err("Mismatched types.".to_string())
        }
    }
    pub fn mul(&self, rhs: Self) -> Result<Self, String> {
        match (self, rhs) {
            (VariableType::Scalar(x), VariableType::Scalar(y)) => Ok(VariableType::Scalar(x.mul(&y))),
            (VariableType::Matrix(x), VariableType::Matrix(y)) => {
                let comp = x.mul(&y);
                if let Ok(z) = comp {
                    Ok(VariableType::Matrix(z))
                }
                else {
                    Err(comp.err().unwrap())
                }
            }
            (VariableType::None, VariableType::None) | (VariableType::None, _) | (_, VariableType::None)  => Err(String::from("Cannot multiply None")),
            (_, _) => Err(String::from("Type does not support multiplication, or mismatched types."))
        }
    }
    pub fn div(&self, rhs: Self) -> Result<Self, String> {
        match (self, rhs) {
            (VariableType::Scalar(x), VariableType::Scalar(y)) => Ok(VariableType::Scalar(x.div(&y))),
            (VariableType::None, VariableType::None) | (VariableType::None, _) | (_, VariableType::None)  => Err(String::from("Cannot divide None")),
            (_, _) => Err("Type does not support division.".to_string())
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

        return match &input[0..3] {
            "SCA" => Ok(VariableType::Scalar(Scalar::from_sterilize(input)?)),
            "VEC" => Ok(VariableType::Vector(Vector::from_sterilize(input)?)),
            "MAT" => Ok(VariableType::Matrix(Matrix::from_sterilize(input)?)),
            "NAN" => Ok(VariableType::None),
            a => Err(format!("Cannot deduce type name '{a}'"))
        }
    }
}