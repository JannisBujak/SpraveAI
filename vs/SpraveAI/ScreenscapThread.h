#pragma once

#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>
#include <semaphore>

extern uint64_t millis();

class ScreenscapThread 
{
public:
/*A buffer based on new, that tracks its own size and reallocates if wanted*/
	template <typename T>
	struct Buffer
	{
		int m_size;
		T* m_buffer;

		Buffer()
			: m_buffer(nullptr), m_size(0)
		{}

		Buffer(int size)
			: m_size(size)
		{
			m_buffer = new T[size];
		}

		~Buffer()
		{
			delete m_buffer;
		}

		void setSize(int new_size)
		{
			if (new_size == m_size)
				// Size does not have to be changed
				return;
			delete m_buffer;
			this->m_size = new_size;
			if (new_size >= 0)
			{
				this->m_buffer = new T[m_size];
			}
		}

		T* detach()
		{
			T* temp = m_buffer;
			this->m_buffer = nullptr;
			this->m_size = 0;
			return temp;
		}
	};


private:
	std::thread* m_thread;
	std::mutex &m_fileAccess, 
		m_runmutex;

	Buffer<char> m_buffer;
	bool m_aborted;
	std::string m_filename;

public:
	ScreenscapThread(const char* a_filename, std::mutex& a_fileAccess);
	~ScreenscapThread();


	void RecreateBufferOfSize(int size);
	/* Give the current buffer away and forget about it */
	char* getFilecontent();
	bool wholeFileIntoBuffer();

public:
	void start();
	void do_work();
	static void run(ScreenscapThread* a_screenscapThread);
};

