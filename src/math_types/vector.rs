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
    pub fn dim(&self) -> usize {
        self.d
    }
}
