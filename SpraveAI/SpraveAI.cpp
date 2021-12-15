// SpraveAI.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>

#include <chrono>

#include "AdbCommunication.h"


char png_folder[] = "screencaps";
char filenane_phone[] = "/sdcard/temp/scrcap.png ";
 
void mkdir(const char* foldername)
{
	char buffer[30];
	sprintf_s(buffer, sizeof(buffer), "mkdir %s", png_folder);

	system(buffer);
}

void string_concat(char* buffer, const char* s1, const char* s2)
{
	sprintf(buffer, "%s%s", s1, s2);
}

int64_t millis()
{
	using namespace std::chrono;
	milliseconds ms = duration_cast<milliseconds>(
		system_clock::now().time_since_epoch()
	);
	return ms.count();
}

int main()
{
	char buffer[100];
	mkdir(png_folder);
	string_concat(buffer, png_folder, "/capture.png");
	
	int64_t time = 0;
	while (1)
	{
		int64_t new_time = millis();
		int64_t diff = new_time - time;
		double frequency = 1000.0 / (diff);
		time = new_time;
		std::cout << diff << "ms\t" << frequency << "fps" << std::endl;
		AdbCommunication::get_screencap(filenane_phone, buffer);
	}
	

	
}












