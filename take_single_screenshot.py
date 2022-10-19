import time
from ppadb.client import Client as AdbClient

def millis():
    return round(time.time_ns() / 1000000)
    
def getSingleDevice():
    # Default is "127.0.0.1" and 5037
    client = AdbClient(host="127.0.0.1", port=5037)
    devices = client.devices()
    
    if(len(devices) != 1):
        print("Detected %d devices"%(len(devices)))
        return None
    return devices[0]
    
def main():
    device = getSingleDevice()
    if device is None:
        print("Initialization failed")
        quit()
        
    start_ts = millis()
    image = device.screencap()
    with open("image.png", "wb") as file:
        file.write(image)
    
    current_ts = millis()
    print(f"Start:\t{start_ts}\nDiff:\t{current_ts}\nDifference:\t{current_ts-start_ts}")
    
if __name__ == "__main__":
    main()