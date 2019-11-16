use std::vec::Vec;
#[cfg(test)]
mod tests {
    #[test]
    fn it_works() {
        let arr = vec![1,2,3,4,5,6,7,8];
        let mut tmp = Vec::new();
        let mut itr = arr.iter();
        for i in tst(*itr.next().unwrap(), itr) {
            println!("{}:{}",i.0,i.1);
        }
    }

    fn tst<I: Iterator>(c: usize, itr: I) -> Vec<(usize, usize)> {
        let mut ret = Vec::new();
        while let Some(i) = itr.next() {
            ret.push((c, i));
            ret.push((itr.next().unwrap_or(0), i));
        }
    }


            
}
