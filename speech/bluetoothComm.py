import serial
import time
import os


port="/dev/tty.HC-06-DevB" 
bluetooth=serial.Serial(port, 9600)
print("Connected")

#bluetooth.flushInput() 

signs={
 "1":"A",
"2":"B",
"3":"C",
"4":"D",
"5":"E",
"6":"F",
"7":"G",
"8":"H",
"9":"I",
"10":"J",
"11":"K",
"12":"L",
"13":"M",
"14":"N",
"15":"O",
"16":"P",
"17":"Q",
"18":"R",
"19":"S",
"20":"T",
"21":"U",
"22":"V",
"23":"W",
"24":"X",
"25":"Y",
"26":"Z"
}

#os.system("espeak {}".format("\"sudip is a boy\""))




while(1):
    input_data=bluetooth.readline()
    x=int(input_data.decode())

    os.system(f"espeak \"{x}\"")

 
print("Done")
