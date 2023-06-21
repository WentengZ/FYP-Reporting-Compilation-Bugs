#include <string>
#include <fstream>
#include <stdlib.h>
#include <iostream>

class Parser {
    private:
        bool extract_compile_from_make=false;
        bool is_git_directory=false;
        std::string correct_compiler;
        std::string correct_compiler_option;
        std::string buggy_compiler;
        std::string buggy_compiler_option;
        std::string source_file_directory;
        std::string buggy_compiler_git_directory;
        std::string buggy_compiler_compile_script;
        std::string make_script;
        std::string check_script;
        std::string clean_script;
        std::string built_directory;
        std::string check_directory;
        int find_delimiter(std::string text, char delimiter);
    public:
        Parser(std::string infile_dir) {
            std::fstream infile;
            infile.open(infile_dir, std::ios::in);
            if (infile.is_open()){
                std::string next_line;
                int pos=-1;
                while(getline(infile, next_line)){ 
                    pos=find_delimiter(next_line, '=');
                    if (pos != -1){
                        std::string field = next_line.substr(0, pos);
                        std::string value = next_line.substr(pos+1, next_line.length()-1);
                        if (!field.compare("Using_make_to_compile")) {
                            extract_compile_from_make = !value.compare("true");
                        }
                        if (!field.compare("make_script")) {
                            make_script = value;
                        }
                        if (!field.compare("clean_script")) {
                            clean_script = value;
                        }
                        if (!field.compare("built_directory")) {
                            built_directory = value;
                        }
                        if (!field.compare("check_script")) {
                            check_script = value;
                        }
                        if (!field.compare("check_directory")) {
                            check_directory = value;
                        }
                        if (!field.compare("correct_compiler")) {
                            correct_compiler = value;
                        }
                        if (!field.compare("correct_compiler_option")) {
                            correct_compiler_option = value;
                        }
                        if (!field.compare("buggy_compiler")) {
                            buggy_compiler = value;
                        }
                        if (!field.compare("buggy_compiler_option")) {
                            buggy_compiler_option = value;
                        }
                        if (!field.compare("source_file_directory")) {
                            source_file_directory = value;
                        }
                        if (!field.compare("is_git_directory")) {
                            is_git_directory = !value.compare("true");
                        }
                        if (!field.compare("buggy_compiler_git_directory")) {
                            buggy_compiler_git_directory = value;
                        }
                        if (!field.compare("buggy_compiler_compile_script")) {
                            buggy_compiler_compile_script = value;
                        }
                    }
                }
            infile.close();
            }
        }
        std::string get_correct_compiler(){return correct_compiler;};
        std::string get_correct_compiler_option(){return correct_compiler_option;};
        std::string get_buggy_compiler() {return buggy_compiler;};
        std::string get_buggy_compiler_option(){return buggy_compiler_option;};
        std::string get_source_file_directory(){return source_file_directory;};
        std::string get_buggy_compiler_compile_script(){return buggy_compiler_compile_script;};
        std::string get_buggy_compiler_git_directory(){return buggy_compiler_git_directory;};

        bool get_extract_compile_from_make(){return extract_compile_from_make;};
        std::string get_make_script(){return make_script;};
        std::string get_check_script(){return check_script;};
        std::string get_clean_script(){return clean_script;};
        std::string get_built_directory(){return built_directory;};
        std::string get_check_directory(){return check_directory;};
        bool get_is_git_directory() {return is_git_directory;};
};

    std::vector<std::string> split(std::string s, char c);
