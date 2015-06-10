#ifndef APOLLO_RANGE_H
#define APOLLO_RANGE_H

/*
template<typename T>
class Range;

typedef Range<Time> TimeRange;
typedef Range<AudioSample> SampleRange;
typedef Range<AudioChunk> ChunkRange;
*/

template<typename T>
struct Range
{
	T	start = (T)0,
		end = (T)0;

	Range();
	Range(T start_time, T end_time);

	bool operator==(const Range<T> &other) const;

	T length() const;
	//Whether this range overlaps the given range
	bool intersects(const Range<T> &r) const;
	//Whether this range contains the given point
	bool contains(const T &p) const;
	//Whether this range +- tol contains the given point.
	bool contains(const T &p, const T &tol) const;

};

template<typename T>
Range<T>::Range()
{ }

template<typename T>
Range<T>::Range(T start_time, T end_time)
	: start(start_time), end(end_time)
{ }

template<typename T>
bool Range<T>::operator==(const Range<T> &other) const
{
	return (start == other.start && end == other.end);
}

template<typename T>
T Range<T>::length() const
{
	return end - start;
}

template<typename T>
bool Range<T>::intersects(const Range<T> &r) const
{
	return (r.start < end && r.end > start);
}

template<typename T>
bool Range<T>::contains(const T &p) const
{
	return (p >= start && p < end);
}

template<typename T>
bool Range<T>::contains(const T &p, const T &tol) const
{
	return (p >= start - tol && p < end + tol);
}

#endif	//APOLLO_RANGE_H