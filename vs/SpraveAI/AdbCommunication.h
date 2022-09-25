#pragma once

#include <iostream>


namespace AdbCommunication
{
	class Device
	{
	private:
		char command[100];


		void screencap_phone(const char* filename_phone)
		{
			sprintf_s(command, sizeof(command), "adb.exe shell screencap -p %s", filename_phone);
			// sprintf_s(command, sizeof(command), "adb.exe shell screencap -p %s", filename_phone);
			system(command);
		}

		void pull_screencap(const char* filename_phone, const char* filename_to)
		{
			sprintf_s(command, sizeof(command), "adb.exe pull %s %s", filename_phone, filename_to);
			system(command);
		}

	public:
		static void get_screencap(const char* filename_phone, const char* filename_to);
	};	
};

