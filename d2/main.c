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

bool is_double(char *str) {
    if (strlen(str) % 2 == 1) {
        return false;
    }

    size_t half_len = strlen(str) / 2;
    for (size_t i = 0; i < half_len; i++) {
        if (str[i] != str[i + half_len]) {
            return false;
        }
    }
    return true;
}

char *next_double(char *str) {
    size_t len = strlen(str);
    if (len % 2 == 1) {
        char *new_str = (char *)malloc(len + 2);
        new_str[0] = '1';
        new_str[(len + 1) / 2] = '1';
        for (size_t i = 1; i < (len + 1) / 2; i++) {
            new_str[i] = '0';
            new_str[i + (len + 1) / 2] = '0';
        }

        return new_str;
    }

    char *first_half = substring(str, 0, len / 2);
    char *second_half = substring(str, len / 2, len / 2);
    int increment = strcmp(first_half, second_half) <= 0 ? 1 : 0;
    long val = strtol(first_half, NULL, 10) + increment;
    char *result = (char *)malloc(len + 2);
    sprintf(result, "%ld%ld", val, val);
    free(first_half);
    free(second_half);
    return result;
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

    char *test = is_double(first) ? clone(first) : next_double(first);

    printf("Counting doubles in range %s to %s, starting with %s\n", first, second, test);
    while (in_range_inclusive(first, test, second)) {
        printf("%s\n", test);
        (*sum) += strtol(test, NULL, 10);
        char *old_test = test;
        test = next_double(test);
        free(old_test);
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