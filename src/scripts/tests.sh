#!/bin/bash

service rsyslog restart
sleep 2
echo "[+] Testing Hooks....\n"

SCRIPTPATH="$( cd "$(dirname "$0")" ; pwd -P )"
SRCPATH=$SCRIPTPATH/..

testCode(){
    echo "#############################"
    echo "Testing command: $1"
    (cd $SRCPATH/modules && php -dextension=./piof.so -r "$1")
    echo "#############################"
}

testCode 'echo md5( "ciao");'
testCode 'echo md5( "asd" );'
testCode 'include("notfound.php");'
testCode 'system( "id");'
testCode 'eval("$var = 41;");'
sleep 2
echo "[i] Log Debug /var/log/piof.debug.log...."
tail /var/log/piof.debug.log
echo ""
echo "[i] Log Info /var/log/piof.info.log...."
tail /var/log/piof.info.log