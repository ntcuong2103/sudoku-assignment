#pragma once

#include "sudoku.h"

typedef struct HiddenPair_impl
{
    Cell *p_cells[2];
    int values[2];
} HiddenPair;
int find_hidden_pairs(Cell **p_cells, HiddenPair *naked_pairs);
void apply_hidden_pair(HiddenPair *hidden_pair);
int hidden_pairs(SudokuBoard *p_board);
