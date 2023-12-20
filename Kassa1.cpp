#include "RGZ_OS.h"

Kassa1::Kassa1()
{
	extern int people0;
	time1 = 0;
	hMutex = CreateMutex(NULL, FALSE, NULL);
	hThread = INVALID_HANDLE_VALUE;                   //касса изначально "закрыта"
	hUpdateEvt = CreateEvent(NULL, TRUE, TRUE, NULL); //изначально событие "свободно"
};

Kassa1::~Kassa1()
{
	stop1();
	CloseHandle(hUpdateEvt);
	CloseHandle(hMutex);
};

void Kassa1::start1()
{
	hThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ThreadFunc, this, 0, 0);
};

void Kassa1::stop1()
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

int Kassa1::get_time1()
{
	MutexLocker guard(hMutex);
	return time1;
};

void Kassa1::set_time1()
{
	MutexLocker guard(hMutex);
	time1--;
};

void Kassa1::new_time1()
{
	MutexLocker guard(hMutex);
	srand(time(NULL));
	time1 += 3 + rand() % 20;
};

bool Kassa1::open1()
{
	return (WaitForSingleObject(hUpdateEvt, 0) == WAIT_OBJECT_0) ? true : false;
};

bool Kassa1::status1()
{
	return (hThread != INVALID_HANDLE_VALUE) ? true : false;
};

DWORD CALLBACK Kassa1::ThreadFunc(LPVOID param)
{
	Kassa1* _this = (Kassa1*)param;
	if (!_this) return 1; //нет объекта, не работаем
	while (1)
	{
		if (people0 > 0) {
			if (_this->open1()) {
				ResetEvent(_this->hUpdateEvt); //переключаем на "занято"
				_this->new_time1();
				people0--;
			}
		}
		if (!_this->open1()) {
			_this->set_time1();
			if (_this->get_time1() <= 0) {
				SetEvent(_this->hUpdateEvt);   //переключаем на "свободно"
			}
		}
		WaitForSingleObject(_this->hThread, 1000);
	}
	return 0;
};