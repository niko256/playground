fn expand_around_center(s: &[u8], mut left: usize, mut right: usize) -> usize {
    let length = s.len();

    while left >= 0 && right < length && s[left] == s[right] {
        left -= 1;
        right += 1;
    }
    right - left - 1
}

fn longest_palindrome_indices(s: &[u8]) -> (usize, usize) {
    let length = s.len();
    let mut max_pldr_len = 0;
    let mut start_idx = 0;

    for center in 0..length {
        let odd = expand_around_center(s, center, center);
        let even = expand_around_center(s, center, center + 1);

        let local_max_len = std::cmp::max(odd, even);

        if local_max_len > max_pldr_len {
            max_pldr_len = local_max_len;
            start_idx = start_index(center, local_max_len);
        }
    }
    (start_idx, max_pldr_len)
}

fn start_index(center: usize, local_len: usize) -> usize {
    center.saturating_sub((local_len - 1) / 2)
}

fn longest_palindrome_substr(s: impl AsRef<str>) -> String {
    let s = s.as_ref();

    if s.len() <= 1 {
        return s.to_string();
    }

    let (start, end) = longest_palindrome_indices(s.as_bytes());

    s[start..start + end].to_string()
}
