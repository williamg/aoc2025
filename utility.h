#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char *clone(const char *str) {
    if (str == NULL) {
        return NULL;
    }
    size_t len = strlen(str);
    char *new_str = (char *)malloc(len + 1);
    if (new_str == NULL) {
        return NULL;
    }
    strcpy(new_str, str);
    return new_str;
}

char *read_stdin() {
    char buffer[1024];
    size_t len = 0;
    char *str = NULL;
    
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        size_t buffer_len = strlen(buffer);
        char *temp = realloc(str, len + buffer_len + 1);
        if (temp == NULL) {
            free(str);
            return NULL;
        }
        str = temp;
        memcpy(str + len, buffer, buffer_len);
        len += buffer_len;
        str[len] = '\0';
    }

    return str;
}

char *substring(const char *str, size_t begin, size_t len) {
    if (str == NULL || strlen(str) < begin || strlen(str) < begin + len) {
        return NULL;
    }
    char *substr = (char *)malloc(len + 1);
    if (substr == NULL) {
        return NULL;
    }
    strncpy(substr, str + begin, len);
    substr[len] = '\0';
    return substr;
}

void split_iter(const char *str, char delimiter, void (*callback)(const char *token, void *user_data), void *user_data) {
    if (str == NULL || callback == NULL) {
        return;
    }

    const char *start = str;
    const char *end = strchr(start, delimiter);

    while (end != NULL) {
        size_t len = end - start;
        char *token = substring(str, start - str, len);
        if (token != NULL) {
            callback(token, user_data);
            free(token);
        }
        start = end + 1;
        end = strchr(start, delimiter);
    }

    // Handle the last token
    if (*start != '\0') {
        char *token = substring(str, start - str, strlen(start));
        if (token != NULL) {
            callback(token, user_data);
            free(token);
        }
    }
}


typedef struct node_t {
    void* data;
    struct node_t* next;
} node_t;

typedef struct {
    node_t *head;
    size_t size;
} list_t;


list_t *new_list() {
    list_t *list = (list_t *)malloc(sizeof(list_t));
    list->head = NULL;
    list->size = 0;
    return list;
}

void insert(list_t *list, void *data) {
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    new_node->data = data;
    new_node->next = list->head;
    list->head = new_node;
    list->size++;
}

bool contains(list_t *list, bool (*predicate)(void *data, void *user_data), void *user_data) {
    node_t *current = list->head;
    while (current != NULL) {
        if (predicate(current->data, user_data)) {
            return true;
        }
        current = current->next;
    }
    return false;
}

void* get_data(list_t *list, bool (*predicate)(void *data, void *user_data), void *user_data) {
    node_t *current = list->head;
    while (current != NULL) {
        if (predicate(current->data, user_data)) {
            return current->data;
        }
        current = current->next;
    }
    return NULL;
}

void* to_array(list_t *list, size_t item_size) {
    if (list->size == 0) {
        return NULL;
    }
    void *array = malloc(list->size * item_size);
    node_t *current = list->head;
    size_t index = 0;
    while (current != NULL) {
        memcpy((char *)array + index * item_size, current->data, item_size);
        current = current->next;
        index++;
    }
    return array;
}

typedef struct grid_t {
    size_t rows;
    size_t cols;
    char *data;
} grid_t;

grid_t *grid_from_string(const char *str) {
    char *data = malloc(strlen(str) + 1);
    int num_cols = 0;
    int num_rows = 0;

    for(int i = 0; i < strlen(str); ++i) {
        if (str[i] == '\n') {
            if (num_cols == 0) {
                num_cols = i;
            }
            num_rows++;
        } else {
            data[i - num_rows] = str[i];
        }
    }

    ++num_rows;

    grid_t *grid = (grid_t *)malloc(sizeof(grid_t));
    grid->rows = num_rows;
    grid->cols = num_cols;
    grid->data = data;
    return grid;
}

char grid_get(grid_t *grid, size_t row, size_t col) {
    if (row >= grid->rows || col >= grid->cols) {
        return '\0';
    }
    return grid->data[row * grid->cols + col];
}

void grid_set(grid_t *grid, size_t row, size_t col, char value) {
    if (row >= grid->rows || col >= grid->cols) {
        return;
    }
    grid->data[row * grid->cols + col] = value;
}

void iter_adjacent(grid_t *grid, size_t row, size_t col, void (*callback)(char value, size_t r, size_t c, void *user_data), void *user_data) {
    for (int dr = -1; dr <= 1; ++dr) {
        for (int dc = -1; dc <= 1; ++dc) {
            if (dr == 0 && dc == 0) {
                continue;
            }
            int new_row = row + dr;
            int new_col = col + dc;
            if (new_row >= 0 && new_row < grid->rows && new_col >= 0 && new_col < grid->cols) {
                char value = grid_get(grid, new_row, new_col);
                callback(value, new_row, new_col, user_data);
            }
        }
    }
}