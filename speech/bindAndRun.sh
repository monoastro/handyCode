#!/bin/bash

echo -n "Enter mode; Serial(0), Blue-toot(1)"
echo -e
read mode

if [[ $mode -eq 0 ]]
then
	sudo chmod a+rw /dev/ttyACM0
elif [[ $mode -eq 1 ]]
then
	rfcomm bind rfcomm0 00:19:10:08:71:24
	sudo chmod a+rw /dev/rfcomm0
else
	echo "Invalid value kiddo. Read the above line carefully" 
fi

python bluetoothCommunication.py

