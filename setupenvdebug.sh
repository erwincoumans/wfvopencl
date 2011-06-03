export PACKETIZER_INSTALL_DIR=$HOME/proj/packetizer
export LLVM_INSTALL_DIR=$HOME/proj/anysl2/install/build-gcc-debug-x86_64/llvm
export PATH=$PATH:./bin/x86_64

export PATH=$PATH:./include
export PATH=$PATH:$LLVM_INSTALL_DIR/bin
export PATH=$PATH:$LLVM_INSTALL_DIR/include

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./build/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$LLVM_INSTALL_DIR/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PACKETIZER_INSTALL_DIR/lib