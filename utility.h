#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char *clone(char *str) {
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
} list_t;


list_t *new_list() {
    list_t *list = (list_t *)malloc(sizeof(list_t));
    list->head = NULL;
    return list;
}

void insert(list_t *list, void *data) {
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    new_node->data = data;
    new_node->next = list->head;
    list->head = new_node;
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