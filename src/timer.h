#ifndef _timer_h
#define _timer_h


#include "includes.h"

////////////////////////////////////////////////////////////////////////////////////////

class Timer {
	
public:
	Timer(int duration = 60.0) {
		_duration  = duration;
		_startTime = time(NULL);
		_delay     = 100.0;
	}
	
	void duration(int value) {
		_duration = value;
	}
	
	int duration() {
		return _duration;
	}
	
	void delay(double value) {
		_delay = value;
	}

	double delay() {
		return _delay;
	}
	
	int expired() {
		if (_duration > 0) {
			if ((time(NULL) - _startTime) > _duration) {
				return true;
			}
		}
		
		return false;
	}
	
	inline void sleep() {
		if (_delay > 0)
			usleep(int(1000.0 * _delay));
	}

private:
	time_t _startTime;
	int _duration;
	double _delay;
};


////////////////////////////////////////////////////////////////////////////////////////


#endif
