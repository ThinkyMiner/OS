use std::fs::File;
use std::io::{self, Read, Write};

fn main() -> io::Result<()> {
    let file_name = "file.txt";
    let file = File::open(file_name)?;

    let mut buffer = String::new();

    file.take(u64::MAX as u64).read_to_string(&mut buffer)?;

    let mut output = String::new();

    for c in buffer.chars() {
        if c.is_lowercase() {
            output.push(c.to_ascii_uppercase());
        } else if c.is_uppercase() {
            output.push(c.to_ascii_lowercase());
        } else {
            output.push(c);
        }
    }

    io::stdout().write_all(output.as_bytes())?;

    Ok(())
}

