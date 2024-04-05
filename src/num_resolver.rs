
pub fn round_f64(a: f64, precis: u32) -> f64
{
    let y = 10i32.pow(precis) as f64;
    (a * y).round() / y
}

pub fn get_number_digits_count(num: f64, precis: u32) -> u32 {
    let whole_nums = num.floor() as i32;
    let dec = round_f64(num - (whole_nums as f64), precis);

    let mut result = whole_nums.checked_ilog10().unwrap_or(0) + 1;
    if dec > 0.00f64 {
        result += 1; //for the decimal

        //There is at most precis digitis. We need to determine how many digits are after the period by multiplying the dec value by 10, and seeing if the result number still has digits. 

        let mut temp_dec = dec;
        while temp_dec > 0f64 {
            temp_dec *= 10f64;
            temp_dec -= temp_dec.floor();
            result += 1;
        }
    }

    result
}