#include "../utility.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool in_range_inclusive(char *start, char *test, char *end) {
    long startl = strtol(start, NULL, 10);
    long testl = strtol(test, NULL, 10);
    long endl = strtol(end, NULL, 10);

    return startl <= testl && testl <= endl;
}

bool is_invalid(char *str) {
    int len = strlen(str);
    for(int num_repeats = 2; num_repeats <= strlen(str); ++num_repeats) {
        if (len % num_repeats != 0) {
            continue;
        }

        int seq_length = len / num_repeats;
        bool all_match = true;
        for(size_t offset = 0; offset < seq_length; ++offset) {
            for(size_t copy = 1; copy < num_repeats; ++copy) {
                if (str[offset + copy * (len / num_repeats)] != str[offset]) {
                    all_match = false;
                    break;
                }
            }
        }

        if (all_match) {
            return true;
        }
    }
    return false;
}

char *make_invalid(long val, int num_repeats) {
    char seq[256];
    sprintf(seq, "%ld", val); 
    size_t seq_length = strlen(seq);
    char *new_str = (char *)malloc(seq_length * num_repeats + 1);
    for(size_t i = 0; i < seq_length; i++) {
        for(int r = 0; r < num_repeats; r++) {
            new_str[i + r * seq_length] = seq[i];
        }
    }
    new_str[seq_length * num_repeats] = '\0';
    return new_str;
}

char *next_invalid(char *str, int num_repeats) {
    size_t len = strlen(str);
    if (len % num_repeats != 0) {
        char *new_str = (char *)malloc(len + num_repeats);
        int seq_length = 1 + (len / num_repeats);
        new_str[0] = '1';
        for(int i = 1; i < seq_length; i++) {
            new_str[i] = '0';
        }
        for(size_t i = 0; i < seq_length; i++) {
            for(int r = 1; r < num_repeats; r++) {
                new_str[i + r * seq_length] = new_str[i];
            }
        }
        new_str[seq_length * num_repeats] = '\0';
        return new_str;
    }

    int seq_length = len / num_repeats;
    char *first_seq = substring(str, 0, seq_length);

    bool all_same = true;
    for(int i = 0; i < seq_length; ++i) {
        all_same &= first_seq[i] == first_seq[0];
    }

    char *second_seq = substring(str, seq_length, seq_length);

    // First see what the value would be without any increment
    long val = strtol(first_seq, NULL, 10) + 0;
    char *result = make_invalid(val, num_repeats);

    if (strcmp(result, str) > 0) {
        free(first_seq);
        free(second_seq);
        return result;
    }

    free(result);
    result = make_invalid(val+1, num_repeats);

    free(first_seq);
    free(second_seq);
    return result;
}

bool double_matches(void *a, void *b) {
    char *str_a = (char *)a;
    char *str_b = (char *)b;
    return strcmp(str_a, str_b) == 0;
}

void count_doubles_in_range(const char *range_str, void *user_data) {
    long *sum = (long *)user_data;
    char *dash_pos = strchr(range_str, '-');
    if (dash_pos == NULL) {
        return;
    }

    char *first = substring(range_str, 0, dash_pos - range_str);
    size_t second_start = dash_pos - range_str + 1U;
    char *second = substring(range_str, second_start, strlen(range_str) - second_start);
    list_t *invalids = new_list();

    for(int num_repeats = 2; num_repeats <= strlen(second); ++num_repeats) {
        char *test = is_invalid(first) ? clone(first) : next_invalid(first, num_repeats);
        printf("Counting %d-repeats in range %s to %s, starting with %s\n", num_repeats, first, second, test);
        while (in_range_inclusive(first, test, second)) {
            printf("%s\n", test);
            if (!contains(invalids, double_matches, test)) {
                insert(invalids, clone(test));
                (*sum) += strtol(test, NULL, 10);
            }
            char *old_test = test;
            test = next_invalid(test, num_repeats);
            free(old_test);
        }
    }

    free(first);
    free(second);
}

int main() {
    char *input = read_stdin();

    long sum = 0;
    split_iter(input, ',', count_doubles_in_range, &sum);
    free(input);

    printf("Sum of doubles in ranges: %ld\n", sum);
    
    return 0;
}