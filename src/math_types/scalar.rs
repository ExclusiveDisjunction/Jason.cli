use std::ops::{Add, Div, Mul, Sub};

use super::mt_base::VarComm;

pub struct Scalar {
    val: f64
}
impl VarComm for Scalar {
    type StoredData = f64;

    fn get_val(&self) -> &Self::StoredData {
        &self.val
    }
    fn set_val(&mut self, new_data: Self::StoredData) -> Result<(), String>{
        self.val = new_data;
        Ok(())
    }
    fn val_eq(&self, other: &Self) -> bool {
        self.val == other.val
    }

    fn sterilize(&self) -> String {
        String::from("SCA ") + &self.val.to_string()
    }
    fn from_sterilize(&mut self, input_string: &str) -> Result<(), String> {
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
                    Ok(a) => {
                        self.val = a;
                        Ok(())
                    }
                    Err(_) => Err(String::from("Value is not of proper numeric format."))
                }
            }
            _ => {
                self.val = 0.00f64;
                Ok(())
            }
        }
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
