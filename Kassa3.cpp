#include "RGZ_OS.h"

Kassa3::Kassa3()
{
	extern int people0;
	time1 = 0;
	hMutex = CreateMutex(NULL, FALSE, NULL);
	hThread = INVALID_HANDLE_VALUE;
	hUpdateEvt = CreateEvent(NULL, TRUE, TRUE, NULL); //изначально событие "свободно"
};

Kassa3::~Kassa3()
{
	stop3();
	CloseHandle(hUpdateEvt);
	CloseHandle(hMutex);
};

void Kassa3::start3()
{
	hThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ThreadFunc, this, 0, 0);
};

void Kassa3::stop3()
{
	if (hThread == INVALID_HANDLE_VALUE) return; //не запущен
	if (WaitForSingleObject(hThread, 500) != WAIT_OBJECT_0)
	{
		//прождали пять минут, не остановился, "приболел" видимо, пристрелим, чтобы не мучался
		TerminateThread(hThread, 1);
	}
	CloseHandle(hThread);
	hThread = INVALID_HANDLE_VALUE;
};

int Kassa3::get_time3()
{
	MutexLocker guard(hMutex);
	return time1;
};

void Kassa3::set_time3()
{
	MutexLocker guard(hMutex);
	time1--;
};

void Kassa3::new_time3()
{
	MutexLocker guard(hMutex);
	srand(time(NULL));
	time1 += 3 + rand() % 20;
};

bool Kassa3::open3()
{
	return (WaitForSingleObject(hUpdateEvt, 0) == WAIT_OBJECT_0) ? true : false;
};

bool Kassa3::status3()
{
	return (hThread != INVALID_HANDLE_VALUE) ? true : false;
};

DWORD CALLBACK Kassa3::ThreadFunc(LPVOID param)
{
	Kassa3* _this = (Kassa3*)param;
	if (!_this) return 1; //нет объекта, не работаем
	while (1)
	{
		if (people0 != 0) {
			if (_this->open3()) {
				ResetEvent(_this->hUpdateEvt); //переключаем на "занято"
				_this->new_time3();
				people0--;
			}
		}
		if (!_this->open3()) {
			_this->set_time3();
			if (_this->get_time3() == 0) {
				SetEvent(_this->hUpdateEvt);   //переключаем на "свободно"
			}
		}
		WaitForSingleObject(_this->hThread, 1000);
	}
	return 0;
};