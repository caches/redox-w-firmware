#!/bin/bash

BINPATH="./redox-w-firmware/bin"
SCRIPT_DIR=$(dirname "$(readlink -f "$0")")
cd $SCRIPT_DIR

echo '=============================== MAKING ================================'
cd custom/armgcc
if [ "$1" == "clean" ];then
	rm -rf _build
    rm -rf $BINPATH/recevier.hex

fi
make
if [[ $? -ne 0 ]] ; then
    exit 0
fi
sleep 0.1
HEX=`readlink -f _build/nrf51822_xxac-receiver.hex`
du -b $HEX

cd /usr/src/nRF5_SDK_11

if [ ! -d "$BINPATH" ];then
  echo "bin path is not exists,create it"
  mkdir $BINPATH
fi
mv $HEX $BINPATH/recevier.hex
# echo
# echo '============================= PROGRAMMING ============================='
# {
# 	echo "reset halt";
# 	sleep 0.1;
# 	echo "flash write_image erase" $HEX;
# 	sleep 11;
# 	echo "reset";
# 	sleep 0.1;
# 	exit;

# } | telnet localhost 4444

echo
echo '============================== FINISHED ==============================='
