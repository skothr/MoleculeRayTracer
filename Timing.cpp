#include "Timing.h"

#include <iostream>
#include <limits>


/////TIME/////
/*
Time::Time()
{ }

Time::Time(PhysTime time)
	: t(time)
{ }
Time::Time(PhysTime time, bool is_global)
	: t(time), global(is_global)
{ }

bool Time::isGlobal() const
{
	return global;
}
	
Time& Time::operator=(const Time &other)
{
	t = other.t;
	global = other.global;

	return *this;
}

Time& Time::operator=(const PhysTime &time)
{
	t = time;
	global = false;
}

Time::operator PhysTime() const
{
	return t;
}
*/

/*
Time::operator Time() const
{ return Time(t); }

Time::operator Time() const
{ return Time(t); }
*/

/////CLOCK/////
const double Clock::ratios[static_cast<unsigned int>(TimeUnits::COUNT)]
{
	1000000000.0,				//NANOSECONDS
	1000000.0,					//MICROSECONDS
	1000.0,						//MILLISECONDS
	1.0,						//SECONDS
	1.0/60.0,					//MINUTES
	1.0/(60.0*60.0),			//HOURS
	1.0/(60.0*60.0*24.0),		//DAYS
	1.0/(60.0*60.0*24.0*365.0),	//YEARS
};

const double		Clock::secondsMult = ((double)Clock::TimeVec::period::num / (double)Clock::TimeVec::period::den);

Clock::TimePoint	Clock::GLOBAL_START_TIME;
bool				Clock::GST_INITIALIZED = false;

Clock::Clock()
{
	if(!GST_INITIALIZED)
		initGlobalTime();
}

Clock::Clock(bool global_time)
{
	if(!GST_INITIALIZED)
		initGlobalTime();

	reset(global_time);
}

void Clock::initGlobalTime()
{
	GLOBAL_START_TIME = Clock::ClockType::now();
	GST_INITIALIZED = true;
}

Time Clock::getGlobalTime(TimeUnits units)
{
	return (Time)(ClockType::now() - GLOBAL_START_TIME).count()*secondsMult*ratios[toIndex(units)] * (Time)GST_INITIALIZED;
}

void Clock::reset(bool global_time)
{
	timeStart = (global_time ?  GLOBAL_START_TIME : ClockType::now());
	globalOffset = (timeStart - GLOBAL_START_TIME).count()*secondsMult;
	
	initialized = true;

	gStartVec = getExactTimeDuration();

	tp = last_tp = TimePoint(gStartVec);
	t = last_t = gStartVec.count()*secondsMult;

	dtp = TimeVec(0);
	dt = 0.0;

	accumulator = 0.0;
}

Clock::TimeVec Clock::getExactTimeDuration() const
{
	return (ClockType::now() - timeStart) * initialized;
	//return (initialized ? (ClockType::now() - timeStart) : TimeVec(0));
}

void Clock::nextTimeFrame()
{
	if(initialized)
	{
		last_tp = tp;
		last_t = t;

		gStartVec = getExactTimeDuration();

		tp = TimePoint(gStartVec);
		dtp = tp - last_tp;
	
		t = (Time)gStartVec.count()*secondsMult;
		dt = (Time)dtp.count()*secondsMult;
	}
}

void Clock::nextTimeFrameWait(Time length)
{
	while(initialized && getExactTime() - t < length)
		;
	nextTimeFrame();
}

void Clock::waitUntil(Time t)
{
	while(initialized && getExactTime() < t)
		;
}

Time Clock::getExactTime(TimeUnits units) const
{
	return (Time)getExactTimeDuration().count()*secondsMult*ratios[toIndex(units)];
}

//Instantiate with different clock types
//TODO: Make sure other files dont implicitly generate (commented out externs in header)
//template class Clock<std::chrono::system_clock>;
//template class Clock<std::chrono::high_resolution_clock>;
//template class Clock<std::chrono::monotonic_clock>;
//template class Clock<std::chrono::steady_clock>;

//HRes_Clock PHYSICS_CLOCK;
//HRes_Clock AUDIO_CLOCK;
//HRes_Clock GLOBAL_CLOCK;



/////TIME TEST/////

TimeTest::TimeTest(TimeUnits units_, const std::string &print_label)
	: units(units_), label(print_label)
{
	switch(units)
	{
	case TimeUnits::NANOSECONDS:
		unitSuffix = "ns";
		break;
	case TimeUnits::MICROSECONDS:
		unitSuffix = "us";
		break;
	case TimeUnits::MILLISECONDS:
		unitSuffix = "ms";
		break;
	case TimeUnits::SECONDS:
		unitSuffix = "s";
		break;
	case TimeUnits::MINUTES:
		unitSuffix = "min";
		break;
	case TimeUnits::HOURS:
		unitSuffix = "hr";
		break;
	}
}

void TimeTest::start(bool reset_avg)
{
	//START TIMING TEST
	tStart = (Time)Clock::getGlobalTime(units);

	t_accumulator = reset_avg ? 0.0 : t_accumulator;
	max = reset_avg ? -1.0 : max;
	min = reset_avg ? -1.0 : min;
	count = reset_avg ? 0 : count;
}

void TimeTest::end()
{
	//END TIMING TEST
	tEnd = (Time)Clock::getGlobalTime(units);
	dt = tEnd - tStart;

	max = dt > max ? dt : max;
	min = (dt != 0.0 && (dt < min || min < 0.0)) ? dt : min;

	t_accumulator += dt;
	count++;
}

void TimeTest::printResults()
{
	std::cout.precision(15);

	std::cout << label << (label == "" ? "" : " -->\n");
	std::cout << "-------------------------------------\n";
	std::cout << "\nTIMING RESULTS\t-->\t" << std::fixed << dt << unitSuffix << "\n";
	std::cout << "AVERAGE\t\t-->\t" << std::fixed << (t_accumulator/(Time)count) << unitSuffix << "\n";
	std::cout << "MAX\t\t-->\t" << std::fixed << max << unitSuffix << "\n";
	std::cout << "MIN\t\t-->\t" << std::fixed << min << unitSuffix << "\n";
	std::cout << "-------------------------------------\n\n";

	//Reset max/min
	max = 0.0;
	min = -1.0;

}



/////TIMER/////

Timer::Timer()
{ }

Timer::Timer(Time t_length)
	: timeLeft(t_length)
{ }

bool Timer::tick(Time dt)
{
	timeLeft -= dt;
	return (timeLeft <= 0.0);
}

void Timer::reset(Time t_length)
{
	timeLeft = t_length;
}

