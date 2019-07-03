#include <iostream>
#include <wiringPi.h>
#include <chrono>
#include <sys/time.h>

uint64_t ms() {
	uint64_t m = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	return m;
}

void sotime() {
	timeval curTime;
	gettimeofday(&curTime, NULL);
	int milli = curTime.tv_usec / 1000;

	char buffer [80];
	strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", localtime(&curTime.tv_sec));

	char currentTime[84] = "";
	sprintf(currentTime, "%s:%d", buffer, milli);
	printf("current time: %s \n", currentTime);
}


int main()
{
	wiringPiSetup() ;
	pinMode(7, INPUT) ;
	pullUpDnControl(7, PUD_UP);

	for (;;)
	{
		bool pressed = !digitalRead(7);
		if(!pressed) {
			sotime();
			return 0;
		}
		//delay(5);
	}
	return 0 ;
}
