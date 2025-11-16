#include "tokenizer.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Token type to string (for debugging)
static const char *token_type_names[] = {
    "NUMBER", "STRING", "SET",      "TO",     "IF",      "FOR",    "WHILE",
    "IN",     "RANGE",  "FUNCTION", "WITH",   "CALL",    "RETURN", "IS",
    "EQUAL",  "NOT",    "GREATER",  "LESS",   "THAN",    "AND",    "OR",
    "PRINT",  "PLUS",   "MINUS",    "TIMES",  "DIVIDED", "BY",     "NAME",
    "COLON",  "COMMA",  "NEWLINE",  "INDENT", "EOF"};

// Helper to create token array
static TokenArray *token_array_new(void) {
  TokenArray *arr = malloc(sizeof(TokenArray));
  if (!arr)
    return NULL;

  arr->capacity = 32;
  arr->count = 0;
  arr->tokens = malloc(sizeof(Token) * arr->capacity);
  if (!arr->tokens) {
    free(arr);
    return NULL;
  }

  return arr;
}

// Helper to add token
static void token_array_add(TokenArray *arr, Token token) {
  if (arr->count >= arr->capacity) {
    arr->capacity *= 2;
    arr->tokens = realloc(arr->tokens, sizeof(Token) * arr->capacity);
  }
  arr->tokens[arr->count++] = token;
}

// Check if string matches keyword
static TokenType match_keyword(const char *text, size_t len) {
  struct {
    const char *keyword;
    TokenType type;
  } keywords[] = {
      {"set", TOK_SET},
      {"let", TOK_LET},
      {"to", TOK_TO},
      {"as", TOK_AS},
      {"if", TOK_IF},
      {"for", TOK_FOR},
      {"while", TOK_WHILE},
      {"in", TOK_IN},
      {"range", TOK_RANGE},
      {"function", TOK_FUNCTION},
      {"with", TOK_WITH},
      {"call", TOK_CALL},
      {"return", TOK_RETURN},
      {"true", TOK_TRUE},
      {"false", TOK_FALSE},
      {"null", TOK_NULL},
      {"is", TOK_IS},
      {"equal", TOK_EQUAL},
      {"not", TOK_NOT},
      {"greater", TOK_GREATER},
      {"less", TOK_LESS},
      {"than", TOK_THAN},
      {"and", TOK_AND},
      {"or", TOK_OR},
      {"print", TOK_PRINT},
      {"plus", TOK_PLUS},
      {"minus", TOK_MINUS},
      {"times", TOK_TIMES},
      {"divided", TOK_DIVIDED},
      {"by", TOK_BY},
  };

  for (size_t i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++) {
    if (strlen(keywords[i].keyword) == len &&
        strncmp(text, keywords[i].keyword, len) == 0) {
      return keywords[i].type;
    }
  }

  return TOK_NAME;
}

// Tokenize a single line
static void tokenize_line(TokenArray *arr, const char *line, int indent) {
  size_t len = strlen(line);
  size_t col = 0;

  // Add indent token if line is not empty
  if (len > 0) {
    Token tok = {TOK_INDENT, NULL, 0, indent};
    token_array_add(arr, tok);
  }

  while (col < len) {
    // Skip whitespace
    while (col < len && (line[col] == ' ' || line[col] == '\t')) {
      col++;
    }

    if (col >= len)
      break;

    // Numbers
    if (isdigit(line[col])) {
      size_t start = col;
      while (col < len && (isdigit(line[col]) || line[col] == '.')) {
        col++;
      }
      Token tok = {TOK_NUMBER, NULL, col - start, 0};
      tok.text = malloc(tok.length + 1);
      strncpy(tok.text, line + start, tok.length);
      tok.text[tok.length] = '\0';
      token_array_add(arr, tok);
      continue;
    }

    // Strings
    if (line[col] == '"') {
      size_t start = col;
      col++; // Skip opening quote
      while (col < len && line[col] != '"') {
        col++;
      }
      if (col < len)
        col++; // Skip closing quote
      Token tok = {TOK_STRING, NULL, col - start, 0};
      tok.text = malloc(tok.length + 1);
      strncpy(tok.text, line + start, tok.length);
      tok.text[tok.length] = '\0';
      token_array_add(arr, tok);
      continue;
    }

    // Names and keywords
    if (isalpha(line[col]) || line[col] == '_') {
      size_t start = col;
      while (col < len && (isalnum(line[col]) || line[col] == '_')) {
        col++;
      }
      size_t word_len = col - start;
      TokenType type = match_keyword(line + start, word_len);
      Token tok = {type, NULL, word_len, 0};
      tok.text = malloc(tok.length + 1);
      strncpy(tok.text, line + start, tok.length);
      tok.text[tok.length] = '\0';
      token_array_add(arr, tok);
      continue;
    }

    // Single character tokens
    if (line[col] == ':') {
      Token tok = {TOK_COLON, strdup(":"), 1, 0};
      token_array_add(arr, tok);
      col++;
      continue;
    }

    if (line[col] == ',') {
      Token tok = {TOK_COMMA, strdup(","), 1, 0};
      token_array_add(arr, tok);
      col++;
      continue;
    }

    // Unknown character, skip it
    col++;
  }

  // Add newline token if line had content
  if (len > 0) {
    Token tok = {TOK_NEWLINE, strdup("\n"), 1, 0};
    token_array_add(arr, tok);
  }
}

// Tokenize source code
TokenArray *tokenize(const char *source) {
  TokenArray *arr = token_array_new();
  if (!arr)
    return NULL;

  // Split source into lines
  const char *line_start = source;
  const char *line_end;

  while (*line_start) {
    // Find end of line
    line_end = line_start;
    while (*line_end && *line_end != '\n') {
      line_end++;
    }

    // Calculate line length
    size_t line_len = line_end - line_start;

    // Calculate indent
    int indent = 0;
    for (size_t i = 0;
         i < line_len && (line_start[i] == ' ' || line_start[i] == '\t'); i++) {
      indent++;
    }

    // Create a copy of the line (stripped)
    const char *content_start = line_start + indent;
    size_t content_len = line_len - indent;

    if (content_len > 0) {
      char *line = malloc(content_len + 1);
      strncpy(line, content_start, content_len);
      line[content_len] = '\0';

      tokenize_line(arr, line, indent);
      free(line);
    }

    // Move to next line
    line_start = line_end;
    if (*line_start == '\n')
      line_start++;
  }

  // Add EOF token
  Token eof = {TOK_EOF, NULL, 0, 0};
  token_array_add(arr, eof);

  return arr;
}

// Free token array
void token_array_free(TokenArray *array) {
  if (!array)
    return;

  for (size_t i = 0; i < array->count; i++) {
    free(array->tokens[i].text);
  }
  free(array->tokens);
  free(array);
}

// Print token (debug)
void token_print(Token *token) {
  if (!token)
    return;

  printf("%-12s", token_type_names[token->type]);
  if (token->type == TOK_INDENT) {
    printf(" (indent=%d)", token->indent_level);
  } else if (token->text) {
    printf(" '%s'", token->text);
  }
  printf("\n");
}
