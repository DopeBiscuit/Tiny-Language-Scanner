#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <cctype>


typedef enum {
    SEMICOLON, IF, THEN, END, REPEAT, UNTIL, IDENTIFIER, ASSIGN, READ, WRITE, LESSTHAN, EQUAL, PLUS, MINUS, MULT, DIV, OPENBRACKET, CLOSEDBRACKET, NUMBER, ERROR, ENDFILE
} TOKEN_TYPE;

struct Token {
    std::string value;     // e.g., "x", ":=", "4"
    TOKEN_TYPE type;       // e.g., IDENTIFIER, ASSIGN, NUMBER
};

// Global List to store all valid tokens
std::vector<Token> token_list;


// NOTE: Added {"-", MINUS} explicitly for completeness.
const std::map<std::string, TOKEN_TYPE> RESERVED_WORDS = {
    {"if", IF}, {";", SEMICOLON}, {"then", THEN}, {"end", END}, {"repeat", REPEAT}, {"until", UNTIL}, {":=", ASSIGN}, {"read", READ}, {"write", WRITE}, {"<", LESSTHAN},
    {"=", EQUAL}, {"+", PLUS}, {"-", MINUS}, {"*", MULT}, {"/", DIV}, {"(", OPENBRACKET}, {")", CLOSEDBRACKET}
};


const std::map<TOKEN_TYPE, std::string> type_convert = {
    {SEMICOLON, "SEMICOLON"},
    {IF, "IF"},
    {THEN, "THEN"},
    {END, "END"},
    {REPEAT, "REPEAT"},
    {UNTIL, "UNTIL"},
    {IDENTIFIER, "IDENTIFIER"},
    {ASSIGN, "ASSIGN"},
    {READ, "READ"},
    {WRITE, "WRITE"},
    {LESSTHAN, "LESSTHAN"},
    {EQUAL, "EQUAL"},
    {PLUS, "PLUS"},
    {MINUS, "MINUS"},
    {MULT, "MULT"},
    {DIV, "DIV"},
    {OPENBRACKET, "OPENBRACKET"},
    {CLOSEDBRACKET, "CLOSEDBRACKET"},
    {NUMBER, "NUMBER"},
    {ERROR, "ERROR"},
    {ENDFILE, "ENDFILE"}
};


// --- Function Declarations ---
Token get_token(std::fstream& fin);
std::string TokenTypeToString(TOKEN_TYPE type);
void print_token_table(std::ostream& os);

// ----------------------------------------------------------------------
// --- IMPLEMENTATIONS ---
// ----------------------------------------------------------------------

/**
 * @brief Converts a TOKEN_TYPE enum value to its string representation.
 */
std::string TokenTypeToString(TOKEN_TYPE type) {
    if (type_convert.count(type)) {
        return type_convert.at(type);
    }
    return "UNKNOWN_TYPE"; 
}

/**
 * @brief Prints the scanned token list as a formatted ASCII table.
 * @param os The output stream (std::cout or an std::ofstream).
 */
void print_token_table(std::ostream& os) {
    const int TYPE_WIDTH = 20;
    const int VALUE_WIDTH = 15;
    
    // Set the output stream to left-justify text
    os << std::left; 

    // --- Print Header ---
    os << "\n+";
    os << std::setw(TYPE_WIDTH) << std::setfill('-') << ""; 
    os << "+";
    os << std::setw(VALUE_WIDTH) << ""; 
    os << "+\n";
    
    os << "| " << std::setw(TYPE_WIDTH - 2) << std::setfill(' ') << "TOKEN TYPE" 
              << " | " << std::setw(VALUE_WIDTH - 2) << "TOKEN VALUE" << " |\n";
    
    // --- Print Separator ---
    os << "+";
    os << std::setw(TYPE_WIDTH) << std::setfill('-') << "";
    os << "+";
    os << std::setw(VALUE_WIDTH) << "";
    os << "+\n";

    // --- Print Data Rows ---
    for(const auto& token : token_list) {
        std::string type_str = TokenTypeToString(token.type);
        
        os << "| " << std::setw(TYPE_WIDTH - 2) << std::setfill(' ') << type_str 
                  << " | " << std::setw(VALUE_WIDTH - 2) << token.value << " |\n";
    }

    // --- Print Footer ---
    os << "+";
    os << std::setw(TYPE_WIDTH) << std::setfill('-') << "";
    os << "+";
    os << std::setw(VALUE_WIDTH) << "";
    os << "+\n";
}


/**
 * @brief Extracts TINY token out the file stream, compares it to the reserved token types and identifies it
 */
Token get_token(std::fstream& fin) {
    std::string token_buffer = "";
    char current_char;

    // --- 0. THE START STATE / SKIP LOOP ---
    while (fin.get(current_char)) { 
        
        // Skip pure whitespace
        if (std::isspace(current_char)) {
            continue; 
        }

        // Handle and skip comment blocks
        if (current_char == '{') {
            while (fin.get(current_char) && current_char != '}'){}
            // Check for EOF immediately after loop break
            if (fin.eof()) 
                return (Token){"Unclosed comment", ERROR};
            
            // If we successfully found '}', restart the outer loop to find the next token
            continue;
        }

        // If we reach here, current_char is the START of a token. Break the loop.
        break; 
    }

    // EOF CHECK: If the loop terminated without finding a token-forming char
    if (fin.eof()) 
        return (Token){"", ENDFILE};

    // --- 1. IDENTIFIERS and RESERVED WORDS (Alpha-Only) ---
    if (std::isalpha(current_char)) {
        token_buffer += current_char;
        
        // Loop while the next character is also a letter
        while (std::isalpha(fin.peek())) { 
            fin.get(current_char);
            token_buffer += current_char;
        }

        // Error check: identifiers are alpha-only (no trailing digits).
        if(std::isdigit(fin.peek())){
            fin.get(current_char);
            token_buffer += current_char;
            return (Token){token_buffer, ERROR}; 
        }
        
        // Lookup and return 
        if (RESERVED_WORDS.count(token_buffer)) {
            return (Token){token_buffer, RESERVED_WORDS.at(token_buffer)};
        } else {
            return (Token){token_buffer, IDENTIFIER};
        }
    }

    // --- 2. NUMBERS (Digit Sequence) ---
    else if (std::isdigit(current_char)) {
        token_buffer += current_char;
        
        // Loop while the next character is a digit
        while (std::isdigit(fin.peek())) { 
            fin.get(current_char);
            token_buffer += current_char;
        }
        
        // Check if a letter follows (a-zA-Z) -> Lexical Error
        if (std::isalpha(fin.peek())) { 
            fin.get(current_char);
            token_buffer += current_char;
            return (Token){token_buffer, ERROR}; // e.g., "12x" is an error
        }
        
        return (Token){token_buffer, NUMBER};
    }

    // --- 3. SPECIAL SYMBOLS (Single and Multi-Character) ---
    else {
        
        // ASSIGN (:=)
        if (current_char == ':') {
            if (fin.peek() == '=') {
                fin.get(current_char); // Consume the '='
                return (Token){":=", ASSIGN};
            }
            // Lone ':' is an error in TINY
        }
        
        // Lookup for all single-character tokens (;, +, -, *, /, (, ), =, <)
        token_buffer += current_char;
        if (RESERVED_WORDS.count(token_buffer)) {
            return (Token){token_buffer, RESERVED_WORDS.at(token_buffer)};
        }
        
        // --- 4. CATCH ALL / ERROR ---
        
        // If the character is not handled by any rule
        return (Token){token_buffer, ERROR};
    }
}


int main() {
    // ----------------------------------------
    // --- File Input and Validation ---
    // ----------------------------------------
    std::fstream fin("input.txt");
    if (!fin.is_open()) {
        std::cerr << "ERROR: Failed to open 'input.txt'. Please ensure the file exists." << std::endl;
        return 1;
    }
    
    // ----------------------------------------
    // --- Scanning and Collection ---
    // ----------------------------------------
    Token current_token;
    
    do {
        current_token = get_token(fin);

        if(current_token.type != ERROR && current_token.type != ENDFILE)
            token_list.push_back(current_token);

        if (current_token.type == ERROR) {
            std::cerr << "\n[LEXICAL FAILURE] Unrecognized token '" << current_token.value 
                      << "'. Scan aborted." << std::endl;
            break; 
        }

    } while(current_token.type != ENDFILE);


    // ----------------------------------------
    // --- Output Handling ---
    // ----------------------------------------
    
    // Option A: Print to Terminal (Standard Output)
    std::cout << "\n--- SCANNER OUTPUT (TERMINAL) ---";
    print_token_table(std::cout);

    // Option B: Write to File (output.txt)
    std::ofstream fout("output.txt");
    if (fout.is_open()) {
        fout << "--- SCANNER OUTPUT (FILE: output.txt) ---";
        print_token_table(fout);
        fout.close();
        std::cout << "\n[SUCCESS] Token list also written to 'output.txt'." << std::endl;
    } else {
        std::cerr << "\n[WARNING] Could not open 'output.txt' for writing." << std::endl;
    }

    return 0;
}
