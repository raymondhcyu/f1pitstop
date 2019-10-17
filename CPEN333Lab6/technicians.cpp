#include "technicians.h"

Supervisor::Supervisor(void) {
}

int Supervisor::main(void) {
	cout << "Accepting a pit stop vehicle, supervisor" << endl;

	CSemaphore entryLight("Entry Light", 0, 1);
	CSemaphore exitLight("Exit Light", 0, 1);
	CSemaphore pitEmpty("Empty", 0, 1);
	CSemaphore pitFull("Full", 0, 1);

	CSemaphore refuelStop("Refuel Stop", 0, 1);
	CSemaphore refuelStart("Refuel Start", 0, 1);

	CSemaphore frontJackDown("Front Jack Down", 0, 1);
	CSemaphore rearJackDown("Rear Jack Down", 0, 1);

	ClassThread <Supervisor> RefuelerThread(this, &Supervisor::Refueler, ACTIVE, NULL);
	ClassThread <Supervisor> FrontJackThread(this, &Supervisor::FrontJack, ACTIVE, NULL);
	ClassThread <Supervisor> RearJackThread(this, &Supervisor::RearJack, ACTIVE, NULL);

	// Enter and accept vehicle to pit
	entryLight.Signal();
	pitFull.Wait();
	cout << "Pit stop has a vehicle inside, supervisor" << endl;

	// Do pit stop stuff
	RefuelerThread.WaitForThread();
	//FrontJackThread.WaitForThread();
	//RearJackThread.WaitForThread();

	exitLight.Signal(); // wait for exit light
	pitEmpty.Wait(); // signal that pit empty

	return 0;
}

int Supervisor::Refueler(void* args) {
	CSemaphore refuelStop("Refuel Stop", 0, 1);
	CSemaphore refuelStart("Refuel Start", 0, 1);

	refuelStart.Signal();
	cout << "Instruction to start refuelling, supervisor" << endl;
	refuelStop.Wait(); // wait until refueler ready

	return 0;
}

int Supervisor::FrontJack(void* args) {
	CSemaphore frontJackDown("Front Jack Down", 0, 1);

	frontJackDown.Wait(); // wait until jack tech ready
	cout << "Waiting to front jack up, supervisor" << endl;

	return 0;
}

int Supervisor::RearJack(void* args) {
	CSemaphore rearJackDown("Rear Jack Down", 0, 1);

	rearJackDown.Wait(); // wait until jack tech ready
	cout << "Waiting to rear jack up, supervisor" << endl;

	return 0;
}

Supervisor::~Supervisor() {}

Refueler::Refueler(void) {
}

int Refueler::main(void) {
	CSemaphore refuelStop("Refuel Stop", 0, 1);
	CSemaphore refuelStart("Refuel Start", 0, 1);

	refuelStart.Wait(); // wait for refuel start signal from supervisor
	cout << "Vehicle being refueled, refueller" << endl;
	Sleep(1000);
	cout << "Vehicle done refuelling, refueller" << endl;
	refuelStop.Signal(); // wait for refuel stop signal from supervisor (or should it send complete status?)
	
	return 0;
}

Refueler::~Refueler() {}

JackTech::JackTech(void) {
}

int JackTech::main(void) {
	CSemaphore jackUp("Jack Up", 0, 1);
	CSemaphore jackDown("Jack Down", 1, 1);

	jackUp.Wait(); // wait for jack up signal from supervisor
	cout << "Vehicle being jacked up, JackTechFront" << endl;
	Sleep(500);
	cout << "Vehicle done jacking up, JackTechFront" << endl;

	return 0;
}

JackTech::~JackTech() {}

int NutTechFrontLeft::main(void) {
	CSemaphore nutOn("Nut On", 0, 2);
	CSemaphore nutOff("Nut Off", 2, 2);

	return 0;
}

NutTechFrontLeft::~NutTechFrontLeft() {}

int Wheeler::main(void) {

	return 0;
}

Wheeler::~Wheeler() {
}