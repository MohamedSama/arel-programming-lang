#pragma once
#include <vector>
#include <string>
#include <array>
#include <string_view>

namespace Tokenizer {


inline bool isPunctuator(char c);
inline bool isOperatorChar(char c);

constexpr inline std::array<std::string_view, 61> keywords = {
    "as", "async", "await", "bool", "break", "case", "catch",
    "class", "const", "continue", "default", "delete", "do",
    "double", "else", "enum", "false", "finally", "float",
    "fn", "for", "from", "goto", "if", "implements", "import",
    "in", "init", "int", "interface", "let", "macro", "match", "module",
    "namespace", "new", "null", "phi", "private",
    "protected", "return", "self",
    "sizeof", "skip", "string", "struct", "super", "switch",
    "system", "this", "throw", "true", "try", "type",
    "undefined", "using", "var", "void", "while", "yield", "zeta"
};


enum class TokenType {
    Keyword,
    Identifier,
    Punctuator,
    Operator,
    NumberLiteral,
    StringLiteral,
    EndOfFile,
    Error
};

struct Token {
    TokenType type;
    std::string value;
    
    std::string stringify();
    Token() = default;
    Token(TokenType t, std::string v) : type(t), value(std::move(v)) {}

};

std::string TypeToString(TokenType type);

class Tokenizer {
public:
    std::vector<Token> tokens;
    Tokenizer(std::string fileName, int reservedTokens, int* charlength);
    std::vector<Token>* getTokens();
};

} // namespace Tokenizer
