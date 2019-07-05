#include <iostream>
#include <wiringPi.h>
#include <chrono>
#include <sys/time.h>
#include <limits>
#include <mutex>

uint64_t ms() {
	uint64_t m = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	return m;
}

void sotime(char *ct) {
	timeval curTime;
	gettimeofday(&curTime, NULL);
	int milli = curTime.tv_usec / 1000;
	char buffer[80];
	strftime(buffer, 80, "%H:%M:%S", localtime(&curTime.tv_sec));
	sprintf(ct, "%s:%d", buffer, milli);
}

const int pins[] = {4, 5};

std::mutex stdout_mutex;
int action(int pin) {
	const int state = digitalRead(pin);

	char buffer[80];
	sotime(buffer);

	stdout_mutex.lock();
	std::cout << "pin " << pin << ": " << state << " - " << buffer << std::endl;
	stdout_mutex.unlock();

}
void ISR_0() { action(pins[0]); }
void ISR_1() { action(pins[1]); }

int main() {
	wiringPiSetup() ;

	for(int i = 0;i < std::size(pins);i ++) {
		pinMode(pins[i], INPUT);
		pullUpDnControl(pins[i], PUD_UP);
	}
	wiringPiISR(pins[0], INT_EDGE_BOTH, &ISR_0);
	wiringPiISR(pins[1], INT_EDGE_BOTH, &ISR_1);

	// "stop" the main thread - we don't need it
	while(true) {
		delay(std::numeric_limits<unsigned int>::max());
	}
	return 0 ;
}
