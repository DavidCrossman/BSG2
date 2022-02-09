cmake --build linux-out/Debug
cmake --build linux-out/Release
strip --strip-all linux-out/Release/libbsg2.so