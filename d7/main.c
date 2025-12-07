#include "../utility.h"

typedef struct memo_entry_t {
    size_t row;
    size_t col;
    size_t paths;
} memo_entry_t;

size_t paths_from(grid_t *grid, size_t row, size_t col, list_t *memo);

size_t paths_from_no_memo(grid_t *grid, size_t row, size_t col, list_t *memo) {
    printf("Computing %zu,%zu\n", row, col);
    if (row >= grid->rows || col >= grid->cols || col < 0 || row < 0) {
        printf("ERROR!\n");
        return 0;
    }
    if(grid_get(grid, row, col) != '.') {
        printf("Error 2!\n");
        return 0;
    }
    char c = grid_get(grid, row, col);
    while(row < grid->rows - 1 && grid_get(grid, row, col) != '^') {
        row++;
    }

    if(row == grid->rows - 1) {
        return 1;
    } else {
        return paths_from(grid, row, col-1, memo) + paths_from(grid, row, col+1, memo);
    }
}

size_t paths_from(grid_t *grid, size_t row, size_t col, list_t *memo) {
    // Check memoization
    node_t *current = memo->head;
    while(current != NULL) {
        memo_entry_t *entry = (memo_entry_t *)current->data;
        if(entry->row == row && entry->col == col) {
            return entry->paths;
        }
        current = current->next;
    }

    size_t result = paths_from_no_memo(grid, row, col, memo);

    printf("Memoizing %zu,%zu = %zu\n", row, col, result);
    memo_entry_t *entry = malloc(sizeof(memo_entry_t));
    entry->row = row;
    entry->col = col;
    entry->paths = result;
    insert(memo, entry);
    return result;
}

int main() {
    const char*input = read_stdin();
    grid_t *grid = grid_from_string(input);
    list_t *memo = new_list();

    for(int x = 0; x < grid->cols; ++x) {
        if (grid_get(grid, 0, x) == 'S') {
            size_t paths = paths_from(grid, 1, x, memo);
            printf("Paths from %d,%d : %zu\n", 0, x, paths);
            return 0;
        }
    }

    return 1;

}