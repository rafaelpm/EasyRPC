cd build
rm -rf *
cmake ..
make
./FunctionsBuilder -in ../../Tests/myFuncions.h -out ../../Tests/