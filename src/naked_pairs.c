#include "naked_pairs.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sudoku.h"

static bool are_naked_pairs_equal(NakedPair *pair1, NakedPair *pair2) {
    return (pair1->values[0] == pair2->values[0] && pair1->values[1] == pair2->values[1]) ||
           (pair1->values[0] == pair2->values[1] && pair1->values[1] == pair2->values[0]);
}

int find_naked_pairs(Cell **p_cells, NakedPair *naked_pairs) {
    int count = 0;

    for (int i = 0; i < BOARD_SIZE - 1; i++) {
        for (int j = i + 1; j < BOARD_SIZE; j++) {
            int *candidates_i = get_candidates(p_cells[i]);
            int *candidates_j = get_candidates(p_cells[j]);

            if (candidates_i[0] == candidates_j[0] && candidates_i[1] == candidates_j[1]) {
                naked_pairs[count].p_cells[0] = p_cells[i];
                naked_pairs[count].p_cells[1] = p_cells[j];
                naked_pairs[count].values[0] = candidates_i[0];
                naked_pairs[count].values[1] = candidates_i[1];
                count++;
            }

            free(candidates_i);
            free(candidates_j);
        }
    }

    return count;
}

void apply_naked_pair(NakedPair *naked_pair, Cell **unit) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (unit[i] != naked_pair->p_cells[0] && unit[i] != naked_pair->p_cells[1]) {
            int *candidates = get_candidates(unit[i]);
            for (int k = 0; k < 2; k++) {
                if (candidates[k] == naked_pair->values[0] || candidates[k] == naked_pair->values[1]) {
                    unset_candidate(unit[i], candidates[k]);
                }
            }
            free(candidates);
        }
    }
}

int naked_pairs(SudokuBoard *p_board) {
    NakedPair pairs[BOARD_SIZE];
    int num_pairs = 0;

    for (int i = 0; i < BOARD_SIZE; i++) {
        num_pairs += find_naked_pairs(p_board->p_rows[i], pairs);
        num_pairs += find_naked_pairs(p_board->p_cols[i], pairs + num_pairs);
        num_pairs += find_naked_pairs(p_board->p_boxes[i], pairs + num_pairs);
    }

    for (int i = 0; i < num_pairs; i++) {
        for (int j = i + 1; j < num_pairs; j++) {
            if (are_naked_pairs_equal(&pairs[i], &pairs[j])) {
                apply_naked_pair(&pairs[i], p_board->p_rows[pairs[i].p_cells[0]->row_index]);
            }
        }
    }

    return num_pairs;
}