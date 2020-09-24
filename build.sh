#!/bin/bash
set -e

if [ -z "$1" ]
then
    echo "Must specify branch to check out"
    exit 1
fi

cd math
git pull
git checkout $1
make clean-all
make -f make/standalone math-libs -j8
cd ../
make clean
if [ -z "$2" ]
then
    make -j8 --ignore-errors all
else
    make -j8 VARMAT=true --ignore-errors all
fi
