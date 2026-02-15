use rayon::prelude::*;
use std::thread;
use std::time::Instant;
use tokio::task;

/// Calculates the sum of integers from 1 to n using the arithmetic progression formula: O(1).
/// Previous implementation was O(n).
fn sum_to_n(n: u64) -> u64 {
    n * (n + 1) / 2
}

fn single_thread_computation(n: u64) -> (u64, f64) {
    println!("Single-threaded approach...");
    let start = Instant::now();

    let total_sum: u64 = (1..=n).map(sum_to_n).sum();

    let duration = start.elapsed().as_secs_f64();
    println!("Total sum : {}", total_sum);
    println!("Execution time : {:.4} seconds", duration);

    (total_sum, duration)
}

fn parallel_computation(max_num: u64) -> (u64, u64, u64, f64) {
    println!("Multithreaded approach (std::thread)...");
    let start = Instant::now();

    let handle_odd = thread::spawn(move || (1..=max_num).step_by(2).map(sum_to_n).sum::<u64>());
    let handle_even = thread::spawn(move || (2..=max_num).step_by(2).map(sum_to_n).sum::<u64>());

    let sum_odd = handle_odd
        .join()
        .expect("Error computing the sum of odd numbers");
    let sum_even = handle_even
        .join()
        .expect("Error computing the sum of even numbers");

    let total_sum = sum_odd + sum_even;
    let duration = start.elapsed().as_secs_f64();

    println!("Sum of odd numbers : {}", sum_odd);
    println!("Sum of even numbers : {}", sum_even);
    println!("Total sum : {}", total_sum);
    println!("Duration : {:.4} seconds", duration);

    (sum_odd, sum_even, total_sum, duration)
}

/// Uses Rayon for data-parallelism.
fn rayon_parallel_computation(max_num: u64) -> (u64, f64) {
    println!("Rayon parallel approach...");
    let start = Instant::now();

    let total_sum: u64 = (1..=max_num).into_par_iter().map(sum_to_n).sum();

    let duration = start.elapsed().as_secs_f64();
    println!("Total sum : {}", total_sum);
    println!("Duration : {:.4} seconds", duration);

    (total_sum, duration)
}

async fn async_computation(max_num: u64) -> (u64, u64, u64, f64) {
    println!("Async approach with Tokio runtime (spawn_blocking)...");
    let start = Instant::now();

    // Use spawn_blocking for CPU-bound tasks to avoid blocking the async executor.
    let task_odd = task::spawn_blocking(move || (1..=max_num).step_by(2).map(sum_to_n).sum::<u64>());
    let task_even = task::spawn_blocking(move || (2..=max_num).step_by(2).map(sum_to_n).sum::<u64>());

    let sum_odd = task_odd.await.expect("Task odd failed");
    let sum_even = task_even.await.expect("Task even failed");

    let total_sum = sum_odd + sum_even;
    let duration = start.elapsed().as_secs_f64();

    (sum_odd, sum_even, total_sum, duration)
}

#[tokio::main]
async fn main() {
    const MAX_NUM: u64 = 100_000;

    let (single_sum, single_time) = single_thread_computation(MAX_NUM);
    let (_odd_std, _even_std, multithr_total_sum, mult_duration) = parallel_computation(MAX_NUM);
    let (rayon_sum, rayon_duration) = rayon_parallel_computation(MAX_NUM);
    let (odd_async, even_async, async_total_sum, async_duration) = async_computation(MAX_NUM).await;

    println!("\nVerification...");
    assert_eq!(single_sum, multithr_total_sum);
    assert_eq!(single_sum, async_total_sum);
    assert_eq!(single_sum, rayon_sum);
    println!("All results match!");

    println!("\nFinal Comparison:");
    println!("Single-threaded   : {:.6} seconds", single_time);
    println!("std::thread       : {:.6} seconds", mult_duration);
    println!("Rayon (Parallel)  : {:.6} seconds", rayon_duration);
    println!("Tokio (Blocking)  : {:.6} seconds", async_duration);

    println!("\nPartial Results (Async):");
    println!("Even sum : {}", even_async);
    println!("Odd sum  : {}", odd_async);
}
