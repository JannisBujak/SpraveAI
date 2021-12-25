#pragma once

#include <iostream>


namespace AdbCommunication
{
	namespace {
		
		void screencap_phone(const char* filename_phone)
		{
			char command[100];
			sprintf_s(command, sizeof(command), "adb shell screencap -p %s", filename_phone);
			system(command);
		}

		void pull_screencap(const char* filename_phone, const char* filename_to)
		{
			char command[100];
			sprintf_s(command, sizeof(command), "adb.exe pull %s %s", filename_phone, filename_to);
			system(command);
		}


	}
	void get_screencap(const char* filename_phone, const char* filename_to);
};

