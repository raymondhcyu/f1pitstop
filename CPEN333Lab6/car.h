/*	Raymond Yu
	14 October 2019
	11298149
*/

#pragma once
#include "..\rt.h"

#ifndef __car__
#define __car__

// Active Class objects are like a mini program with a main() and multiple threads
class car : public ActiveClass
{
private:
	int carNum;
	int speed; // vehicle speed
	int main(void) {
		cout << "Car number " << carNum << " has speed " << speed << endl;
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