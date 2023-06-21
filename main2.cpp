#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <llvm/Support/CommandLine.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/AST/AST.h>
#include <clang/AST/ASTContext.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm> 
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


using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::tooling;

std::string extractStringAfterO(const std::string& input) {
    std::istringstream iss(input);
    std::string token;

    while (iss >> token) {
        if (token == "-o" && iss >> token) {
            return token;
        }
    }

    return "";
}


std::string cutUntilFirstBrace(const std::string& input) {
    size_t pos = input.find('{');
    if (pos != std::string::npos) {
        return input.substr(0, pos);
    }
    // If there is no '{', return the whole string
    return input;
}

bool run_this = false;

bool firstRun = true;

std::vector<std::string> all_function_names; 
std::vector<bool> has_tested; 

bool startsWith(const std::string& line, const std::string& prefix) {
    if (line.size() >= prefix.size()) {
        return std::equal(prefix.begin(), prefix.end(), line.begin(), [](char a, char b) {
            return std::tolower(a) == std::tolower(b);
        });
    }
    return false;
}

int global_index = 0;
int current_index = 0;

void replaceString(std::string& A, const std::string& B, const std::string& C) {
    size_t pos = 0;
    while ((pos = A.find(B, pos)) != std::string::npos) {
        if (pos == 0 || (!(A[pos-1] > 'a' && A[pos-1] < 'z') && !(A[pos-1] > 'A' && A[pos-1] < 'Z') && A[pos-1] != '_') )
        A.replace(pos, B.length(), C);
        pos += C.length();
    }
}

void copy_code(const std::string& input_path, const std::string& output_path) {
    std::ifstream input_file(input_path);
    std::ofstream output_file(output_path, std::ios::app);

    std::string line;
    bool is_copying = false;

    while (std::getline(input_file, line)) {
        if (line.find("#ifdef") != std::string::npos) {
            std::string next_line;
            if (std::getline(input_file, next_line)) {
                if (next_line.find("#define") != std::string::npos) {
                    is_copying = true;
                    output_file << line << "\n";
                    output_file << next_line << "\n";
                }
            }
        } else if (line.find("#endif") != std::string::npos && is_copying) {
            is_copying = false;
            output_file << line << "\n";
        } else if (is_copying) {
            output_file << line << "\n";
        }
    }
}

void extractLeftHandSide(std::string& str) {
    size_t equalsPos = str.find('=');
    if (equalsPos != std::string::npos) {
        str = str.substr(0, equalsPos);
    }
}

void removeNewlines(std::string& str) {
    auto position = std::find(str.begin(), str.end(), '\n');
    if (position != str.end()) {
        *position = ' ';
    }
}

void deleteBetweenBraces(std::string& input) {
    std::size_t startPos = input.find('{');
    std::size_t endPos = input.rfind('}');
    
    if (startPos != std::string::npos && endPos != std::string::npos && startPos < endPos) {
        input.erase(startPos, endPos - startPos + 1);
    }
}

void addExternIfNeeded(std::string& str) {
    size_t nonSpacePos = str.find_first_not_of(' ');
    if (nonSpacePos != std::string::npos && nonSpacePos < str.length()) {
        std::string firstWord = str.substr(nonSpacePos, 6); // Check the first 6 characters
        if (firstWord != "extern") {
            str.insert(nonSpacePos, "extern ");
        }
    }
}

// Matcher to match function declarations and definitions
DeclarationMatcher FunctionMatcher = functionDecl().bind("function");
DeclarationMatcher VariableMatcher = varDecl(isExpansionInMainFile(), 
                                           hasGlobalStorage()).bind("varDecl");

// Callback to handle matched functions
class FunctionHandler : public MatchFinder::MatchCallback {
public:
  // Called when a function declaration or definition is matched
  virtual void run(const MatchFinder::MatchResult &Result) {
    if (const VarDecl *VD = Result.Nodes.getNodeAs<VarDecl>("varDecl")) {
      if (VD->isFileVarDecl()) {
        SourceRange SR = VD->getSourceRange();

        const SourceManager *SM = Result.SourceManager;

        const FileEntry *FE = SM->getFileEntryForID(SM->getMainFileID());
        std::string FileName = FE->getName().str();
        unsigned StartLine = SM->getSpellingLineNumber(SR.getBegin());
        unsigned EndLine = SM->getSpellingLineNumber(SR.getEnd());

        std::string VarDelcCode = getSourceCode(SM, SR);
        VarDelcCode += ";\n\n";

        std::cout << "Variable Definition/Declaration found in " << FileName << " (lines " << StartLine << "-" << EndLine << "):\n";
        std::cout << VarDelcCode << "\n";
  
        // Write the function code to a separate file
        std::string OutputFileName = "function_code.c";
        std::ofstream OutputFile(OutputFileName, std::ios::app);
        if (OutputFile.is_open()) {
          OutputFile << VarDelcCode;
          OutputFile.close();
          std::cout << "Function code written to " << OutputFileName << "\n";
        } else {
          std::cerr << "Failed to open file " << OutputFileName << " for writing.\n";
        }
      }
    }

    if (const VarDecl *VD = Result.Nodes.getNodeAs<VarDecl>("varDecl")) {
      if (VD->isFileVarDecl()) {
        SourceRange SR = VD->getSourceRange();

        const SourceManager *SM = Result.SourceManager;

        const FileEntry *FE = SM->getFileEntryForID(SM->getMainFileID());
        std::string FileName = FE->getName().str();
        unsigned StartLine = SM->getSpellingLineNumber(SR.getBegin());
        unsigned EndLine = SM->getSpellingLineNumber(SR.getEnd());

        std::string VarDelcCode = getSourceCode(SM, SR);
        extractLeftHandSide(VarDelcCode);
        addExternIfNeeded(VarDelcCode);

        VarDelcCode += ";\n\n";

        std::cout << "Variable Definition/Declaration found in " << FileName << " (lines " << StartLine << "-" << EndLine << "):\n";
        std::cout << VarDelcCode << "\n";
  
        // Write the function code to a separate file
        std::string OutputFileName = "myHeader.h";
        std::ofstream OutputFile(OutputFileName, std::ios::app);
        if (OutputFile.is_open()) {
          OutputFile << VarDelcCode;
          OutputFile.close();
          std::cout << "Function code written to " << OutputFileName << "\n";
        } else {
          std::cerr << "Failed to open file " << OutputFileName << " for writing.\n";
        }
      }
    }


    if (const FunctionDecl *FD = Result.Nodes.getNodeAs<FunctionDecl>("function")) {
      if (FD -> hasBody() && firstRun) {
        std::string FunctionName =  FD->getNameAsString();
        all_function_names.push_back(FunctionName);
        has_tested.push_back(false);
      }
    }

    if (const FunctionDecl *FD = Result.Nodes.getNodeAs<FunctionDecl>("function")) {
      if (FD -> hasBody()) {
        // Extract function source range
        SourceRange SR = FD->getSourceRange();
  
        // Get the source manager
        const SourceManager *SM = Result.SourceManager;
  
        // Get the file name and line numbers
        const FileEntry *FE = SM->getFileEntryForID(SM->getMainFileID());
        std::string FileName = FE->getName().str();
        unsigned StartLine = SM->getSpellingLineNumber(SR.getBegin());
        unsigned EndLine = SM->getSpellingLineNumber(SR.getEnd());
  
        // Get the function source code
        std::string FunctionCode = getSourceCode(SM, SR);

        for (int func_i = 0; func_i < all_function_names.size(); func_i++) {
          if (all_function_names[func_i].compare(FD->getNameAsString()) == 0) {
            deleteBetweenBraces(FunctionCode);
            if (FunctionCode.find("#ifdef") == std::string::npos) {
              FunctionCode += (std::string)"\n{\n  " + (std::string)"my_magic_123" + all_function_names[func_i] +"(";
              for (const ParmVarDecl* param : FD->parameters()) {
                  FunctionCode += param->getNameAsString() + ", ";
              }
              FunctionCode.erase(FunctionCode.length() - 2, 2);
              FunctionCode +=");\n}";
            } else {
              FunctionCode = getSourceCode(SM, SR);
            }

          } else {
            replaceString(FunctionCode, all_function_names[func_i]+"(", "my_magic_123" + all_function_names[func_i]+"(");

          }
        }

        FunctionCode += "\n\n";
  
        // Write the function code to a separate file
        std::string OutputFileName = "function_code.c";
        std::ofstream OutputFile(OutputFileName, std::ios::app);
        if (OutputFile.is_open()) {
          OutputFile << FunctionCode;
          OutputFile.close();
          std::cout << "Function code written to " << OutputFileName << "\n";
        } else {
          std::cerr << "Failed to open file " << OutputFileName << " for writing.\n";
        }
      }
    }



    if (const FunctionDecl *FD = Result.Nodes.getNodeAs<FunctionDecl>("function")) {
      if (FD -> hasBody()) {
        
        // Extract function source range
        SourceRange SR = FD->getSourceRange();
  
        // Get the source manager
        const SourceManager *SM = Result.SourceManager;
  
        // Get the file name and line numbers
        const FileEntry *FE = SM->getFileEntryForID(SM->getMainFileID());
        std::string FileName = FE->getName().str();
        unsigned StartLine = SM->getSpellingLineNumber(SR.getBegin());
        unsigned EndLine = SM->getSpellingLineNumber(SR.getEnd());
  
        // Get the function source code
        std::string FunctionCode = getSourceCode(SM, SR);

        FunctionCode += "\n\n";

        if (FD -> isStatic()) {
          removeFirstKeyword(FunctionCode, "static ");
        }

        for (int func_i = 0; func_i < all_function_names.size(); func_i++) {
          replaceString(FunctionCode, all_function_names[func_i]+"(", "my_magic_123" + all_function_names[func_i]+"(");
        }

        // Print the function source code
        std::cout << "Function Definition/Declaration found in " << FileName << " (lines " << StartLine << "-" << EndLine << "):\n";
        std::cout << FunctionCode << "\n";
  
        // Write the function code to a separate file
        std::string OutputFileName;
        auto file_ios = std::ios::app;
        if (!firstRun && (FD->getNameAsString().compare(all_function_names[global_index]) == 0)) {
          std::cout << "$$$$$$$$$$$$$$$$$$$" << global_index << std::endl;

          std::cout << FD->getNameAsString() << std::endl;
          std::cout << "$$$$$$$$$$$$$$$$$$$" << current_index << std::endl;

          OutputFileName = "my_temp.c";
          file_ios = std::ios::trunc;
          std::ofstream OutputFile(OutputFileName, file_ios);
          if (OutputFile.is_open()) {
            OutputFile << "#include \"myHeader.h\"\n\n";
            OutputFile << FunctionCode;
            OutputFile.close();
            std::cout << "Function code written to " << OutputFileName << "\n";
          } else {
            std::cerr << "Failed to open file " << OutputFileName << " for writing.\n";
          }

        } else {
          OutputFileName = "function_code.c";
          std::ofstream OutputFile(OutputFileName, file_ios);
          if (OutputFile.is_open()) {
            OutputFile << FunctionCode;
            OutputFile.close();
            std::cout << "Function code written to " << OutputFileName << "\n";
          } else {
            std::cerr << "Failed to open file " << OutputFileName << " for writing.\n";
          }
        }
        
      }
    }

    if (const FunctionDecl *FD = Result.Nodes.getNodeAs<FunctionDecl>("function")) {
      if (FD -> hasBody()) {
        
        // Extract function source range
        SourceRange SR = FD->getSourceRange();
  
        // Get the source manager
        const SourceManager *SM = Result.SourceManager;
  
        // Get the file name and line numbers
        const FileEntry *FE = SM->getFileEntryForID(SM->getMainFileID());
        std::string FileName = FE->getName().str();
        unsigned StartLine = SM->getSpellingLineNumber(SR.getBegin());
        unsigned EndLine = SM->getSpellingLineNumber(SR.getEnd());
  
        // Get the function source code
         std::string FunctionCode = getSourceCode(SM, SR);
        //std::string FunctionCode = FD->getNameAsString();
        FunctionCode = cutUntilFirstBrace(FunctionCode);
        removeNewlines(FunctionCode);
        if (FunctionCode.length() > 3) {
          FunctionCode += ";\n\n";
        }


        if (FD -> isStatic()) {
          removeFirstKeyword(FunctionCode, "static ");
        }

        for (int func_i = 0; func_i < all_function_names.size(); func_i++) {
          if (all_function_names[func_i].compare(FD->getNameAsString()) == 0) {
            replaceString(FunctionCode, all_function_names[func_i]+"(", "my_magic_123" + all_function_names[func_i]+"(");
          } 
        }


        // Print the function source code
        std::cout << "Function Definition/Declaration found in " << FileName << " (lines " << StartLine << "-" << EndLine << "):\n";
        std::cout << FunctionCode << "\n";
  
        // Write the function code to a separate file
        std::string OutputFileName = "myHeader.h";
        std::ofstream OutputFile(OutputFileName, std::ios::app);
        if (OutputFile.is_open()) {
          OutputFile << FunctionCode;
          OutputFile.close();
          std::cout << "Function code written to " << OutputFileName << "\n";
        } else {
          std::cerr << "Failed to open file " << OutputFileName << " for writing.\n";
        }
      }
      if (!firstRun) {
        current_index++;
      }
    }


  }

private:
  // Helper function to extract source code given a source range
  std::string getSourceCode(const SourceManager *SM, const SourceRange &SR) {
    std::string FunctionCode;
    llvm::StringRef SourceText = Lexer::getSourceText(CharSourceRange::getTokenRange(SR), *SM, LangOptions(), 0);
    FunctionCode = SourceText.str();
    return FunctionCode;
  }

  void removeFirstKeyword(std::string& sentence, const std::string& keyword) {
    size_t foundPos = sentence.find(keyword);
    if (foundPos != std::string::npos) {
        sentence.erase(foundPos, keyword.length());
    }
}
};

int find_delimiter(std::string text, char delimiter) {
    for (int i = 0; i < text.length(); i++){
        if (text.at(i) == delimiter) {
            return i;
        }
    }
    return -1;
}


std::string source_file_directory;
std::string correct_compiler_command;
std::string buggy_compiler_command;
std::string built_directory;
std::string source_directory;
std::string make_script;
std::string check_script;
std::string clean_script;
std::string original_file; 
std::string check_directory; 

std::string getSubstringAfterMarker(const std::string& input) {
    std::string marker = " -o";
    size_t markerPos = input.find(marker);
    if (markerPos == std::string::npos) {
        // Marker not found
        return "";
    }
    size_t spacePos = input.find(' ', markerPos + marker.length());
    if (spacePos == std::string::npos) {
        // Space not found after the marker
        return "";
    }
    return input.substr(markerPos + marker.length(), spacePos - (markerPos + marker.length()));
}



std::string getSubstringBeforeLastSlash(const std::string& input) {
    size_t lastSlashPos = input.find_last_of('/');
    if (lastSlashPos == std::string::npos) {
        // No slash found in the input string
        return ".";
    }
    return input.substr(0, lastSlashPos);
}

std::string getSubstringAfterLastSlash(const std::string& input) {
    size_t lastSlashPos = input.find_last_of('/');
    if (lastSlashPos == std::string::npos || lastSlashPos == input.length() - 1) {
        // No slash found in the input string or it's the last character
        return "";
    }
    return input.substr(lastSlashPos + 1);
}

std::string replaceSubstring(const std::string& input, const std::string& oldSubstr, const std::string& newSubstr) {
    std::string result = input;
    size_t pos = result.find(oldSubstr);
    while (pos != std::string::npos) {
        result.replace(pos, oldSubstr.length(), newSubstr);
        pos = result.find(oldSubstr, pos + newSubstr.length());
    }
    return result;
}

std::vector<std::string> buggy_function;


int main(int argc, const char *argv[]) {

  std::fstream infile;
  infile.open(argv[1], std::ios::in);
  if (infile.is_open()){
      std::string next_line;
      int pos=-1;
      while(getline(infile, next_line)){ 
          pos=find_delimiter(next_line, '=');
          if (pos != -1){
              std::string field = next_line.substr(0, pos);
              std::string value = next_line.substr(pos+1, next_line.length()-1);
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
              if (!field.compare("correct_compiler_command")) {
                  correct_compiler_command = value;
              }
              if (!field.compare("buggy_compiler_command")) {
                  buggy_compiler_command = value;
              }
              if (!field.compare("buggy_file")) {
                  source_file_directory = value;
              }
              if (!field.compare("check_directory")) {
                  check_directory = value;
              }
          }
      }
  infile.close();
  }
  source_directory = getSubstringBeforeLastSlash(source_file_directory);
  original_file = getSubstringAfterLastSlash(source_file_directory);

  cd(source_directory.c_str());

  // Set up the Clang tooling options
  llvm::cl::OptionCategory Category("Function Extraction");
  argv[1] = source_file_directory.c_str();
  CommonOptionsParser OptionsParser(argc, argv, Category);
  ClangTool Tool(OptionsParser.getCompilations(), OptionsParser.getSourcePathList());

  // Create an instance of the match finder
  MatchFinder Finder;

  // Create an instance of the match callback
  FunctionHandler Handler;

  // Register the match callback with the finder
  Finder.addMatcher(FunctionMatcher, &Handler);
  Finder.addMatcher(VariableMatcher, &Handler);
  int Result = Tool.run(newFrontendActionFactory(&Finder).get());

  std::ifstream inputFile(source_file_directory);
  std::ofstream outputFile("myHeader.h", std::ios::trunc);
  
  if (!inputFile.is_open()) {
      std::cerr << "Failed to open input file." << std::endl;
      return 1;
  }
  
  if (!outputFile.is_open()) {
      std::cerr << "Failed to create output file." << std::endl;
      return 1;
  }
  
  std::string line;
  while (std::getline(inputFile, line)) {
      if (startsWith(line, "#include") || 
          startsWith(line, "#define") ||
          startsWith(line, "#ifdef") ||
          startsWith(line, "#ifndef") ||
          startsWith(line, "#if") ||
          startsWith(line, "#elif") ||
          startsWith(line, "#else") ||
          startsWith(line, "#endif") ) {
          outputFile << line << std::endl;
      }
  }
  
  std::cout << "Header file created successfully." << std::endl;
  
  inputFile.close();
  outputFile.close();
    

  std::string OutputFileName = "function_code.c";
  std::ofstream OutputFile(OutputFileName, std::ios::trunc);
  OutputFile << "#include \"myHeader.h\" \n\n" << std::endl;
  OutputFile.close();
  copy_code(source_file_directory, "myHeader.h");
  firstRun = false;
  cd(check_directory.c_str());
  std::system(clean_script.c_str());
  std::system(make_script.c_str());
  int correct_exit_code = std::system(check_script.c_str());

  // Run the tool using the match finder
  for (int i = 0; i < all_function_names.size(); i++) {
    cd(source_directory.c_str());
    current_index = 0;
    global_index = i;
    std::string OutputFileName = "function_code.c";
    std::ofstream OutputFile(OutputFileName, std::ios::trunc);
    OutputFile << "#include \"myHeader.h\" \n" << std::endl;
    OutputFile.close();

    std::string OutputFileName2 = "myHeader.h";
    std::ofstream OutputFile2(OutputFileName2, std::ios::trunc);

    std::string OutputFileName3 = "my_temp.c";
    std::ofstream OutputFile3(OutputFileName3, std::ios::trunc);
    OutputFile3.close();


    std::ifstream inputFile2(source_file_directory);
  
    if (!inputFile2.is_open()) {
        std::cerr << "Failed to open input file." << std::endl;
        return 1;
    }
  

    std::string line;
    while (std::getline(inputFile2, line)) {
        if (startsWith(line, "#include") || 
            startsWith(line, "#define") ||
            startsWith(line, "#ifdef") ||
            startsWith(line, "#ifndef") ||
            startsWith(line, "#if") ||
            startsWith(line, "#elif") ||
            startsWith(line, "#else") ||
            startsWith(line, "#endif") ) {
            OutputFile2 << line << std::endl;
        }
    }
    
    std::cout << "Header file created successfully." << std::endl;
    
    inputFile2.close();
    OutputFile2.close();
      


    copy_code(source_file_directory, "myHeader.h");
    Result = Tool.run(newFrontendActionFactory(&Finder).get());
    //TODO: check here, if bug, record global index, output all_function_names[i].
    
    cd(built_directory.c_str());
    std::string this_command1 = replaceSubstring(correct_compiler_command, original_file, "function_code.c");
    this_command1 = this_command1 + " -o my_tempA.o";
    std::string this_command2 = replaceSubstring(buggy_compiler_command, original_file, "my_temp.c");
    this_command2 = this_command2 + " -o my_tempB.o";
    std::string original_o_file = extractStringAfterO(buggy_compiler_command);
    std::string this_command3 = "ld -r my_tempA.o my_tempB.o -o " + original_o_file;
    
    std::cout << "====================================" << std::endl;
    std::cout << "====================================" << std::endl;
    std::cout << "====================================" << std::endl;
    std::cout << "==========Testing Function==========" << std::endl;
    std::cout << all_function_names[i] << std::endl << std::endl;
    std::cout << this_command1 << std::endl << std::endl;
    std::cout << this_command2 << std::endl << std::endl;
    std::cout << this_command3 << std::endl << std::endl;
    std::cout << buggy_compiler_command << std::endl << std::endl;
    std::cout << original_o_file << std::endl << std::endl;
    std::cout << built_directory << std::endl << std::endl;
    std::cout << check_directory << std::endl << std::endl;

    if (std::system(this_command1.c_str()) != 0) {
      std::cout << this_command1 << std::endl;
      exit(1);
    }
    if (std::system(this_command2.c_str()) != 0) {
      std::cout << this_command2 << std::endl;
      exit(1);
    }
    if (std::system(this_command3.c_str()) != 0) {
      std::cout << this_command3 << std::endl;
      exit(1);
    }
    if (std::system(make_script.c_str()) != 0) {
      std::cout << make_script << std::endl;
      exit(1);
    }
   
    cd(check_directory.c_str());
    int this_exit_code = std::system(check_script.c_str());
    if (this_exit_code != correct_exit_code) {
      buggy_function.push_back(all_function_names[i]);
    }
  }

  for (int i = 0; i < buggy_function.size(); i++) {
    std::cout << "----------buggy_function-------" << std::endl;
    std::cout << "----------buggy_function-------" << std::endl;
    std::cout << buggy_function[i] << std::endl;
    std::cout << "----------buggy_function-------" << std::endl;
    std::cout << "----------buggy_function-------" << std::endl;
  }

  return Result;
}
