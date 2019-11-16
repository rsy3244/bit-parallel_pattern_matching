use std::Vec;
struct TNFA{
    type Data = u64;

    array : Vec<Data>;
}

impl TNFA{
    fn new(pattern : Vec<u8>) -> TNFA {
        let itr = pattern.iter();
        while let Some(c) = itr.next() {
            match c {
                '(' => ,
            }
        }
    }

    fn concat() -> 





