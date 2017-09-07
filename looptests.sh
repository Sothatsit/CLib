#!/usr/bin/env bash

trap "exit;" SIGINT SIGTERM

while true; do
    find ./src ./test | entr -d ./test.sh
done