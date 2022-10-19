#include "ScreenscapThread.h"
#include <sys/stat.h>
#include <fstream>


ScreenscapThread::ScreenscapThread(const char* a_filename, std::mutex& a_fileAccess)
	: m_filename(std::move(a_filename)),
	m_thread(nullptr),
	m_fileAccess(a_fileAccess),
	m_aborted(false)
{
}

ScreenscapThread::~ScreenscapThread()
{
	if (m_thread)
	{
		std::lock_guard<std::mutex> guard(m_fileAccess);
		m_runmutex.lock();
		m_aborted = true;
	}
}

char* ScreenscapThread::getFilecontent()
{
	if (m_buffer.m_buffer)
	{
		std::lock_guard<std::mutex>(this->m_fileAccess);
		return m_buffer.detach();
	}
	return nullptr;
}

bool ScreenscapThread::wholeFileIntoBuffer()
{
	struct stat filestats;
	int rc = stat(m_filename.c_str(), &filestats);
	if (rc == 0)
	{
		std::cout << "Size " << filestats.st_size << std::endl;
		
		m_buffer.setSize(filestats.st_size+1);
		std::ifstream file(m_filename.c_str(), std::ios::binary);
		
		file.read(m_buffer.m_buffer, m_buffer.m_size);

		std::cout << "Buffer of size\t" << m_buffer.m_size << std::endl;
		std::cout << "Further Bytes available:\t" << (bool)file << std::endl;		
		return !file;
	}
	else 
	{
		std::cout << "Konnte Dateigröße nicht lesen"; 
		return false;
	}
}

void ScreenscapThread::start()
{
	m_thread = new std::thread (ScreenscapThread::run, this);
}

void ScreenscapThread::do_work()
{
	char formated_command[200];
	sprintf_s(formated_command, sizeof(formated_command), "adb.exe shell screencap > %s", m_filename.c_str());
	while (m_runmutex.try_lock())
	{
		uint64_t start_ms = millis(), end_ms;
		m_fileAccess.lock();
		
		// Pull next screenshot from phone and read the file
		system(formated_command);
		
		if (!wholeFileIntoBuffer())
		{
			m_buffer.setSize(0);
		}
		
		end_ms = millis();
		std::cout << "Start\t" << start_ms << std::endl
			<< "End\t" << end_ms << std::endl
			<< "Difference\t" << (end_ms - start_ms) << std::endl;
		
		m_fileAccess.unlock();

		// File-access ended. Cleanup
		
		

		
						
		// std::this_thread::sleep_for(std::chrono::milliseconds{ 100 });
		m_runmutex.unlock();	
	}
}

void ScreenscapThread::run(ScreenscapThread* a_screenscapThread)
{
	a_screenscapThread->do_work();
}
