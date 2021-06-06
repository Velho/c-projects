/*
Briefly, a program would solve a puzzle by placing the digit "1" in
the first cell and checking if it is allowed to be there.

If there are no violations (checking row, column, and box constraints)
then the algorithm advances to the next cell and places a "1" in that cell.

When checking for violations, if it is discovered that the "1" is not allowed,
the value is advanced to "2".

If a cell is discovered where none of the 9 digits is allowed,
then the algorithm leaves that cell blank and moves back to the previous cell.

The value in that cell is then incremented by one.
This is repeated until the allowed value in the last (81st) cell is discovered.
*/

// TODO rename folder to sudoku

// first version prolly will not be ideal
// but hope I get this to work

// arrays starting from 0 and positions starting at 1
// when thinking in math terms is goind to be a problem :)

// prolly need to comment alot cos cos of all the spagetti

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include <assert.h>

#define SD_EMPTY_CELL    -1

enum sd_status {
	sd_inv_col = 0x1,
	sd_inv_row = 0x2,
	sd_inv_box = 0x4,

	sq_invalid = sd_inv_col | sd_inv_row | sd_inv_box,

	// sd_not_appl, //< Value not applicable
};

//
// Keeps the field state.
// Used in the solver algo. Perhaps better name would be field, cell?
//
struct position
{
	int row;
	int col;

	int value;
	int status;
};

//
// Global structure for position memory on stack.
//
struct position sd_tree[81] = { 0 };

//
// simple print for 9x9 sudoku
//
void print_sudoku(int sudoku[9][9])
{
	for (size_t i = 0; i < 9; i++)
	{
		for (size_t j = 0; j < 9; j++)
		{
			if (j == 3 || j == 6)
				printf("   ");
			printf("%d ", sudoku[i][j]);
		}
		printf("\n");
		if (i == 2 || i == 5)
			printf("\n");
	}
	printf("\n");
}

//
// finds next empty position in 9x9 sudoku
//
struct position* find_next(int sudoku[9][9])
{
	int row, col;
	struct position* p;

	for (p = sd_tree; p != NULL; p++)
	{
		if (p->value == 0)
			return p;
	}

	return NULL;
}

//
// Increase the value and advance.
//
struct position* advance_position(struct position* p, int value)
{
	p->value = value;
	return ++p;
}

//
// test if guess is found on row
// No duplicates based on the rules.
//
int check_row(int sudoku[9][9], struct position* p, int guess)
{
	for (size_t i = 0; i < 9; i++)
	{
		if (sudoku[p->row][i] == guess)
		{
			p->status |= sd_inv_row;
			return 0;
		}
	}
	return 1;
}

//
// test if guess is found on col
// No duplicates based on the rules.
// \param sudoku Game state
//
int check_col(int sudoku[9][9], struct position* p, int guess)
{
	for (size_t i = 0; i < 9; i++)
	{
		int value = sudoku[i][p->col];
		if (value == guess)
		{
			p->status |= sd_inv_col;
			return 0;
		}
	}
	return 1;
}

//
// test if guess is found on box.
// TODO : FIX ME
//
int check_box(int sudoku[9][9], struct position* p, int guess)
{
	int row_start = p->row / 3 * 3;
	int col_start = p->col / 3 * 3;

	for (size_t row = row_start; row < 3; row++)
	{
		for (size_t col = col_start; col < 3; col++)
		{
			if (sudoku[row][col] == guess)
			{
				p->status |= sd_inv_box;
				return 0;
			}
		}
	}

	return 1;
}

//
// Values is advanced if there's no violations.
//
int set_guess(int** /* UNUSED */ sudoku, struct position* p)
{
	p->status = 0; // Clear the status flag.
	int val = ((p->value)++) + 1; // Increment the value as well.

	if (val > 9 || val == SD_EMPTY_CELL)
		val = 1;

	return val;
}



//
// combined guess function
//
int is_valid(int sudoku[9][9], struct position* p, int guess)
{
	if (check_row(sudoku, p, guess))
	{
		if (check_col(sudoku, p, guess))
		{
			if (check_box(sudoku, p, guess))
			{
				printf("guess %d is valid\n", guess);
				return 1;
			}
		}
	}
	return 0;
}

void set_value(int sudoku[9][9], struct position* p, int value)
{
	sudoku[p->row][p->col] = value;
	p->value = value;
}

struct position* backtrack_position(struct position* p)
{
	struct position* op = p;
	op->value = SD_EMPTY_CELL;

	// Backtracking algorithm.
	// Loop til the start?
	--p;
	if (p->value > 9)
		p->value = 1;
	p->value += 1;

	return p;
}

struct position* solve_sudoku(int sudoku[9][9], struct position* p)
{
	static int g_counter = 0;
	g_counter++;

	printf("Running :: %d\n", g_counter);

	assert(p);

	//if (p == NULL)
	//    p = find_next(sudoku);

	//if (!p)
	//    return 1;


	printf("\nEmpty cell, \nrow: \t%d, \ncol: \t%d \n", p->row, p->col);
	printf("Value at empty cell =>\t%d\n", sudoku[p->row][p->col]);

	int guess;
	guess = set_guess(sudoku, p);
	printf("Value => %d to \t[%d][%d]\n", guess, p->row, p->col);

	//    if (is_valid(sudoku, p, guess))
	//    {

			// Check this condition.
	if (is_valid(sudoku, p, guess))
	{
		sudoku[p->row][p->col] = guess;

		printf("Advancing the value => \t%d\n", guess);
		p = advance_position(p, guess);
	}

	if (p->value > 9)
	{
		// Should start backtracking til the beginning.
		printf("Backtracking the position, \n");
		p = backtrack_position(p);
	}

	// Set the value in both game state and position object.
	// set_value(sudoku, p, guess);

//        print_sudoku(sudoku);
//        solve_sudoku(sudoku, p);
//    }

//    printf("return one layer back\n");
	return p;
}

//
// Entry-Point
//
int main()
{
	//
	// Default sudoku configuration to the application.
	//
	int sudoku2[9][9] = {
		{9, 0, 0, 0, 7, 0, 0, 0, 0},
		{2, 0, 0, 0, 9, 0, 0, 5, 3},
		{0, 6, 0, 0, 1, 2, 4, 0, 0},
		{8, 4, 0, 0, 0, 1, 0, 9, 0},
		{5, 0, 0, 0, 0, 0, 8, 0, 0},
		{0, 3, 1, 0, 0, 4, 0, 0, 0},
		{0, 0, 3, 7, 0, 0, 6, 8, 0},
		{0, 9, 0, 0, 5, 0, 7, 4, 1},
		{4, 7, 0, 0, 0, 0, 0, 0, 0} };

	int sudoku[9][9] = {
	{1, 0, 0, 	9, 0, 4, 	0, 8, 2},
	{0, 5, 2, 	6, 8, 0, 	3, 0, 0},
	{8, 6, 4, 	2, 0, 0, 	9, 1, 0},

	{0, 1, 0, 	0, 4, 9, 	8, 0, 6},
	{4, 9, 8, 	3, 0, 0, 	7, 0, 1},
	{6, 0, 7, 	0, 1, 0, 	0, 9, 3},

		{0, 8, 6, 	0, 3, 5, 	2, 0, 9},
		{5, 0, 9, 	0, 0, 2, 	1, 3, 0},
		{0, 3, 0, 	4, 9, 7, 	0, 0, 8} };

	//
	// Allocate the Position structs.
	//
	struct position* p_sd = sd_tree;
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++) {
			if (sudoku[i][j] != 0)
				continue;

			struct position p = { .row = i, .col = j, .value = sudoku[i][j] };
			*(p_sd++) = p;
		}

	printf("Sudoku solver v1\n");

	struct position* p = sd_tree;
	while (p) {
		//
		// Prints out the sudoku.
		//
		print_sudoku(sudoku);


		//
		// Starts the solving algo function.
		//
		p = solve_sudoku(sudoku, p);
	}
	//
	// Wait for input.
	//
	(void)getchar();

	// now how to write recursive funcion that solves the sudoku???
}
