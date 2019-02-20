#!/bin/bash

pushd () {
    command pushd "$@" > /dev/null
}

popd () {
    command popd "$@" > /dev/null
}

SCRIPTPATH="$( cd "$(dirname "$0")" ; pwd -P )"
SRCPATH=$SCRIPTPATH/..

pwd
./scripts/buildhooks.sh

pushd $SRCPATH

echo "[+] Building Extension....\n"


make clean
phpize clean

phpize
./configure --enable-piof

make
make install

popd