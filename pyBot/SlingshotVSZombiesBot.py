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

class GameState(Enum):
    Menu = auto()
    inGame = auto()
    unidentifiable = auto()

def getVectorAToB(A, B):
    return { B[0] - A[0], B[1] - A[1] }


# Start Button          2200, 945
startButton = 2200, 945

# LeftWeapon menu       980, 940
# RightWeapon menu      1200, 940
LeftWeapon_menu = 980, 940
RightWeapon_menu = 1200, 940

# LeftWeapon ingame     975, 940
# RightWeapon ingame    1435, 940
LeftWeapon_ingame = 975, 940
RightWeapon_ingame = 1435, 940

# Joerg topl - botr     1045, 680 -  1370, 850
JoergSquare = 1045, 680, 1370, 850

def getJoergCenter():
   return (int)((JoergSquare[0] + JoergSquare[2]) / 2), (int)((JoergSquare[1] + JoergSquare[3]) / 2)
   
JoergCenter = getJoergCenter()   

zombie_bright_color = [230, 217, 147]
zombie_orange_color = [224, 161, 109]
zombie_grey_color = [222, 220, 206]
bats_color = [85, 83, 74]

device = getSingleDevice()

start = cv2.imread('start.png')
endrun = cv2.imread('endrun.png')

def pixelHasColor(pixel, color):
    #pixel has bgr, color is rgb
    for i in range(3):
        if(pixel[2-i] != color[i]):
            return False
    return True        

def isEnemyPixel(pixel):
    return pixelHasColor(pixel, zombie_bright_color) or pixelHasColor(pixel, zombie_orange_color) or pixelHasColor(pixel, zombie_grey_color)# or pixelHasColor(pixel, bats_color)

def findClosestEnemy(image):
    for y in range(0, len(image), 25):
        width = len(image[y])
        
        for x in range(0, (int)(width/2), 5):
            x_min = (int)((width / 2) - x)
            x_max = (int)((width / 2) + x)
            
            if isEnemyPixel(image[y][x_min]):
                return x_min,y
            elif isEnemyPixel(image[y][x_max]):
                return x_max,y   
    return None


class Bot():
    
    def __init__(self, image, device):
        self.image = image
        #self.device = device
        
        self.method = cv2.TM_SQDIFF_NORMED
            
    
    def getBotRightImage(self):
        w, h = self.getScreenSize()
        im = self.image[int(h*3/4):h, int(w*7/8):w]
        return im
    
    
    
    def getGameState(self):
        result1 = cv2.matchTemplate(start, self.getBotRightImage(), self.method) 
        min_max_loc1 = cv2.minMaxLoc(result1)
        
        result2 = cv2.matchTemplate(endrun, self.getBotRightImage(), self.method)
        min_max_loc2 = cv2.minMaxLoc(result2)
        
        
        gs_menu = ((min_max_loc1[0] < 0.1) or (min_max_loc2[0] < 0.1))
        return GameState.Menu if (gs_menu) else GameState.inGame 
    
    def getScreenSize(self):
        return [len(self.image[0]), len(self.image)]
        
    def getStartPoint(self, screen_size, vector):
        x = (0.90 if (vector[0] < 0) else 0.1) * screen_size[0]
        y = (0.90 if (vector[1] < 0) else 0.1) * screen_size[1]
        return [x, y]
    
    def strengthen_shot(self, vector):
        s_size = self.getScreenSize()
        factor = min(s_size[0]/max(1, abs(vector[0])), s_size[1]/max(1, abs(vector[1])))
        # take the vector times 0.9 to not collide with the frame of the window
        factor = int(0.8 * factor)
        vector = [vector[0]*factor, vector[1]*factor]
        sp = self.getStartPoint(s_size, vector)
        return sp[0] + vector[0],  sp[1] + vector[1], sp[0],  sp[1]
    
    def shootFor(self, enemy):       
        vector = [ enemy[0] - JoergCenter[0],  enemy[1] - JoergCenter[1] ]
        swipe = self.strengthen_shot(vector)
        
        command1 = f"input touchscreen swipe {swipe[0]} {swipe[1]} {swipe[2]} {swipe[3]} 500"
        
        if command_q.empty():
            command_q_sem.acquire()
            command_q.put(command1)
            command_q_sem.release()
        
    
    def process(self):
        print("Analyzing game")
        gs = self.getGameState()
        if gs is GameState.Menu:
            # press start
            device.shell(f"input touchscreen tap {startButton[0]} {startButton[1]}")
        elif gs is GameState.inGame:
            print("Looking for enemy")
         
            closest_enemy = findClosestEnemy(self.image)
                
            if(closest_enemy is not None):
                self.shootFor(closest_enemy)
            

def millis():
    return round(time.time_ns() / 1000000)

command_q = queue.Queue()
command_q_sem = threading.Semaphore()

def exec_commands():
    while(1):
        if not command_q.empty():
            command_q_sem.acquire()
            command = command_q.get()
            command_q_sem.release()
            device.shell(command)

command_thread = threading.Thread(target=exec_commands,daemon=True)

def main():
    if device is None:
        print("Initialization failed")
        quit()
    
    command_thread.start()
    
    points = startButton, LeftWeapon_menu, RightWeapon_menu, LeftWeapon_ingame, RightWeapon_ingame, JoergSquare
    print(points)
    
    last_ts = millis()
    for i, b in enumerate(iter(bool, True)):
        current_ts = millis()
        print()
        print(f"New run\t{current_ts - last_ts}")
        last_ts = current_ts
        
        image = device.screencap()
        
        #ssName = f"C:/temp/SvZ_Bot_Cap{(i%10)}.png"
        ssName = f"C:/Users/JannisB98/mydata/Prog/vsProjekte/SpraveAI/SpraveAI/screencaps/SvZ_Bot_Cap{(i%10)}.png"
        print(ssName)
        with open(ssName, "wb") as file:
            file.write(image)
            
        """
        image = cv2.imread(ssName)

        bot = Bot(image, device)
        print("Bot created")
        print(millis() - last_ts)
        
        bot.process()
        print(millis() - last_ts)
        """

main()