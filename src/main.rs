
pub mod stack;
pub mod testing;
pub mod variables;
pub mod math_types;
pub mod ops;
pub mod functions;

use std::io::{stdin as stdin, stdout as stdout, Write};

use variables::variable_storage::VarStorage;
use ops::operators::Operators;
use ops::parsing::Expression;
use log::{debug, info, warn, LevelFilter};


fn main() -> Result<(), String> {
    if simple_logging::log_to_file("run.log", LevelFilter::Info).is_err() {
        return Err(String::from("Could not start logger."));
    }
    info!("Starting up calc-cmd");

    debug!("Indexing variables...");
    let storage_r = VarStorage::new("run_variables/");
    if storage_r.is_none() {
        debug!("Indexing could not be completed because the storage could not be loaded.");
        return Err(String::from("Storage could not be loaded!"));
    }
    let mut storage = storage_r.unwrap();
    if !storage.index_variables() {
        warn!("Variables could not be indexed, asking user for their choice...");

        print!("Variables could not be indexed. Operating without indexing could cause undefined behavior. Quit program? (Y/n) ");
        stdout().flush().expect("Could not flush!");
        let mut choice = String::new();
        stdin().read_line(&mut choice).expect("Could not resolve input.");

        if choice.trim().to_lowercase() == "y" {
            warn!("User chose to not have indexing.");
            println!("Continuing without indexing, all previous variable values (including ans) are not loaded. Saving of variables is also disabled.");
        }
        else {
            warn!("User chose to exit.");
            println!("Goodbye!");
            return Ok(());
        }
    }

    info!("All storage loaded.");
    debug!("Loading operators.");
    let ops = Operators::new();

    let mut input_expr = String::new();
    print!("Please enter an expression: ");
    stdout().flush().expect("Could not flush!");
    stdin().read_line(&mut input_expr).expect("Could not read!");

    println!("Ans is {}", storage.get_ans());

    let expr = Expression::from_infix(input_expr.trim(), &ops);
    if let Ok(e) = expr {
        let result = e.evaluate(&storage);
        println!("Result is {}", result);
        storage.set_ans(result);
    }
    else {
        println!("{}", expr.unwrap_err());
    
    }

    Ok(())

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
