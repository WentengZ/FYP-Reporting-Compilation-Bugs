#include <string>
#include <stdlib.h>
#include <iostream>
#include <vector>

class Compiler {
    private:
        std::string compiler_dir;
        std::vector<std::string> compile_option;
    public:
        Compiler(std::string _compiler_dir,
                 std::vector<std::string> _compiler_op) {
            compiler_dir = _compiler_dir;
            compile_option = _compiler_op;
        }
        void compile(std::vector<std::string> source_files, 
                     std::vector<std::string> options); 
        void compile(std::vector<std::string> source_files); 
        void compile(std::string source_file, std::vector<std::string> options);

};
