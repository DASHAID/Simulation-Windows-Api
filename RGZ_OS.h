#include <windows.h>
#include <ctime>
extern int people0;         //количество ожидающих человек
class Kassa1
{
private:
	int time1;			    //время обслуживания
	HANDLE hUpdateEvt;		//состояние кассы (открыта/закрыта)
	HANDLE hThread;			//поток 
	HANDLE hMutex;			//мьютекс
	void new_time1();		//генерация времени обслуживания
	static DWORD CALLBACK ThreadFunc(LPVOID param);

public:
	Kassa1();
	~Kassa1();

	void start1();          //запуск кассы
	void stop1();           //закрытие кассы

	void set_time1();		//время -1
	int get_time1();		//возврат времени

	bool open1();           //статус занят/свободен
	bool status1();         //статус открыт/закрыт
};

class Kassa2
{
private:
	int time1;			    //время обслуживания
	HANDLE hUpdateEvt;		//состояние кассы (открыта/закрыта)
	HANDLE hThread;			//поток 
	HANDLE hMutex;			//мьютекс
	void new_time2();		//генерация времени обслуживания
	static DWORD CALLBACK ThreadFunc(LPVOID param);

public:
	Kassa2();
	~Kassa2();

	void start2();          //запуск кассы
	void stop2();           //закрытие кассы

	void set_time2();		//время -1
	int get_time2();		//возврат времени

	bool open2();           //статус занят/свободен
	bool status2();         //статус открыт/закрыт
};


class Kassa3
{
private:
	int time1;			    //время обслуживания
	HANDLE hUpdateEvt;		//состояние кассы (открыта/закрыта)
	HANDLE hThread;			//поток 
	HANDLE hMutex;			//мьютекс
	void new_time3();		//генерация времени обслуживания
	static DWORD CALLBACK ThreadFunc(LPVOID param);

public:
	Kassa3();
	~Kassa3();

	void start3();          //запуск кассы
	void stop3();           //закрытие кассы

	void set_time3();		//время -1
	int get_time3();		//возврат времени

	bool open3();           //статус занят/свободен
	bool status3();         //статус открыт/закрыт
};


class MutexLocker
{
	HANDLE hMutex;
public:
	MutexLocker(HANDLE mutex) : hMutex(mutex)
	{
		WaitForSingleObject(hMutex, INFINITE);
	}
	~MutexLocker()
	{
		ReleaseMutex(hMutex);
	}
};

