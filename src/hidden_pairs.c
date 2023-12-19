#include "hidden_pairs.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sudoku.h"

int find_hidden_pairs(Cell **p_cells, HiddenPair *hidden_pairs) {
    int num_pairs = 0;

    for (int i = 0; i < BOARD_SIZE - 1; i++) {
        if (p_cells[i]->num_candidates == 2) {
            for (int j = i + 1; j < BOARD_SIZE; j++) {
                if (p_cells[j]->num_candidates == 2) {
                    int *candidates_i = get_candidates(p_cells[i]);
                    int *candidates_j = get_candidates(p_cells[j]);

                    if ((candidates_i[0] == candidates_j[0] && candidates_i[1] == candidates_j[1]) ||
                        (candidates_i[0] == candidates_j[1] && candidates_i[1] == candidates_j[0])) {
                        hidden_pairs->p_cells[0] = p_cells[i];
                        hidden_pairs->p_cells[1] = p_cells[j];
                        hidden_pairs->values[0] = candidates_i[0];
                        hidden_pairs->values[1] = candidates_i[1];
                        num_pairs++;

                        free(candidates_i);
                        free(candidates_j);
                        break;  // Move to the next cell in the outer loop
                    }

                    free(candidates_i);
                    free(candidates_j);
                }
            }
        }
    }

    return num_pairs;
}
void apply_hidden_pair(HiddenPair *hidden_pair) {
    for (int i = 0; i < 2; i++) {
        Cell *cell = hidden_pair->p_cells[i];
        
        // Iterate over the naked pair
        for (int j = 0; j < 2; j++) {
            // Check if the current cell is part of the naked pair
            if (cell == hidden_pair->p_cells[j]) {
                // Get the candidates for the cell
                // Get the candidates for the cell
                int *candidates = get_candidates(cell);

                // Remove other candidates from the cell
                for (int k = 0; k < cell->num_candidates; k++) {
                    int value = candidates[k];

                    // Check if the candidate is not part of the naked pair
                    if (value != hidden_pair->values[0] && value != hidden_pair->values[1]) {
                        unset_candidate(cell, value);
                    }
                }

                // Free the memory allocated for candidates
                free(candidates);
            }
        }
    }
}
int hidden_pairs(SudokuBoard *p_board) {
    HiddenPair naked_pairs[BOARD_SIZE * BOARD_SIZE];
    int num_pairs = 0;

    // Check naked pairs in rows
    for (int i = 0; i < BOARD_SIZE; i++) {
        num_pairs += find_hidden_pairs(p_board->p_rows[i], naked_pairs + num_pairs);
    }

    // Check naked pairs in columns
    for (int i = 0; i < BOARD_SIZE; i++) {
        num_pairs += find_hidden_pairs(p_board->p_cols[i], naked_pairs + num_pairs);
    }

    // Check naked pairs in boxes
    for (int i = 0; i < BOARD_SIZE; i++) {
        num_pairs += find_hidden_pairs(p_board->p_boxes[i], naked_pairs + num_pairs);
    }

    // Apply naked pairs to the board
    for (int i = 0; i < num_pairs; i++) {
        apply_hidden_pair(naked_pairs + i);
    }

    return num_pairs;
}