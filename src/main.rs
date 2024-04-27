
pub mod stack;
pub mod testing;
pub mod variables;
pub mod math_types;
pub mod ops;
pub mod functions;

use std::io::{stdin as stdin, stdout as stdout, Write};

/*;

use operators::Operators;
use parsing::{to_postfix, evaluate_postfix};
use num_resolver::round_f64;
*/

use math_types::{scalar::Scalar as Scalar, vector::Vector as Vector};
use variables::{variable_storage::VarStorage, variable_types::VariableType};


fn main() {
    let storage_r = VarStorage::new("run_variables/");
    if let Some(mut storage) = storage_r {
        if !storage.index_variables() {
            println!("Could not index variables.");
        }

        println!("Ans = {}", storage.get_ans());

        print!("Please enter a new name of a varaible: ");
        if stdout().flush().is_err() {
            return;
        }

        let mut input_val: String = String::new();
        let mut input_name: String = String::new();

        if stdin().read_line(&mut input_name).is_err() {
            println!("Could not read value.");
            return;
        }

        print!("Please enter a new value of the variable: ");
        if stdout().flush().is_err() {
            return;
        }

        if stdin().read_line(&mut input_val).is_err() {
            println!("Could not read value.");
            return;
        }

        let new_ans = input_val.trim().parse::<f64>();
        if new_ans.is_err() {
            println!("Input could not be formated because \"{}\"", new_ans.err().unwrap());
            return;
        }

        {
            let prev_val = storage.get_variable_value(&input_name);
            if let Some(t) = prev_val {
                println!("Previous value of this variable: {t}");
            }
            else {
                println!("No previous value in this variable.");
            }
        }
        let new_value = new_ans.unwrap();

        let mut new_core = Vector::new(2);
        new_core[0] = new_value;
        new_core[1] = new_value;

        let new_data = VariableType::Vector(new_core);
        storage.set_variable_value(&input_name, new_data);
        println!("Ans = {}", storage.get_variable_value(&input_name).unwrap());
    }
    else {
        println!("Could not load storage.");
    }

    /*
    let ops = Operators::new();

    let mut cmd_line:Vec<String> = std::env::args().collect();
    if cmd_line.len() != 1 { //Always 1 since the caller is part of it.
        //Work as a single expression.
        cmd_line.remove(0); //Removes the exe from the arguments.

        let print_postfix = cmd_line[0] == "-p";
        if print_postfix {
            cmd_line.remove(0); //Removes the flat
        }

        let infix: String = cmd_line.join(" "); //Since the args are split by spaces.

        let postfix_r = to_postfix(&infix, &ops);
        if let Err(t) = &postfix_r {
            println!("Error with input: \"{t}\"");
            return;
        }

        let postfix = postfix_r.unwrap();
        if print_postfix {
            println!("Postfix expression: {}", &postfix);
        }

        let eval_r = evaluate_postfix(&postfix, &ops);

        if let Err(t) = &eval_r {
            println!("Error in postfix: \"{t}\"");
            return; 
        }

        let eval = round_f64(eval_r.unwrap(), 3);
        println!("{eval}");
        return;
    }

    loop {
        print!("Please enter an expression in infix notation, or type \"exit\" to exit: ");
        if let Err(_) = stdout().flush() { return; }

        let mut command = String::new();
        if let Err(_) = stdin().read_line(&mut command) { return; }
        command = command.trim().to_string();

        if command.to_lowercase() == "exit" { return; }

        let postfix_r = to_postfix(&command, &ops);
        if let Err(t) = &postfix_r {
            println!("\tError with Expression: \"{t}\"");
            continue;
        }

        let postfix = postfix_r.unwrap();
        println!("Posftix expression: {}", &postfix);

        let eval_r = evaluate_postfix(&postfix, &ops);
        if let Err(t) = &eval_r {
            println!("\tError with postfix: \"{t}\"");
            continue;
        }

        let eval = round_f64(eval_r.unwrap(), 3);
        println!("Result: {eval}");
    }
    */
}
