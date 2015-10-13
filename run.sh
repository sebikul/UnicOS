#!/bin/bash

if [[ "$1" == "debug" ]]; then
	
	qemu-system-x86_64 -s -S -hda Image/x64BareBonesImage.qcow2 -m 512 -serial stdio

elif [[ "$1" == "monitor" ]]; then

	qemu-system-x86_64 -monitor stdio -hda Image/x64BareBonesImage.qcow2 -m 512 -serial stdio

else

	qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512 -serial stdio

fi

