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

## Performance

For the cases I tested, ``fast_solve`` always completed in less than 0.1s. With some more optimization to the Python code, times of less than 0.05s should be easily achievable. If the same code was written in C++, less than 0.01s should be achievable on most machines. Usually no more than 4-6 guesses should be required to solve medium/hard puzzles and no more than 2-3 backtracks should be needed.

## Tests

I tested the sudoku solver uses some puzzles I found in my local newspaper and on this website.

https://www.researchgate.net/figure/Sudoku-puzzles-used-as-Test-Data_fig5_224113980
