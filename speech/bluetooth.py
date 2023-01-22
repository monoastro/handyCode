import serial
import time
import os
import keyboard

USB="/dev/ttyACM0"
BT="/dev/rfcomm0"


bluetooth=serial.Serial(BT, 9600)
print("Connected")

#bluetooth.flushInput() 

signs=['A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'hello', ]

#os.system("espeak {}".format("\"sudip is a boy\""))



def slide(x):
    if(x==69420):
        keyboard.press_and_release('right')
    if(x==42069):
        keyboard.press_and_release('left')


while(1):
    try:
        input_data=bluetooth.readline()
        x=(int(input_data.decode()))

        if(x==69420 or x==42069):
            slide(x)
        else:
            os.system(f"espeak \"{signs[x]}\"")
    except:
        print("dbf")
 
print("Done")