use std::{cmp::Ordering, fmt::Display};

use crate::variables::variable_types::VariableType;
use crate::math_types::Scalar;

#[derive(Clone, Debug)]
pub struct Operator {
    symbol: char,
    precedence: i32,
    eval: fn(VariableType,VariableType)->Result<VariableType, String>
}

impl PartialEq for Operator {
    fn eq(&self, other: &Self) -> bool {
        self.symbol == other.symbol && self.precedence == other.precedence
    }
}
impl PartialEq<char> for Operator {
    fn eq(&self, other: &char) -> bool {
        &self.symbol == other
    }
}
impl PartialOrd for Operator {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        if self == other { Some(Ordering::Equal) }
        else if self < other { Some(Ordering::Less) }
        else if self > other { Some(Ordering::Greater) }
        else { None }
    }
    fn lt(&self, other: &Self) -> bool {
        self.precedence < other.precedence
    }
    fn le(&self, other: &Self) -> bool {
        self.precedence <= other.precedence
    }
    fn gt(&self, other: &Self) -> bool {
        self.precedence > other.precedence
    }
    fn ge(&self, other: &Self) -> bool {
        self.precedence >= other.precedence
    }
}
impl Display for Operator {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}", self.symbol)
    }
}
impl Operator {
    pub fn new(s: char, p: i32, e : fn(VariableType, VariableType) -> Result<VariableType, String>) -> Self {
        Self { symbol: s, precedence: p, eval: e }
    }

    pub fn evaluate(&self, a: VariableType, b: VariableType) -> Result<VariableType, String> {
        (self.eval)(a, b)
    }

    pub fn get_precedence(&self) -> i32 { self.precedence }
}

pub struct Operators{
    opers: Vec<Operator>
}

impl Default for Operators {
    fn default() -> Self {
        Self::new()
    }
}
impl Operators {
    pub fn new() -> Self {
        Self {
            opers: vec![
                Operator::new('+', 1, |a:VariableType, b:VariableType| a.add(&b) ),
                Operator::new('-', 1, |a:VariableType, b:VariableType| a.sub(&b) ),
                Operator::new('*', 2, |a:VariableType, b:VariableType| a.mul(&b) ),
                Operator::new('/', 2, |a:VariableType, b:VariableType| a.div(&b) ),
                Operator::new('%', 2, |a:VariableType, b:VariableType| a.modulo(&b) ),
                Operator::new('^', 3, |a:VariableType, b:VariableType| a.powf(&b) ),
                Operator::new('(', 4, |_:VariableType, _:VariableType| Ok(VariableType::Scalar(Scalar::new(0.))) ),
                Operator::new('{', 4, |_:VariableType, _:VariableType| Ok(VariableType::Scalar(Scalar::new(0.))) ),
                Operator::new('[', 4, |_:VariableType, _:VariableType| Ok(VariableType::Scalar(Scalar::new(0.))) )
            ]
        }
    }

    pub fn get_operator(&self, oper: char) -> Option<&Operator> {
        self.opers.iter().find(|&item| item == &oper)
    }
    pub fn is_operator(&self, oper: char) -> bool {
        self.get_operator(oper).is_some()
    }

    pub fn get_brace_precedence(&self) -> i32 {
        match self.get_operator('(') {
            Some(x) => x.get_precedence(),
            _ => 0
        }
    }
}