import time
import subprocess
import cv2
import numpy as np
from ppadb.client import Client as AdbClient

import uiautomator2 as u2


device = u2.connect('192.168.178.28')
#print(device.info)


def millis():
    return round(time.time_ns() / 1000000)

start_time = millis()

import pyautogui

i = 0
while(i < 10):
    
    myscreen = pyautogui.screenshot()
    myscreen.save('myscreen.png')
    
    end_time = millis()
    i = i+1

end_time = millis()
print('screencap cost', end_time - start_time)