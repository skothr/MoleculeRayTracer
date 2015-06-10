#include "OwnedMutex.h"

#include <iostream>
#include <string>

/////OWNED MUTEX/////

OwnedMutex::OwnedMutex(std::string identifier)
	: id(identifier)
{
	/*
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
	mutexLock = CreateMutex(NULL, FALSE, NULL);
	if(!mutexLock)
		std::cout << "ERROR creating mutex (WINDOWS)!\n";
#endif
		*/
}

void OwnedMutex::lockWait()
{
	std::thread::id t_id = std::this_thread::get_id();

	if(t_id == owner)
	{
		//Calling thread owns the lock --> increment lockLevel.
		lockLevel++;
		//std::cout << id << " --> " << lockLevel << "\n";
	}
	else
	{
		mutexLock.lock();

		//Update variables
		lockLevel = 1;
		locked = true;
		owner = t_id;
	}
}


bool OwnedMutex::tryLocking()
{
	std::thread::id t_id = std::this_thread::get_id();
	
	bool lock_succeeded = false;

	if(t_id == owner)
	{
		lockLevel++;
		lock_succeeded = true;
	}
	else if(lock_succeeded = !locked)
	{
		mutexLock.lock();
		
		//Update variables
		lockLevel = 1;
		locked = true;
		owner = t_id;
	}

	return lock_succeeded;
}


void OwnedMutex::unlock()
{
	std::thread::id t_id = std::this_thread::get_id();
	
	if(t_id == owner)// && (--lockLevel <= 0))
	{
		lockLevel--;
		//std::cout << id << " --> " << lockLevel << "\n";
		
		//Only unlock if lockLevel is back down to 0
		if(lockLevel <= 0)
		{
			lockLevel = 0;
			locked = false;
			owner = std::thread::id();
		
			mutexLock.unlock();
		}
	}
	//else the current thread is not the owner; cant unlock
}

void OwnedMutex::waitForUnlocked()
{
	std::thread::id t_id = std::this_thread::get_id();

	if(t_id != owner)
	{
		//std::cout << "WAITING FOR UNLOCK\n";
		//Wait for unlocked node, and lock
		mutexLock.lock();
		//Unlock node
		mutexLock.unlock();
	}
	else
	{
		unlock();
		//completelyUnlock();
	}
	//else already unlocked, or this thread is already the owner
}

void OwnedMutex::completelyUnlock()
{
	std::thread::id t_id = std::this_thread::get_id();

	if(locked && t_id == owner)
	{
		lockLevel = 0;
		locked = false;
		owner = std::thread::id();

		mutexLock.unlock();
	}
}


bool OwnedMutex::isLocked() const
{
	return locked;
}