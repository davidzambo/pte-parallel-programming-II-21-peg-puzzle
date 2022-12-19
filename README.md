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
  | 0 | 1 | 2 | 3 | 4
0 | - | X | X | X | - 
1 | X | X | X | X | X 
2 | X | X | O | X | X
3 | X | X | X | X | X
4 | - | X | X | X | -
```

  X = peg

  O = empty hole

  - = space that cannot be used.

```
  1 |    |    |    |  5 
  x |    |    |    |    
    |  x |    |    |   
    |    |    |    |    
 21 |    |    |    | 25
```
4->12 | 
20->18 | 
10->20 | 
2->14 | x
20->18 | x
22->14 | x
24->22 | x
6->18 | x
22->12 | x
19->17 | x
3->15 | x
17->9 | +
16->6 | +
15->13 | +
13->11 | +
11->3 | +
9->7 | +
6->8 |  +
3->13 | +