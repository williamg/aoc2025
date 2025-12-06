#include "../utility.h"

typedef struct range_t {
    const char *start;
    const char *end;
} range_t;

int num_comp(void *a_in, void *b_in) {
    char* a = (char*)a_in;
    char* b = (char*)b_in;
    size_t len_a = strlen(a);
    size_t len_b = strlen(b);
    if (len_a != len_b) {
        return (int)(len_a - len_b);
    }
    return strcmp(a, b);
}

bool range_contains(range_t range, const char *test) {
    if (strlen(test) < strlen(range.start) || strlen(test) > strlen(range.end)) {
        return false;
    }
    if (strlen(test) == strlen(range.start) && strcmp(test, range.start) < 0) {
        return false;
    }
    if (strlen(test) == strlen(range.end) && strcmp(test, range.end) > 0) {
        return false;
    }
    return true;
}

bool ranges_overlap(range_t a, range_t b) {
    if (range_contains(a, b.start) || range_contains(a, b.end) ||
        range_contains(b, a.start) || range_contains(b, a.end)) {
        return true;
    }
    return false;
}

range_t merge_ranges(range_t a, range_t b) {
    range_t merged;
    if (strcmp(a.start, b.start) < 0) {
        merged.start = a.start;
    } else {
        merged.start = b.start;
    }
    if (strcmp(a.end, b.end) > 0) {
        merged.end = a.end;
    } else {
        merged.end = b.end;
    }
    return merged;
}

typedef struct input_t {
    list_t *ranges;
    list_t *ids;
} input_t;

void read_input(const char *line, void* user_data) {
    input_t *input = (input_t *)user_data;

    if (strchr(line, '-') != NULL) {
        char *dash_pos = strchr(line, '-');
        range_t *range = (range_t *)malloc(sizeof(range_t));
        range->start = substring(line, 0, dash_pos - line);
        range->end = substring(line, dash_pos - line + 1, strlen(dash_pos + 1));
        insert(input->ranges, range);
    } else if (strlen(line) > 0) {
        char** id_ptr = (char **)malloc(sizeof(char *));
        *id_ptr = clone(line);
        insert(input->ids, id_ptr);
    }
}

int compare_ranges(const void *a, const void *b) {
    const range_t *range_a = (const range_t *)a;
    const range_t *range_b = (const range_t *)b;

    if(strlen(range_a->start) != strlen(range_b->start)) {
        return (int)(strlen(range_a->start) - strlen(range_b->start));
    }
    return strcmp(range_a->start, range_b->start);
}

int main() {
    const char *input = read_stdin();

    input_t parsed_input;
    parsed_input.ranges = new_list();
    parsed_input.ids = new_list();
    split_iter(input, '\n', read_input, &parsed_input);
    range_t* ranges = (range_t*) to_array(parsed_input.ranges, sizeof(range_t));
    char** ids = (char**) to_array(parsed_input.ids, sizeof(char *));
    qsort(ranges, parsed_input.ranges->size, sizeof(range_t), compare_ranges);

    int count_fresh = 0;
    for(int i = 0; i < parsed_input.ids->size; ++i) {
        char* id = ids[i];
        for(int j = 0; j < parsed_input.ranges->size; ++j) {
            range_t range = ranges[j];
            if (range_contains(range, id)) {
                ++count_fresh;
                break;
            }
        }
    }
    printf("Number of valid IDs: %d\n", count_fresh);

    list_t *total_ranges = new_list();

    range_t *current_range = &ranges[0];
    for(int i = 0; i < parsed_input.ranges->size; ++i) {
        if (ranges_overlap(*current_range, ranges[i])) {
            *current_range = merge_ranges(*current_range, ranges[i]);
            continue;
        }

        insert(total_ranges, current_range);
        current_range = &ranges[i];
    }

    insert(total_ranges, current_range);

    size_t fresh_counts = 0;
    node_t *node = total_ranges->head;
    while (node != NULL) {
        range_t *range = (range_t *)node->data;
        fresh_counts += strtoul(range->end, NULL, 10) - strtoul(range->start, NULL, 10) + 1;
        node = node->next;
    }

    printf("Number of fresh IDs: %zu\n", fresh_counts);
    return 0;
}