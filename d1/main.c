#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char buffer[1024];
    size_t len = 0;
    char *str = NULL;
    
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        size_t buffer_len = strlen(buffer);
        char *temp = realloc(str, len + buffer_len + 1);
        if (temp == NULL) {
            free(str);
            return 1;
        }
        str = temp;
        memcpy(str + len, buffer, buffer_len);
        len += buffer_len;
        str[len] = '\0';
    }
    
    if (str == NULL) {
        return 2;
    }

    // Read input line by line
    char *cur_line = str;
    int accum = 50;
    int dial = 50;
    int part_1_count = 0;
    int part_2_count = 0;

    while (cur_line && *cur_line) {
        char *end_n1 = strchr(cur_line, '-');
        char *end_n2 = strchr(cur_line, ',');

        if (next_line) {
            *next_line = '\0';
        }

        char dir = *cur_line;
        int amt = strtol(cur_line + 1, NULL, 10);
        int loops = amt / 100;
        amt = amt % 100;

        part_2_count += loops;

        if (dir == 'L') {
            accum -= amt;
            if (amt >= dial && dial != 0) {
                part_2_count++;
            }
            dial -= amt;
        } else if (dir == 'R') {
            accum += amt;
            if (amt >= (100 - dial)) {
                part_2_count++;
            }
            dial += amt;
        }

        while (dial < 0) {
            dial += 100;
        }
        while (dial >= 100) {
            dial -= 100;
        }

        printf("Direction: %c, Amount: %d dial: %d, accum: %d %d\n", dir, amt, dial, accum, part_2_count);

    if (accum % 100 == 0) {
        part_1_count++;
    }

        cur_line = next_line ? next_line + 1 : NULL;
    }

    printf("Password: %d\n", part_1_count);
    printf("Password 2: %d\n", part_2_count);
    
    return 0;
}