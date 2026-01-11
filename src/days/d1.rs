mod test {
    use super::*;

    const INPUT: &str = include_str!("../../inputs/day1.txt");
    const INPUT_BYTES: &[u8] = INPUT.as_bytes();
    const L: char = 'L';
    const R: char = 'R';

    fn parse_entry_num(entry: &str) -> anyhow::Result<i32> {
        let s = &entry[1..];
        let n = s.parse::<i32>()?;
        Ok(n)
    }

    #[test]
    fn day_solution() -> anyhow::Result<()> {
        let mut dial: i32 = 0;
        let mut count: u32 = 0;

        for entry in INPUT.split_whitespace() {
            println!("Dial => {dial} :: Count => {count}");
            let c = entry.as_bytes()[0] as char;
            let num = parse_entry_num(entry)?;

            match c {
                L => {
                    println!("Turning dial left: {num}");
                    let num = -num;
                }
                R => {
                    println!("Turning dial right: {num}");
                }
                _ => panic!("Invalid Input!, first character of input must be 'L' or 'R'"),
            }
        }
        // // let mut dial = Dial::start();
        // let mut count: u32 = 0;
        //
        // for entry in INPUT.split_whitespace() {
        //     println!("Dial at: {}", dial.value());
        //     let bytes = entry.as_bytes();
        //     let d = entry.as_bytes()[0] as char;
        //     match d {
        //         LEFT => {
        //             let num_str = &entry[1..];
        //             let n = num_str.parse::<i32>()?;
        //             println!("Turning dial left: {n}");
        //
        //             let now = dial.0;
        //             let next = now - n;
        //         }
        //         RIGHT => {
        //             let num_str = &entry[1..];
        //             let n = num_str.parse::<i32>()?;
        //             println!("Turning dial right: {n}");
        //             dial.turn_right(n);
        //         }
        //         _ => panic!("Invalid Input!, first character of input must be 'L' or 'R'"),
        //     }
        //
        //     if dial.is_zero() {
        //         count += 1;
        //     }
        //     println!("Dial now at: {}", dial.value());
        // }

        // println!("{count}");
        Ok(())
    }
}
