// SpraveAI.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>

#include <chrono>

#include "AdbCommunication.h"

#include <conio.h>
#include <Python.h>

#include <thread>

char png_folder[] = "screencaps";
char filenane_phone[] = "/sdcard/temp/scrcap.png ";

#define PY_SSIZE_T_CLEAN
#include <Python.h>

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

void thread_routine(char* arg)
{
	{
		wchar_t* program = Py_DecodeLocale(arg, NULL);
		if (program == NULL)
		{
			fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
			exit(1);
		}
		Py_SetProgramName(program);  /* optional but recommended */
		Py_Initialize();
		int64_t time = 0;
		for (int i = 0; i < 100; i++)
		{
			int64_t new_time = millis();
			int64_t diff = new_time - time;
			double frequency = 1000.0 / (diff/1);
			time = new_time;
			std::cout << diff << "ms\t" << frequency << "fps" << std::endl;


			char command[800];
			sprintf_s(command, sizeof(command),
				"from ppadb.client import Client as AdbClient \n\
client = AdbClient() \n\
devices = client.devices() \n\
device = devices[0] \n\
if device is None: \n\
	print('Initialization failed') \n\
	quit() \n\
i = 0\n\
while(i < 1): \n\
	image = device.screencap()\n\
	ssName = f'screencaps/SvZ_Bot_Cap{i}.png' \n\
	#print(ssName) \n\
	#with open(ssName, 'wb') as file: \n\
	#	file.write(image)\n\
	i = i + 1");


			PyRun_SimpleString(command);

		}		

		if (Py_FinalizeEx() < 0) {
			exit(120);
		}
		PyMem_RawFree(program);
	}
}

int main(int argc, char* argv[])
{
	char buffer[100];
	mkdir(png_folder);
	string_concat(buffer, png_folder, "/capture.png");
	
	
	// std::thread png_save_thread(thread_routine, argv[0]);
	thread_routine(argv[0]);

	/*
	while (!png_save_thread.joinable())
	{
		std::cout << "LELELE" << std::endl;
	}
	png_save_thread.join();
	std::cout << "Finished" << std::endl;
	*/

	/*
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
	*/
	
	

	
}












