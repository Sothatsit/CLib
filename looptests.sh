#!/usr/bin/env bash

find $PWD/src $PWD/test | entr ./runtests.sh