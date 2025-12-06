#include "../utility.h"
int main() {
    char *input = read_stdin();
    grid_t *grid = grid_from_string(input);
    size_t cum_result = 0;
    for(int i = 0; i < grid->cols; ++i) {
        char op = grid_get(grid, grid->rows-1, i);

        if (op == '+' || op == '*') {
            unsigned long prob_result = 0;
            for(int c = i; c < grid->cols; ++c) {
                unsigned long val = 0;
                bool found_any = false;
                for(int j = 0; j < grid->rows - 1; ++j) {
                    if (grid_get(grid, j, c) != ' ') {
                        found_any = true;
                        val *= 10;
                        val += (unsigned)(grid_get(grid, j, c) - '0');
                    }
                }
                if (found_any) {
                    printf("Foudn val: %lu\n", val);
                    if (prob_result == 0) {
                        prob_result = val;
                    } else {
                        if (op == '+') {
                            prob_result += val;
                        } else if (op == '*') {
                            prob_result *= val;
                        }
                    }
                } else {
                    break;
                }
            }
            cum_result += prob_result;
            printf("Prob result: %lu\n", prob_result);
        }
    }
    printf("Cumulative result: %zu\n", cum_result);
}