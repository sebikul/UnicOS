#!/bin/bash

if [[ "$1" = "reset" ]]; then
	unset GCC
	unset GPP
	unset LD
	unset AR
	unset ASM

	echo "Reseteando variables..."
else
	export GCC=/Users/sebikul/Cross64/bin/x86_64-elf-gcc
	export GPP=/Users/sebikul/Cross64/bin/x86_64-elf-g++
	export LD=/Users/sebikul/Cross64/bin/x86_64-elf-ld
	export AR=/Users/sebikul/Cross64/bin/x86_64-elf-ar
	export ASM=/Users/sebikul/Cross64/bin/x86_64-elf-as

	echo "GCC Cross-Compiler configurado..."

fi


