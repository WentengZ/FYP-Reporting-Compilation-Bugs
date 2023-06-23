# FYP-Reporting-Compilation-Bugs

All details manual is in Final Report. Please have a look inside.

All the tests can be run, but modify the input file to your own directory,

and build the compiler and the large project yourself

# A simple example steps to run testcase 7.
Clone the project

In root directory of FYP-Reporting-Compilation-Bugs

build C-Locate and C-Locate-Function
```
make
```

Build the compiler
```
cd test/testcase7/llvm-project-331fb804c96008bbceec8e5f305fc27f06d58ba6/

mkdir build && cd build

cmake -DLLVM_EXTERNAL_CLANG_SOURCE_DIR=../clang   ../llvm/

make
```

Go back to root directory of FYP-Reporting-Compilation-Bugs

Build ZShell project
```
cd test/testcase7/

tar -xf zsh-5.9.tar.xz

cd zsh-5.9/

mkdir build && cd build

# This should be a correct version of compiler.
# only clang is supported in current implementation.
# Basically any normal clang should work.
export CC=clang

../configure

make
```

Go back to root directory of FYP-Reporting-Compilation-Bugs

Modify the input configuration file for running this test. 
```
vim test/testcase7/test_input.config

# --------------in vim-------------------
# search for all FYP
/FYP

# add your own abosolute path in front.
# In this case, there should be three
# On my system for example, the change would be
built_directory=/home/alan/FYP-Reporting-Compilation-Bugs/test/testcase7/zsh-5.9/build
buggy_compiler=/home/alan/FYP-Reporting-Compilation-Bugs/test/testcase7/llvm-project-331fb804c96008bbceec8e5f305fc27f06d58ba6/build/bin/clang
source_file_directory=/home/alan/FYP-Reporting-Compilation-Bugs/test/testcase7/zsh-5.9/

# exit vim
ESC
:wq

#-------------out of vim-----------------
```

Run C-Locate on this testcase
```
./C-Locate test/testcase7/test_input.config
```

It should generate expected output and generate an input to C-Locate-Function

Run C-Locate-Function with generated output.
```
./C-Locate-Function input_to_2nd_tool.txt
```


