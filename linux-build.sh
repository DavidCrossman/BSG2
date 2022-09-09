cmake --build out/build/linux-Debug
cmake --build out/build/linux-Release
strip --strip-all out/build/linux-Release/libbsg2.so