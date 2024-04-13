use std::fmt::Display;
use super::mt_base::VarComm;

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
        write!(f, "{}", self.val.to_string())
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
