#include "AdbCommunication.h"


void AdbCommunication::Device::get_screencap(const char* filename_phone, const char* filename_to)
{
	static AdbCommunication::Device device;
	device.screencap_phone(filename_phone);
	device.pull_screencap(filename_phone, filename_to);
}
