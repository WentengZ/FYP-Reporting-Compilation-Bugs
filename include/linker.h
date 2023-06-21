#include <string>
#include <stdlib.h>
#include <iostream>
#include <vector>

class Linker {
    private:
        std::string linker_dir;
        std::string link_option;
    public:
        Linker(std::string _linker_dir,
               std::string _linker_op) {
            linker_dir = _linker_dir;
            link_option = _linker_op;
        }
        void link(); 

};