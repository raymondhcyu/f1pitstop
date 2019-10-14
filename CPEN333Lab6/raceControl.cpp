/*	Raymond Yu
	14 October 2019
	11298149
*/

#include "..\rt.h"
#include "car.h"
#include "technicians.h"

int main(void) {
	const int instances = 10;
	car* myCars[instances]; // create cars as active class objects

	for (int i = 0; i < instances; i++)
		myCars[i] = new car(i);

	// Start race
	cout << "Race starting!" << endl;
	for (int i = 0; i < instances; i++)
		myCars[i]->Accelerate(100);

	// Resume main function
	for (int i = 0; i < instances; i++) {
		myCars[i]->Resume();
		Sleep(100);
	}
	
	// Create supervisor active thread
	Supervisor S1(99);
	S1.Resume();
	S1.WaitForThread();
	//// Call pit stop (should actually be supervisor)
	//entryLight.Signal();
	//pitFull.Wait();
	//// Do pit stop stuff
	//cout << "Pit stop has a vehicle inside!" << endl;
	//exitLight.Signal(); // wait for exit light
	//pitEmpty.Wait(); // signal that pit empty

	for (int i = 0; i < instances; i++)
		myCars[i]->WaitForThread();

	cout << "Race ended..." << endl;
	return 0;
}