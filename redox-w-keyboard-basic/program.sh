#!/bin/bash
BINPATH="./redox-w-firmware/bin"
if [ "$1" == "clean" ];then
	echo "clean build path"
	rm -rf $(dirname "$(readlink -f "$0")")/custom/armgcc/_build
    rm -rf $BINPATH/left-keyboard.hex
    rm -rf $BINPATH/right-keyboard.hex
fi
MAKEDIR=$(dirname "$(readlink -f "$0")")/custom/armgcc/

echo '=============================== MAKING ================================'
make -C ${MAKEDIR}
if [[ $? -ne 0 ]] ; then
    exit 0
fi
sleep 0.1

if [ ! -d "$BINPATH" ];then
  echo "bin path is not exists,create it"
  mkdir $BINPATH
fi


mv $MAKEDIR/_build/nrf51822_xxac-keyboard-left.hex $BINPATH/left-keyboard.hex
mv $MAKEDIR/_build/nrf51822_xxac-keyboard-right.hex $BINPATH/right-keyboard.hex
echo "move hex file to bin path success!"
echo '============================== FINISHED ==============================='
