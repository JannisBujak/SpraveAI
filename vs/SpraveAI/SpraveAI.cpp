// SpraveAI.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>

#include <string>
#include <chrono>
#include <thread>
#include <mutex>

#include "AdbCommunication.h"

const char png_folder[] = "screencaps";
// const char filenane_phone[] = "/sdcard/temp/scrcap.png";
const char filenane_phone[] = "/storage/emulated/0/mytemp/scrcap.png";

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

void getNextScreenshot(std::mutex* run_mutex, std::mutex* mutex)
{
	std::cout << "Thread geht ab" << std::endl;
	uint64_t counter;
	for(counter = 0; run_mutex->try_lock(); counter++)
	{
		run_mutex->unlock();
		AdbCommunication::Device::get_screencap(filenane_phone, png_folder);
	}	
	std::cout << "Thread finished after " << counter << " screenshots" << std::endl;
}

int main(int argc, char* argv[])
{
	std::mutex thread_running_mutex, file_mutex;
	char buffer[100];
	mkdir(png_folder);
	string_concat(buffer, png_folder, "/capture.png");
	
	uint64_t start_millis = millis(), end_millis;

	std::thread imggetter_thread(getNextScreenshot, &thread_running_mutex , &file_mutex);

	std::this_thread::sleep_for(std::chrono::seconds(5));
	std::lock_guard<std::mutex> guardiola(thread_running_mutex);
	std::cout << "locked" << std::endl;
	imggetter_thread.join();
	
	end_millis = millis();
	std::cout << "Start:\t" << start_millis << std::endl
		<< "End:\t" << end_millis << std::endl
		<< "Difference:\t" << (end_millis - start_millis) << std::endl;
	
	// std::thread png_save_thread(thread_routine, argv[0]);
	// thread_routine(argv[0]);

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

