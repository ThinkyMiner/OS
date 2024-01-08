use std::io::{self, Read, Write};

fn main() {
    let mut buffer = [0; 1];  
    let mut non_alpha_count = 0;

    loop {
        match io::stdin().read_exact(&mut buffer) {
            Ok(_) => {
                let ch = buffer[0] as char;
                
                if !ch.is_ascii_alphabetic() {
                    non_alpha_count += 1;
                }

                io::stdout().write_all(&buffer).expect("Failed to write to stdout");
            }
            Err(_) => break, 
        }
    }
    println!("\nNumber of non-alphabetic characters: {}", non_alpha_count);
}
