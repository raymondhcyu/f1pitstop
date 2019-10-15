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
	CSemaphore refuelStop("Refuel End", 0, 1);

	//CSemaphore ready("Pit Crew Ready", 0, 15); // not counting supervisor

	// Accept pit stop
	// Wait for all technicians to be ready
	//for (int i = 0; i < 16; i++)
	//	ready.Wait();

	// Enter and accept vehicle to pit
	entryLight.Signal();
	pitFull.Wait();
	cout << "Pit stop has a vehicle inside, supervisor" << endl;

		// Do pit stop stuff
		refuelStart.Signal();
		refuelStop.Signal();

	exitLight.Signal(); // wait for exit light
	pitEmpty.Wait(); // signal that pit empty

	return 0;
}

Supervisor::~Supervisor() {
}

Refueler::Refueler(int num) {
	data = num;
}

int Refueler::main(void) {
	CSemaphore refuelStart("Refuel Start", 0, 1);
	CSemaphore refuelStop("Refuel End", 0, 1);
	//CSemaphore ready("Pit Crew Ready", 0, 15); // not counting supervisor
	//
	//ready.Signal(); // Refueler ready
	refuelStart.Wait(); // wait for refuel start signal from supervisor
	cout << "\nVehicle being refueled, refueller\n" << endl;
	Sleep(1000);
	cout << "\nVehicle done refuelling, refueller\n" << endl;
	refuelStop.Wait(); // wait for refuel stop signal from supervisor (or should it send complete status?)
	return 0;
}

Refueler::~Refueler() {
}