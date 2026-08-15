#!/bin/bash
set -e
echo "Running ASan/UBSan build..."
cmake -B build_asan -DCMAKE_BUILD_TYPE=Debug -DUSE_ASAN=ON -DUSE_UBSAN=ON
cmake --build build_asan
./build_asan/tests/klyro_test_suite

echo "Running TSan build..."
cmake -B build_tsan -DCMAKE_BUILD_TYPE=Debug -DUSE_TSAN=ON
cmake --build build_tsan
./build_tsan/tests/klyro_test_suite
