# PTE - Parallel Programming II. - Homework 2.
Find solutions for 21 peg puzzle

The puzzle has 21 holes Like in the figure. Initially every hole is filled with a peg, except for the
center hole. Pegs are moved and removed by doing a hopping. A hop is allowed in a straight line
(horizontal, vertical or diagonal) from its hole over an occupied hole to an empty hole on the other
side. The peg is removed from the occupied hole. The object of the puzzle is to remove all pegs
until only one peg remains in the center hole. (Theoretically 19 moves – hops – is necessary to
reduce the original 20 pegs to a single peg
Write a program to find a solution to this puzzle.

```
 - | X | X | X | - 
 X | X | X | X | X 
 X | X | O | X | X
 X | X | X | X | X
 - | X | X | X | -
```

  X = peg

  O = empty hole

  - = space that cannot be used.