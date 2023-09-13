#!/bin/bash

# Default flags
clean_project=false

# Parse command-line arguments
while getopts "c" opt; do
    case $opt in
        c)
            clean_project=true
            ;;
        \?)
            usage
            ;;
    esac
done

if [ "$clean_project" = true ]; then
    make clean
fi

make


input_files=("test1.mat test2.mat" "test3.mat test4.mat" "test5.mat test6.mat")

# Run tests with a loop
for input_file in "${input_files[@]}"; do
    valgrind --leak-check=full ./main.exe <<< "$input_file"
done


echo "Script completed successfully."
