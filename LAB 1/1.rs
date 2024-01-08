use std::env;

fn main() {
    let args: Vec<String> = env::args().collect();
    let mut count = 1;
    while count < args.len() {
        if !args[count].starts_with('-') {
            println!("{}", &args[count]);
        }
        count += 1;
    }
}
