#!/bin/bash


if [[ "$1" = "linux" ]]; then
	unset GCC
	unset GPP
	unset LD
	unset AR
	unset ASM
	export MACOS=0

	echo "Reseteando variables..."
elif [[ "$1" == "macos" ]]; then
	#statements
	
	WORKDIR="$(pwd)"

	echo "Seteando basedir a ${WORKDIR}"

	export GCC="\"$WORKDIR/Cross64/bin/x86_64-elf-gcc\""
	export GPP="\"$WORKDIR/Cross64/bin/x86_64-elf-g++\""
	export LD="\"$WORKDIR/Cross64/bin/x86_64-elf-ld\""
	export AR="\"$WORKDIR/Cross64/bin/x86_64-elf-ar\""
	export ASM="/usr/local/bin/nasm"
	export MACOS=1

	echo "GCC Cross-Compiler configurado..."

else

	echo -e "Uso: source set-variables.sh [linux|macos]\n\tPor defecto se utiliza linux."

fi


