use std::ops::{Add, Index, IndexMut, Mul, Sub};

use super::vector::Vector;
use super::mt_base::VarComm;
use crate::num_resolver::*;

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
}
impl Mul for Matrix {
    type Output = Result<Self, String>;

    fn mul(self, rhs: Self) -> Self::Output {
        todo!()
    }
}
impl ToString for Matrix {
    fn to_string(&self) -> String {

        let mut result = String::new();
        match (self.m, self.n) {
            (1, _) => { //Only one row
                result += "[ ";
                let target = self.val.get(0).unwrap(); //We know that size == 1.
                for i in target.get_val() {
                    result += &format!("{} ", i.to_string());
                }
                result += "]";
            }
            (_, _) => { //Mutiple rows
                // ⌊ ⌋ ⌈ ⌉ |

                let mut result = String::new();
                for (i, row) in self.val.iter().enumerate() {
                    let open;
                    let close;
                    if i == 0 {
                        open = '⌈';
                        close = '⌉';
                    }
                    else if i == self.val.len() - 1 {
                        open = '|';
                        close = open;
                    }
                    else {
                        open = '⌊';
                        close = '⌉';
                    }

                    let mut this_row_result = String::from(format!("{open} "));

                    for i in row.get_val() {
                        this_row_result += &format!("{} ", i.to_string());
                    }

                    result += &format!("{this_row_result}{close}\n");
                }
            }
        }

        result
    }
}
impl VarComm for Matrix {
    type StoredData = Vec<Vector>;

    fn val_eq(&self, obj: &Self) -> bool {
        todo!()
    }
    fn get_val(&self) -> &Self::StoredData {
        todo!()
    }
    fn set_val(&mut self, new_data: Self::StoredData)
    {
        todo!()
    }

    fn steralize(&self) -> String {
        todo!()
    }
    fn from_steralize(&mut self, input: &str) -> Result<(), String> {
        todo!()
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

    
    fn largest_num_digits(&self) -> u32 {
        let mut largest = 0;
        for row in 0..self.m {
            for col in 0..self.n {
                let result = get_number_digits_count(self.val[row][col], 3);

                largest = std::cmp::max(largest, result);
            }
        }

        largest
    }
}