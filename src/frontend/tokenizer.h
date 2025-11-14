#ifndef KRONOS_TOKENIZER_H
#define KRONOS_TOKENIZER_H

#include <stddef.h>

// Token types matching the Python implementation
typedef enum {
    TOK_NUMBER,
    TOK_STRING,
    TOK_SET,
    TOK_TO,
    TOK_IF,
    TOK_FOR,
    TOK_WHILE,
    TOK_IN,
    TOK_RANGE,
    TOK_FUNCTION,
    TOK_WITH,
    TOK_CALL,
    TOK_RETURN,
    TOK_IS,
    TOK_EQUAL,
    TOK_NOT,
    TOK_GREATER,
    TOK_LESS,
    TOK_THAN,
    TOK_AND,
    TOK_OR,
    TOK_PRINT,
    TOK_PLUS,
    TOK_MINUS,
    TOK_TIMES,
    TOK_DIVIDED,
    TOK_BY,
    TOK_NAME,
    TOK_COLON,
    TOK_COMMA,
    TOK_NEWLINE,
    TOK_INDENT,
    TOK_EOF,
} TokenType;

typedef struct {
    TokenType type;
    char* text;
    size_t length;
    int indent_level; // For INDENT tokens
} Token;

typedef struct {
    Token* tokens;
    size_t count;
    size_t capacity;
} TokenArray;

// Tokenize source code
TokenArray* tokenize(const char* source);
void token_array_free(TokenArray* array);

// Debug
void token_print(Token* token);

#endif // KRONOS_TOKENIZER_H

