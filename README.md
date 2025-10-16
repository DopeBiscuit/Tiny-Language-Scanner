# 🤖 TINY Language Lexical Analyzer (Scanner)

This project implements a complete Lexical Analyzer (Scanner) for the TINY programming language, developed as a core component for a compilers course.

The scanner's primary function is to read TINY source code, ignore comments and whitespace, and break the input down into a structured sequence of tokens, which is the foundational step for the subsequent parsing stage.

## ✨ Features

* **Complete Tokenization**: Recognizes all reserved words (`if`, `then`, `read`, etc.), identifiers (alpha-only), numbers, and special symbols (`:=`, `;`, `+`, `<`, etc.).
* **Finite State Automaton (FSA)**: Implemented using a character-by-character state machine approach for precise token recognition.
* **Robust Error Handling**: Detects and flags lexical errors such as unclosed comments (`{...`) and invalid token structures (e.g., `12x`).
* **Structured Output**: Generates a formatted ASCII table showing the identified Token Type and Token Value for easy analysis.
* **Output Flexibility**: Results are printed to the console and automatically saved to `output.txt`.

## 🛠️ Usage

### Prerequisites

You need a C++ compiler that supports C++11 or later (like g++ or Clang).

### 1. Compilation

Compile the source file using your standard C++ compiler:

```bash
g++ main.cpp -o tiny_scanner
```

### 2. Input File

The program is hardcoded to look for a file named `input.txt` in the same directory as the executable.

Create an `input.txt` file containing your TINY code snippet.

**Example `input.txt`:**

```
read x;
{This is a comment}
if x < 10 then
  x := x + 1;
end
```

### 3. Running the Scanner

Execute the compiled program:

```bash
./tiny_scanner
```

## 📄 Output

The scanner will print the results directly to the console and create an `output.txt` file with the same content.

**Example Console Output:**

```
--- SCANNER OUTPUT (TERMINAL) ---
+--------------------+---------------+
| TOKEN TYPE         | TOKEN VALUE   |
+--------------------+---------------+
| READ               | read          |
| IDENTIFIER         | x             |
| SEMICOLON          | ;             |
| IF                 | if            |
| IDENTIFIER         | x             |
| LESSTHAN           | <             |
| NUMBER             | 10            |
| THEN               | then          |
| IDENTIFIER         | x             |
| ASSIGN             | :=            |
| IDENTIFIER         | x             |
| PLUS               | +             |
| NUMBER             | 1             |
| SEMICOLON          | ;             |
| END                | end           |
+--------------------+---------------+

[SUCCESS] Token list also written to 'output.txt'.
```

## ⚙️ Code Structure Highlights

The core functionality is built around clear C++ design patterns:

* **`TOKEN_TYPE` (enum)**: Defines all possible token categories using compile-time constants for speed and type safety.
* **`Token` (struct)**: A simple structure to bind the token's `value` (the string) and its `type` (the enum).
* **`get_token(fstream& fin)`**: The main Lexical Analyzer function, implementing the FSA logic using `peek()` and `get()` for precise character control.
* **`print_token_table(ostream& os)`**: Handles all output formatting, creating the clean ASCII table structure.