#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <cctype>
using namespace std;

// Enum for token types
enum TokenType {
    ASSIGN_OP, ADD_OP, SUB_OP, MULT_OP, DIV_OP, MOD_OP,
    LESSER_OP, GREATER_OP, EQUAL_OP, NEQUAL_OP, LEQUAL_OP, GEQUAL_OP,
    BOOL_AND, BOOL_OR, BOOL_NOT,
    IDENT, INT_LIT, KEY_IN, KEY_OUT, KEY_IF, KEY_ELSE,
    OPEN_CURL, CLOSE_CURL, OPEN_PAREN, CLOSE_PAREN, SEMICOLON,
    UNKNOWN
};

// Function to classify keywords and operators
TokenType classifyLexeme(const string& lexeme) {
    static const unordered_map<string, TokenType> keywords = {
        {"input", KEY_IN}, {"output", KEY_OUT}, {"if", KEY_IF}, {"else", KEY_ELSE}
    };
    static const unordered_map<string, TokenType> operators = {
        {"=", ASSIGN_OP}, {"+", ADD_OP}, {"-", SUB_OP}, {"*", MULT_OP}, {"/", DIV_OP}, {"%", MOD_OP},
        {"<", LESSER_OP}, {">", GREATER_OP}, {"==", EQUAL_OP}, {"!=", NEQUAL_OP},
        {"<=", LEQUAL_OP}, {">=", GEQUAL_OP}, {"&&", BOOL_AND}, {"||", BOOL_OR}, {"!", BOOL_NOT},
        {"{", OPEN_CURL}, {"}", CLOSE_CURL}, {"(", OPEN_PAREN}, {")", CLOSE_PAREN}, {";", SEMICOLON}
    };
    if (keywords.count(lexeme)) return keywords.at(lexeme);
    if (operators.count(lexeme)) return operators.at(lexeme);
    if (isdigit(lexeme[0])) return INT_LIT;
    if (isalpha(lexeme[0])) return IDENT;
    return UNKNOWN;
}

// Function to print a token
void printToken(const string& lexeme, TokenType tokenType) {
    cout << lexeme << "\t";
    static const unordered_map<TokenType, string> tokenNames = {
        {ASSIGN_OP, "ASSIGN_OP"}, {ADD_OP, "ADD_OP"}, {SUB_OP, "SUB_OP"}, {MULT_OP, "MULT_OP"},
        {DIV_OP, "DIV_OP"}, {MOD_OP, "MOD_OP"}, {LESSER_OP, "LESSER_OP"}, {GREATER_OP, "GREATER_OP"},
        {EQUAL_OP, "EQUAL_OP"}, {NEQUAL_OP, "NEQUAL_OP"}, {LEQUAL_OP, "LEQUAL_OP"}, {GEQUAL_OP, "GEQUAL_OP"},
        {BOOL_AND, "BOOL_AND"}, {BOOL_OR, "BOOL_OR"}, {BOOL_NOT, "BOOL_NOT"}, {IDENT, "IDENT"},
        {INT_LIT, "INT_LIT"}, {KEY_IN, "KEY_IN"}, {KEY_OUT, "KEY_OUT"}, {KEY_IF, "KEY_IF"},
        {KEY_ELSE, "KEY_ELSE"}, {OPEN_CURL, "OPEN_CURL"}, {CLOSE_CURL, "CLOSE_CURL"},
        {OPEN_PAREN, "OPEN_PAREN"}, {CLOSE_PAREN, "CLOSE_PAREN"}, {SEMICOLON, "SEMICOLON"}, {UNKNOWN, "UNKNOWN"}
    };
    cout << tokenNames.at(tokenType) << "\n";
}

// Helper function to process a multi-character operator
bool handleMultiCharacterOperator(char ch, ifstream& file, string& lexeme) {
    char nextChar = file.peek();
    if ((ch == '!' && nextChar == '=') || (ch == '=' && nextChar == '=') ||
        (ch == '<' && nextChar == '=') || (ch == '>' && nextChar == '=') ||
        (ch == '&' && nextChar == '&') || (ch == '|' && nextChar == '|')) {
        lexeme = ch;
        lexeme += nextChar;
        file.get(); // Consume the next character
        printToken(lexeme, classifyLexeme(lexeme));
        lexeme.clear();
        return true;
    }
    return false;
}

// Lexical analyzer function
void analyzeLexemes(const string& filePath) {
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Error: Could not open file.\n";
        return;
    }

    cout << "Cooke Analyzer :: R11854183\n\n";

    char ch;
    string lexeme;

    while (file.get(ch)) {
        // Handle multi-character operators
        if (ch == '!' || ch == '=' || ch == '<' || ch == '>' || ch == '&' || ch == '|') {
            if (handleMultiCharacterOperator(ch, file, lexeme)) continue;
        }

        // Check for single-character operators or delimiters
        if (isspace(ch) || ch == ';' || ch == '(' || ch == ')' || ch == '{' || ch == '}' ||
            ch == '=' || ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%' ||
            ch == '<' || ch == '>' || ch == '!' || ch == '@') {

            if (!lexeme.empty()) {
                printToken(lexeme, classifyLexeme(lexeme));
                lexeme.clear();
            }

            // Treat `!` as BOOL_NOT when it appears alone
            if (!isspace(ch)) {
                string symbol(1, ch);
                TokenType token = classifyLexeme(symbol);
                printToken(symbol, token);
            }
        }
        // Handle any unrecognized characters as `UNKNOWN`
        else if (!isalnum(ch)) {
            if (!lexeme.empty()) {
                printToken(lexeme, classifyLexeme(lexeme));
                lexeme.clear();
            }
            printToken(string(1, ch), UNKNOWN);
        }
        else {
            // Accumulate characters for identifiers or numbers
            lexeme += ch;
        }
    }

    // Process any remaining lexeme
    if (!lexeme.empty()) {
        printToken(lexeme, classifyLexeme(lexeme));
    }

    file.close();
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: cooke_analyzer <path_to_source_file>\n";
        return 1;
    }

    analyzeLexemes(argv[1]);
    return 0;
}
