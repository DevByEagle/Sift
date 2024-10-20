#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "view.h"

typedef enum {
    TT_WRITE,
    TT_STRING,
} Token_Type;

typedef struct {
    size_t row;
    size_t col;
    char* filename;
} Location;

typedef struct {
    Token_Type type;
    Location loc;
} Token;

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

}

Token* lex(String_View view) {
    if (isalpha(*view.data)) {
        char* word[128];
        size_t word_s = 0;
        word[word_s++] = *view.data;
        view = view_chop_left(view);
        while (view.len > 0 && isword(*view.data)) {
            word[word_s++] = *view.data;
            view = view_chop_left(view);
        }
        Token token = {0};
        token.type = get_token_type(word, word_s);
    }
}

int main() {
    String_View view = read_file_to_view("sift.sift");
    lex(view);
    printf(View_Print"\n", View_Arg(view));
}