import serial
import time
import os


USB="/dev/ttyACM0"
BT="/dev/rfcomm0"


bluetooth=serial.Serial(BT, 9600)
print("Connected")

#bluetooth.flushInput() 

signs=['A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'hello', ]

#os.system("espeak {}".format("\"sudip is a boy\""))




while(1):
    input_data=bluetooth.readline()
    x=(int(input_data.decode()))

    os.system(f"espeak \"{signs[x]}\"")

 
print("Done")
