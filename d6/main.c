#include "../utility.h"

typedef enum op_type_t {
    OP_ADD,
    OP_MULTIPLY
} op_type_t;

typedef struct input_t {
    list_t *value_lists;
    list_t *op_list;
} input_t;

void read_token(const char *line, void* user_data) {
    input_t *input = (input_t *)user_data;
    if (strlen(line) == 0) {
        return;
    }
    if (strchr(line, '+') != NULL) {
        op_type_t *op = malloc(sizeof(op_type_t));
        *op = OP_ADD;
        insert(input->op_list, op);
    } else if (strchr(line, '*') != NULL) {
        op_type_t *op = malloc(sizeof(op_type_t));
        *op = OP_MULTIPLY;
        insert(input->op_list, op);
    } else {
        list_t *cur_list = *(list_t **)input->value_lists->head->data;
        unsigned* value = (unsigned *)malloc(sizeof(unsigned));
        *value = (unsigned)strtoul(line, NULL, 10);
        printf("Inserting value %u to %p\n", *value, cur_list);
        insert(cur_list, (void *)value);
    }
}

void read_lines(const char *line, void* user_data) {
    input_t *input = (input_t *)user_data;
    list_t **line_values = malloc(sizeof(list_t *));
    *line_values = new_list();
    insert(input->value_lists, line_values);
    split_iter(line, ' ', read_token, user_data);
}

int main() {
    const char *input = read_stdin();

    input_t parsed_input;
    parsed_input.value_lists = new_list();
    parsed_input.op_list = new_list();
    split_iter(input, '\n', read_lines, &parsed_input);

    // Will end up with one empty list at the start; remove it
    parsed_input.value_lists->head = parsed_input.value_lists->head->next;
    parsed_input.value_lists->size--;

    list_t **value_lists = to_array(parsed_input.value_lists, sizeof(list_t *));
    enum op_type_t *ops = to_array(parsed_input.op_list, sizeof(enum op_type_t));
    unsigned **value_arrays = malloc(parsed_input.value_lists->size * sizeof(unsigned*));
    printf("Ops size: %zu\n", parsed_input.op_list->size);
    printf("Num rows: %zu\n", parsed_input.value_lists->size);

    for(size_t i = 0; i < parsed_input.value_lists->size; ++i) {
        printf("Value size: (%p) %zu\n", value_lists[i], value_lists[i]->size);
        value_arrays[i] = to_array(value_lists[i], sizeof(unsigned));
    }

    long long cum_result = 0;

    for(int i = 0; i < parsed_input.op_list->size; ++i) {
        enum op_type_t op = ops[i];
        long long result = value_arrays[0][i];
        for(int v = 1; v < parsed_input.value_lists->size; ++v) {
            switch (op) {
                case OP_ADD:
                    result += value_arrays[v][i];
                    break;
                case OP_MULTIPLY:
                    result *= value_arrays[v][i];
                    break;
            }
        }

        cum_result += result;
    }

    printf("Cumulative result: %lld\n", cum_result);

    return 0;
}