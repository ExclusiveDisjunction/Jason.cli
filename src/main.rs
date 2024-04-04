
pub mod stack;
pub mod operators;
pub mod parsing;
pub mod testing;
//pub mod variables;

use std::io::{stdin as stdin, stdout as stdout, Write};

use operators::Operators;
use parsing::{to_postfix, evaluate_postfix, round_f64};

fn main() {
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
}
