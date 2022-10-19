set PHONE_PATH="/sdcard/image.png"
set LOCAL_PATH="C:/Users/JannisB98/mydata/vsProjekte/SpraveAI"

adb.exe shell screencap -p %PHONE_PATH%
adb.exe pull %PHONE_PATH% %LOCAL_PATH%
