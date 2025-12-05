#include "../utility.h"

void count_adjacent_rolls(char value, size_t r, size_t c, void *user_data) {
    size_t *count = (size_t *)user_data;
    if (value == '@' || value == 'X') {
        (*count)++;
    }
}

int main() {
    const char *input = read_stdin();
    const grid_t *grid = grid_from_string(input);
    size_t num_reachable = 0;
    size_t num_removed = 0;

    do {
        num_reachable = 0;
        for(int i = 0; i < grid->rows; ++i) {
            for(int j = 0; j < grid->cols; ++j) {
                if (grid_get((grid_t *)grid, i, j) != '@') {
                    continue;
                }
                size_t count = 0;
                iter_adjacent((grid_t *)grid, i, j, count_adjacent_rolls, &count);
                if (count < 4) {
                    grid_set((grid_t *)grid, i, j, 'X');
                    num_reachable++;
                }
            }
        }
        for(int i = 0; i < grid->rows; ++i) {
            for(int j = 0; j < grid->cols; ++j) {
                if (grid_get((grid_t *)grid, i, j) == 'X') {
                    grid_set((grid_t *)grid, i, j, '.');
                    num_removed++;
                }
            }
        }
    } while (num_reachable > 0);

    printf("Number of reachable positions: %zu\n", num_reachable);
    printf("Number of removed positions: %zu\n", num_removed);
    return 0;
}