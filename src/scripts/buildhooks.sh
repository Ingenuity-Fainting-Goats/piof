#!/bin/bash

pushd () {
    command pushd "$@" > /dev/null
}

popd () {
    command popd "$@" > /dev/null
}

CFLAGS="-g -Wall -I. -I/opt/piof -I/usr/local/include/php -I/usr/local/include/php/main -I/usr/local/include/php/Zend -I/usr/local/include/php/TSRM -I/usr/local/include/php/Zend -I/usr/local/include/php/ext -I/usr/local/include/php/ext/date/lib -I/usr/include/php/20170718 -I/usr/include/php/20170718/main -I/usr/include/php/20170718/TSRM -I/usr/include/php/20170718/Zend -I/usr/include/php/20170718/ext -I/usr/include/php/20170718/ext/date/lib"
SCRIPTPATH="$( cd "$(dirname "$0")" ; pwd -P )"
HOOKSPATH=$SCRIPTPATH/../hooks

pushd $HOOKSPATH


echo "cleaning up compiled artifacts..."
rm -rf *.h *.so
rm -rf build/

for dir in */                                                     
do
  mkdir -p build
  pushd $dir                                                                                 
  CFILE=${dir::-1}
  echo "compiling file $CFILE from $(pwd)..."
  gcc -shared -Wall -fPIC $CFLAGS -o ../build/$CFILE.so hook.c
  popd
done

popd
