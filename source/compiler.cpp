#ifndef INCLUDE_COMPILER
#define INCLUDE_COMPILER
#include "../include/compiler.h"
#endif
#include<vector>

void Compiler::compile(std::vector<std::string> source_files, 
                       std::vector<std::string> options) {
    std::string compile_command = Compiler::compiler_dir;
    options.push_back("-c");
    for (int i = 0; i < options.size(); i++) {
        compile_command += " " + options[i];
    }
    for (int i = 0; i < source_files.size(); i++) {
        compile_command += " " + source_files[i];
    }
    std::system(compile_command.c_str());
}

void Compiler::compile(std::vector<std::string> source_files){
    Compiler::compile(source_files, compile_option);
}

void Compiler::compile(std::string source_file, std::vector<std::string> options){
    std::vector<std::string> tem_file_vect;
    tem_file_vect.push_back(source_file);
    Compiler::compile(tem_file_vect, options);
}
