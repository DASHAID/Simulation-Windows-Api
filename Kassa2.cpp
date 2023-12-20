#include "RGZ_OS.h"

Kassa2::Kassa2()
{
	extern int people0;
	time1 = 0;
	hMutex = CreateMutex(NULL, FALSE, NULL);
	hThread = INVALID_HANDLE_VALUE;
	hUpdateEvt = CreateEvent(NULL, TRUE, TRUE, NULL); //изначально событие "свободно"
};

Kassa2::~Kassa2()
{
	stop2();
	CloseHandle(hUpdateEvt);
	CloseHandle(hMutex);
};

void Kassa2::start2()
{
	hThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ThreadFunc, this, 0, 0);
};

void Kassa2::stop2()
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

int Kassa2::get_time2()
{
	MutexLocker guard(hMutex);
	return time1;
};

void Kassa2::set_time2()
{
	MutexLocker guard(hMutex);
	time1--;
};

void Kassa2::new_time2()
{
	MutexLocker guard(hMutex);
	srand(time(NULL));
	time1 += 3 + rand() % 20;
};

bool Kassa2::open2()
{
	return (WaitForSingleObject(hUpdateEvt, 0) == WAIT_OBJECT_0) ? true : false;
};

bool Kassa2::status2()
{
	return (hThread != INVALID_HANDLE_VALUE) ? true : false;
};

DWORD CALLBACK Kassa2::ThreadFunc(LPVOID param)
{
	Kassa2* _this = (Kassa2*)param;
	if (!_this) return 1; //нет объекта, не работаем
	while (1)
	{
		if (people0 != 0) {
			if (_this->open2()) {
				ResetEvent(_this->hUpdateEvt); //переключаем на "занято"
				_this->new_time2();
				people0--;
			}
		}
		if (!_this->open2()) {
			_this->set_time2();
			if (_this->get_time2() == 0) {
				SetEvent(_this->hUpdateEvt);   //переключаем на "свободно"
			}
		}
		WaitForSingleObject(_this->hThread, 1000);
	}
	return 0;
};