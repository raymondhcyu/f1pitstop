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
	int laps; // lap counter
	int lapTime;
	int pitStopOne; // select lap to pit stop
	int pitStopTwo;
	string status[2] = {"In Pit", "Racing"};

	int main(void) { // return winning car
		CMutex console("Console");
		CSemaphore entryLight("Entry Light", 0, 1);
		CSemaphore exitLight("Exit Light", 0, 1);
		CSemaphore pitEmpty("Empty", 0, 1);
		CSemaphore pitFull("Full", 0, 1);

		while (laps < 60) {
			Sleep(lapTime * 25);
			laps++;

			console.Wait();
			MOVE_CURSOR(0, carNum + 2);
			cout << carNum << "\t" << laps << "\t" << endl;
			console.Signal();

			// Debugging
			console.Wait();
			MOVE_CURSOR(0, 13);
			cout << entryLight.Read() << endl;
			console.Signal();

			// Check for pit condition
			if ((entryLight.Read() == 1) && ((laps == pitStopOne) || (laps == pitStopTwo))) {
				entryLight.Wait();
				pitFull.Signal();

				// Do pit stop stuff
				console.Wait();
				MOVE_CURSOR(0, carNum + 2);
				cout << "\t\t" << status[0] << endl;
				console.Signal();

				exitLight.Wait(); // wait for exit light
				pitEmpty.Signal(); // signal that pit empty

				//console.Wait();
				//MOVE_CURSOR(0, carNum + 2);
				//cout << "\t\t" << status[1] << endl;
				//console.Signal();

				laps++; // hotfix
			}
			else {
				console.Wait();
				MOVE_CURSOR(0, carNum + 2);
				cout << "\t\t" << status[1] << endl;
				console.Signal();
			}
		}

		return 0;
	};

public:
	car(int num, int pitOne, int pitTwo) { // default constructor, needs to be public to be access outside
		carNum = num;
		laps = 0;
		lapTime = rand() % 5 + 1; // range 1 to 5
		pitStopOne = pitOne;
		pitStopTwo = pitTwo;
	};
	int GetNum(void) {
		return carNum;
	};
};
#endif