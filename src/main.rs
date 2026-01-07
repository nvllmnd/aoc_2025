use std::ops::{Add, AddAssign, Sub, SubAssign};

use anyhow::bail;

const PRACTICE_INPUT: &str = "L68
L30
R48
L5
R60
L55
L1
L99
R14
L82
";

const PRACTICE_EXPECTED: i32 = 3;
const PRACTICE_DIAL_START: i32 = 50;

#[repr(transparent)]
#[derive(Debug, Clone, Copy)]
struct Dial(i32);

impl Dial {
    pub const MIN: i32 = 0;
    pub const MAX: i32 = 99;

    pub const START: i32 = 50;

    pub const fn new() -> Self {
        Self(Self::START)
    }
}

impl Add<i32> for Dial {
    type Output = Self;

    fn add(self, rhs: i32) -> Self::Output {
        let n = self.0;
        let next = n + rhs;
        if next > Self::MAX {
            let diff = next - Self::MAX;
            Self(diff)
        } else {
            Self(next)
        }
    }
}

impl Sub<i32> for Dial {
    type Output = Self;

    fn sub(self, rhs: i32) -> Self::Output {
        let n = self.0;
        let next = n - rhs;
        if next < 0 {
            Self(next + Self::MAX + 1)
        } else {
            Self(next)
        }
    }
}

impl AddAssign<i32> for Dial {
    fn add_assign(&mut self, rhs: i32) {
        *self = *self + rhs;
    }
}

impl SubAssign<i32> for Dial {
    fn sub_assign(&mut self, rhs: i32) {
        *self = *self - rhs;
    }
}

fn tick_right(dial: Dial, n: i32) -> Dial {
    assert!(n >= 0, "cannot tick dial with a negative number!");
    let curr = dial.0;

    let rem = n % Dial::MAX;
    let next = rem + curr - 1;

    if next > Dial::MAX {
        let next = next - Dial::MAX;
        println!(
            "Ticking dial Right =>from: {} to {}, by {}",RA
            curr, next, rem
        );
        return Dial(next);
    }

    println!(
        "Ticking dial Right => from: {} to: {}, by: {}",
        curr, next, rem
    );

    Dial(next)
}

fn tick_left(dial: Dial, n: i32) -> Dial {
    assert!(n >= 0, "cannot tick dial with a negative number!");
    let curr = dial.0;

    let rem = -n % Dial::MAX;

    let next = rem + curr;

    if next < Dial::MIN {
        let next = Dial::MAX + next + 1;
        println!(
            "Ticking dial Left => from: {} to {}, by {}",
            curr, next, rem
        );
        return Dial(next);
    }

    println!(
        "Ticking dial Left => from: {} to: {}, by: {}",
        curr, next, rem
    );

    Dial(next)
}

fn main() -> anyhow::Result<()> {
    let mut dial = Dial::new();
    let mut count = 0;
    for entry in PRACTICE_INPUT.split_whitespace() {
        let dir = entry.as_bytes()[0] as char;
        let n = &entry[1..];
        let n = n.parse::<i32>()?;
        match dir {
            'L' => {
                dial = tick_left(dial, n);
            }
            'R' => {
                dial = tick_right(dial, n);
            }
            _ => {
                bail!(
                    "Invalid Direction input!, got: '{}', expected: 'L' or 'R'",
                    dir,
                );
            }
        }

        if dial.0 == 0 {
            count += 1;
        }
    }
    println!("Count: {count}");
    Ok(())
}
