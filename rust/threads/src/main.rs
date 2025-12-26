use std::thread;
use std::time::Instant;
use tokio::task;

fn sum_to_n(n: u64) -> u64 {
    let mut sum = 0;
    for i in 1..=n {
        sum += i;
    }
    sum
}

fn single_thread_computation(n: u64) -> (u64, f64) {
    println!("Single-threaded approach...");

    let start = Instant::now();

    let mut total_sum: u64 = 0;

    for i in 1..=n {
        total_sum += sum_to_n(i);
    }

    let duration = start.elapsed().as_secs_f64();

    println!("Total sum : {}", total_sum);
    println!("Execution time : {:.4} seconds", duration);

    (total_sum, duration)
}

fn parallel_computation(max_num: u64) -> (u64, u64, u64, f64) {
    println!("Multithreaded approach...");
    let start = Instant::now();

    let handle_odd = thread::spawn(move || {
        let mut total_odd: u64 = 0;

        // skip even numbers
        for n in (1..=max_num).step_by(2) {
            total_odd += sum_to_n(n);
        }
        total_odd
    });

    let handle_even = thread::spawn(move || {
        let mut total_even: u64 = 0;

        // skip odd numbers
        for n in (2..=max_num).step_by(2) {
            total_even += sum_to_n(n);
        }
        total_even
    });

    let sum_odd = handle_odd
        .join()
        .expect("Error in computation the sum of odd numbers");
    let sum_even = handle_even
        .join()
        .expect("Error in computation the sum of even numbers");

    let total_sum = sum_odd + sum_even;

    let duration = start.elapsed().as_secs_f64();

    println!("Sum of odd numers : {}", sum_odd);
    println!("Sum of even numbers : {}", sum_even);

    println!("Total sum : {}", total_sum);
    println!("Duration : {:.4} seconds", duration);

    (sum_odd, sum_even, total_sum, duration)
}

async fn async_computation(max_num: u64) -> (u64, u64, u64, f64) {
    println!("Async approach with Tokio runtime...");

    let start = Instant::now();

    let task_odd = task::spawn(async move {
        let mut total_odd: u64 = 0;

        for n in (1..=max_num).step_by(2) {
            total_odd += sum_to_n(n);
        }
        total_odd
    });

    let task_even = task::spawn(async move {
        let mut total_even: u64 = 0;

        for n in (2..=max_num).step_by(2) {
            total_even += sum_to_n(n);
        }
        total_even
    });

    let sum_odd = task_odd.await.unwrap();
    let sum_even = task_even.await.unwrap();

    let total_sum = sum_odd + sum_even;

    let suration = start.elapsed().as_secs_f64();

    (sum_odd, sum_even, total_sum, suration)
}

#[tokio::main]
async fn main() {
    const MAX_NUM: u64 = 100_000;

    let (single_sum, single_time) = single_thread_computation(MAX_NUM);

    let (odd_sum, even_sum, multithr_total_sum, mult_duration) = parallel_computation(MAX_NUM);

    let (odd_sum, even_sum, async_total_sum, async_duration) = async_computation(MAX_NUM).await;

    println!("Cheeeck...");
    assert_eq!(single_sum, multithr_total_sum);
    assert_eq!(single_sum, async_total_sum);

    println!("Duration...");
    println!("Single-threaded approach : {:.4} seconds", single_time);

    println!("Multi-threaded approach : {:.4} seconds", mult_duration);
    println!("Async approach : {:.4} seconds", async_duration);

    println!("Even sum : {}", even_sum);
    println!("Odd sum : {}", odd_sum);
}
