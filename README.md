# sudoku_solver

Fast sudoku solver written in python

## Algorithm

See ``fast_solve.py``. This is the algorithm I figured out. You can probably find similar strategies online.

### Solve Strategy (can fully solve most easy puzzles - for medium/hard puzzles see backtracking section)

I was inspired by this approach:

![](https://www.logicgamesonline.com/sudoku/images/sudoku.1.png)

Humans will often keep track of possibilities for unsolved squares using small numbers like shown in the pictures. The computer uses a similar approach and by process of elimination it can fill in some missing numbers. The function for this is ``solve_strategy``.

### Backtracking

When making a guess, I make a guess in a row/col/box which is almost solved (typically, with 7 out of 9 solved). The function for this is ``get_guess``. 

Typically, we are making a guess between two possible numbers (not always but usually). We apply our guess, solve as far as we can. If we get stuck again, make another guess. If the solve fails, this means our guess was invalid in which case we go back to the previous guess we made, change and apply it and keep going as far as we can. See ``solve`` function.

## Tests

I tested the sudoku solver uses some puzzles I found in my local newspaper and on this website.

https://www.researchgate.net/figure/Sudoku-puzzles-used-as-Test-Data_fig5_224113980
