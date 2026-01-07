#[repr(transparent)]
#[derive(Debug, Clone, Copy)]
pub struct Dial(i32);

impl Dial {}

const LEFT: char = 'L';
const RIGHT: char = 'R';

// #[repr(C)]
// #[derive(Debug, Clone, Copy)]
// pub struct Turn {
//     val: u8,
//     dir: Direction,
// }
//
// impl Direction {
//     pub const fn as_char(&self) -> char {
//         let n = *self as u8;
//         n as char
//     }
// }
// //
// impl Turn {
//     pub const fn get(&self) -> i32 {
//         self.inner() as i32
//     }
//
//     pub const fn inner(&self) -> u8 {
//         self.val
//     }
//
//     pub const fn direction(&self) -> Direction {
//         self.dir
//     }
// }

impl Dial {
    pub const MIN: i32 = 0;
    pub const MAX: i32 = 99;
    pub const START: i32 = 50;

    pub const fn start() -> Self {
        Self(Self::START)
    }

    pub const fn value(&self) -> i32 {
        self.0
    }

    pub fn turn_right(&mut self, n: i32) {
        let res = self.0 + n;
        if res > Self::MAX {
            let remainder = res - Self::MAX;
            self.0 = remainder;
        } else {
            self.0 = res;
        }
    }

    pub fn turn_left(&mut self, n: i32) {
        let res = self.0 - n;
        if res < Self::MIN {
            // res is definitely negative, so this addition is actually a subtraction
            self.0 = Self::MAX + res;
        } else {
            self.0 = res;
        }
    }

    pub const fn is_zero(&self) -> bool {
        self.value() == 0
    }
}

impl Default for Dial {
    fn default() -> Self {
        Self::start()
    }
}

#[repr(i32)]
#[derive(Debug, Clone, Copy)]
pub enum Turn {
    Left(i32),
    Right(i32),
}

mod test {
    use super::*;

    const INPUT: &str = include_str!("../../inputs/day1.txt");
    const INPUT_BYTES: &[u8] = INPUT.as_bytes();

    #[test]
    fn day_solution() -> anyhow::Result<()> {
        let mut dial = Dial::start();
        let mut count: u32 = 0;

        for entry in INPUT.split_whitespace() {
            println!("Dial at: {}", dial.value());
            let bytes = entry.as_bytes();
            let d = entry.as_bytes()[0] as char;
            match d {
                LEFT => {
                    let num_str = &entry[1..];
                    let n = num_str.parse::<i32>()?;
                    println!("Turning dial left: {n}");

                    let now = dial.0;
                    let next = now - n;
                }
                RIGHT => {
                    let num_str = &entry[1..];
                    let n = num_str.parse::<i32>()?;
                    println!("Turning dial right: {n}");
                    dial.turn_right(n);
                }
                _ => panic!("Invalid Input!, first character of input must be 'L' or 'R'"),
            }

            if dial.is_zero() {
                count += 1;
            }
            println!("Dial now at: {}", dial.value());
        }

        println!("{count}");
        Ok(())
    }
}
