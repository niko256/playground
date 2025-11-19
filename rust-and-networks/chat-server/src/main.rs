use std::collections::VecDeque;
use std::io::{Read, Write};
use std::net::TcpStream;
use std::thread::{self, JoinHandle};

fn handle_client(mut stream: TcpStream) {
    let client_addr = stream.peer_addr().unwrap();
    println!("New client {} has been connected!!!", client_addr);

    let mut buf = [0; 1024];

    loop {
        match stream.read(&mut buf) {
            Ok(0) => {
                println!("Client {} disconnected", client_addr);
                break;
            }
            Ok(n) => {
                let received = String::from_utf8_lossy(&buf[..n]);
                println!(
                    "Received from {}: '{}' ({} bytes)",
                    client_addr,
                    received.trim(),
                    n
                );

                if let Err(e) = stream.write_all(&buf[..n]) {
                    eprintln!("Write error to {}: {}", client_addr, e);
                    break;
                }
                stream.flush().unwrap();
                println!("Echoed back to {}: '{}'", client_addr, received.trim());
            }
            Err(e) => {
                eprintln!("Read error from {}: {}", client_addr, e);
                break;
            }
        }
    }
    println!("Client {} disconnected", client_addr);
}
fn main() -> std::io::Result<()> {
    let listener = std::net::TcpListener::bind("127.0.0.1:8080")?;
    let mut handles = VecDeque::new();

    println!("Sync echo server listening on 127.0.0.1:8080");

    // cleaning finished threads.
    handles.retain(|handle: &JoinHandle<()>| !handle.is_finished());

    for stream in listener.incoming() {
        match stream {
            Ok(stream) => {
                let handle = thread::spawn(move || {
                    handle_client(stream);
                });
                handles.push_back(handle);
            }

            Err(e) => {
                eprintln!("Failed to accept connection : {}", e);
            }
        }
        if handles.len() > 100 {
            if let Some(handle) = handles.pop_front() {
                let _ = handle.join();
            }
        }
    }
    Ok(())
}
