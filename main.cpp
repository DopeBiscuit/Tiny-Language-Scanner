#include <bits/stdc++.h>


typedef enum {
    SEMICOLON, IF, THEN, END, REPEAT, UNTIL, IDENTIFIER, ASSIGN, READ, WRITE, LESSTHAN, EQUAL, PLUS, MINUS, MULT, DIV, OPENBRACKET, CLOSEDBRACKET, NUMBER, ERROR, ENDFILE
} TOKEN_TYPE;

const std::map<std::string, TOKEN_TYPE> RESERVED_WORDS = {
    {"if", IF}, {";", SEMICOLON}, {"then", THEN}, {"end", END}, {"repeat", REPEAT}, {"until", UNTIL}, {":=", ASSIGN}, {"read", READ}, {"write", WRITE}, {"<", LESSTHAN},
    {"=", EQUAL}, {"+", PLUS}, {"-", MINUS}, {"*", MULT}, {"/", DIV}, {"(", OPENBRACKET}, {")", CLOSEDBRACKET}
};

struct Token {
    std::string value;     // e.g., "x", ":=", "4"
    TOKEN_TYPE type;       // e.g., IDENTIFIER, ASSIGN, NUMBER
};

/**
 * @brief Extracts TINY token out the file stream, compares it to the reserved token types and identifies it
 * @param fin input file stream the contains the TINY Language code
 * @return A Token type that binds the token value extracted to its identified type
 */
Token get_token(std::fstream& fin);


int main() {

    // Create the file input stream
    std::fstream fin("input.txt");
    Token current_token;

    while(true){
        current_token = get_token(fin);
    }

}


get_token(std::fstream& fin){
    // TODO: create token extraction logic
}