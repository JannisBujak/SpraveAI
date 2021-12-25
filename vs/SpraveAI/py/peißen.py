from ppadb.client import Client as AdbClient
import time

client = AdbClient(host='127.0.0.1', port=5037)
devices = client.devices() 
device = devices[0] 

def millis():
    return round(time.time_ns() / 1000000)


if device is None: 
	print('Initialization failed') 
	quit() 
while(1):
    i = 0
    last_ms = millis()
    while(i < 10): 
        image = device.screencap()
        ssName = f'screencaps/SvZ_Bot_Cap{i}.png' 
        print(ssName) 
        with open(ssName, 'wb') as file: 
            file.write(image)
        i = i + 1
    now_ms = millis()
    print(now_ms-last_ms)
    time.sleep(1)
    