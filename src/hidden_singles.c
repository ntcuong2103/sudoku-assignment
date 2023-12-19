#include "sudoku.h"  
#include "hidden_singles.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int find_hidden_single_values(Cell **p_cells, int *hidden_single_values) {
    int num_candidates[BOARD_SIZE] = {0};
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (p_cells[i]->num_candidates > 1) {
            int *candidates = get_candidates(p_cells[i]);
            for (int j = 0; j < p_cells[i]->num_candidates; j++) {
                num_candidates[candidates[j] - 1]++;
            }
            free(candidates);
        }

    }
    int counter = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (num_candidates[i] == 1) {
            hidden_single_values[counter++] = i + 1;
        }

    }

    return counter;
}
bool is_processed(Cell *cell, HiddenSingle *processed_cells,int processed_count) {
    for (int i = 0; i < processed_count; ++i) {
        if (processed_cells[i].p_cell == cell) {
            return true;
        }

    }
    return false;
}

void find_hidden_single(Cell **p_cells, HiddenSingle *p_hidden_singles, int *p_counter,HiddenSingle *processed_cells,int *processed_count) {
    int hidden_single_values[BOARD_SIZE];
    int num_hidden_single_values = find_hidden_single_values(p_cells, hidden_single_values);
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (p_cells[i]->num_candidates > 1 && !is_processed(p_cells[i], processed_cells, *processed_count)) {
            int *candidates = get_candidates(p_cells[i]);  
            for (int j = 0; j < num_hidden_single_values; j++) {
                if (is_candidate(p_cells[i], hidden_single_values[j])) {
                    p_hidden_singles[*p_counter].p_cell = p_cells[i];
                    p_hidden_singles[*p_counter].value = hidden_single_values[j];
                    (*p_counter)++;
                    processed_cells[*processed_count].p_cell = p_cells[i];
                    processed_cells[*processed_count].value = hidden_single_values[j];
                    (*processed_count)++;

                    break;
                }
            }
            free(candidates);
        }
}
}

int hidden_singles(SudokuBoard *p_board) {
    HiddenSingle hidden_singles[BOARD_SIZE * BOARD_SIZE];
    int counter = 0;
    HiddenSingle processed_cells[BOARD_SIZE * BOARD_SIZE];
    int processed_count = 0;
    for (int i = 0; i < BOARD_SIZE; i++) 
    {
        find_hidden_single(p_board->p_rows[i], hidden_singles, &counter, processed_cells, &processed_count);
    }
    for (int i = 0; i < BOARD_SIZE; i++) 
    {
        find_hidden_single(p_board->p_cols[i], hidden_singles, &counter, processed_cells, &processed_count);
    }
    for (int i = 0; i < BOARD_SIZE; i++)
     {
        find_hidden_single(p_board->p_boxes[i], hidden_singles, &counter, processed_cells, &processed_count);
    }
    for (int i = 0; i < counter; i++) 
    {
        Cell *cell = hidden_singles[i].p_cell;
        int value = hidden_singles[i].value;
        set_candidates(cell, &value, 1);
    }
    return counter;
}