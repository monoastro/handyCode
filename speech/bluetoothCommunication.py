import serial
import os
import keyboard


USB = "/dev/ttyACM0"
BT = "/dev/rfcomm0"
df = 10
dajfl= 100
gloveOutput = serial.Serial(USB,9600,timeout=1)
signs = ['','How are you','','cats','Welcome','I love electronics', 'welcome', 'Have a nice day', 'Nepal Television', 'to Thapathali','name', '', '', '']
hasPlayed = 0 #to prevent multiple loopy instances of the same sign 
index = 0

while(1):
    try:
        prevIndex = index
        index = (int(gloveOutput.readline().decode()))
        print(index)
        #print(signs[index])

        #presentation control
        if(index == 69420 or index == 42069):
            if(index == 69420):
                keyboard.press_and_release("right")
            elif(index==42069):
                keyboard.press_and_release("left")
        
        #piano
        #else if():
        
        #asl - kinda
        else:
            if(hasPlayed == 0):
                os.system(f"espeak \"{signs[index]}\"")
                hasPlayed = 1
            elif(prevIndex != index):
                hasPlayed = 0

    except Exception as exception:
        print(exception)
