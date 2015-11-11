#!/bin/bash

if [[ "$1" == "debug" ]]; then

	qemu-system-x86_64 -s -S -hda Image/x64BareBonesImage.qcow2 -m 512 -serial stdio

elif [[ "$1" == "monitor" ]]; then

	qemu-system-x86_64 -monitor stdio -hda Image/x64BareBonesImage.qcow2 -m 512 -serial stdio

else

	qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512 -serial stdio -no-reboot # -d cpu_reset,int

	#RIP=`grep "RIP" last_run.log | tail -n1 | awk -F'=' '{print $2}' | awk -F' ' '{print $1}'`
	#RIP=$((RIP + 2))

	#gobjdump -D Kernel/kernel.o | grep $RIP
fi

