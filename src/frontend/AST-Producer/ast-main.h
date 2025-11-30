#pragma once
#include "../tokenizer/tokenizer.h"
#include <vector>
#include <memory>
#include <deque>
#include <string>
#include <iterator>

enum NodeType {
    WhileStatement,
    ImportNode,
    IfStatement,
    ForStatement,
    VariableDecleration,
    FunctionDecleration,
    ModuleStatements,
    Expression,
    SwitchStatement,
    MatchStatement,
    ClassDecleration,
    StructDecleration,
    NamespaceDecleration,
    Block,
    Scope,
    Literal,
    CaseNode,
    Type,
    BreakStatement,
    SkipStatement,
    ReturnStatement,
    ParamHolder,
    ParamBuilder,
    MemoryType,
    Parameters,
    EnumDeclaration,
    TupleLiteral,
    ArrayLiteral,
    prefixOperator,
    suffixOperator,
    PrimaryLiteral,
    GenericType,
    FunctionCaller,
    FunctionCallerParam,
    AttributeHolder,
    TypesForStructs,
    SimpleListOfNodes,
    RangeType,
    PostfixNode,
    IndexAccessor,
    Header,
    GotoStatement,
    MacroSTM,
    InitDecleration,
    File,
    Phi
};

std::string NodeTypeToString(NodeType type);

struct Node {
    NodeType type;
    std::string value;
    std::vector<std::unique_ptr<Node>> children;
    Node() : type(NodeType::Expression), value("") {}
    Node(NodeType t, std::string val = "") : type(t), value(std::move(val)) {}
    std::string stringify(int indent);
};

struct Macro {
    bool isThat;
    std::string name;
    std::vector<Tokenizer::Token> expandoe;
    Macro(std::string name, std::vector<Tokenizer::Token> expandoe) : name(name), expandoe(expandoe) {};
};

// std::vector<std::string> idealTypes;
// std::vector<std::string> varDeclerationKeyword;
// std::vector<std::string> controlFlowKeywords;

struct ASTProducer {
    std::deque<Tokenizer::Token> tokens;
    std::unique_ptr<Node> output;
    int tot;
    ASTProducer(const std::vector<Tokenizer::Token>* tkn);
    Tokenizer::Token eat();
    void Eat();
    bool is(Tokenizer::TokenType type, std::string value);
    std::string expectToken(Tokenizer::TokenType type, std::string id);
    std::string expectToken(std::string value, std::string id);
    bool ExpectToken(std::string value);
    bool ExpectToken(Tokenizer::TokenType type);
    std::unique_ptr<Node> parsePrimary();
    int getPrecedence(const std::string& op);
    std::unique_ptr<Node> parseExpression(int minPrecedence);
    bool matchValues(const std::vector<std::string>& list, Tokenizer::TokenType type);
    std::unique_ptr<Node> expectTYPE();
    std::unique_ptr<Node> expectSCOPE();
    std::unique_ptr<Node> expectBLOCK();
    std::unique_ptr<Node> expectVAR_DEC();
    std::unique_ptr<Node> expectIF();
    std::unique_ptr<Node> expectParameters();
    std::unique_ptr<Node> expectFUNCTION();
    std::unique_ptr<Node> expectRANGE();
    std::unique_ptr<Node> parseSTM();
    std::unique_ptr<Node> expectIMPORT();
    std::unique_ptr<Node> expectSWITCH();
    std::unique_ptr<Node> expectMATCH();
    std::unique_ptr<Node> expectHEADER();
    
    void throwError(std::string msg);
    
    std::unique_ptr<Macro> expectMACRO(); 
};
