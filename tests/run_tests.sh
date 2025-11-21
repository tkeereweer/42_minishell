#!/bin/bash

# Main test runner script
# This script compiles and runs all builtin tests

echo "Building tests..."
make

if [ $? -ne 0 ]; then
    echo "Error: Failed to build tests"
    exit 1
fi

echo ""
echo "Running tests..."
./run_all_tests.sh

exit $?
