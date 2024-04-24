use std::fmt::Display as Display;
use std::fmt::Formatter as Formatter;

pub struct Stack<T: PartialEq + ToString> {
    data: Vec<T>
}

impl<T: PartialEq + ToString> PartialEq for Stack<T> {
    fn eq(&self, other: &Self) -> bool{
        self.data == other.data
    }
}
impl<T: PartialEq + ToString> Display for Stack<T> {
    fn fmt(&self, f: &mut Formatter<'_>) -> Result<(), std::fmt::Error> {
        match self.data.len() {
            0 => { write!(f, "Empty Stack") }
            k => {
                let mut result_str: String = "{ ".to_string();
                for (j, i) in self.data.iter().rev().enumerate() {
                    let data_as_str: String = i.to_string();
                    result_str += &data_as_str;

                    if j + 1 != k {
                        result_str += " -> ";
                    }
                }

                result_str += " }";

                write!(f, "{}", result_str)
            }
        }
    }
}
impl<T: PartialEq + ToString> Default for Stack<T> {
    fn default() -> Self {
        Self::new()
    }
}
impl<T: PartialEq + ToString> Stack<T> {
    pub fn new() -> Self {
        Stack::<T> { data: Vec::<T>::new() }
    }

    pub fn len(&self) -> usize { self.data.len() }
    pub fn is_empty(&self) -> bool { self.data.is_empty() || self.data.len() == 0 }

    pub fn peek(&self) -> Option<&T> {
        self.data.last()
    }

    pub fn push(&mut self, data: T) {
        self.data.push(data);
    }
    pub fn pop(&mut self) -> Result<(), String> {
        if self.is_empty() { return Err("Empty stack.".to_string()); }

        self.data.pop();

        Ok(())
    }
    pub fn clear(&mut self) {
        self.data.clear();
    }
}