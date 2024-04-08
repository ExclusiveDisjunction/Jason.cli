#[cfg(test)]
pub mod tests {
    use crate::parsing::{to_postfix, evaluate_postfix};
    use crate::num_resolver::{round_f64, get_number_digits_count};
    use crate::operators::Operators as Operators;

    use crate::math_types::{matrix::Matrix as Matrix};

    fn expect_pass(input: &str, expected: f64) {
        let ops = Operators::new();
        let postfix = to_postfix(&input, &ops);

        println!("Is postfix ok? {}", postfix.is_ok());
        if postfix.is_err() {
            println!("\tError because of \"{}\".", &postfix.clone().err().unwrap());
        }
        else {
            println!("\tPostfix = \"{}\"", &postfix.clone().ok().unwrap());
        }
        assert!(postfix.is_ok());

        let postfix_ok = postfix.unwrap();
        let result = evaluate_postfix(&postfix_ok, &ops);

        println!("Is result ok? {}", result.is_ok());
        if result.is_err() {
            println!("\tError because of \"{}\"", &result.clone().err().unwrap());
        }
        else {
            println!("\tResult = \"{}\"", &result.clone().ok().unwrap());
        }
        assert!(result.is_ok());

        let val = round_f64(result.unwrap(), 3);
        let expected_r = round_f64(expected, 3);
        println!("Value = {val}, Expected = {expected_r}");
        assert_eq!(&val, &expected_r);
    }
    fn expect_fail(input: &str) {
        let ops = Operators::new();
        let postfix = to_postfix(&input, &ops);

        if postfix.is_ok() {
            println!("\tPostfix ok is ok... value = \"{}\"", postfix.clone().ok().unwrap());
        }
        assert!(postfix.is_err());
    }

    #[test]
    fn empty_string() {
        expect_pass("", 0.00f64);
    }
    #[test]
    fn plus_only() {
        expect_fail("+");
    }

    #[test]
    fn one_plus() {
        expect_fail("1+");
    }

    #[test]
    fn plus_one() {
        expect_fail("+1");
    }

    #[test]
    fn one_minus_plus_four() {
        expect_fail("1-+4");
    }

    #[test]
    fn mismatched_braces() {
        expect_fail("{7+4)");
    }

    #[test]
    fn no_operators() {
        expect_fail("3 4");
    }

    #[test]
    fn expr1() {
        expect_pass("2+3-5+2^2", 4.0f64);
    }

    #[test]
    fn equals_as_oper() {
        expect_fail("2=3");
    }

    #[test]
    fn expr2() {
        expect_pass("(2+3*(2+2))-4", 10.00f64);
    }

    #[test]
    fn expr3() {
        expect_pass("{2+3*(2+2)}-4", 10.00f64);
    }

    #[test]
    fn expr4() {
        expect_pass("(8-3)/(4+2)", (8f64-3f64)/(4f64+2f64));
    }

    #[test]
    fn expr5() {
        expect_pass("(28-3)/(4+2)", (28f64-3f64)/(4f64+2f64));
    }

    #[test]
    fn expr6() {
        expect_pass("{(6 * 5 - 16 + 61 % 4) ^ 2}", (6f64 * 5f64 - 16f64 + 61f64 % 4f64).powf(2f64));
    }

    #[test]
    fn f_expr1() {
        expect_pass("3.2 + 1.6", 4.8f64);
    }

    #[test]
    fn f_expr2() {
        expect_pass("(28.1-3)/(4.6+2.1)", (28.1f64-3f64)/(4.6f64+2.1f64));
    }

    #[test]
    fn constant_expr1() {
        expect_pass("c", (3i32 * 10i32.pow(8)) as f64);
    }

    #[test]
    fn test_count_digits() {
        assert_eq!(get_number_digits_count(1.6f64, 3), 3);
        assert_eq!(get_number_digits_count(16f64, 3), 2);
        assert_eq!(get_number_digits_count(0.16f64, 3), 4);
    }

    #[test]
    fn test_matrix_schematic() {
        /*
            [4 3 1 2]
            [33 4 1 -2]
            [5 -44.3 2 1]
            [-33.1 -1.5, 2.6 1]

            Which evaluates to:
            [(true, 4), (true, 4), (false, 3), (true, 1)]

            [ 4     3     1    2]
            [ 33    4     1   -2]
            [ 5    -44.3  2    1]
            [-33.1 -1.5   2.6  1]

         */
        let mut mat = Matrix::new(4, 4);
        mat[0][0] = 4f64;
        mat[0][1] = 3f64;
        mat[0][2] = 1f64;
        mat[0][3] = 2f64;
        mat[1][0] = 33f64;
        mat[1][1] = 4f64;
        mat[1][2] = 1f64;
        mat[1][3] = -2f64;
        mat[2][0] = 5f64;
        mat[2][1] = -44.3f64;
        mat[2][2] = 2f64;
        mat[2][3] = 1f64;
        mat[3][0] = -33.1f64;
        mat[3][1] = -1.6f64;
        mat[3][2] = 2.6f64;
        mat[3][3] = 1f64;
        println!("{}", mat.to_string());

        assert_eq!(mat.get_column_width_schematic(), vec![(true, 4), (true, 4), (false, 3), (true, 1)]);

    }
}