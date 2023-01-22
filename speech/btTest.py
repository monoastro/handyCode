import serial
import os
import keyboard

x=serial.Serial("/dev/ttyACM0",9600,timeout=1)

played=0

while(1):
    try:
        y=(float(x.readline().decode()))
        if(y<0.5 and played==0):
            keyboard.press_and_release("right")
            played=1
        if(y>0.5):
            played=0

    except Exception as e:
        print(e)