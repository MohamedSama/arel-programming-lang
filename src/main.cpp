#include <iostream>
#include <fstream>
#include <chrono>
#include "frontend/tokenizer/tokenizer.h"
#include "frontend/AST-Producer/ast-main.h"

int main(int argc, char** argv) {
    // Usage:
    //  - Interactive mode (no args): program prompts for token count on stdin repeatedly.
    //  - Non-interactive: `./arel <token_count>` or `./arel <token_count> <input_file>`
    auto is_number = [](const std::string &s) {
        if (s.empty()) return false;
        for (char c : s) if (!std::isdigit(static_cast<unsigned char>(c))) return false;
        return true;
    };

    bool interactive = true;
    std::string input_path = "../test/one.el"; // default input file
    int token_count = 0;

    if (argc >= 2) {
        std::string a1 = argv[1];
        if (is_number(a1)) {
            token_count = std::stoi(a1);
            interactive = false;
        } else {
            input_path = a1;
        }
    }
    if (argc >= 3) {
        std::string a2 = argv[2];
        if (is_number(a2)) {
            token_count = std::stoi(a2);
            interactive = false;
        } else {
            input_path = a2;
        }
    }

    if (!interactive && token_count == 0) {
        // sensible default if user provided a file but no count
        token_count = 1600;
    }

    auto run_once = [&](int used_token_count) {
        auto start = std::chrono::high_resolution_clock::now();

        int charlength = 0;
        int* char_length = &charlength;

        Tokenizer::Tokenizer tokenizer(input_path, used_token_count, char_length);
        std::vector<Tokenizer::Token>* tokens = tokenizer.getTokens();

        auto end = std::chrono::high_resolution_clock::now();

        double elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / 1e6;
        std::cout << "\n\n\nTokenization Started!\n\n";
        int count = 1;
        for (Tokenizer::Token token : *tokens) {
            std::cout << count++ << ": " << token.stringify();
        }
        std::cout << "\n\nTokenization complete! in " << elapsed_time << "ms for " << (*tokens).size() << " tokens.\n\n\nAST Production Started..\n\n";

        auto start1 = std::chrono::high_resolution_clock::now();

        ASTProducer astproduer(tokens);

        auto end1 = std::chrono::high_resolution_clock::now();

        double elapsed_time1 = std::chrono::duration_cast<std::chrono::nanoseconds>(end1 - start1).count() / 1e6;
        std::cout << astproduer.output->stringify(0) << "\n\n\n";

        std::cout << "Tokenizer:      \033[33m" << charlength << "\033[0m char, \033[96m" << elapsed_time << "ms\033[0m\n";
        std::cout << "AST Production: \033[33m" << (*tokens).size() << "\033[0m tokens, \033[96m" << elapsed_time1 << "ms\033[0m.\n\n";
        std::cout << "Total Time:     \033[96m" << (elapsed_time1 + elapsed_time) << "ms\033[0m.\n\n";

        std::ofstream file("output.txt"); // creates or overwrites

        if (!file.is_open()) {
            std::cerr << "Failed to open file\n";
            return 1;
        }

        file << astproduer.output->stringify(0) << "\n\n\n";
        file << "\n\nAST Production Ended in a time of: " << elapsed_time1 << "ms.\n\n";
        file << "Tokenizer:      " << charlength << " char, " << elapsed_time << "ms\n";
        file << "AST Production: " << (*tokens).size() << " tokens, " << elapsed_time1 << "ms.\n\n";
        file << "Total Time:     " << (elapsed_time1 + elapsed_time) << "ms.\n\n";

        file.close();
        return 0;
    };

    if (interactive) {
        while (true) {
            std::string inp;
            if (!(std::cin >> inp)) break;
            if (inp == "exit" || inp == "e") break;
            if (!is_number(inp)) {
                std::cerr << "Please enter a numeric token count or 'exit' to quit.\n";
                continue;
            }
            int n = std::stoi(inp);
            run_once(n);
        }
        return 0;
    } else {
        return run_once(token_count);
    }
}
