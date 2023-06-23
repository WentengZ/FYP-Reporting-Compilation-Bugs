#include <sstream>
#include <string>
#include <stdlib.h>
#include <cassert>
#include <iostream>
#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
#include <algorithm>

#ifndef INCLUDE_PARSER
#define INCLUDE_PARSER
#include "../include/parser.h"
#endif

#ifndef INCLUDE_COMPILER
#define INCLUDE_COMPILER
#include "../include/compiler.h"
#endif

#ifndef INCLUDE_LOADER
#define INCLUDE_LOADER
#include "../include/loader.h"
#endif

#ifndef INCLUDE_LINKER
#define INCLUDE_LINKER
#include "../include/linker.h"
#endif

#ifdef _WIN32
#include <direct.h>
// MSDN recommends against using getcwd & chdir names
#define cwd _getcwd
#define cd _chdir
#else
#include "unistd.h"
#define cwd getcwd
#define cd chdir
#endif

std::vector<std::string> GetLastElement(const std::string& str, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);

    while (std::getline(tokenStream, token, delimiter))
    {
        if (!token.empty())
            tokens.push_back(token);
    }

    return tokens;

}


std::string replaceUntilFirstSpace(const std::string& text, const std::string& replacement) {
    std::string result = text;
    
    // Find the position of the first empty space
    size_t spacePos = result.find(' ');
    
    // Replace characters until the first empty space
    if (spacePos != std::string::npos) {
        result.replace(0, spacePos, replacement);
    }
    
    return result;
}

std::string find_command(std::string source_files, 
                         std::vector<std::string> clang_commands){
        
    std::string file_name = GetLastElement(source_files, '/').back();
    for(int i = 0; i < clang_commands.size(); i++) {
        if (clang_commands[i].find(file_name) != std::string::npos) {
            return clang_commands[i];
        }
    }
    return "";
}

void splitString(const std::string& input, char delimiter, std::vector<std::string>& tokens) {
    std::stringstream ss(input);
    std::string token;
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
}

std::string findFile(const fs::path& directory, const std::string& filename) {
    if (!fs::is_directory(directory)) return "";
    for (const auto& entry : fs::recursive_directory_iterator(directory)) {
        if (entry.is_regular_file() && entry.path().filename() == filename) {
            return entry.path().parent_path();
        }
    }
    return "";
}



 std::string find_directory(std::string built_directory, std::string command){
    std::string token;
    std::string flag = "-o";
    size_t pos = command.find(flag);
    if (pos != std::string::npos && pos + flag.length() < command.length()) {
        if (command[pos + flag.length()] == ' ') {
            token = command.substr(pos + flag.length() + 1);
        } else {
            token = command.substr(pos + flag.length());
        }
        size_t spacePos = token.find(' ');
        if (spacePos != std::string::npos) {
            token = token.substr(0, spacePos);
        }
    }
    return findFile(built_directory, token);

}



int main(int argc, char *argv[]){
    assert(argc==2);
    Parser parser(argv[1]);

    Loader loader(parser.get_source_file_directory());

    std::vector<std::string> buggy_compiler_option = 
                             split(parser.get_buggy_compiler_option(),' ');
    
    Compiler buggy_compiler(parser.get_buggy_compiler(),
                            buggy_compiler_option);

    std::vector<std::string> correct_compiler_option = 
                             split(parser.get_correct_compiler_option(),' ');

    Compiler correct_compiler(parser.get_correct_compiler(),
                              correct_compiler_option);

    std::vector<std::string> source_files = loader.get_all_source_files();

    Linker linker(parser.get_correct_compiler(), "");

    //=============================Functionality 1=================================
    std::vector<std::string> buggy_files;
    std::vector<std::string> buggy_files_compile_commands;
    if (parser.get_extract_compile_from_make()) {
        char* cwdir = cwd(0, 0);
        std::string working_dir(cwdir);
        cd(parser.get_built_directory().c_str());
        std::system(parser.get_clean_script().c_str());
        std::string command = parser.get_make_script() + " > ./make_script.dump";
        int compile_exitCode = std::system(command.c_str());
        int correct_exitCode = std::system(parser.get_check_script().c_str());

        std::fstream infile;
        std::vector<std::string> clang_commands;
        infile.open("./make_script.dump", std::ios::in);
        if (infile.is_open()){
            std::string next_line;
            while(getline(infile, next_line)){
                
                if (next_line.find("clang") != std::string::npos){
                    std::vector<std::string> tokens;
                    splitString(next_line, ' ', tokens);
                    std::vector<std::string> tokenToBeReplace;
                    for (int j = 0; j < tokens.size(); j++) {
                        if (tokens[j].find("clang") != std::string::npos) {
                            size_t pos = next_line.find(tokens[j]);
                            next_line.replace(pos, tokens[j].length(), parser.get_buggy_compiler());
                        }
                    }
                    
                    std::string this_clang_command = next_line;
                    
                    this_clang_command += " -fPIC " + parser.get_buggy_compiler_option();


                    
                    clang_commands.push_back(this_clang_command);
                }
            }
            infile.close();
        }

        std::cout << "=====================================" << std::endl;

        for (int i = 0; i < clang_commands.size(); i++) {
            std::cout << clang_commands[i] << std::endl;
        }
        std::cout << "=====================================" << std::endl;
        for (int i = 0; i < source_files.size(); i++) {
            std::string clang_command = find_command(source_files[i], clang_commands);
            std::string file_directory = find_directory(parser.get_built_directory(), clang_command);
            if (clang_command.length() < 3) continue;
            cd(file_directory.c_str());
            std::system(clang_command.c_str());
            cd(parser.get_built_directory().c_str());
            std::system(parser.get_make_script().c_str());
            int this_exit_code = std::system(parser.get_check_script().c_str());

            if (this_exit_code != correct_exitCode) {
                buggy_files.push_back(source_files[i]);
                buggy_files_compile_commands.push_back(clang_command.c_str());
                for (int i = 0; i < buggy_files.size(); i++) {
                    std::cout << "buggy files are \n" << buggy_files[i] << std::endl;
                }
                std::system(parser.get_clean_script().c_str());
                std::system(parser.get_make_script().c_str());
            }
        }

        cd(working_dir.c_str());
        std::cout << "==============================================" << std::endl;
    
    for (int i = 0; i < buggy_files.size(); i++) {
        std::cout << "buggy files are \n" << buggy_files[i] << std::endl;
    }
    std::cout << "==============================================" << std::endl;
    } else {
        correct_compiler.compile(source_files);
        linker.link();
        std::system("./test.out 1>ref_out.dump");
        std::system("./test.out 2>ref_err.dump");
        std::vector<int> buggy_file;
    // TODO:: maybe I can use OPENMP for this loop
    // And there are many other ways to increase efficiency here
    //  Locate the buggy files
        for (int i = 0; i < source_files.size(); i++) {
        
            std::vector<std::string> temp;
            temp.push_back(source_files[i]);
            correct_compiler.compile(source_files);
            buggy_compiler.compile(temp);
            linker.link();
            std::system("./test.out 1>out.dump");
            std::system("./test.out 2>err.dump");
            int err_diff = std::system("diff err.dump ref_err.dump");
            int out_diff = std::system("diff out.dump ref_out.dump");
            if (err_diff != 0 || out_diff != 0) {
                buggy_file.push_back(i);
            }
        }
        for (int i = 0; i < buggy_file.size(); i++) {
            buggy_files.push_back(source_files[buggy_file[i]]);
        }
    }
       

//=============================Functionality 2=================================
// Get Maximum Optimization pass number

    std::string optibisect = " -mllvm -opt-bisect-limit=200";
    int maxPassNumber=10000;

    for (int k = 0; k < buggy_files.size(); k++) {
        std::string file_directory = find_directory(parser.get_built_directory(), buggy_files_compile_commands[k]);
        char* cwdir_temp = cwd(0, 0);
        cd(file_directory.c_str());
        std::string clang_command = buggy_files_compile_commands[k] + optibisect + " > input.txt";
        std::system(clang_command.c_str());
        cd (cwdir_temp);
    
        std::ifstream inputFileTem("input.txt");  // Replace "input.txt" with the actual input file name
        std::string lineTem;
        std::vector<int> passNumbers;
    
        // Read input file line by line
        while (std::getline(inputFileTem, lineTem)) {
            if (lineTem.find("running pass") != std::string::npos || lineTem.find("NOT running pass") != std::string::npos) {
                // Extract the pass number from the lineTem
                size_t startPos = lineTem.find("(") + 1;
                size_t endPos = lineTem.find(")");
                int passNumber = std::stoi(lineTem.substr(startPos, endPos - startPos));
                passNumbers.push_back(passNumber);
            }
        }
    
        inputFileTem.close();
    
        if (!passNumbers.empty()) {
            // Find the maximum pass number
            if (maxPassNumber < *std::max_element(passNumbers.begin(), passNumbers.end())) {
                maxPassNumber = *std::max_element(passNumbers.begin(), passNumbers.end());
            }
            
            //std::cout << "Maximum pass number: " << maxPassNumber << std::endl;
        } else {
            //std::cout << "No pass numbers found." << std::endl;
        }


    }



//  Locate the optimization pass(es).
    std::vector<int> for_debug_only(maxPassNumber, 0);
    std::vector<bool> found_opt_pass(buggy_files.size() ,false);
    std::vector<int> current_pass(buggy_files.size(), maxPassNumber / 2);
    std::vector<int> upper_bound(buggy_files.size(), maxPassNumber);
    std::vector<int> lower_bound(buggy_files.size(), 0);
    std::vector<bool> go_up(buggy_files.size(), true);
    for (int k = 0; k < buggy_files.size(); k++){
    if (parser.get_extract_compile_from_make()) {
        char* cwdir = cwd(0, 0);
        std::string working_dir(cwdir);
        cd(parser.get_built_directory().c_str());
        std::system(parser.get_clean_script().c_str());
        std::string command = parser.get_make_script() + " > ./make_script.dump";
        int compile_exitCode = std::system(command.c_str());
        int correct_exitCode = std::system(parser.get_check_script().c_str());

        while (!found_opt_pass[k]) {
            std::string optibisect = " -mllvm -opt-bisect-limit="+
                                     std::to_string(current_pass[k]);

            std::string file_directory = find_directory(parser.get_built_directory(), buggy_files_compile_commands[k]);
            char* cwdir_temp = cwd(0, 0);
            cd(file_directory.c_str());
            std::string clang_command = buggy_files_compile_commands[k] + optibisect;
            std::system(clang_command.c_str());
            cd(parser.get_built_directory().c_str());
            std::system(parser.get_make_script().c_str());
            std::cout << cwdir_temp << std::endl;
            int this_exit_code = std::system(parser.get_check_script().c_str());
                    
            if (this_exit_code != correct_exitCode) {
                for_debug_only[current_pass[k]] = -1;
                go_up[k] = false;
            } else {
                for_debug_only[current_pass[k]] = 1;
                go_up[k] = true;
            }
            if (go_up[k]){
                lower_bound[k] = current_pass[k];
                current_pass[k] = std::ceil((current_pass[k] + upper_bound[k]) / 2);
            } else {
                upper_bound[k] = current_pass[k];
                current_pass[k] = std::ceil((current_pass[k] + lower_bound[k]) / 2);
            }
     
            if (abs(upper_bound[k] - lower_bound[k]) < 2) {
                found_opt_pass[k] = true;
                std::system(parser.get_clean_script().c_str());
                std::system(parser.get_make_script().c_str());
            }
        }
        cd(working_dir.c_str());
    } else {
        while (!found_opt_pass[k]) {
            correct_compiler.compile(source_files);
            std::string optibisect = "-mllvm -opt-bisect-limit="+
                                       std::to_string(current_pass[k]);
            std::vector<std::string> temp_option = buggy_compiler_option;
            temp_option.push_back(optibisect);
            buggy_compiler.compile(buggy_files[k], temp_option);
            linker.link();
    
            std::system("./test.out 1>out.dump");
            std::system("./test.out 2>err.dump");
            int err_diff = std::system("diff err.dump ref_err.dump");
            int out_diff = std::system("diff out.dump ref_out.dump");
    
            if (err_diff != 0 || out_diff != 0) {
                go_up[k] = false;
            } else {
                go_up[k] = true;
            }
            int temp = current_pass[k];
            if (go_up[k]){
                lower_bound[k] = current_pass[k];
                current_pass[k] = std::ceil((current_pass[k] + upper_bound[k]) / 2);
            } else {
                upper_bound[k] = current_pass[k];
                current_pass[k] = std::ceil((current_pass[k] + lower_bound[k]) / 2);
            }
            if (abs(upper_bound[k] - lower_bound[k]) < 2) {
                found_opt_pass[k] = true;
            }
        }
    }
    }


//=============================Functionality 3=================================
//  Locate the git commit.
    int commit_upper_bound = -1;
    int commit_lower_bound = -1;
    int current_id_index = -1;
    std::vector<std::string> commit_IDs;
    std::vector<bool> commits_result;
    if (parser.get_is_git_directory()){
        char* cwdir = cwd(0, 0);
        std::string working_dir(cwdir);
        cd(parser.get_buggy_compiler_git_directory().c_str());
        std::system("git log -25 > log.dump");
        // get last 10 commits

        std::fstream infile;
        infile.open("./log.dump", std::ios::in);
        if (infile.is_open()){
            std::string next_line;
            int pos=-1;
            while(getline(infile, next_line)){
                std::string field = next_line.substr(0, 6);
                if (!field.compare("commit")){
                    std::string this_commit_id = next_line.substr(7, next_line.length()-7);
                    commit_IDs.push_back(this_commit_id);
                    commits_result.push_back(false);
                }
            }
            infile.close();
        }

        bool found = false;
        bool go_up_git = false;

        commit_upper_bound = commit_IDs.size(); 
        current_id_index = commit_IDs.size() / 2;
        commit_lower_bound = 0;
        while (!found){
            std::string checkout_id_command = "git checkout " + 
                                              commit_IDs[current_id_index];
            std::system(checkout_id_command.c_str());
            std::system(parser.get_buggy_compiler_compile_script().c_str());
            cd(working_dir.c_str());

            buggy_compiler.compile(buggy_files, buggy_compiler_option);
            linker.link();

            std::system("./test.out 1>out.dump");
            std::system("./test.out 2>err.dump");
            int err_diff = std::system("diff err.dump ref_err.dump");
            int out_diff = std::system("diff out.dump ref_out.dump");

            if (err_diff != 0 || out_diff != 0) {
                go_up_git = true;
            } else {
                commits_result[current_id_index] = true;
                go_up_git = false;
            }
       
            if (go_up_git){
                commit_lower_bound = current_id_index;
                current_id_index = (current_id_index + commit_upper_bound) / 2;
            } else {
                commit_upper_bound = current_id_index;
                current_id_index = (current_id_index + commit_lower_bound) / 2;
            }
            
            if (abs(commit_upper_bound - commit_lower_bound) < 2) {
                found = true;
            }
            cd(parser.get_buggy_compiler_git_directory().c_str());
        }


        std::string checkout_id_command = "git checkout " + 
                                              commit_IDs[0];
        std::system(checkout_id_command.c_str());
        std::system(parser.get_buggy_compiler_compile_script().c_str());
        cd(working_dir.c_str());
    }

    // Output all the Result
    std::ofstream outputFile("input_to_2nd_tool.txt", std::ios::trunc);
    outputFile.close();
    std::cout << "==============================================" << std::endl;
    
    for (int i = 0; i < buggy_files.size(); i++) {
        std::cout << "buggy files are \n" << buggy_files[i] << std::endl;

        std::ofstream outputFile("input_to_2nd_tool.txt", std::ios::app); 
        if (outputFile.is_open()) { 
            outputFile << "buggy_file=" << buggy_files[i] << std::endl << std::endl;
            outputFile << "buggy_compiler_command=" << buggy_files_compile_commands[i] << std::endl << std::endl;
            outputFile << "correct_compiler_command=" << replaceUntilFirstSpace(buggy_files_compile_commands[i], parser.get_correct_compiler()) << std::endl << std::endl;
            outputFile << "built_directory=" << find_directory(parser.get_built_directory(), buggy_files_compile_commands[i]) << std::endl << std::endl;
            outputFile << "check_directory=" << parser.get_built_directory() << std::endl << std::endl;
            outputFile << "make_script=" << parser.get_make_script() << std::endl << std::endl;
            outputFile << "check_script=" << parser.get_check_script() << std::endl << std::endl;
            outputFile << "clean_script=" << parser.get_clean_script() << std::endl << std::endl;
            outputFile.close(); 
        } else {
            std::cout << "Failed to open the file." << std::endl;
        }
    

    std::cout << "==============================================" << std::endl;
    std::cout << "buggy optimization passes are between\n";
    std::cout << lower_bound[i] << " and " << upper_bound[i] << std::endl;
    }
    std::cout << "==============================================" << std::endl;
    
    std::cout << "buggy commits are between\n";
    std::cout << commit_lower_bound << " and " << commit_upper_bound << std::endl;

    std::cout << "==============================================" << std::endl;
    if (commit_lower_bound != -1) {
    
        for (int i = commit_lower_bound; i < commit_upper_bound; i++){
            std::cout << commit_IDs[i] << std::endl;
            std::cout << commits_result[i] << std::endl;
        }
    }
    std::cout << "==============================================" << std::endl;

    std::string romove_all_temp = "rm *.o && rm *.out && rm *.dump";
    std::system(romove_all_temp.c_str());
}
