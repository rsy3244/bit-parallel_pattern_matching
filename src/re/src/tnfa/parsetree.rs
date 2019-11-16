use std::iter::Iterator;
use std::boxed::Box;
use super::node;
use super::symbol;

struct ParseTree{
    root: Node,
}

impl ParseTree{
    fn new<I: Iterator>(&mut itr: I) -> ParseTree{
        ParseTree{
            root: Box::new(parser(itr)),
        }
    }

    fn parser<I: Iterator>(&mut itr: I) -> Node {
        let mut child = Vec::new();
        while let Some(c) = itr.next() {
            match c {
                '|' => child.push(term(c, itr)),
                ')' => break,
                _ => child.push(term(c, itr)),
    }

    fn concat(c: char, itr: I) -> Node {
        let mut child = Vec::new();
        child.push(term(c, itr));
        while let Some(c) = itr.next() {
            match c {
                '|' => child.push(term(itr.next(), itr)),
                ')' => break,
                _ => panic!("parser error");
            }
        }
        Node {
            label: Symbol::Concat,
            to: child,
        }
    }

            









