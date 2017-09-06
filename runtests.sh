#!/usr/bin/env bash

clear
if make; then
    echo ""
    ./testclib
else
    echo -e "\x1B[31m\x1B[1m"
    echo "-------------------------------------"
    echo "         Compilation Failed"
    echo "-------------------------------------"
    echo -e "\x1B[0m"
fi