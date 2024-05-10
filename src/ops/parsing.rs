use std::fmt::Display;

use log::{error, warn, info};

use super::operators::Operator;
use super::operators::Operators;
use crate::stack::Stack;
use crate::math_types::Scalar;
use crate::variables::variable_storage::EnvironmentVariables;
use crate::variables::variable_storage::VarStorage;
use crate::variables::variable_types::VariableType;


#[derive(Debug, Clone)]
pub enum ExpressionType {
    NumericExpression(Scalar), //Returns the specified number.
    VariableExpression(String), //Returns the name of the variable.
    DeclorationExpression(String), //Returns the name of the temporary
    EmptyExpression,
    AnsExpression,
    EnvironmentVariable(String),
    InvalidExpression(String) //Contains the error message. 
}

impl ExpressionType {
    fn parse_expression(input: &str) -> Self {
        if !is_balanced_string(input) {
            return ExpressionType::InvalidExpression(String::from("Braces are not balanced"));
        }
        else if input.is_empty() {
            return ExpressionType::EmptyExpression;
        }

        if input.to_lowercase().trim() == "ans" {
            return ExpressionType::AnsExpression;
        }

        let num_result = Self::is_numeric_expression(input);
        if let Ok(n) = num_result {
            return ExpressionType::NumericExpression(n);
        }

        let dec_result = Self::is_decloration_expression(input);
        if let Ok(d) = dec_result {
            return ExpressionType::DeclorationExpression(d);
        }

        let var_result = Self::is_variable_expression(input);
        if let Ok(v) = var_result {
            return ExpressionType::VariableExpression(v);
        }

        //At this point, all cases have failed
        let error_string = format!("\tAns could not be deduced\n\tNumeric type cannot be deduced because of \"{}\"\n\tDecloration type cannot be deduced because of \"{}\"\n\tVariable type cannot be deduced because of \"{}\"", num_result.unwrap_err(), dec_result.unwrap_err(), var_result.unwrap_err());

        ExpressionType::InvalidExpression(error_string)
    }

    pub fn is_numeric_expression(obj: &str) -> Result<Scalar, String>  {
        let mut last_was_number = false;
        let mut last_was_space = false;
        let mut has_period = false;
    
        for c in obj.chars() {
            match c {
                _ if c.is_numeric() => {
                    if last_was_space {
                        return Err(String::from("Last character was a space, but a number was provided"));
                    } else {
                        last_was_number = true;
                    }
                }
                _ if c.is_whitespace() => {
                    if last_was_space {
                        continue;
                    } else if last_was_number {
                        last_was_space = true;
                    }
                }
                _ if c == '.' => {
                    if has_period {
                        return Err(String::from("More than one period was provided"));
                    }
                    //period was found
                    else {
                        has_period = true;
                    }
                }
                _ => {
                    return Err(String::from("A character that was not a period, number, or white space was found"));
                }
            }
        }
    
        Ok(Scalar::new(obj.parse::<f64>().unwrap()))
    }
    pub fn is_decloration_expression(input: &str) -> Result<String, String> {
        Err(String::from("Not a decloration your mom"))
    }
    pub fn is_variable_expression(input: &str) -> Result<String, String> {
        Err(String::from("not a variable your dad"))
    }
}
impl Display for ExpressionType {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        todo!()
    }
}

#[derive(Debug)]
pub enum ExpressionElement<'a> {
    SubExpr(ExpressionType),
    Oper(&'a Operator)
}
impl<'a> Display for ExpressionElement<'a> {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            Self::SubExpr(s) => s.fmt(f),
            Self::Oper(o) => o.fmt(f)
        }
    }
}

#[derive(Debug)]
pub struct Expression<'a> {
    elements: Vec<ExpressionElement<'a>>
}
impl<'a> Display for Expression<'a>{
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        let mut result = String::new();
        for item in &self.elements {
            result += &format!("{item} ");
        }

        write!(f, "{result}")
    }
}
impl<'a> Expression<'a> {
    fn new() -> Self {
        Self { 
            elements: Vec::<ExpressionElement>::new()
        }
    }

    pub fn from_infix(input: &str, ops: &'a Operators) -> Result<Self, String> {
        Self::infix_to_postfix(input, ops)
    }
    pub fn from_postfix(input: &str, ops: &'a Operators) -> Result<Self, String> {
        todo!()
    }

    pub fn infix_to_postfix(infix: &str, ops: &'a Operators) -> Result<Self, String> {
        if !is_balanced_string(infix) {
            return Err(String::from("There is not a balance of braces."));
        }
    
        let mut result = Self::new();
        let mut prev_expr = String::new();
        let mut prev_was_oper = false;
        let mut prev_was_num = false;
        let mut first_char = true;
    
        let mut opers = Stack::<&Operator>::new();
    
        for item in infix.chars() {
            match item {
                _ if item.is_whitespace() => {
                    result.flush_prev_expr(&mut prev_expr)?;
    
                    prev_was_num = false;
                }
                '(' => {
                    result.flush_prev_expr(&mut prev_expr)?;
    
                    prev_was_oper = true;
                    prev_was_num = false;
                    first_char = false;
    
                    let operator = ops.get_operator(item);
                    match operator {
                        Some(a) => opers.push(a),
                        _ => {
                            return Err(format!(
                                "The operator \'{item}\' was not recognized in this context."
                            ))
                        }
                    }
                }
                ')' => {
                    result.flush_prev_expr(&mut prev_expr)?;
    
                    prev_was_oper = false;
                    prev_was_num = false;
                    first_char = false;
    
                    if opers.is_empty() {
                        return Err(String::from("Found closing brace when no other operators have been supplied."));
                    }
    
                    while !opers.is_empty() {
                        let top = opers.pop();
                        if let Some(t) = top {
                            if t == &'(' { //We exit
                                break;
                            }

                            result.elements.push(ExpressionElement::Oper(t));
                        }
                        else {
                            break;
                        }
                    }
    
                    if !opers.is_empty() && *opers.peek().unwrap() == &'(' && opers.pop().is_none() { //This will check to see if there are any operators left, see if the top operator is the opening brace, and then pop it. If the pop fails, then we return an error.
                            return Err(String::from("Could not pop starting brace."));
                    }
                }
                _ if ops.is_operator(item) => {
                    if first_char {
                        return Err("The first character cannot be an operator.".to_string());
                    } else if prev_was_oper {
                        return Err(
                            "The previous item was an operator, and a number was expected.".to_string(),
                        );
                    }
                    
                    result.flush_prev_expr(&mut prev_expr)?;
    
                    let curr_oper: &Operator;
                    {
                        let curr_oper_t = ops.get_operator(item);
                        match curr_oper_t {
                            Some(a) => curr_oper = a,
                            _ => return Err(format!("Cannot understand operator \'{item}\'")),
                        }
                    }
    
                    if !opers.is_empty() {
                        let mut last_oper: &Operator = opers.pop().unwrap(); //Since opers is not empty, we know this has a value.
    
                        match (last_oper.get_precedence(), curr_oper.get_precedence())
                            {
                                (0, 0) => return Err(format!("The operators \'{curr_oper}\' or \'{last_oper}\' could not be determined.")),
                                (mut p1, p2) =>
                                    {
                                        while !opers.is_empty() && p1 != ops.get_brace_precedence() && (p2 < p1 || (p1 == p2 && item != '^'))
                                        {
                                            result.elements.push(ExpressionElement::Oper(last_oper));

                                            if !opers.is_empty() {
                                                last_oper = opers.pop().unwrap();
                                                p1 = last_oper.get_precedence();
                                            }
                                        }
                                        opers.push(curr_oper);
                                    }
                            }
                    } else {
                        opers.push(curr_oper);
                    }
    
                    prev_was_oper = true;
                    prev_was_num = false;
                    first_char = false;
                }
                _ => { //We dont have to check for proper expressions because these are validated when flush_prev_expr is called
                    if !first_char && !prev_was_oper && !prev_was_num {
                        return Err(
                            "An operator or brace was expected, but a number was given.".to_string()
                        );
                    }
    
                    prev_expr += &item.to_string();
                    prev_was_num = true;
                    prev_was_oper = false;
                    first_char = false;
                }
            }
        }
    
        //We expect that the last thing was a number or space, not an operator.
        if prev_was_oper {
            return Err("The string does not contain enough numbers to operators.".to_string());
        }
    
        result.flush_prev_expr(&mut prev_expr)?;
    
        while !opers.is_empty() {
            result.elements.push(ExpressionElement::Oper(opers.pop().unwrap()));
        }
    
        Ok(result)
    }

    fn flush_prev_expr(&mut self, prev_expr: &mut String) -> Result<(), String> {
        if !prev_expr.is_empty() {
            let parsed = ExpressionType::parse_expression(prev_expr);
    
            if let ExpressionType::InvalidExpression(e) = parsed {
                return Err(format!("The expression '{prev_expr}' is not in proper format because of:\n{}", &e));
            }
    
            self.elements.push(ExpressionElement::SubExpr(parsed));
            prev_expr.clear();
        }
    
        Ok(())
    }

    pub fn evaluate(&self, vars: &VarStorage, env: &EnvironmentVariables) -> VariableType {
        if self.elements.is_empty() {
            return VariableType::Scalar(Scalar::new(0.00f64));
        }

        let mut result = Stack::<VariableType>::new();
        for elem in &self.elements {
            match elem {
                ExpressionElement::Oper(o) => {
                    let b_t = result.pop();
                    let a_t = result.pop();

                    match (a_t, b_t) {
                        (Some(a), Some(b)) => {
                            info!("Performing operation {o} on {} and {}", a.display_type(), b.display_type());
                            let evaluation = o.evaluate(a, b);
                            if let Ok(t) = evaluation {
                                result.push(t);
                            }
                            else {
                                error!("Evaluation failed");
                                return VariableType::None;
                            }
                        }
                        (_, _) => {
                            error!("Could not pop two elements from stack, even though it was expected.");
                            return VariableType::None;
                        }
                    }
                }
                ExpressionElement::SubExpr(expr) => {
                    /*
                        We need to either:
                        1. Push the scalar value to the expressions
                        2. Push the value of the variable
                        3. Push the value of the temporary
                        4. Throw an error for an error string
                        5. Push 0.00 for an empty expression.
                    */

                    match expr {
                        ExpressionType::AnsExpression => {
                            result.push(vars.get_ans().clone());
                        }
                        ExpressionType::DeclorationExpression(tmp) => {
                            let retrived = vars.get_temporary_value(tmp);
                            if let Some(r) = retrived {
                                result.push(r.clone());
                            }
                            else {
                                error!("Could not resolve variable '{tmp}' while parsing.");
                                return VariableType::None;
                            }
                        }
                        ExpressionType::VariableExpression(name) => {
                            let retrived = vars.get_variable_value(name);
                            if let Some(r) = retrived {
                                result.push(r.clone());
                            }
                            else {
                                error!("Could not resolve variable '{name}' while parsing.");
                                return VariableType::None;
                            }
                        }
                        ExpressionType::EnvironmentVariable(var) => {
                            if let Some(v) = env.get_var(var) {
                                result.push(v.clone());
                            }
                            else {
                                error!("Could not retrive the environment variable '{var}'.");
                                return VariableType::None;
                            }
                        }
                        ExpressionType::EmptyExpression => result.push(VariableType::Scalar(Scalar::default())),
                        ExpressionType::InvalidExpression(_) => return VariableType::None,
                        ExpressionType::NumericExpression(n) => result.push(VariableType::Scalar(n.clone()))
                    }
                }
            }
        }

        if result.len() != 1 {
            return VariableType::None;
        }

        result.pop().unwrap()
    }
}

pub fn is_balanced_string(obj: &str) -> bool {
    let mut st = Stack::<char>::new();

    for c in obj.chars() {
        match c {
            _ if c == '[' || c == '(' || c == '{' => st.push(c),
            _ if c == ']' || c == ')' || c == '}' => {
                let top_t = st.peek();
                if let Some(top) = top_t {
                    let is_match = (top == &'[' && c == ']')
                        || (top == &'(' && c == ')')
                        || (top == &'{' && c == '}');

                    if !is_match {
                        return false;
                    }

                    if st.pop().is_none() {
                        return false;
                    } else {
                        continue;
                    }
                } else {
                    return false;
                }
            }
            _ => continue,
        }
    }

    st.is_empty()
}

pub fn evaluate_postfix(postfix: &str, ops: &Operators) -> Result<f64, String> {
    todo!("Hi {postfix} using operators")

    /*
    if postfix.is_empty() {
        return Ok(0.00f64);
    } else if is_numeric_string(postfix) {
        let num: Result<f64, _> = postfix.parse::<f64>();

        return match num {
            Ok(n) => Ok(n),
            Err(e) => Err(e.to_string()),
        }
    }

    let literals: Vec<&str> = postfix.split(' ').collect();
    let mut results = Stack::<f64>::new();
    for curr in literals {
        let first_char = curr.chars().next().unwrap();
        if curr.len() == 1 && ops.is_operator(first_char) {
            let operator;
            {
                let oper_temp = ops.get_operator(first_char);
                if let Some(a) = oper_temp {
                    operator = a;
                } else {
                    return Err(format!("\'{curr}\' is not a recognized operator."));
                }
            }

            if results.len() < 2 {
                return Err(
                    "The expression calls for an operator, but does not contain enough numbers."
                        .to_string(),
                );
            }

            let b = *results.peek().unwrap();
            results.pop()?;
            let a = *results.peek().unwrap();
            results.pop()?;

            results.push(operator.evaluate(a, b));
        } else if is_numeric_string(curr) {
            let converted: Result<f64, _> = curr.parse::<f64>();
            if let Ok(a) = converted {
                results.push(a);
            } else {
                return Err(format!("Could not parse the numeric string \'{curr}\'."));
            }
        } else {
            return Err("Invalid operator/numerical expression.".to_string());
        }
    }

    if results.len() != 1 {
        return Err(
            "The expression does not have the correct number of operators to numbers.".to_string(),
        );
    }

    Ok(*results.peek().unwrap())
     */
}
