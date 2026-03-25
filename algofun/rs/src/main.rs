mod problems;
mod utils;

/* +---+---+---+---+---+---+---+---+---+---+---+ */

fn main() {
    let n = 12545456561;

    let res = problems::count_1s_in_number::count_bits(n);

    println!("result for {} is {}", n, res);
}
