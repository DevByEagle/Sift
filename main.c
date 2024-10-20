#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define VIEW_IMPLEMENTATION
#include "view.h"

#define DATA_START_CAPACITY 16

#define ASSERT(cond, ...) \
    do { \
        if (!(cond)) { \
            fprintf(stderr, "%s:%d: ASSERTION FAILED: ", __FILE__, __LINE__); \
            fprintf(stderr, __VA_ARGS__); \
            fprintf(stderr, "\n"); \
            exit(1); \
        } \
    } while (0)

#define DA_APPEND(da, item) do {                                                       \
    if ((da)->count >= (da)->capacity) {                                               \
        (da)->capacity = (da)->capacity == 0 ? DATA_START_CAPACITY : (da)->capacity*2; \
        (da)->data = realloc((da)->data, (da)->capacity*sizeof(*(da)->data));       \
        ASSERT((da)->data != NULL, "outta ram");                               \
    }                                                                                  \
    (da)->data[(da)->count++] = (item);                                               \
} while (0)

typedef enum {
    TT_NONE = 0,
    TT_WRITE,
    TT_STRING,
    TT_COUNT,
} Token_Type;

char* types[TT_COUNT] = {"none", "write", "string"};

typedef struct {
    size_t row;
    size_t col;
    char* filename;
} Location;

typedef struct {
    Token_Type type;
    Location loc;
} Token;

typedef struct {
    Token* data;
    size_t count;
    size_t capacity;
} Token_Arr;

String_View read_file_to_view(char* filename) {
    FILE* file = fopen(filename, "r");

    fseek(file, 0, SEEK_END);
    size_t len = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* data = malloc(sizeof(char)*len);
    fread(data, sizeof(char), len, file);

    fclose(file);
    return (String_View){.data=data, .len=len};
}

bool isword(char c) {
    return isalpha(c) || isdigit(c) || c == '_';
}

Token_Type get_token_type(char* str, size_t str_t) {
    if (strncmp(str, "write", str_t) == 0) {
        return TT_STRING;
    }
    return TT_NONE;
}

void print_token_arr(Token_Arr arr) {
    for (size_t i = 0; i < arr.count; i++) {
        printf("%zu: %s\n", i, types[arr.data[i].type]);
    }
}

Token_Arr lex(String_View view) {
    Token_Arr tokens = {0};
    if (isalpha(*view.data)) {
        char word[128];
        size_t word_s = 0;
        word[word_s++] = *view.data;
        view = view_chop_left(view);
        while (view.len > 0 && isword(*view.data)) {
            word[word_s++] = *view.data;
            view = view_chop_left(view);
        }
        Token token = {0};
        token.type = get_token_type(word, word_s);
        if (token.type == TT_NONE) exit(1);
        DA_APPEND(&tokens, token);
    }
    return tokens;
}

int main() {
    String_View view = read_file_to_view("sift.sift");
    Token_Arr tokens = lex(view);
    print_token_arr(tokens);
}