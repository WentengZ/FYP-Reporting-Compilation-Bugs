cd ./build

cmake ../llvm -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_PROJECTS="clang" -G "Unix Makefiles"

make -j8
