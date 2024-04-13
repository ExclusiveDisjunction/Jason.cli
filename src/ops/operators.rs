use std::{cmp::Ordering, fmt::Display};

#[derive(Clone)]
pub struct Operator {
    symbol: char,
    precedence: i32,
    eval: fn(f64,f64)->f64
}

impl PartialEq for Operator {
    fn eq(&self, other: &Self) -> bool {
        self.symbol == other.symbol && self.precedence == other.precedence
    }
    fn ne(&self, other: &Self) -> bool {
        self.symbol != other.symbol || self.precedence != other.precedence
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
    pub fn new(s: char, p: i32, e : fn(f64, f64) -> f64) -> Self {
        Self { symbol: s, precedence: p, eval: e }
    }

    pub fn evaluate(&self, a: f64, b: f64) -> f64 {
        (self.eval)(a, b)
    }

    pub fn get_precedence(&self) -> i32 { self.precedence }
}

pub struct Operators{
    opers: Vec<Operator>
}

impl Operators {
    pub fn new() -> Self {
        let mut op = Vec::<Operator>::new();

        op.push(Operator::new('+', 1, |a:f64, b:f64| a + b ));
        op.push(Operator::new('-', 1, |a:f64, b:f64| a - b ));
        op.push(Operator::new('*', 2, |a:f64, b:f64| a * b ));
        op.push(Operator::new('/', 2, |a:f64, b:f64| a / b ));
        op.push(Operator::new('%', 2, |a:f64, b:f64| a % b ));
        op.push(Operator::new('^', 3, |a:f64, b:f64| a.powf(b) ));
        op.push(Operator::new('(', 4, |_:f64, _:f64| 0.00f64 ));
        op.push(Operator::new('{', 4, |_:f64, _:f64| 0.00f64 ));
        op.push(Operator::new('[', 4, |_:f64, _:f64| 0.00f64 ));

        Self {
            opers: op
        }
    }

    pub fn get_operator(&self, oper: char) -> Option<&Operator> {
        for item in &self.opers {
            if item == &oper { return Some(item); }
        }

        None
    }
    pub fn is_operator(&self, oper: char) -> bool {
        let result = self.get_operator(oper);
        match result {
            Some(_) => true,
            None => false
        }
    }

    pub fn get_brace_precedence(&self) -> i32 {
        match self.get_operator('(') {
            Some(x) => x.get_precedence(),
            _ => 0
        }
    }
}