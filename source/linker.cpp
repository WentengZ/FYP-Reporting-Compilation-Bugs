
#ifndef INCLUDE_LINKER
#define INCLUDE_LINKER
#include "../include/linker.h"
#endif

void Linker::link() {
    std::string link_command = Linker::linker_dir + " *.o -o test.out";
    std::system(link_command.c_str());
}