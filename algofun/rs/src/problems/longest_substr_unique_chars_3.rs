use std::collections::HashMap;

pub fn longest_substr_with_unique_characters(string: impl AsRef<str>) -> usize {
    let mut max_len = 0;
    let mut left = 0;
    let mut freq = HashMap::new();
    let string = string.as_ref();

    for (right, ch) in string.chars().enumerate() {
        *freq.entry(ch).or_insert(0) += 1;

        while freq[&ch] > 1 {
            let left_ch = string.chars().nth(left).unwrap();
            freq.entry(left_ch).and_modify(|count| *count -= 1);
            left += 1;
        }

        max_len = max_len.max(right - left + 1);
    }

    max_len
}
