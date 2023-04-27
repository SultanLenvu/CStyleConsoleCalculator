mkdir build
mkdir bin
cd build
cmake ..\sources -G "MinGW Makefiles"
mingw32-make
copy Calculator.exe ..\bin