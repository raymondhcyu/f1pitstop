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

	// Accept pit stop
	entryLight.Signal();
	pitFull.Wait();
	// Do pit stop stuff
	cout << "Pit stop has a vehicle inside, supervisor" << endl;
	exitLight.Signal(); // wait for exit light
	pitEmpty.Wait(); // signal that pit empty

	return 0;
}

Supervisor::~Supervisor() {
}