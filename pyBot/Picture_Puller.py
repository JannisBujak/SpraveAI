from ppadb.client import Client as AdbClient
from PIL import Image 
import numpy as np
from enum import Enum, auto
import cv2
import time
import threading, queue

def getSingleDevice():
    # Default is "127.0.0.1" and 5037
    client = AdbClient(host="127.0.0.1", port=5037)
    devices = client.devices()

    if(len(devices) != 1):
        print("Detected %d devices"%(len(devices)))
        return None
    return devices[0]

def millis():
    return round(time.time_ns() / 1000000)

device = getSingleDevice()

def main():
    if device is None:
        print("Initialization failed")
        quit()
    
    for i, b in enumerate(iter(bool, True)):
        current_ts = millis()
        begin_ts = millis()
        
        image = device.screencap()
        
        ssName = f"./pictures/SvZ_Bot_Cap{(i%10)}.png"
        print(ssName)
        with open(ssName, "wb") as file:
            file.write(image)
            
        end_ts = millis()
        
        print()
        print(f"Finished:\t{end_ts-begin_ts} ms")
        """
        image = cv2.imread(ssName)

        bot = Bot(image, device)
        print("Bot created")
        print(millis() - last_ts)
        
        bot.process()
        print(millis() - last_ts)
        """

main()