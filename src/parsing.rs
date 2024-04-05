use crate::stack::Stack;
use crate::operators::Operator;
use crate::operators::Operators;

pub fn is_balanced_string(obj: &str) -> bool 
{
    let mut st = Stack::<char>::new();

    for (_, c) in obj.chars().enumerate() 
    {
        match c 
        {
            _ if c == '[' || c == '(' || c == '{' => st.push(c.clone()),
            _ if c == ']' || c == ')' || c == '}' => 
            {
                let top_t = st.peek();
                if let Some(top) = top_t 
                {
                    let is_match = (top == &'[' && c == ']') || (top == &'(' && c == ')') || (top == &'{' && c == '}');
    
                    if !is_match { return false; }
    
                    if let Err(_) = st.pop() {
                        return false;
                    }
                    else {
                        continue;
                    }
                }
                else {
                    return false;
                }
            }
            _ => continue
        }
    }

    st.is_empty()
}
pub fn is_numeric_string(obj: &str) -> bool 
{
    let mut last_was_number = false;
    let mut last_was_space = false;
    let mut has_period = false;

    for c in obj.chars() 
    {
        match c 
        {
            _ if c.is_numeric() =>
            {
                if last_was_space { return false; }
                else { last_was_number = true; }
            }
            _ if c.is_whitespace() =>
            {
                if last_was_space { continue; }
                else if last_was_number { last_was_space = true; }
            }
            _ if c == '.' =>
            {
                if has_period { return false; } //period was found
                else { has_period = true; }
            }
            _ => { return false; }
        }
    }

    true
}
pub fn flush_prev_num(result: &mut String, prev_num: &mut String) -> Result<(), String>
{
    if !prev_num.is_empty() 
    {
        if !is_numeric_string(prev_num) { return Err(format!("The expression '{prev_num}' is not of proper format.")); }

        result.push_str(&format!("{prev_num} "));
        prev_num.clear();
    }

    Ok(())
}


pub fn to_postfix(infix: &str, ops: &Operators) -> Result<String, String> 
{
    if !is_balanced_string(infix) {
        return Err("There is not a balance of braces.".to_string());
    }

    let mut result = String::new();
    let mut prev_num = String::new();
    let mut prev_was_oper = false;
    let mut prev_was_num = false;
    let mut first_char = true;

    let mut opers = Stack::<Operator>::new();

    for item in infix.chars() 
    {
        match item
        {
            _ if item.is_numeric() || item == '.' => 
            {
                if !first_char && !prev_was_oper && !prev_was_num {
                    return Err("An operator or brace was expected, but a number was given.".to_string());
                }

                prev_num += &item.to_string();
                prev_was_num = true;
                prev_was_oper = false;
                first_char = false;
            }
            _ if item.is_alphabetic() => 
            {
                if !first_char && !prev_was_oper && !prev_was_num {
                    return Err("An operator or brace was expected, but a number was given.".to_string());
                }

                let constant_val_o: Option<f64>;
                match item 
                {
                    'c' => constant_val_o = Some((3i32 * 10i32.pow(8)) as f64),
                    _ => return Err(format!("The alphabetical character \'{item}\' is not allowed,"))
                }

                if let Some(constant_val) = constant_val_o
                {
                    result += &constant_val.to_string();

                    prev_was_oper = false;
                    prev_was_num = false;
                    first_char = false;
                }
            }
            _ if item.is_whitespace() => 
            {
                flush_prev_num(&mut result, &mut prev_num)?;

                prev_was_num = false;
            }
            '(' | '{' | '[' => 
            {
                flush_prev_num(&mut result, &mut prev_num)?;

                prev_was_oper = true;
                prev_was_num = false;
                first_char = false;

                let operator = ops.get_operator(item);
                match operator
                {
                    Some(a) => opers.push(a.clone()),
                    _ => return Err(format!("The operator \'{item}\' was not recognized in this context."))  
                }
            }
            ')' | '}' | ']' => 
            {
                flush_prev_num(&mut result, &mut prev_num)?;

                prev_was_oper = false;
                prev_was_num = false;
                first_char = false;

                let mut stack_oper: Operator;
                let target_brace: Operator;

                {
                    let stack_oper_t = opers.peek();
                    let target_brace_t;

                    let match_open_char;
                    if item == ')' { match_open_char = '(' }
                    else if item == '}' { match_open_char = '{' }
                    else if item == ']' { match_open_char = '[' }
                    else { return Err("Unmatched brace".to_string()); }

                    target_brace_t = ops.get_operator(match_open_char);
                    match (stack_oper_t, target_brace_t) 
                    {
                        (Some(a), Some(b)) => 
                        {
                            stack_oper = a.clone();
                            target_brace = b.clone();
                        }
                        (_, _) => { return Err("Unrecognized brace pattern.".to_string()); }
                    }
                }

                while !opers.is_empty() && stack_oper != target_brace 
                {
                    result += &format!("{stack_oper} ");

                    opers.pop()?;

                    let peeked = opers.peek();

                    if let Some(a) = peeked { stack_oper = a.clone(); }
                    else { continue; }
                }

                if opers.peek() == Some(&target_brace) 
                {
                    opers.pop()?;
                }
            }
            _ if ops.is_operator(item) => 
            {
                if first_char {
                    return Err("The first character cannot be an operator.".to_string());
                }
                else if prev_was_oper {
                    return Err("The previous item was an operator, and a number was expected.".to_string());
                }

                flush_prev_num(&mut result, &mut prev_num)?; 

                let curr_oper: Operator;
                {
                    let curr_oper_t = ops.get_operator(item);
                    match curr_oper_t 
                    {
                        Some(a) => curr_oper = a.clone(),
                        _ => return Err(format!("Cannot understand operator \'{item}\'"))
                    }
                } 

                if !opers.is_empty() 
                {
                    let mut last_oper: Operator = opers.peek().unwrap().clone(); //Since opers is not empty, we know this has a value.

                    match (last_oper.get_precidence(), curr_oper.get_precidence()) 
                    {
                        (0, 0) => return Err(format!("The operators \'{curr_oper}\' or \'{last_oper}\' coult not be determined.")),
                        (mut p1, p2 ) => 
                        {
                            while !opers.is_empty() && p1 != ops.get_brace_precidence() && (p2 < p1 || (p1 == p2 && item != '^')) 
                            {
                                result += &format!("{} ", last_oper.to_string());

                                if !opers.is_empty() { //Evaluated before getting the next one.
                                    opers.pop()?;
                                }

                                if !opers.is_empty() 
                                {
                                    last_oper = opers.peek().unwrap().clone();
                                    p1 = last_oper.get_precidence();
                                }
                            }
                            opers.push(curr_oper.clone());
                        }
                    }
                }   
                else
                {
                    opers.push(curr_oper.clone());
                }  

                prev_was_oper = true;
                prev_was_num = false;
                first_char = false;
            }
            _ => return Err(format!("Invalid character \'{}\'", item ))
        }
    }

    //We expect that the last thing was a number or space, not an operator.
    if prev_was_oper 
    {
        return Err("The string does not contain enough numbers to operators.".to_string());
    }

    flush_prev_num(&mut result, &mut prev_num)?;

    while !opers.is_empty() 
    {
        result += &opers.peek().unwrap().to_string();
        if opers.len() != 1 {
            result += " ";
        }
        opers.pop()?;
    }

    Ok(result.trim().to_string())
}
pub fn evaluate_postfix(postfix: &str, ops: &Operators) -> Result<f64, String> 
{
    if postfix.is_empty() 
    {
        return Ok(0.00f64);
    }
    else if is_numeric_string(postfix) 
    { 
        let num: Result<f64, _> = postfix.parse::<f64>();

        match num 
        {
            Ok(n) => return Ok(n),
            Err(e) => return Err(e.to_string())
        }
    }

    let literals: Vec<&str> = postfix.split(' ').collect();
    let mut results = Stack::<f64>::new();
    for curr in literals 
    {
        if curr.len() == 1 && ops.is_operator(curr.chars().nth(0).unwrap()) 
        {
            let operator;
            {
                let oper_temp = ops.get_operator(curr.chars().nth(0).unwrap());
                if let Some(a) = oper_temp {
                    operator = a;
                }
                else {
                    return Err(format!("\'{curr}\' is not a recognized operator."));
                }
            }

            if results.len() < 2 {
                return Err("The expression calls for an operator, but does not contain enough numbers.".to_string());
            }

            let b = results.peek().unwrap().clone();
            results.pop()?;
            let a = results.peek().unwrap().clone();
            results.pop()?;

            results.push(operator.evaluate(a, b));
        }
        else if is_numeric_string(curr) 
        {
            let converted: Result<f64, _> = curr.parse::<f64>();
            if let Ok(a) = converted {
                results.push(a);
            }
            else {
                return Err(format!("Could not parse the numeric string \'{curr}\'."));
            }
        }
        else 
        {
            return Err("Invalid operator/numerical expression.".to_string());
        }
    }

    if results.len() != 1 
    {
        return Err("The expression does not have the correct number of operators to numbers.".to_string());
    }

    Ok(results.peek().unwrap().clone())
}