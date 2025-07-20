#include <iostream>
#include <fstream>
#include <sstream>
#include "src/parser.h"
#include "src/interpreter.h"
#include "src/semantic_analyzer.h"
#include "src/codegen.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string> // For std::string
#include <vector> // For std::vector
#include "src/parser.h"
#include "src/interpreter.h"
#include "src/semantic_analyzer.h"
#include "src/codegen.h"

int main(int argc, char* argv[]) {
    std::string inputFilePath;
    std::string outputExecutableName = "output"; // Default output name

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-o") {
            if (i + 1 < argc) {
                outputExecutableName = argv[++i];
            } else {
                std::cerr << "Error: -o option requires an argument." << std::endl;
                return 1;
            }
        } else {
            inputFilePath = arg;
        }
    }

    if (inputFilePath.empty()) {
        std::cerr << "Usage: " << argv[0] << " <input_file> [-o <output_executable_name>]" << std::endl;
        return 1;
    }
    
    // Read input file
    std::ifstream inputFile(inputFilePath);
    if (!inputFile.is_open()) {
        std::cerr << "Error: Could not open file " << inputFilePath << std::endl;
        return 1;
    }
    
    std::stringstream buffer;
    buffer << inputFile.rdbuf();
    std::string input = buffer.str();
    
    // Lex, parse, and interpret
    Lexer lexer(input);
    Parser parser(lexer);
    auto ast = parser.parse();

    SemanticAnalyzer analyzer;
    if (!analyzer.analyze(ast)) {
        std::cerr << "Compilation failed due to semantic errors." << std::endl;
        return 1;
    }
    
    // Code generation
    CodeGenerator generator("temp.asm"); // Use a temporary assembly file
    generator.generate(ast);

    // Assemble and link
    std::string nasmCommand = "nasm -f elf64 temp.asm -o " + outputExecutableName + ".o";
    std::string gccCommand = "gcc -no-pie " + outputExecutableName + ".o src/runtime.c -o " + outputExecutableName + " -lc";
    
    system(nasmCommand.c_str());
    system(gccCommand.c_str());
    system("rm temp.asm"); // Clean up temporary assembly file
    system(("rm " + outputExecutableName + ".o").c_str()); // Clean up temporary object file
    
    return 0;
}