#!/bin/bash
if [ "$1" == "clean" ];then
	echo "clean build path"
	rm -rf $(dirname "$(readlink -f "$0")")/custom/_build
fi
MAKEDIR=$(dirname "$(readlink -f "$0")")/custom/armgcc/

echo '=============================== MAKING ================================'
make -C ${MAKEDIR}
if [[ $? -ne 0 ]] ; then
    exit 0
fi
sleep 0.1
BINPATH="./redox-w-firmware/bin"
if [ ! -d "$BINPATH" ];then
  echo "bin path is not exists,create it"
  mkdir $BINPATH
fi


mv $MAKEDIR/_build/nrf51822_xxac-keyboard-left.hex $BINPATH/left-keyboard.hex
mv $MAKEDIR/_build/nrf51822_xxac-keyboard-right.hex $BINPATH/right-keyboard.hex
echo "move hex file to bin path success!"
echo '============================== FINISHED ==============================='
