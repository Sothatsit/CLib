#!/usr/bin/env bash

clear
if make test; then
    echo ""
    ./build/test
else
    echo -e "\x1B[31m\x1B[1m"
    echo "-------------------------------------"
    echo "         Compilation Failed"
    echo "-------------------------------------"
    echo -e "\x1B[0m"
fi