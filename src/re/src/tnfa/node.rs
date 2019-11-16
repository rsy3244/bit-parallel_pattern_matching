use super::symbol;

struct Node{
    label: Symbol,
    to: Vec<Box<Node>>,
}

impl Node{
    fn new(label: Symbol) -> Node{
        Node{
            label,
            to: Vec::new(),
        }
    }

    fn insert(&mut self, child: Box<Node>) {
        self.to.push(child);
    }
}
