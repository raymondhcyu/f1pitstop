#include "technicians.h"

Supervisor::Supervisor(int num) {
	data = num;
}

int Supervisor::main(void) {
	cout << "Accepting a pit stop vehicle, supervisor" << endl;

	CSemaphore entryLight("Entry Light", 0, 1);
	CSemaphore exitLight("Exit Light", 0, 1);
	CSemaphore pitEmpty("Empty", 0, 1);
	CSemaphore pitFull("Full", 0, 1);

	CSemaphore refuelStart("Refuel Start", 0, 1);
	CSemaphore refuelStop("Refuel Stop", 1, 1);

	ClassThread <Supervisor> RefuelerThread(this, &Supervisor::Refueler, ACTIVE, NULL);

	/* NEXT STEPS:
	- Add ClassThread for Jacker and Wheel Nut, Remove, and Fit
	- Add ClassThread for entry/exit pit full/empty for concurrency
	- Somehow add ready contition for all pit crewrefuel.stop(wait)?

	(Replicate ReFuelerThread technique, start = 0 stop = 1 and trigger if else)
	*/

	// Enter and accept vehicle to pit
	entryLight.Signal();
	pitFull.Wait();
	cout << "Pit stop has a vehicle inside, supervisor" << endl;

	// Do pit stop stuff
	RefuelerThread.WaitForThread();

	exitLight.Signal(); // wait for exit light
	pitEmpty.Wait(); // signal that pit empty

	return 0;
}

int Supervisor::Refueler(void* args) {
	CSemaphore refuelStart("Refuel Start", 0, 1);
	CSemaphore refuelStop("Refuel Stop", 1, 1);

	refuelStop.Wait(); // wait until refueler ready
	cout << "Waiting for refueler, supervisor" << endl;
	refuelStart.Signal(); // signal to fuel tech to refuel
	
	return 0;
}

int Supervisor::RearJack(void* args) {

	return 0;
}

int Supervisor::FrontJack(void* args) {

	return 0;
}

Supervisor::~Supervisor() {
}

Refueler::Refueler(int num) {
	data = num;
}

int Refueler::main(void) {
	CSemaphore refuelStart("Refuel Start", 0, 1);
	CSemaphore refuelStop("Refuel Stop", 1, 1);

	refuelStart.Wait(); // wait for refuel start signal from supervisor
	cout << "Vehicle being refueled, refueller" << endl;
	Sleep(1000);
	cout << "Vehicle done refuelling, refueller" << endl;
	refuelStop.Signal(); // wait for refuel stop signal from supervisor (or should it send complete status?)
	
	return 0;
}

Refueler::~Refueler() {
}

int Jacker::main(void) {

	return 0;
}

Jacker::~Jacker() {
}

int Wheeler::main(void) {

	return 0;
}

Wheeler::~Wheeler() {
}