pub fn count_bits(n: i64) -> i64 {
    let mut num = n as u64;
    let mut count = 0;

    while num != 0 {
        num &= num - 1;
        count += 1;
    }

    count
}
