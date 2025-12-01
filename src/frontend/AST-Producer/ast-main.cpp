#include "../tokenizer/tokenizer.h"
#include "ast-main.h"
#include <iostream>
#include <vector>
#include <memory>
#include <unordered_map>
#include <string>
#include <iterator>

std::string Node::stringify(int indent) {
    std::string result;
    std::string indentStr;
    for (int i = 0; i < indent; i++) indentStr += "|   ";
    result += indentStr;

    result += NodeTypeToString(type) + "(" + value + ")\n";
    for (auto& node : children) {
        if (node) result += node->stringify(indent + 1);
    }
    return result;
}

std::string NodeTypeToString(NodeType type) {
    switch (type) {
        case NodeType::WhileStatement: return "WhileStatement";
        case NodeType::IfStatement: return "IfStatement";
        case NodeType::ForStatement: return "ForStatement";
        case NodeType::VariableDecleration: return "VariableDecleration";
        case NodeType::FunctionDecleration: return "FunctionDecleration";
        case NodeType::ModuleStatements: return "ModuleStatements";
        case NodeType::Expression: return "Expression";
        case NodeType::SwitchStatement: return "SwitchStatement";
        case NodeType::ClassDecleration: return "ClassDecleration";
        case NodeType::StructDecleration: return "StructDecleration";
        case NodeType::NamespaceDecleration: return "NamespaceDecleration";
        case NodeType::Block: return "Block";
        case NodeType::MatchStatement: return "MatchStatement";
        case NodeType::Scope: return "Scope";
        case NodeType::Literal: return "Literal";
        case NodeType::Type: return "Type";
        case NodeType::BreakStatement: return "BreakStatement";
        case NodeType::SkipStatement: return "SkipStatement";
        case NodeType::ReturnStatement: return "ReturnStatement";
        case NodeType::ParamHolder: return "ParamHolder";
        case NodeType::ParamBuilder: return "ParamBuilder";
        case NodeType::MemoryType: return "MemoryType";
        case NodeType::Parameters: return "Parameters";
        case NodeType::EnumDeclaration: return "EnumDeclaration";
        case NodeType::TupleLiteral: return "TupleLiteral";
        case NodeType::ArrayLiteral: return "ArrayLiteral";
        case NodeType::suffixOperator: return "suffixOperator";
        case NodeType::PrimaryLiteral: return "PrimaryLiteral";
        case NodeType::prefixOperator: return "prefixOperator";
        case NodeType::FunctionCallerParam: return "FunctionCallerParam";
        case NodeType::ImportNode: return "ImportNode";
        case NodeType::GenericType: return "GenericType";
        case NodeType::FunctionCaller: return "FunctionCaller";
        case NodeType::AttributeHolder: return "AttributeHolder";
        case NodeType::TypesForStructs: return "TypesForStructs";
        case NodeType::PostfixNode: return "PostfixNode";
        case NodeType::RangeType: return "RangeType";
        case NodeType::CaseNode: return "CaseNode";
        case NodeType::SimpleListOfNodes: return "SimpleListOfNodes";
        case NodeType::IndexAccessor: return "IndexAccessor";
        case NodeType::Header: return "Header";
        case NodeType::MacroSTM: return "MacroSTM";
        case NodeType::GotoStatement: return "GotoStatement";
        case NodeType::InitDecleration: return "InitDecleration";
        case NodeType::File: return "File";
        case NodeType::Phi: return "Phi";
        default: return "Something";
    }
}

std::vector<std::string> idealTypes = std::vector<std::string>({ "int", "float", "double", "char", "string", "auto", "bool", "void" });
std::vector<std::string> varDeclerationKeyword = std::vector<std::string>({ "let", "var", "const" });
std::vector<std::string> controlFlowKeywords = std::vector<std::string>({ "if", "for", "do", "while" });
std::vector<std::string> prefixUnaryOperators = std::vector<std::string>({ "!", "~", "&", "*", "++", "--", "-", "+" });
std::vector<std::string> suffixUnaryOperators = std::vector<std::string>({ "++", "--", "`" });
std::vector<std::string> literableKeywords = std::vector<std::string>({ "this", "self", "system", "true", "false", "null", "infinify", "undefined", "NaN", "super" });
std::vector<std::string> classDecVarAttriubtes = std::vector<std::string>({ "private", "inline", "protected", "internal", "static", "override", "final", "extern" });

ASTProducer::ASTProducer(const std::vector<Tokenizer::Token>* tkn) {
    for (const auto& tk : *tkn) tokens.push_back(tk);
    tot = tokens.size();

    // initer;

    output = std::make_unique<Node>(NodeType::File);
    auto header = this->expectHEADER();
    output->children.push_back(std::move(header));

    // cont
    output->children.push_back(this->expectBLOCK());
}
Tokenizer::Token ASTProducer::eat() {
    Tokenizer::Token token1 = tokens.front();
    tokens.pop_front();
    return token1;
}
void ASTProducer::Eat() {
    tokens.pop_front();
}

bool ASTProducer::is(Tokenizer::TokenType type, std::string value = "") {
    Tokenizer::Token& token = tokens.front();
    return value.empty() ? token.type == type : (token.type == type && token.value == value);
}

std::string ASTProducer::expectToken(Tokenizer::TokenType type, std::string id = "") {
    Tokenizer::Token token = this->eat();
    if (token.type == type) return token.value;
    std::cerr << "Uncaught Token Error: Expected a token of \ntype: " + Tokenizer::TypeToString(type) + "\t - instead got -> " + Tokenizer::TypeToString(token.type) + ": " + token.value + "   at " << (tot - tokens.size()) << "\nid: " + id + "\n";
    // std::cout << output->stringify(0);
    std::string temp;

    std::cout << "\n\n";
    std::cout << output->stringify(0);
    std::cout << "\n\n";
    
    std::cin >> temp;
    return temp;
}
std::string ASTProducer::expectToken(std::string value, std::string id = "") {
    Tokenizer::Token token = this->eat();
    if (token.value == value) return token.value;
    std::cerr << "Uncaught Token Error: Expected a token of \nvalue: " + value + "\t\t - instead got -> " + token.value + "   at " << (tot - tokens.size()) << "\nid: " + id + "\n";
    // std::cout << output->stringify(0);
    std::string temp;
    
    std::cout << "\n\n";
    std::cout << output->stringify(0);
    std::cout << "\n\n";

    std::cin >> temp;
    return temp;
}
bool ASTProducer::ExpectToken(std::string value) {
    Tokenizer::Token& token = tokens.front();
    if (token.value == value) { this->Eat(); return true; };
    return false;
}
bool ASTProducer::ExpectToken(Tokenizer::TokenType type) {
    Tokenizer::Token& token = tokens.front();
    if (token.type == type) { this->Eat(); return true; };
    return false;
}
std::unique_ptr<Node> ASTProducer::expectRANGE() {
    auto node = std::make_unique<Node>(NodeType::RangeType);
    this->expectToken("[", "879768427980693");
    
    do {
        node->children.push_back(this->parseExpression(0));
    } while (this->ExpectToken(","));

    this->expectToken("]", "649850957648368");
    return node;
}
std::unique_ptr<Node> ASTProducer::parsePrimary() {
    auto node = std::make_unique<Node>(NodeType::PrimaryLiteral);

    auto front = tokens.front().value;
    if (front == "if") {
        node->children.push_back(this->expectIF());
    } else if (front == "for") {
        this->Eat();
        this->expectToken("(", "67887894768468");
        auto forNode = std::make_unique<Node>(NodeType::ForStatement);
        if (this->ExpectToken("`")) {
            auto type = this->expectTYPE();
            auto typeDec = std::make_unique<Node>(NodeType::VariableDecleration, this->expectToken(Tokenizer::TokenType::Identifier, "786577495478648"));
            typeDec->children.push_back(std::move(type));
            forNode->children.push_back(std::move(typeDec));

            this->expectToken(":", "4484968799789");
            auto range = this->parseExpression(0);
            forNode->children.push_back(std::move(range));
            this->expectToken(";", "78790486934789");
        } else {
            forNode->children.push_back(this->expectVAR_DEC());
            this->expectToken(";", "5849674869038");
            forNode->children.push_back(this->parseExpression(0));
            this->expectToken(";", "747847678673889");
            forNode->children.push_back(this->parseExpression(0));
            this->expectToken(";", "768948967490678");
        }
        forNode->children.push_back(this->expectVAR_DEC());
        this->expectToken(")", "789768948649794");
        forNode->children.push_back(this->expectSCOPE());
        node->children.push_back(std::move(forNode));
    } else if (front == "$") {
        this->Eat();
        if (this->ExpectToken("(")) {
            node->value = "-tuple";
            while (!this->is(Tokenizer::TokenType::Punctuator, ")")) {
                node->children.push_back(this->parseExpression(0));
                if (!this->ExpectToken(",")) break;
            }
            this->expectToken(")", "767849784789");
        }
    } else if (front == "[") {
        this->Eat();
        node->value = "-array";
        while (!this->is(Tokenizer::TokenType::Punctuator, "]")) {
            node->children.push_back(this->parseExpression(0));
            if (!this->ExpectToken(",")) break;
        }
        this->expectToken("]", "789787947874");
    } else if (front == "{") {
        this->Eat();
        node->value = "-typecast";
        node->children.push_back(this->expectTYPE());
        this->expectToken("}", "6890578457849");
        node->children.push_back(this->parseExpression(0));
    } else if (front == "fn") {
        this->Eat();
        node->value = "-lambda-fn";
        tokens.push_front(Tokenizer::Token {Tokenizer::TokenType::Identifier, ""});
        tokens.push_front(Tokenizer::Token {Tokenizer::TokenType::Keyword, "fn"});
        node->children.push_back(this->expectFUNCTION());
    } else if (front == "new") {
        this->Eat();
        node->value = "-new";
        while (!this->is(Tokenizer::TokenType::Punctuator, "(") && !this->is(Tokenizer::TokenType::Punctuator, "<")) {
            auto& curT = tokens.front();
            if (curT.type == Tokenizer::TokenType::Identifier || curT.value == "system") {
                node->children.push_back(std::make_unique<Node>(NodeType::Literal, this->eat().value));
                if (!this->ExpectToken("::")) break;
            } else {
                this->expectToken(Tokenizer::TokenType::Error, "667890487567489");
            }
        }
        if (this->ExpectToken("<")) {
            auto genericListNode = std::make_unique<Node>(NodeType::GenericType);
            do {
                genericListNode->children.push_back(this->expectTYPE());
            } while (!this->ExpectToken(">") && this->ExpectToken(","));
            node->children.push_back(std::move(genericListNode));
        }
        this->expectToken("(", "77590584060679870");
        node->children.push_back(this->expectParameters());
        this->expectToken(")", "267579857645789");
    } else if (front == "match") {
        node->children.push_back(this->expectMATCH());
    } else if (front == "phi") {
        this->Eat();
        node->type = NodeType::Phi;
        do {
            auto identName = this->expectToken(Tokenizer::TokenType::Identifier, "63987394857987");
            this->expectToken("from", "63987394857987");
            auto blockName = this->expectToken(Tokenizer::TokenType::Identifier, "58439458767659");

            auto node2 = std::make_unique<Node>(NodeType::SimpleListOfNodes, "-pair");
            node2->children.push_back(std::make_unique<Node>(NodeType::Literal, identName));
            node2->children.push_back(std::make_unique<Node>(NodeType::Literal, blockName));
            node->children.push_back(std::move(node2));
        } while (this->ExpectToken(","));
    } else {
        // prefix
        if (this->matchValues(prefixUnaryOperators, Tokenizer::TokenType::Operator)) {
            node->children.push_back(std::make_unique<Node>(NodeType::prefixOperator, this->eat().value));
        }

        // actual literal
        Tokenizer::Token frontT = tokens.front();
        if (
            frontT.type == Tokenizer::TokenType::Identifier ||
            frontT.type == Tokenizer::TokenType::NumberLiteral ||
            frontT.type == Tokenizer::TokenType::StringLiteral ||
            this->matchValues(literableKeywords, Tokenizer::TokenType::Keyword)
        ) {
            node->children.push_back(std::make_unique<Node>(NodeType::Literal, this->eat().value));
        } else if (frontT.value == "(") {
            this->Eat();
            node->children.push_back(this->parseExpression(0));
            this->expectToken(")", "7468908976988");
        } else this->expectToken(Tokenizer::TokenType::Error, "67898689489786");

        // postfixes
        auto front = tokens.front().value;
        while (
            front == "<" ||
            front == "(" ||
            front == "["
        ) {
            auto postfix = std::make_unique<Node>(NodeType::PostfixNode);
            if (front == "<") {
                this->Eat();

                auto genericListNode = std::make_unique<Node>(NodeType::GenericType);
                do {
                    genericListNode->children.push_back(this->expectTYPE());
                } while (!this->ExpectToken(">") && this->ExpectToken(","));
                postfix->children.push_back(std::move(genericListNode));
                
                this->expectToken("(", "6776948647980");
                postfix->children.push_back(this->expectParameters());
                this->expectToken(")", "78847897689");
            } else if (front == "(") {
                this->Eat();
                postfix->children.push_back(this->expectParameters());
                this->expectToken(")", "594768949");
            } else if (front == "[") {
                auto indexNode = std::make_unique<Node>(NodeType::IndexAccessor);
                do {
                    indexNode->children.push_back(this->parseExpression(0));
                } while (!this->ExpectToken("]") && this->ExpectToken(","));
                postfix->children.push_back(std::move(indexNode));
            } else {
                this->expectToken(Tokenizer::TokenType::Error, "849684684869848");
            }
            node->children.push_back(std::move(postfix));
            front = tokens.front().value;
        }

        // suffix
        if (this->matchValues(suffixUnaryOperators, Tokenizer::TokenType::Operator)) {
            node->children.push_back(std::make_unique<Node>(NodeType::suffixOperator, this->eat().value));
        }

    }
    return std::move(node);
}
int ASTProducer::getPrecedence(const std::string& op) {
    if (op == "+" || op == "-") return 0;
    if (op == "*" || op == "/" || op == "%") return 1;
    if (op == "**") return 2;
    if (op == "<" || op == ">" || op == "<=" || op == ">=") return 3;
    if (op == "==" || op == "!=") return 4;
    if (op == "&&") return 5;
    if (op == "||") return 6;
    if (op == "=") return 7;
    return 8;
}


std::unique_ptr<Node> ASTProducer::parseExpression(int minPrecedence = 0) {

    auto left = this->parsePrimary();

    while (!tokens.empty()) {
        const auto& tok = tokens.front();
        if (tok.type != Tokenizer::TokenType::Operator) break;

        int precedence = this->getPrecedence(tok.value);
        if (precedence < minPrecedence) break;

        std::string op = tok.value;
        this->Eat(); // consume operator

        auto right = this->parseExpression(precedence + 1);

        auto node = std::make_unique<Node>(NodeType::Expression, op);
        node->children.push_back(std::move(left));
        node->children.push_back(std::move(right));
        left = std::move(node);
    }

    return left;
}
bool ASTProducer::matchValues(const std::vector<std::string>& list, Tokenizer::TokenType type) {
    auto tk1 = tokens.front();
    if (type != tk1.type) return false;
    for (auto& val : list) {
        if (val == tk1.value) return true;
    }
    return false;
}
std::unique_ptr<Node> ASTProducer::expectTYPE() {
    auto node = std::make_unique<Node>(NodeType::Type);
    this->ExpectToken("type");
    if (this->matchValues(idealTypes, Tokenizer::TokenType::Keyword)) {
        node->value = this->eat().value;
        if (this->ExpectToken(":")) {
            auto node2 = std::make_unique<Node>(NodeType::Literal, this->expectToken(Tokenizer::TokenType::Identifier, "7587624292"));
            node->children.push_back(std::move(node2));
        }
        if (this->ExpectToken("<")) {
            do {
                node->children.push_back(this->expectTYPE());
            } while (this->ExpectToken(","));
            this->expectToken(">", "9826429429856");
        }
    } else if (tokens.front().value == "fn") {
        this->Eat();
        node->value = "-fn";
        auto retType = this->expectTYPE();
        this->expectToken("-", "467486789678");
        this->expectToken("(", "756856349");
        auto typeList = std::make_unique<Node>(NodeType::ParamHolder);
        while (!this->is(Tokenizer::TokenType::Punctuator, ")")) {
            typeList->children.push_back(this->expectTYPE());
            if (!this->ExpectToken(",")) break;
        }
        this->expectToken(")", "87676859789");
        node->children.push_back(std::move(typeList));
        node->children.push_back(std::move(retType));
    } else {
        node->children.push_back(std::make_unique<Node>(NodeType::Literal, this->is(Tokenizer::TokenType::Keyword, "system") ? this->eat().value : this->expectToken(Tokenizer::TokenType::Identifier, "2345678998765431")));
        
        while (this->ExpectToken("::")) {
            node->children.push_back(std::make_unique<Node>(NodeType::Literal, this->expectToken(Tokenizer::TokenType::Identifier, "234567899810431")));
        }

        if (this->ExpectToken("<")) {
            do {
                node->children.push_back(this->expectTYPE());
            } while (this->ExpectToken(","));
            this->expectToken(">", "9826429429856");
        }
    }
    auto& front = tokens.front().value;
    if (front == "*" || front == "@") {
        node->children.push_back(std::make_unique<Node>(NodeType::MemoryType, this->eat().value));
    }

    std::unique_ptr<Node> result = std::move(node);
    while (tokens.front().value == "[" || tokens.front().value == "(") {
        auto front3 = this->eat().value;
        auto StructT = std::make_unique<Node>(NodeType::TypesForStructs);
        if (front3 == "(") {
            StructT->value = "-tuple";
            if (tokens.front().value != ")") {
                StructT->children.push_back(this->parseExpression(0));
                this->expectToken(")", "65859084378");
            } else this->Eat();
        } else {
            StructT->value = "-array";
            if (tokens.front().value != "]") {
                StructT->children.push_back(this->parseExpression(0));
                this->expectToken("]", "567483923874");
            } else this->Eat();
        }

        auto& front2 = tokens.front().value;
        if (front2 == "*" || front2 == "@") {
            StructT->children.push_back(std::make_unique<Node>(NodeType::MemoryType, this->eat().value));
        }

        StructT->children.push_back(std::move(result));
        result = std::move(StructT);
    }

    return result;
}

std::unique_ptr<Node> ASTProducer::expectSCOPE() {
    auto node = std::make_unique<Node>(NodeType::Scope);

    if (tokens[1].value == ":" && tokens[0].type == Tokenizer::TokenType::Identifier) {
        node->value = this->eat().value;
        this->Eat();
    }
    std::string curT = tokens.front().value;
    if (curT != "{" && curT != "=>" && curT != "->") {
        node->children.push_back(this->expectTYPE());
    } else {
        node->children.push_back(nullptr);
    }
    if (this->ExpectToken("{")) {
        node->children.push_back(this->expectBLOCK());
        this->expectToken("}", "0836549473");
    } else if (this->ExpectToken("=>")) {
        auto block = std::make_unique<Node>(NodeType::Block);
        auto STM = std::make_unique<Node>(NodeType::ReturnStatement, node->value);
        STM->children.push_back(this->parseExpression(0));
        block->children.push_back(std::move(STM));
        node->children.push_back(std::move(block));
    } else if (this->ExpectToken("->")) {
        auto block = std::make_unique<Node>(NodeType::Block);
        block->children.push_back(this->parseSTM());
        node->children.push_back(std::move(block));
    } else if (this->ExpectToken("$")) {
        this->expectToken("{", "875743987465784");
        while (this->ExpectToken("%")) {
            auto name = this->expectToken(Tokenizer::TokenType::Identifier, "890938745849398475");
            this->expectToken(":", "7893847894857");
            auto block = this->expectBLOCK();
            block->value = name;
            node->children.push_back(std::move(block));
        }
        this->expectToken("}", "039489304982039893049");
    } else {
        this->expectToken(Tokenizer::TokenType::Error, "677896578657389");
    }
    return node;
}

std::unique_ptr<Node> ASTProducer::parseSTM() {
    auto& tk1 = tokens.front();
    if (this->matchValues(varDeclerationKeyword, Tokenizer::TokenType::Keyword) || tk1.value == "type") {
        return this->expectVAR_DEC();
    } else if (tk1.type == Tokenizer::TokenType::Keyword) {
        if (tk1.value == "if") return this->expectIF();
        else if (tk1.value == "while") {
            auto node2 = std::make_unique<Node>(NodeType::WhileStatement);

            this->expectToken("while", "674839293847");
            this->expectToken("(", "782758995829");
            node2->children.push_back(this->parseExpression(0));
            this->expectToken(")", "765784658468");
            
            node2->children.push_back(this->expectSCOPE());
            
            if (this->ExpectToken("else")) {
                node2->children.push_back(this->expectSCOPE());
            }
            
            return std::move(node2);
        } else if (tk1.value == "for") {
            this->Eat();
            auto node2 = std::make_unique<Node>(NodeType::ForStatement);
            this->expectToken("(", "782758995829");
            if (this->ExpectToken("`")) {
                auto type = this->expectTYPE();
                auto typeDec = std::make_unique<Node>(NodeType::VariableDecleration, this->expectToken(Tokenizer::TokenType::Identifier, "786577495478648"));
                typeDec->children.push_back(std::move(type));
                node2->children.push_back(std::move(typeDec));

                this->expectToken(":", "6578908745784988");
                auto range = this->parseExpression(0);
                node2->children.push_back(std::move(range));
            } else {
                node2->children.push_back(this->expectVAR_DEC());
                this->expectToken(";", "578476898");
                node2->children.push_back(this->parseExpression(0));
                this->expectToken(";", "8767898978");
                node2->children.push_back(this->parseExpression(0));
            }
            
            this->expectToken(")", "868768347");
            node2->children.push_back(this->expectSCOPE());
            
            if (this->ExpectToken("else")) {
                node2->children.push_back(this->expectSCOPE());
            }
            return std::move(node2);
        } else if (tk1.value == "switch") {
            return this->expectSWITCH();
        } else if (tk1.value == "break") {
            auto SimpleSTMNode = std::make_unique<Node>(NodeType::BreakStatement);
            this->Eat();
            if (tokens.front().type == Tokenizer::TokenType::Identifier) {
                SimpleSTMNode->value = this->expectToken(Tokenizer::TokenType::Identifier, "782457986937489");
            }
            return std::move(SimpleSTMNode);
        } else if (tk1.value == "continue") {
            auto SimpleSTMNode = std::make_unique<Node>(NodeType::SkipStatement);
            this->Eat();
            if (tokens.front().type == Tokenizer::TokenType::Identifier) {
                SimpleSTMNode->value = this->expectToken(Tokenizer::TokenType::Identifier, "782457986937489");
            }
            SimpleSTMNode->children.push_back(std::make_unique<Node>(NodeType::Literal, "1"));
            return std::move(SimpleSTMNode);
        } else if (tk1.value == "skip") {
            auto SimpleSTMNode = std::make_unique<Node>(NodeType::SkipStatement);
            this->Eat();
            if (this->ExpectToken(":")) {
                SimpleSTMNode->value = this->expectToken(Tokenizer::TokenType::Identifier, "784890378964");
            }
            SimpleSTMNode->children.push_back(this->parseExpression(0));
            return std::move(SimpleSTMNode);
        } else if (tk1.value == "goto") {
            this->Eat();
            auto token = this->eat();
            if (token.type == Tokenizer::TokenType::Identifier || token.value == "default") {
                auto SimpleSTMNode = std::make_unique<Node>(NodeType::GotoStatement, token.value);
                return SimpleSTMNode;
            } else return std::make_unique<Node>(NodeType::Literal, this->expectToken(Tokenizer::TokenType::Error, "779858949649"));
        } else if (tk1.value == "return") {
            this->Eat();
            auto node2 = std::make_unique<Node>(NodeType::ReturnStatement);
            if (this->ExpectToken(":")) {
                node2->value = this->expectToken(Tokenizer::TokenType::Identifier, "4579867876456798");
            }
            node2->children.push_back(this->parseExpression(0));
            return std::move(node2);
        } else if (tk1.value == "fn") {
            return this->expectFUNCTION();
        } else if (tk1.value == "enum") {
            this->Eat();
            auto node2 = std::make_unique<Node>(NodeType::EnumDeclaration, this->expectToken(Tokenizer::TokenType::Identifier, "6468459297"));
            this->expectToken("{", "684509478579");
            do {
                if (tokens.front().value == "}") break;
                node2->children.push_back(std::make_unique<Node>(NodeType::Literal, this->expectToken(Tokenizer::TokenType::Identifier, "7687803476838")));
            } while (this->ExpectToken(","));
            this->expectToken("}", "684509478578");
            return std::move(node2);
        } else if (tk1.value == "struct") {
            this->Eat();
            auto node2 = std::make_unique<Node>(NodeType::StructDecleration, this->expectToken(Tokenizer::TokenType::Identifier, "646845956297"));

            if (this->ExpectToken("<")) {
                auto genericTypesList = std::make_unique<Node>(NodeType::GenericType);
                do {
                    genericTypesList->children.push_back(std::make_unique<Node>(NodeType::Literal, this->expectToken(Tokenizer::TokenType::Identifier, "096839333")));
                } while (this->ExpectToken(","));
                this->expectToken(">", "897984684963489");
                node2->children.push_back(std::move(genericTypesList));
            }

            this->expectToken("{", "6258946t7379");
            while (!this->ExpectToken("}")) {
                node2->children.push_back(this->expectVAR_DEC());
                this->expectToken(";", "66478765348798");
            }
            return std::move(node2);
        } else if (this->matchValues(literableKeywords, Tokenizer::TokenType::Keyword)) {
            return this->parseExpression(0);
        } else if (tk1.value == "class") {
            this->Eat();
            auto node_2 = std::make_unique<Node>(NodeType::ClassDecleration, this->expectToken(Tokenizer::TokenType::Identifier, "658475479768487"));

            if (this->ExpectToken("<")) {
                auto genericTypesList = std::make_unique<Node>(NodeType::GenericType);
                do {
                    genericTypesList->children.push_back(std::make_unique<Node>(NodeType::Literal, this->expectToken(Tokenizer::TokenType::Identifier, "096839333")));
                } while (this->ExpectToken(","));
                this->expectToken(">", "897984684963489");
                node_2->children.push_back(std::move(genericTypesList));
            }

            auto extendersNode = std::make_unique<Node>(NodeType::SimpleListOfNodes, "classExtenders");

            if (this->ExpectToken(":")) {
                do {
                    extendersNode->children.push_back(std::make_unique<Node>(NodeType::Literal, this->expectToken(Tokenizer::TokenType::Identifier, "867348997439")));
                } while (this->ExpectToken(","));
            }
            node_2->children.push_back(std::move(extendersNode));
            this->expectToken("{", "65879826787");

            auto blockNode = std::make_unique<Node>(NodeType::Block);

            auto attrNode = std::make_unique<Node>(NodeType::AttributeHolder);

            while (this->ExpectToken("@")) {
                attrNode->children.push_back(std::make_unique<Node>(NodeType::Literal, this->expectToken(Tokenizer::TokenType::Identifier, "87429678")));
            }
            if (tokens.front().value == "struct") {
                this->Eat(); this->expectToken("(", "935728459");
                auto basicStructDec = std::make_unique<Node>(NodeType::StructDecleration);
                basicStructDec->children.push_back(this->expectTYPE());
                node_2->children.push_back(std::move(basicStructDec));
                this->expectToken(")", "67905937678");
                this->expectToken(";", "9085793706");
            }
            while (!this->ExpectToken("}")) {
                auto attrNode = std::make_unique<Node>(NodeType::AttributeHolder);

                while (this->ExpectToken("@")) {
                    attrNode->children.push_back(std::make_unique<Node>(NodeType::Literal, this->expectToken(Tokenizer::TokenType::Identifier, "87429678")));
                }
                auto front = tokens.front().value;
                if (front == "fn") {
                    auto node2 = this->expectFUNCTION();
                    node2->children.push_back(std::move(attrNode));
                    blockNode->children.push_back(std::move(node2));
                } else if (front == "init") {
                    tokens.front().type = Tokenizer::TokenType::Identifier;
                    tokens.push_front(Tokenizer::Token{Tokenizer::TokenType::Keyword, "fn"});
                    blockNode->children.push_back(this->expectFUNCTION());
                } else if (front == "~") {
                    this->Eat();
                    auto& curT = tokens.front();
                    curT.type = Tokenizer::TokenType::Identifier;
                    curT.value = "~init";
                    tokens.push_front(Tokenizer::Token{Tokenizer::TokenType::Keyword, "fn"});
                    blockNode->children.push_back(this->expectFUNCTION());
                } else {
                    auto vardec = this->expectVAR_DEC();
                    this->expectToken(";", "767678768978738");
                    vardec->children.push_back(std::move(attrNode));
                    blockNode->children.push_back(std::move(vardec));
                }
            }
            node_2->children.push_back(std::move(blockNode));
            return std::move(node_2);
        } else if (tk1.value == "namespace") {
            this->Eat();
            auto NamespaceNode = std::make_unique<Node>(NodeType::NamespaceDecleration, this->expectToken(Tokenizer::TokenType::Identifier, "878947643"));
            this->expectToken("{", "768468947948");
            NamespaceNode->children.push_back(this->expectBLOCK());
            this->expectToken("}", "496946839");
            return std::move(NamespaceNode);
        } else if (tk1.value == "do") {
            this->Eat();

            auto whilestm = std::make_unique<Node>(NodeType::WhileStatement);

            whilestm->children.push_back(this->expectSCOPE());
            this->expectToken("while", "795829578468309");
            this->expectToken("(", "57487847");
            whilestm->children.push_back(this->parseExpression(0));
            this->expectToken(")", "87878687");
            return std::move(whilestm);
        } else if (tk1.value == "init") {
            this->Eat();
            auto node = std::make_unique<Node>(NodeType::InitDecleration);
            auto type = this->expectTYPE();
            type->children.push_back(std::make_unique<Node>(NodeType::MemoryType, "*"));
            node->children.push_back(std::move(type));
            node->value = this->expectToken(Tokenizer::TokenType::Identifier, "876567890654");
            if (this->ExpectToken("=")) {
                node->children.push_back(this->parseExpression(0));
            }
            return node;
        } else return std::make_unique<Node>(NodeType::Literal);
    } else if (tk1.type == Tokenizer::TokenType::Identifier) {
        auto exp = this->parseExpression(0);
        return std::move(exp);
    } else if (tk1.value == "@") {
        auto attrNode = std::make_unique<Node>(NodeType::AttributeHolder);
        while (this->ExpectToken("@")) {
            auto litVal = this->expectToken(Tokenizer::TokenType::Identifier, "675874786");
            if (litVal == "operator") {
                if (tokens.front().type == Tokenizer::TokenType::Operator) {
                    tokens.front().type = Tokenizer::TokenType::Identifier;
                    tokens.push_front(Tokenizer::Token{Tokenizer::TokenType::Keyword, "fn"});
                }
                attrNode->children.push_back(std::make_unique<Node>(NodeType::Literal, litVal));
                break;
            }
            attrNode->children.push_back(std::make_unique<Node>(NodeType::Literal, litVal));
        }

        if (tokens.front().value == "fn") {
            auto fnNode = this->expectFUNCTION();
            fnNode->children.push_back(std::move(attrNode));
            return std::move(fnNode);
        } else {
            auto varNode = this->expectVAR_DEC();
            this->expectToken(";", "778969347868");
            varNode->children.push_back(std::move(attrNode));
            return std::move(varNode);
        }
    } else if (tk1.value == "$") {
        this->Eat();
        this->expectToken("{", "784309857684945");
        auto list = std::make_unique<Node>(NodeType::SimpleListOfNodes);
        while (this->ExpectToken("%")) {
            auto name = this->expectToken(Tokenizer::TokenType::Identifier, "75807658986758");
            this->expectToken(":", "857909548953456");

            auto block = this->expectBLOCK();
            block->value = name;
            list->children.push_back(std::move(block));
        }
        this->expectToken("}", "67489387456789");
        return list;
    } else {
        auto exp = this->parseExpression(0);
        return exp;
    }
    return std::make_unique<Node>(NodeType::Literal, this->expectToken(Tokenizer::TokenType::Error, "867589768967"));
}
std::unique_ptr<Node> ASTProducer::expectFUNCTION() {
    this->expectToken("fn", "4783920203");

    auto node2 = std::make_unique<Node>(NodeType::FunctionDecleration, this->expectToken(Tokenizer::TokenType::Identifier, "76564957467866332"));                    
    
    if (this->ExpectToken("<")) {
        do {
            node2->children.push_back(std::make_unique<Node>(NodeType::GenericType, this->expectToken(Tokenizer::TokenType::Identifier, "0188756435795838")));
        } while (this->ExpectToken(","));


        this->expectToken(">", "6784676783687");
    } else {
        node2->children.push_back(nullptr);
    }
    
    this->expectToken("(", "64783920203");
    auto paramHolder = std::make_unique<Node>(NodeType::ParamHolder);

    do {
        if (tokens.front().value == ")") break;
        auto param = std::make_unique<Node>(NodeType::ParamBuilder);

        param->children.push_back(this->expectTYPE());
        param->value = this->expectToken(Tokenizer::TokenType::Identifier, "678929437");

        if (this->ExpectToken("=")) {
            param->children.push_back(this->parseExpression(0));
        }
        paramHolder->children.push_back(std::move(param));
    } while (this->ExpectToken(","));

    node2->children.push_back(std::move(paramHolder));

    this->expectToken(")", "64763920203");

    node2->children.push_back(this->expectSCOPE());
    return node2;
}


std::unique_ptr<Node> ASTProducer::expectBLOCK() {
    auto node = std::make_unique<Node>(NodeType::Block);
    auto curT = tokens.front().value;
    while (!tokens.empty()) {
        if (curT == "}" || curT == "case" || curT == "else" || curT == "%") break;
        auto STM = this->parseSTM();
        if (STM->type == NodeType::SimpleListOfNodes)
            node->children.insert(node->children.end(), std::make_move_iterator(STM->children.begin()), std::make_move_iterator(STM->children.end()));
        else
            node->children.push_back(std::move(STM));
        this->expectToken(";", "04769885947564789");
        curT = tokens.front().value;
    }
    return node;
}
std::unique_ptr<Node> ASTProducer::expectParameters() {
    auto node = std::make_unique<Node>(NodeType::Parameters);
    while (!this->is(Tokenizer::TokenType::Punctuator, ")")) {
        if (this->ExpectToken("@")) {
            node->children.push_back(std::make_unique<Node>(NodeType::Literal, this->expectToken(Tokenizer::TokenType::Identifier, "77588989797")));
            this->expectToken("=", "8489375749");
            node->children.push_back(this->parseExpression(0));
        } else {
            node->children.push_back(this->parseExpression(0));
        }
        if (!this->ExpectToken(",")) break;
    };
    return node;
}

std::unique_ptr<Node> ASTProducer::expectVAR_DEC() {
    auto node = std::make_unique<Node>(NodeType::VariableDecleration);
    std::string tk_val = tokens.front().value;
    if (tk_val == "const" || tk_val == "var" || tk_val == "let") node->children.push_back(std::make_unique<Node>(NodeType::Literal, this->eat().value));
    node->children.push_back(this->expectTYPE());
    
    node->value = this->expectToken(Tokenizer::TokenType::Identifier, "8737439792");

    if (this->ExpectToken("=")) {
        node->children.push_back(this->parseExpression(0));
    } else if (this->ExpectToken("(")) {
        node->children.push_back(this->expectParameters());
        this->expectToken(")", "75483902938");
    } else {
    }
    return node;
}

std::unique_ptr<Node> ASTProducer::expectIF() {
    auto node = std::make_unique<Node>(NodeType::IfStatement, "");
    
    this->expectToken("if", "5457845678");
    this->expectToken("(", "757897918");
    node->children.push_back(this->parseExpression(0));
    this->expectToken(")", "545892337050");
    node->children.push_back(this->expectSCOPE());
    
    while (this->ExpectToken("else")) {
        if (this->ExpectToken("if")) {
            this->expectToken("(", "7576587465");
            node->children.push_back(this->parseExpression(0));
            this->expectToken(")", "27952851");
            node->children.push_back(this->expectSCOPE());
        } else {
            node->children.push_back(this->expectSCOPE());
            break;
        }
    }
    return node;
}
std::unique_ptr<Node> ASTProducer::expectIMPORT() {
    this->expectToken("import", "40876346787");
    auto importNode = std::make_unique<Node>(NodeType::ImportNode);
    importNode->children.push_back(std::make_unique<Node>(NodeType::Literal, this->expectToken(Tokenizer::TokenType::Identifier, "8496348679")));
    if (this->ExpectToken("as")) {
        importNode->children.push_back(std::make_unique<Node>(NodeType::Literal, this->expectToken(Tokenizer::TokenType::Identifier, "7688898")));
    } else {
        importNode->children.push_back(nullptr);
    }
    if (this->ExpectToken("from")) {
        auto curT = tokens.front().type;
        if (curT == Tokenizer::TokenType::Identifier || curT == Tokenizer::TokenType::StringLiteral) importNode->children.push_back(std::make_unique<Node>(NodeType::Literal, this->eat().value));
        else importNode->children.push_back(nullptr);
    } else {
    }
    this->expectToken(";", "789036367898");
    return std::move(importNode);
}
std::unique_ptr<Node> ASTProducer::expectSWITCH() {
    auto node = std::make_unique<Node>(NodeType::SwitchStatement);
    this->expectToken("switch", "7868796706939");
    this->expectToken("(", "49678698763797");
    node->children.push_back(this->parseExpression(0));
    this->expectToken(")", "69869603989");
    this->expectToken("{", "7847986785485");
    
    while (!this->ExpectToken("}")) {

        if (this->is(Tokenizer::TokenType::Keyword, "case")) {    
            auto caseNode = std::make_unique<Node>(NodeType::CaseNode);
            this->expectToken("case", "854976574947");
            do {
                auto front = tokens.front().type;
                if (front == Tokenizer::TokenType::Identifier || front == Tokenizer::TokenType::NumberLiteral) {
                    caseNode->children.push_back(std::make_unique<Node>(NodeType::Literal, this->eat().value));
                } else this->expectToken(Tokenizer::TokenType::Identifier, "796487347866");
            } while (this->ExpectToken(","));
            this->expectToken(":", "789896078658908");
            caseNode->children.push_back(this->expectBLOCK());
            node->children.push_back(std::move(caseNode));
        } else {
            auto caseNode = std::make_unique<Node>(NodeType::CaseNode);
            caseNode->value = "-else-case";
            this->expectToken("else", "854976574947");
            this->expectToken(":", "789896078658908");
            caseNode->children.push_back(this->expectBLOCK());
            node->children.push_back(std::move(caseNode));
        }
    }
    return std::move(node);
}
std::unique_ptr<Node> ASTProducer::expectMATCH() {
    auto node = std::make_unique<Node>(NodeType::MatchStatement);
    this->expectToken("match", "7848947674876984");
    this->expectToken("(", "7886379683890368");
    node->children.push_back(this->parseExpression(0));
    this->expectToken(")", "7886379683890368");

    this->expectToken("{", "9469047849");
    
    do {
        auto caseNode = std::make_unique<Node>(NodeType::CaseNode);
        if (this->ExpectToken("else")) {
            this->expectToken(">", "847896378");
            caseNode->value = "-else-case";
            caseNode->children.push_back(this->parseExpression(0));
            node->children.push_back(std::move(caseNode));
            this->expectToken("}", "49468695085795");
            break;
        } else {
            auto front = tokens.front().type;
            if (
                front == Tokenizer::TokenType::NumberLiteral ||
                front == Tokenizer::TokenType::Identifier ||
                front == Tokenizer::TokenType::StringLiteral
            ) {
                caseNode->children.push_back(std::make_unique<Node>(NodeType::Literal, this->eat().value));
                while(this->ExpectToken(",")) {
                    caseNode->children.push_back(this->parseExpression(0));
                }
                this->expectToken(">", "847598496063978");
                caseNode->children.push_back(this->parseExpression(0));
            } else this->expectToken(Tokenizer::TokenType::Error, "55432097684367");
        }
        node->children.push_back(std::move(caseNode));
        if (!this->ExpectToken(",")) break;
    } while(true);
    return node;
}
std::unique_ptr<Node> ASTProducer::expectHEADER() {
    std::cout << "\ndoing header\n";
    auto node = std::make_unique<Node>(NodeType::Header);
    this->expectToken("<", "78496047968767596890");
    node->value = this->eat().value;
    this->expectToken(">", "78496047968767596890");

    std::vector<std::unique_ptr<Macro>> macros;
    
    while (this->ExpectToken("macro")) {
        macros.push_back(this->expectMACRO());
    }
    // std::cout << "\ndoing macro\n";

    // this->expandMacro(std::move(macros));
    
    while (this->is(Tokenizer::TokenType::Keyword, "import")) {
        node->children.push_back(this->expectIMPORT());
    }
    return node;
}
std::unique_ptr<Macro> ASTProducer::expectMACRO() {
    auto macro = std::make_unique<Macro>("", std::vector<Tokenizer::Token>{});
    macro->isThat = this->ExpectToken("`");
    macro->name = this->expectToken(Tokenizer::TokenType::Identifier, "358907986928684");
    this->expectToken("as", "8945789674");
    while (true) {
        auto tk = this->eat();
        if (tk.value == ";") break;
        else macro->expandoe.push_back(tk);
    }
    return macro;
}
void ASTProducer::throwError(std::string msg) {
    std::cout << msg << "\n";
    std::string temp;
    std::cin >> temp;
}