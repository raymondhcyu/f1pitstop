/*	Raymond Yu
	14 October 2019
	11298149
*/

#pragma once
#include "..\rt.h"
#include "technicians.h"

#ifndef __car__
#define __car__

// Active Class objects are like a mini program with a main() and multiple threads
class car : public ActiveClass
{
private:
	int carNum;
	int speed; // vehicle speed
	int main(void) {
		CMutex console("Console");
		CSemaphore entryLight("Entry Light", 0, 1);
		CSemaphore exitLight("Exit Light", 0, 1);
		CSemaphore pitEmpty("Empty", 0, 1);
		CSemaphore pitFull("Full", 0, 1);

		cout << "Zoom zoom " << carNum << endl;

		// Wait for pit stop, example car 4 going to pit
		if (carNum == 4) {
			entryLight.Wait();
			pitFull.Signal();

			// Do pit stop stuff
			console.Wait();
			cout << "Car " << carNum << " is in pitstop!" << endl;
			console.Signal();

			exitLight.Wait(); // wait for exit light
			pitEmpty.Signal(); // signal that pit empty

			console.Wait();
			cout << "Car " << carNum << " has left the pitstop!" << endl;
			console.Signal();
		}

		return 0;
	}; // active class main function

public:
	car(int num) {
		carNum = num;
		speed = 0;
	}; // default constructor, needs to be public to be access outside
	void Accelerate(int accel) {
		speed = speed + accel;
	};
	void Stop(void) {
		speed = 0;
	};
	int GetSpeed(void) {
		return speed;
	};
	int GetNum(void) {
		return carNum;
	};
	void PrintData(void) {
		cout << "I'm car number " << carNum << " and my speed is " << speed << endl;
	};
};
#endif