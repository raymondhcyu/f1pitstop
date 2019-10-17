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
	int laps;
	int lapTime;
	string statusPit = "In pit";
	string statusRacing = "Racing";

	int main(void) { // return winning car
		CMutex console("Console");
		CSemaphore entryLight("Entry Light", 0, 1);
		CSemaphore exitLight("Exit Light", 0, 1);
		CSemaphore pitEmpty("Empty", 0, 1);
		CSemaphore pitFull("Full", 0, 1);

		while (laps < 60) {
			if (false)
				break;
			else {
				Sleep(lapTime * 25);
				laps++;
				console.Wait();
				MOVE_CURSOR(0, carNum + 2);
				cout << carNum << "\t" << laps << "\t" << statusRacing << "\t" << endl;
				console.Signal();
			}
		}

		//// Wait for pit stop, example car 4 going to pit
		//if (carNum == 4) {
		//	entryLight.Wait();
		//	pitFull.Signal();

		//	// Do pit stop stuff
		//	console.Wait();
		//	cout << "Car " << carNum << " is in pitstop!" << endl;
		//	console.Signal();

		//	exitLight.Wait(); // wait for exit light
		//	pitEmpty.Signal(); // signal that pit empty

		//	console.Wait();
		//	cout << "Car " << carNum << " has left the pitstop!" << endl;
		//	console.Signal();
		//}

		return 0;
	};

public:
	car(int num) { // default constructor, needs to be public to be access outside
		carNum = num;
		laps = 0;
		lapTime = rand() % 5 + 1; // range 1 to 5
	};
	int GetNum(void) {
		return carNum;
	};
};
#endif