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

	cout << "Race starting!" << endl;
	for (int i = 0; i < instances; i++)
		myCars[i]->Accelerate(100);

	for (int i = 0; i < instances; i++) {
		myCars[i]->Resume();
		Sleep(100);
	}
	
	// Create active objects for pit crew
	Supervisor S = Supervisor();
	Refueler R = Refueler();
	JackTech J = JackTech();
	NutTech N = NutTech();
	WheelRemoveTech WOFF = WheelRemoveTech();
	WheelReplaceTech WON = WheelReplaceTech();

	S.Resume();
	R.Resume();
	J.Resume();
	N.Resume();
	WOFF.Resume();
	WON.Resume();

	S.WaitForThread();
	R.WaitForThread();
	J.WaitForThread();
	N.WaitForThread();
	WOFF.WaitForThread();
	WON.WaitForThread();

	for (int i = 0; i < instances; i++)
		myCars[i]->WaitForThread();

	cout << "Race ended, winner winner chicken dinner!" << endl;
	return 0;
}