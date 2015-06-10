#ifndef APOLLO_OWNED_MUTEX_H
#define APOLLO_OWNED_MUTEX_H

#include <thread>
#include <mutex>

/*
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#include <Windows.h>
typedef HANDLE mutex_t;
#else
#include <pthread>
typedef pthread_mutex_t mutex_t;
#endif
*/

//A mutex that keeps track of which thread locks it, so only that thread can unlock it.
class OwnedMutex
{
private:
	bool				locked = false;

	std::string			id = "";

	//mutex_t				mutexLock;
	std::mutex			mutexLock;
	std::thread::id		owner;

	//If the same threads locks multiple times, this number is incremented. Must be unlocked the same number of times.
	int lockLevel = 0;

public:
	OwnedMutex(std::string identifier = "");
	
	//Waits until mutex is unlocked, then locks it and returns.
	void lockWait();
	//Tries to lock mutex --> if it was unlocked lock it and return true, otherwise immediately return false
	bool tryLocking();

	//Unlocks mutex if the calling thread is the owner (may not actually unlock if it was locked multiple times)
	void unlock();
	//Waits for mutex is unlocked, then just returns.
	void waitForUnlocked();

	//If the calling thread is the owner, decrements lockLevel to 0 and unlocks mutex.
	void completelyUnlock();

	bool isLocked() const;
};


#endif	//APOLLO_OWNED_MUTEX_H