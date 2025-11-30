#include "tokenizer.h"
#include <fstream>
#include <array>
#include <string_view>
#include <algorithm>
#include <iostream>

namespace Tokenizer {


inline bool isPunctuator(char c) {
    switch (c) {
        case '{': case '}': case '[': case ']': case '(': case ')':
        case ';': case ',': case '<': case '>': return true;
        default: return false;
    }
}

inline bool isOperatorChar(char c) {
    switch (c) {
        case '~': case '!': case '@': case '#': case '$': case '%': case '^':
        case '&': case '*': case '-': case '=': case '+': case ':': case '\\':
        case '|': case '.': case '/': case '?': case '`': return true;
        default: return false;
    }
}


inline bool isKeyword(std::string_view s) {
    return std::binary_search(keywords.begin(), keywords.end(), s);
}
    

// Define Token::stringify
std::string Token::stringify() {
    std::string typeAsString;
    std::string color;
    switch (type) {
        case TokenType::Keyword: typeAsString = "Keyword"; color = "      \033[34m"; break;
        case TokenType::Identifier: typeAsString = "Identifier"; color = "   \033[96m"; break;
        case TokenType::Punctuator: typeAsString = "Punctuator"; color = "   \033[33m"; break;
        case TokenType::Operator: typeAsString = "Operator"; color = "     \033[37m"; break;
        case TokenType::NumberLiteral: typeAsString = "NumberLiteral"; color = "\033[35m"; break;
        case TokenType::StringLiteral: typeAsString = "StringLiteral"; color = "\033[32m"; break;
        case TokenType::EndOfFile: typeAsString = "EndOfFile"; color = "\033[0m"; break;
        case TokenType::Error: typeAsString = "Error"; color = "\033[31m"; break;
        default: typeAsString = "Something Else"; color = "\033[0m"; break;
    }
    return "" + typeAsString + ": " + color + value + "\033[0m\n";
}
std::string TypeToString(TokenType type) {
    switch (type) {
        case TokenType::Keyword: return "Keyword"; break;
        case TokenType::Identifier: return "Identifier"; break;
        case TokenType::Punctuator: return "Punctuator"; break;
        case TokenType::Operator: return "Operator"; break;
        case TokenType::NumberLiteral: return "NumberLiteral"; break;
        case TokenType::StringLiteral: return "StringLiteral"; break;
        case TokenType::EndOfFile: return "EndOfFile"; break;
        case TokenType::Error: return "Error"; break;
        default: return "Something Else"; break;
    }
} 

// Define Tokenizer::Tokenizer constructor
Tokenizer::Tokenizer(std::string fileName, int reservedTokens = 1000, int* charlength = 0) {
    std::ifstream in(fileName);

    if (!in) {
        std::cerr << "file: '" << fileName << "' is not found.!";
        return;
    }

    tokens.reserve(reservedTokens);

    std::string line;
    std::string code = "";
    while (std::getline(in, line)) {
        code += line + "\n";
    }
    *charlength = code.length(); 
    
    enum TokenType curType = TokenType::Error;
    std::string curString = "";

    int i = 0;
    int length = code.length();

    bool isEscaped = false;

    while (i < length) {
        char c = code[i];

        switch (curType) {
            case TokenType::Error:
                if (isalpha(c) || c == '_') {
                    curString = c;
                    curType = TokenType::Identifier;
                } else if (isdigit(c)) {
                    curString = c;
                    curType = TokenType::NumberLiteral;
                } else if (isPunctuator(c)) {
                    tokens.push_back({TokenType::Punctuator, std::string{c}});
                } else if (isOperatorChar(c)) {
                    if (c == '`') {
                        tokens.push_back({TokenType::Operator, "`"});
                        curType = TokenType::Error;
                    } else {
                        curString = c;
                        curType = TokenType::Operator;
                    }
                } else if (c == '\'' || c == '\"') {
                    curString = c;
                    curType = TokenType::StringLiteral;
                } else if (c != ' ' && c != '\n' && c != '\t' && c != '\r' && c != '\v') {
                    tokens.push_back({TokenType::Error, std::string{c}});
                }
                i++;
                break;
            case TokenType::Identifier:
                if (isalpha(c) || c == '_' || isdigit(c)) {
                    curString += c;
                    i++;
                } else {
                    tokens.push_back({isKeyword(curString) ? TokenType::Keyword : TokenType::Identifier, curString});
                    curType = TokenType::Error;
                }
                break;
            case TokenType::Operator:
                if (curString == "/" && c == '/') {
                    while (code[i] != '\n') {
                        i++;
                    }
                    i++;
                    curType = TokenType::Error;
                } else if (curString == "/" && c == '*') {
                    while (code[i] != '*' && code[i + 1] != '/') {
                        i++;
                    }
                    i += 2;
                    curType = TokenType::Error;
                } else if (c == '>' && curString == "-") {
                    i++;
                    tokens.push_back({TokenType::Operator, std::string{curString + '>'}});
                    curType = TokenType::Error;
                } else if (c == '>' && curString == "=") {
                    i++;
                    tokens.push_back({TokenType::Operator, std::string{curString + '>'}});
                    curType = TokenType::Error;
                } else if (isOperatorChar(c)) {
                    curString += c;
                    i++;
                } else {
                    tokens.push_back({TokenType::Operator, curString});
                    curType = TokenType::Error;
                }
                break;
            case TokenType::NumberLiteral:
                if (isalnum(c) || c == '.') {
                    curString += c;
                    i++;
                } else {
                    tokens.push_back({TokenType::NumberLiteral, curString});
                    curType = TokenType::Error;    
                }
                break;
            case TokenType::StringLiteral:
                if (c == '\\' && !isEscaped) {
                    curString += c;
                    isEscaped = true;
                } else if (c == curString[0]) {
                    if (isEscaped) {
                        curString += c;
                        isEscaped = false;
                    } else {
                        tokens.push_back({TokenType::StringLiteral, curString + c});
                        curType = TokenType::Error;
                    }
                } else {
                    if (isEscaped) isEscaped = false;
                    curString += c;
                }
                i++;
                break;
            case TokenType::EndOfFile:
                std::cout << "Unnoticed";
                break;
            default:
                std::cout << "Unnoticed 2";
                break;
                
        }
    }

};

// Define Tokenizer::getTokens
std::vector<Token>* Tokenizer::getTokens() {
    return &tokens;
}

} // namespace Tokenizer
