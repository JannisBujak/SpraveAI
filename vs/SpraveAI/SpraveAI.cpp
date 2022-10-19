// SpraveAI.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>
#include "ScreenscapThread.h"

extern uint64_t millis()
{
    using namespace std::chrono;
    uint64_t ms = duration_cast<milliseconds>(
        system_clock::now().time_since_epoch()
        ).count();
    return ms;
}

void take_screenshot()
{
    std::cout << "Waiting" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds{ 2000 });
}

int main(int argc, char* argv[])
{
    std::mutex fileAccess;
    ScreenscapThread tScreencap("dump.bin", fileAccess);
    tScreencap.start();

    while (1)
    {
        char* buffer;
        while (buffer = tScreencap.getFilecontent())
        {
            uint32_t* bufferAsU4 = (uint32_t*)buffer;
            uint32_t width = bufferAsU4[0],
                height = bufferAsU4[1];
            std::cout << width << "(w) x " << height << "(h)" << std::endl;
        }
    }
}
