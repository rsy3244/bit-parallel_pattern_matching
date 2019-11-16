enum Symbol{
    Atom(char),
    Concat,
    Union,
    Wildcard,
    Optional(char),
    Repeat(char),
    BoundedRepeat(char, usize),
}
