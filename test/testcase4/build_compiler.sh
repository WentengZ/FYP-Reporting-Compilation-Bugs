mkdir llvm-build
cd ./llvm-build

cmake ../llvm -DLLVM_ENABLE_PROJECTS=clang -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release 


make -j8
