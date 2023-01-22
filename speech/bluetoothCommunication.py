import serial
import os
import keyboard


USB="/dev/ttyACM0"
BT="/dev/rfcomm0"

x=serial.Serial(BT,9600,timeout=1)


# class Sign:
#     def __init__(self,bcd,sign):
#         self.played=0
#         self.bcd=bcd
#         self.sign=sign
    
#     def setPlayed(self,x):
#         self.played=x

signs=['A','E','I','O','U','my','hello', 'world','i love you','good morning']

# signObjs=[Sign(s) for s in signs]

played=0
y=0

while(1):
    try:
        prev=y
        y=(int(x.readline().decode()))
        print(y)
        # if(y<0.5 and played==0):
        #     keyboard.press_and_release("right")
        #     played=1
        # if(y>0.5):
        #     played=0

        if(y==69420 or y==42069):
            if(y==69420):
                keyboard.press_and_release("right")
            if(y==42069):
                keyboard.press_and_release("left")

        else:
            if(played==0):
                os.system(f"espeak \"{signs[y]}\"")
                played=1
        
            if(prev!=y):
                played=0
        

    except Exception as e:
        print(e)
