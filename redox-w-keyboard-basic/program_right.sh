#!/bin/bash

MAKEDIR=$(dirname "$(readlink -f "$0")")/custom/armgcc/
HEX=_build/nrf51822_xxac-keyboard-right.hex

echo '=============================== MAKING ================================'
rm -rf $(dirname "$(readlink -f "$0")")/custom/armgcc/_build
make -C ${MAKEDIR}
if [[ $? -ne 0 ]] ; then
    exit 0
fi
sleep 0.1
HEX=`readlink -f $(dirname "$(readlink -f "$0")")/custom/armgcc/${HEX}`
du -b $HEX

echo
echo '============================= PROGRAMMING ============================='
{
	echo "reset halt";
	sleep 0.2;
	echo "nrf51 mass_erase";
	sleep 0.3;
	echo "reset halt";
	sleep 0.1;
	echo "flash write_image erase" $HEX;
	sleep 11;
	echo "reset";
	sleep 0.1;
	exit;

} | telnet 127.0.0.1 4444

echo
echo '============================== FINISHED ==============================='
