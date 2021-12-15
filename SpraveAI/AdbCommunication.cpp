#include "AdbCommunication.h"


void AdbCommunication::get_screencap(const char* filename_phone, const char* filename_to)
{
	screencap_phone(filename_phone);
	pull_screencap(filename_phone, filename_to);
}
