#pragma once

#include "sudoku.h"

typedef struct NakedPair_impl {
    Cell *p_cells[2];
    int values[2];
} NakedPair;

int find_naked_pairs(Cell **p_cells, NakedPair *naked_pairs);
void apply_naked_pair(NakedPair *naked_pair, Cell **unit);
int naked_pairs(SudokuBoard *p_board);