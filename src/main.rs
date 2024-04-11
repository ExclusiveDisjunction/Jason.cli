
pub mod stack;
pub mod testing;
pub mod variables;
pub mod math_types;
pub mod ops;
pub mod functions;

/*
use std::io::{stdin as stdin, stdout as stdout, Write};

use operators::Operators;
use parsing::{to_postfix, evaluate_postfix};
use num_resolver::round_f64;
*/

use math_types::matrix::Matrix as Matrix;

fn main() {

    let mut mat = Matrix::new(4, 4);
    mat[0][0] = 4f64;
    mat[0][1] = 3f64;
    mat[0][2] = 1f64;
    mat[0][3] = 2f64;
    mat[1][0] = 33f64;
    mat[1][1] = 4f64;
    mat[1][2] = 1f64;
    mat[1][3] = -2f64;
    mat[2][0] = 5f64;
    mat[2][1] = -44.3f64;
    mat[2][2] = 2f64;
    mat[2][3] = 1f64;
    mat[3][0] = -33.1f64;
    mat[3][1] = -1.6f64;
    mat[3][2] = 2.6f64;
    mat[3][3] = 1f64;
    println!("{}", mat.to_string());

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
