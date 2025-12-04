#include "../utility.h"

long long pow10(size_t exp) {
    long long result = 1;
    for (size_t i = 0; i < exp; ++i) {
        result *= 10;
    }
    return result;
}

typedef struct memoized_t {
    const char *str;
    size_t digits;
    long long value;
} memoized_t;

bool matching_memo(void *a, void *b) {
    memoized_t *memo_a = (memoized_t *)a;
    memoized_t *memo_b = (memoized_t *)b;
    return memo_a->str == memo_b->str && memo_a->digits == memo_b->digits;
}

long long max_with_digits(const char *str, size_t digits, list_t *memos) {
    memoized_t key = {str, digits, 0};
    memoized_t *cached = (memoized_t *)get_data(memos, matching_memo, &key);

    if(cached) {
        return cached->value;
    }

    if (digits == 1) {
        char max = str[0];
        for (size_t i = 1; i < strlen(str); ++i) {
            if (str[i] > max) {
                max = str[i];
            }
        }
        return max - '0';
    }

    if (strlen(str) <= digits) {
        return strtol(str, NULL, 10);
    }

    long long using_index = ((str[0] - '0') * pow10(digits - 1)) + max_with_digits(str + 1, digits - 1, memos);
    long long not_using_index = max_with_digits(str + 1, digits, memos );
    long long result = using_index > not_using_index ? using_index : not_using_index;
    memoized_t *new_memo = (memoized_t *)malloc(sizeof(memoized_t));
    new_memo->str = str;
    new_memo->digits = digits;
    new_memo->value = result;
    insert(memos, new_memo);
    return result;
}

void add_bank_joltage(const char *bank, void* data) {
    long long *max_joltage = (long long *)data;

    list_t *memos = new_list();
    long long max = max_with_digits(bank, 12, memos);

    printf("bank %s has max joltage %lld\n", bank, max);
    *max_joltage += max;
}

int main() {
    char *input = read_stdin();

    long long max_joltage = 0;

    split_iter(input, '\n', &add_bank_joltage, &max_joltage);
    free(input);

    printf("Max joltage: %lld\n", max_joltage);
    return 0;
}