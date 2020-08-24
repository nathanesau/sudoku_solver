// re-write fast_solve.py in C++ for performance comparison
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

using namespace std;

using board_t = vector<vector<vector<int>>>;

// testing function
void print_board(const board_t &board)
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (board[i][j].size() == 1)
            {
                cout << board[i][j][0] << " ";
            }
            else
            {
                cout << "X"
                     << " ";
            }
        }
        cout << endl;
    }
    cout << endl;
}

struct guess_t
{
    guess_t(pair<int, int> rcb, pair<int, int> sq, vector<int> opt)
        : rcb(rcb), sq(sq), opt(opt) {}

    guess_t(){};

    pair<int, int> rcb;
    pair<int, int> sq;
    vector<int> opt;
};

struct node_t
{
    node_t(const board_t &board, node_t *parent, int depth, const guess_t &guess)
        : board(board), parent(parent), child(nullptr), depth(depth), guess(guess)
    {
    }

    board_t board;
    node_t *parent;
    node_t *child;
    int depth;
    guess_t guess;
};

vector<int> get_pvalues(const board_t &board, int row, int col)
{

    vector<int> pvals = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int tl_row = row - row % 3;
    int tl_col = col - col % 3;
    for (int i = 0; i < 9; i++)
    {
        if (board[row][i].size() == 1)
        {
            int value = board[row][i][0];
            pvals.erase(remove(pvals.begin(), pvals.end(), value), pvals.end());
        }

        if (board[i][col].size() == 1)
        {
            int value = board[i][col][0];
            pvals.erase(remove(pvals.begin(), pvals.end(), value), pvals.end());
        }

        if (board[tl_row + i / 3][tl_col + i % 3].size() == 1)
        {
            int value = board[tl_row + i / 3][tl_col + i % 3][0];
            pvals.erase(remove(pvals.begin(), pvals.end(), value), pvals.end());
        }
    }
    return pvals;
}

void fill_missing_entries(board_t &board, int b)
{
    pair<int, int> rse = (b < 3) ? make_pair(0, 2) : (b < 6) ? make_pair(3, 5) : make_pair(6, 8);
    pair<int, int> cse = (b % 3 == 0) ? make_pair(0, 2) : (b % 3 == 1) ? make_pair(3, 5) : make_pair(6, 8);
    for (int row = rse.first; row < rse.second + 1; row++)
    {
        for (int col = cse.first; col < cse.second + 1; col++)
        {
            if (board[row][col].size() == 1) // entry already solved
                continue;
            board[row][col] = get_pvalues(board, row, col);
        }
    }
}

void solve_missing_entries(board_t &board, int b)
{
    pair<int, int> rse = (b < 3) ? make_pair(0, 2) : (b < 6) ? make_pair(3, 5) : make_pair(6, 8);
    pair<int, int> cse = (b % 3 == 0) ? make_pair(0, 2) : (b % 3 == 1) ? make_pair(3, 5) : make_pair(6, 8);
    vector<vector<pair<int, int>>> psquares = {{}, {}, {}, {}, {}, {}, {}, {}, {}};
    for (int row = rse.first; row < rse.second + 1; row++)
    {
        for (int col = cse.first; col < cse.second + 1; col++)
        {
            for (auto e : board[row][col])
            {
                psquares[e - 1].push_back(make_pair(row, col));
            }
        }
    }
    for (int i = 0; i < 9; i++)
    {
        if (psquares[i].size() == 1)
        {
            pair<int, int> rc = psquares[i][0];
            if (board[rc.first][rc.second].size() != 1) // solve entry
            {
                board[rc.first][rc.second] = {i + 1};
            }
        }
    }
}

// 0: solved, 1: stuck, 2: failed
int solve_strategy(board_t &board)
{
    // 3: continue
    auto get_state = [](const board_t &board) {
        bool solved = true;
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                if (board[i][j].empty())
                    return 2; // failed
                if (board[i][j].size() != 1)
                    solved = false;
            }
        }
        return (solved) ? 0 : 3;
    };

    for (int i = 0; i < 25; i++) // max iter = 25
    {
        board_t initial_board = board; // copy
        for (int b = 0; b < 9; b++)
        {
            fill_missing_entries(board, b);
            solve_missing_entries(board, b);
        }
        if (board == initial_board)
            return 1; // stuck
        int state = get_state(board);
        if (state == 0 || state == 2) // solved or failed
            return state;
    }
    // should not happen
    return -1;
}

guess_t get_guess(const board_t &board)
{
    vector<int> rsc = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    vector<int> csc = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    vector<int> bsc = {0, 0, 0, 0, 0, 0, 0, 0, 0};

    // 0: row, 1: col, 2: box
    auto max_index = [&rsc, &csc, &bsc](const board_t &board) {
        pair<int, int> max_rsc, max_csc, max_bsc = make_pair(0, 0);
        for (int i = 0; i < 9; i++)
        {
            if (rsc[i] > max_rsc.second)
                max_rsc = make_pair(i, rsc[i]);
            if (csc[i] > max_csc.second)
                max_csc = make_pair(i, csc[i]);
            if (bsc[i] > max_bsc.second)
                max_bsc = make_pair(i, bsc[i]);
        }
        if (max_rsc.second >= max_csc.second && max_rsc.second >= max_bsc.second)
            return make_pair(0, max_rsc.first);
        else if (max_csc.second >= max_bsc.second)
            return make_pair(1, max_csc.first);
        return make_pair(2, max_bsc.first);
    };

    for (int i = 0; i < 9; i++)
    {
        int rc = 0;
        int cc = 0;
        int bc = 0;
        for (int j = 0; j < 9; j++)
        {
            if (board[i][j].size() == 1)
                rc += 1;
            if (board[i][j].size() == 1)
                cc += 1;
            if (board[i / 3 * 3 + j / 3][i % 3 * 3 + j % 3].size() == 1)
                bc += 1;
        }
        if (rc < 9)
            rsc[i] = rc;
        if (cc < 9)
            csc[i] = cc;
        if (bc < 9)
            bsc[i] = bc;
    }

    pair<int, int> rcb = max_index(board);
    pair<int, int> square = make_pair(0, 0);
    vector<int> options = {};

    int t = rcb.first;
    int i = rcb.second;
    for (int j = 0; j < 9; j++)
    {
        if (t == 0 && board[i][j].size() > 1)
        {
            square = make_pair(i, j);
            options = board[i][j];
            break;
        }
        if (t == 1 && board[j][i].size() > 1)
        {
            square = make_pair(j, i);
            options = board[j][i];
            break;
        }
        if (t == 2 && board[i / 3 * 3 + j / 3][i % 3 * 3 + j % 3].size() > 1)
        {
            square = make_pair(i / 3 * 3 + j / 3, i % 3 * 3 + j % 3);
            options = board[i / 3 * 3 + j / 3][i % 3 * 3 + j % 3];
            break;
        }
    }

    return guess_t(rcb, square, options);
}

void apply_guess(board_t &board, const guess_t &guess, int val)
{
    int row = guess.sq.first;
    int col = guess.sq.second;
    board[row][col] = {val};
}

void free_memory(node_t *bottom)
{
    node_t *current = bottom;
    while (current->parent)
    {
        node_t *parent = current->parent;
        delete parent->child;
        parent->child = nullptr;
        current = parent;
    }
}

board_t solve(const board_t &initial_board)
{
    node_t root(initial_board, nullptr, 0, guess_t());
    node_t *current = &root;
    while (true)
    {
        int state = solve_strategy(current->board);
        if (state == 0) // solved
        {
            board_t solved_board = current->board;
            free_memory(current);
            return solved_board;
        }
        if (state == 1) // stuck
        {
            if (current->child)
            {
                // rare case - didn't occur for 1,000,000 kaggle puzzles
                // occurs for very very hard (unsolvable by human puzzles)
                // go back up the tree!
                delete current->child;
            }
            current->child = new node_t(current->board, current, current->depth + 1, guess_t());
            current = current->child;
            current->guess = get_guess(current->board);
            int value = current->guess.opt[0];
            apply_guess(current->board, current->guess, value);
        }
        if (state == 2) // failed - backtrack and change guess
        {
            while (current->guess.opt.size() <= 1)
            {
                // rare case - didn't occur for 1,000,000 kaggle puzzles
                // occurs for very very hard (unsolvable by human puzzles)
                // go back up the tree!
                current = current->parent;
            }
            current->board = current->parent->board;
            current->guess.opt.erase(current->guess.opt.begin());
            int value = current->guess.opt[0];
            apply_guess(current->board, current->guess, value);
        }
    }
}

void read_board_line(vector<board_t> &board_list, const string &line)
{
    board_t board = {{{}, {}, {}, {}, {}, {}, {}, {}, {}},
                     {{}, {}, {}, {}, {}, {}, {}, {}, {}},
                     {{}, {}, {}, {}, {}, {}, {}, {}, {}},
                     {{}, {}, {}, {}, {}, {}, {}, {}, {}},
                     {{}, {}, {}, {}, {}, {}, {}, {}, {}},
                     {{}, {}, {}, {}, {}, {}, {}, {}, {}},
                     {{}, {}, {}, {}, {}, {}, {}, {}, {}},
                     {{}, {}, {}, {}, {}, {}, {}, {}, {}},
                     {{}, {}, {}, {}, {}, {}, {}, {}, {}}};

    for (int i = 0; i < 81; i++)
    {
        int value = int(line[i]) - 48;
        if (value != 0)
            board[i / 9][i % 9] = {value};
    }

    board_list.push_back(board);
}

void load_cases(vector<board_t> &board_list, string file_name)
{
    ifstream fstream(file_name);
    string line;
    getline(fstream, line); // skip first line
    while (getline(fstream, line))
    {
        read_board_line(board_list, line);
    }
}

// https://www.kaggle.com/bryanpark/sudoku
void test_kaggle()
{
    vector<board_t> board_list;
    load_cases(board_list, "sudoku.csv");

    auto start_time = chrono::high_resolution_clock::now();
    for (auto &initial_board : board_list)
    {
        board_t solved_board = solve(initial_board);
    }
    auto end_time = chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;
    std::cout << "Elapsed time: " << elapsed.count() << " s\n";
}

// https://attractivechaos.wordpress.com/2011/06/19/an-incomplete-review-of-sudoku-solver-implementations/
void test_extreme_cases()
{
    vector<board_t> board_list;
    load_cases(board_list, "extreme.csv");

    auto start_time = chrono::high_resolution_clock::now();
    for (int i = 0; i < 50; i++)
    {
        for (auto &initial_board : board_list)
        {
            board_t solved_board = solve(initial_board);
        }
    }
    auto end_time = chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;
    std::cout << "Elapsed time: " << elapsed.count() << " s\n";
}

// testing code
int main()
{
    test_extreme_cases();

    return 0;
}