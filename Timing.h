#ifndef APOLLO_TIMING_H
#define APOLLO_TIMING_H

#include <inttypes.h>
#include <chrono>
#include <ratio>
#include <string>
#include <functional>

#include "Range.h"

typedef int64_t s_time;		//Sample time, in number of samples
typedef int32_t c_time;		//Chunk time, in number of chunks
typedef double PhysTime;	//Physical time, in seconds

//Range of time (seconds)
typedef Range<PhysTime> PhysTimeRange;
//Range of sample time
typedef Range<s_time> SampleRange;
//Range of chunk time
typedef Range<c_time> ChunkRange;

//class Time;

class Time
{
protected:
	PhysTime t = 0.0;

public:
	Time() { }
	Time(PhysTime time)
		: t(time)
	{ }
	Time(const Time &other)
		: t(other.t)
	{ }

	virtual bool isGlobal() const
	{ return false; }
	
	//explicit operator Time() const;

	operator PhysTime() const
	{ return t; }

	// +
	Time& operator+=(const Time &rhs)
	{ t += rhs.t;  return *this; }
	//Time operator+(const Time& rhs) const
	//{ return Time(*this) += rhs; }

	Time& operator+=(double rhs)
	{ t += rhs;  return *this; }
	//Time operator+(double rhs) const
	//{ return Time(*this) += rhs; }

	// -
	Time& operator-=(const Time &rhs)
	{ t -= rhs.t;  return *this; }
	//Time operator-(const Time& rhs) const
	//{ return Time(*this) -= rhs; }

	Time& operator-=(double rhs)
	{ t -= rhs;  return *this; }
	//Time operator-(double rhs) const
	//{ return Time(*this) -= rhs; }
	
	// *
	Time& operator*=(const Time &rhs)
	{ t *= rhs.t;  return *this; }
	//Time operator*(const Time& rhs) const
	//{ return Time(*this) *= rhs; }
	
	Time& operator*=(double rhs)
	{ t *= rhs;  return *this; }
	//Time operator*(double rhs) const
	//{ return Time(*this) *= rhs; }
	
	// /
	Time& operator/=(const Time &rhs)
	{ t /= rhs.t;  return *this; }
	//Time operator/(const Time& rhs) const
	//{ return Time(*this) /= rhs; }
	
	Time& operator/=(double rhs)
	{ t /= rhs;  return *this; }
	//Time operator/(double rhs) const
	//{ return Time(*this) /= rhs; }
};
/*
class Time : public Time
{
public:
	Time() { }
	Time(PhysTime time)
		: Time(time)
	{ }
	Time(const Time &other)
		: Time(other.t)
	{ }
	
	explicit operator Time() const;

	virtual bool isGlobal() const
	{ return true; }
	
	operator PhysTime() const
	{ return t; }
	
	// +
	Time& operator+=(const Time &rhs)
	{ t += rhs.t;  return *this; }
	//Time operator+(const Time& rhs) const
	//{ return Time(*this) += rhs; }

	Time& operator+=(double rhs)
	{ t += rhs;  return *this; }
	//Time operator+(double rhs) const
	//{ return Time(*this) += rhs; }

	// -
	Time& operator-=(const Time &rhs)
	{ t -= rhs.t;  return *this; }
	//Time operator-(const Time& rhs) const
	//{ return Time(*this) -= rhs; }

	Time& operator-=(double rhs)
	{ t -= rhs;  return *this; }
	//Time operator-(double rhs) const
	//{ return Time(*this) -= rhs; }
	
	// *
	Time& operator*=(const Time &rhs)
	{ t *= rhs.t;  return *this; }
	//Time operator*(const Time& rhs) const
	//{ return Time(*this) *= rhs; }
	
	Time& operator*=(double rhs)
	{ t *= rhs;  return *this; }
	//Time operator*(double rhs) const
	//{ return Time(*this) *= rhs; }
	
	// /
	Time& operator/=(const Time &rhs)
	{ t /= rhs.t;  return *this; }
	//Time operator/(const Time& rhs) const
	//{ return Time(*this) /= rhs; }
	
	Time& operator/=(double rhs)
	{ t /= rhs;  return *this; }
	//Time operator/(double rhs) const
	//{ return Time(*this) /= rhs; }
};
*/

typedef Range<Time> TimeRange;
//typedef Range<Time> TimeRange;

//typedef TimeClass<false> Time;
//typedef TimeClass<true> Time;

/*
//Not global by default
class Time
{
private:
	bool global = false;
	PhysTime t = 0.0;

public:
	Time();
	Time(PhysTime time);
	Time(PhysTime time, bool is_global);

	~Time() = default;

	bool isGlobal() const;
	
	Time& operator=(const Time &other);
	Time& operator=(const PhysTime &time);

	operator PhysTime() const;
};
*/


enum class TimeUnits
{
	INVALID = -1,

	NANOSECONDS = 0,
	MICROSECONDS,
	MILLISECONDS,
	SECONDS,
	MINUTES,
	HOURS,
	DAYS,
	YEARS,

	COUNT
};

inline unsigned int toIndex(TimeUnits tu)
{
	return static_cast<unsigned int>(tu);
}

//TODO: Finish PTime class in ScratchConsole

//TODO: Make Clock have different types for different std::chrono clocks (template? inheritance?)

//Object that keeps track of the time --> can handle fps-independent updating.
//	- C is the type of chrono clock to use
//template<typename C>
class Clock
{
private:
	//A list of ratios to convert seconds to each TimeUnit
	static const double ratios[static_cast<unsigned int>(TimeUnits::COUNT)];

public:

	Clock();
	//global_time denotes whether the clock's t should match up to the global time, or start from zero
	Clock(bool global_time);
	~Clock() = default;

	typedef std::chrono::high_resolution_clock ClockType;
	typedef ClockType::time_point TimePoint;
	typedef ClockType::duration TimeVec;
	//Units for doubles (t, dt, accumulator)
	//typedef std::chrono::milliseconds UnitDuration;


	static const double			secondsMult;			//Converts clock's base unit to seconds


	static TimePoint			GLOBAL_START_TIME;		//Global start time (now() at program start)
	static bool					GST_INITIALIZED;		//Whether the global start time is initialized yet

	//(All in seconds)
	Time						t,						//start time of current block
								last_t,					//start time of last block
								dt,						//delta time between this and last blocks
								accumulator,			//accumulates leftover time for physics steps
								globalOffset;			//time between timeStart and GLOBAL_START_TIME

	bool						initialized = false;	//Whether this clock's startTime has been initialized

	TimePoint					timeStart,				//Start point for this clock (t = 0 at this TimePoint)
								tp,
								last_tp;
	TimeVec						dtp,
								gStartVec;
	
	static void initGlobalTime();
	static Time getGlobalTime(TimeUnits units = TimeUnits::SECONDS);
	
	//global_time denotes whether the clock's t should match up to the global time, or start from zero
	void reset(bool global_time);
	
	TimeVec getExactTimeDuration() const;
	
	//Returns the exact time in the specified units.
	Time getExactTime(TimeUnits units = TimeUnits::SECONDS) const;

	void nextTimeFrame();

	//Waits until the specified amount of time (seconds) has passed since the last frame, then steps time
	void nextTimeFrameWait(Time length);

	//Waits until the specified time.
	void waitUntil(Time t);
};

/*
inline void initGlobalTime()
{
	GLOBAL_START_TIME = Clock::ClockType::now();
}
*/

/*
template<typename C>	const std::chrono::time_point<std::chrono::high_resolution_clock> Clock<C>::startTime = std::chrono::time_point_cast<std::chrono::duration<std::chrono::high_resolution_clock>>(GLOBAL_START_TIME);
template<typename C>	const std::chrono::time_point<std::chrono::high_resolution_clock> Clock<C>::baseTime = std::chrono::time_point<std::chrono::high_resolution_clock>::min();
template<typename C>	const double Clock<C>::secondsMult = (Clock<C>::UnitDuration::period::num / Clock<C>::UnitDuration::period::den);

template<typename C>
Clock<C>::Clock()
	:tp(baseTime + getExactTimeDuration()), last_tp(tp)
{ }

template<typename C>
std::chrono::high_resolution_clock::duration Clock<C>::getExactTimeDuration()
{
	return (std::chrono::high_resolution_clock::now() - startTime);
}

template<typename C>
template<typename U = Clock<C>::UnitDuration>
double Clock<C>::getExactTime()
{
	return (double)std::chrono::duration_cast<U>(getExactTimeDuration()).count();
}

template<typename C>
void Clock<C>::nextTimeFrame()
{
	last_tp = tp;
	tp = baseTime + getExactTimeDuration();
	//dtp = tp - last_tp;
	
	t = getExactTime<UnitDuration>()*secondsMult;

	//t = (double)std::chrono::time_point_cast<UnitDuration>(tp).count()*secondsMult;
	dt = (double)std::chrono::duration_cast<UnitDuration>(tp - last_tp).count()*secondsMult;
}
*/
//extern template class Clock<std::chrono::system_clock>;
//extern template class Clock<std::chrono::high_resolution_clock>;
//extern template class Clock<std::chrono::monotonic_clock>;
//extern template class Clock<std::chrono::steady_clock>;

//typedef Clock<std::chrono::high_resolution_clock> HRes_Clock;
//typedef Clock<std::chrono::system_clock> System_Clock;
//typedef Clock<std::chrono::monotonic_clock> Mono_Clock;
//typedef Clock<std::chrono::steady_clock> Steady_Clock;

typedef Clock HRes_Clock;

//extern HRes_Clock GLOBAL_CLOCK;

//inline void initGlobalClock()
//{
//	if(!GLOBAL_CLOCK.initialized)
//		GLOBAL_CLOCK.reset(true);
//}

//Declare global Times
//extern HRes_Clock PHYSICS_CLOCK;	//Updated for physics (per frame)
//extern HRes_Clock AUDIO_CLOCK;		//Updated for audio (per speaker chunk request)
									//TODO: This may not work well with multiple speaker nodes

//Measures the time between two events
struct TimeTest
{
	TimeUnits		units;

	std::string		unitSuffix = "unknown",
					label;

	Time			tStart = 0.0,
					tEnd = 0.0,
					dt = 0.0,

					max = -1.0,
					min = -1.0,

					t_accumulator = 0.0;

	unsigned int	count = 0;

	TimeTest(TimeUnits units_, const std::string &print_label = "");

	//Times the given function (with return type R and no arguments)
	//	- Timer will printResults() every print_every_n calculations
	//	- Time for the function to complete is returned in result
	template<typename R>
	R timeFunction(std::function<R(void)> func, int print_every_n, Time &result);

	void start(bool reset_avg = false);
	void end();
	void printResults();
};

template<typename R>
R TimeTest::timeFunction(std::function<R(void)> func, int print_every_n, Time &result)
{
	//Time function
	start();
	R ret = func();
	end();

	//Print if it's been print_every_n timings
	if(count % print_every_n == 0)
		printResults();

	result = dt;
	return ret;
}


class Timer
{
private:
	Time timeLeft = 0.0;

public:
	Timer();
	Timer(Time t_length);

	//Ticks timer down by dt, returns whether time is up.
	bool tick(Time dt);
	void reset(Time t_length);
};


//Represents the time signature of a piece.
struct TimeSignature
{
	unsigned int	upper = 4,
					lower = 4;

	TimeSignature()
	{ }
	TimeSignature(unsigned int upper_num, unsigned int lower_num)
		: upper(upper_num), lower(lower_num)
	{ }
};

#endif	//APOLO_TIMING_H