#include "technicians.h"

Supervisor::Supervisor(void) {
}

int Supervisor::main(void) {
	cout << "Accepting a pit stop vehicle, supervisor" << endl;
	CMutex console("Console");

	CSemaphore entryLight("Entry Light", 0, 1);
	CSemaphore exitLight("Exit Light", 0, 1);
	CSemaphore pitEmpty("Empty", 0, 1);
	CSemaphore pitFull("Full", 0, 1);

	CSemaphore refuelStop("Refuel Stop", 0, 1);
	CSemaphore refuelStart("Refuel Start", 0, 1);

	CSemaphore frontJackUp("Front Jack Up", 0, 1); // jack up front of vehicle for servicing
	CSemaphore frontJackDone("Front Jack Done", 0, 1);
	CSemaphore frontJackDown("Front Jack Down", 0, 1); // jack down front of vehicle to race

	CSemaphore frontNutRemovalDone("Front Nuts Removal Done", 0, 2); // one for each left and right nut
	CSemaphore frontNutInstallDone("Front Nuts Install Done", 0, 2);

	CSemaphore frontWheelRemovalDone("Front Wheels Removal Done", 0, 2); // one for each left and right wheel
	CSemaphore frontWheelReplaceDone("Front Wheels Replace Done", 0, 2);

	ClassThread <Supervisor> RefuelerThread(this, &Supervisor::Refueler, ACTIVE, NULL);
	ClassThread <Supervisor> FrontJackThread(this, &Supervisor::FrontJack, ACTIVE, NULL);
	ClassThread <Supervisor> RearJackThread(this, &Supervisor::RearJack, ACTIVE, NULL);

	// Enter and accept vehicle to pit
	entryLight.Signal();
	pitFull.Wait();

	console.Wait(); // Protect console (shared resource)
	cout << "Pit stop has a vehicle inside, supervisor" << endl;
	console.Signal();

	// Do pit stop stuff
	RefuelerThread.WaitForThread();
	FrontJackThread.WaitForThread();
	//RearJackThread.WaitForThread();

	console.Wait();
	cout << "Pit stop vehicle serviced and releasing, supervisor" << endl;
	console.Signal();

	exitLight.Signal(); // wait for exit light
	pitEmpty.Wait(); // signal that pit empty

	return 0;
}

int Supervisor::Refueler(void* args) {
	CMutex console("Console");
	CSemaphore refuelStop("Refuel Stop", 0, 1);
	CSemaphore refuelStart("Refuel Start", 0, 1);

	refuelStart.Signal();

	console.Wait();
	cout << "Instruction to start refuelling, supervisor" << endl;
	console.Signal();

	refuelStop.Wait();

	return 0;
}

int Supervisor::FrontJack(void* args) {
	CMutex console("Console");
	CSemaphore frontJackUp("Front Jack Up", 0, 1);
	CSemaphore frontJackDown("Front Jack Down", 0, 1);

	frontJackUp.Signal(); // start front jack up

	console.Wait();
	cout << "Instruction to jack front up, supervisor" << endl;
	console.Signal();

	frontJackDown.Wait(); // wait for jack down

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
	CMutex console("Console");
	CSemaphore refuelStop("Refuel Stop", 0, 1);
	CSemaphore refuelStart("Refuel Start", 0, 1);

	refuelStart.Wait(); // wait for refuel start signal from supervisor

	console.Wait();
	cout << "Vehicle being refueled, refueller" << endl;
	console.Signal();

	Sleep(1000);
	refuelStop.Signal(); // wait for refuel stop signal from supervisor (or should it send complete status?)
	
	console.Wait();
	cout << "Vehicle done refuelling, refueller" << endl;
	console.Signal();
	
	return 0;
}

Refueler::~Refueler() {}

JackTech::JackTech(void) {}

int JackTech::main(void) {
	CMutex console("Console");
	CSemaphore frontJackUp("Front Jack Up", 0, 1);
	CSemaphore frontJackDone("Front Jack Done", 0, 1);
	CSemaphore frontNutInstallDone("Front Nuts Install Done", 0, 2);
	CSemaphore frontJackDown("Front Jack Down", 0, 1);

	frontJackUp.Wait(); // wait for jack up signal from supervisor

	console.Wait();
	cout << "Vehicle front being jacked up, JackTech" << endl;
	console.Signal();

	Sleep(750);
	frontJackDone.Signal(); // front jack done jacking up, signal to NutTech

	console.Wait();
	cout << "Vehicle front done jacking up, JackTech" << endl;
	console.Signal();

	frontNutInstallDone.Wait(); // wait twice for nuts to reinstall
	frontNutInstallDone.Wait();

	console.Wait();
	cout << "Vehicle front being jacked down, JackTech" << endl;
	console.Signal();

	Sleep(500);
	frontJackDown.Signal(); // signal completion, tires on asphalt

	console.Wait();
	cout << "Vehicle done jacking down, JackTech" << endl;
	console.Signal();

	return 0;
}

JackTech::~JackTech() {}

NutTech::NutTech() {}

int NutTech::main(void) {
	CMutex console("Console");
	CSemaphore frontJackDone("Front Jack Done", 0, 1);
	CSemaphore frontNutRemovalDone("Front Nuts Removal Done", 0, 2);
	CSemaphore frontWheelReplaceDone("Front Wheels Replace Done", 0, 2);
	CSemaphore frontNutInstallDone("Front Nuts Install Done", 0, 2);

	frontJackDone.Wait(); // wait for signal from JackTech

	console.Wait();
	cout << "Removing nuts, NutTech" << endl;
	console.Signal();

	Sleep(750);
	frontNutRemovalDone.Signal(); // signal nut removal process done
	frontNutRemovalDone.Signal();

	console.Wait();
	cout << "Done removing nuts, NutTech" << endl;
	console.Signal();

	frontWheelReplaceDone.Wait(); // wait for signal from WheelReplaceTech
	frontWheelReplaceDone.Wait();

	console.Wait();
	cout << "Replacing nuts, NutTech" << endl;
	console.Signal();

	Sleep(500);
	frontNutInstallDone.Signal();
	frontNutInstallDone.Signal();

	console.Wait();
	cout << "Done replacing nuts, NutTech" << endl;
	console.Signal();

	return 0;
}

NutTech::~NutTech() {}

WheelRemoveTech::WheelRemoveTech() {}

int WheelRemoveTech::main(void) {
	CMutex console("Console");
	CSemaphore frontNutRemovalDone("Front Nuts Removal Done", 0, 2);
	CSemaphore frontWheelRemovalDone("Front Wheels Removal Done", 0, 2); // one for each left and right wheel

	frontNutRemovalDone.Wait(); // wait for signals from nut techs
	frontNutRemovalDone.Wait(); // wait for signals from nut techs

	console.Wait();
	cout << "Removing wheels, WheelRemoveTech" << endl;
	console.Signal();

	Sleep(1000);
	frontWheelRemovalDone.Signal();
	frontWheelRemovalDone.Signal();

	console.Wait();
	cout << "Done removing wheel, WheelRemoveTech" << endl;
	console.Signal();

	return 0;
}

WheelRemoveTech::~WheelRemoveTech() {}

WheelReplaceTech::WheelReplaceTech() {}

int WheelReplaceTech::main(void) {
	CMutex console("Console");
	CSemaphore frontWheelRemovalDone("Front Wheels Removal Done", 0, 2);
	CSemaphore frontWheelReplaceDone("Front Wheels Replace Done", 0, 2);

	frontWheelRemovalDone.Wait(); // wait for signals from wheel removal tech
	frontWheelRemovalDone.Wait(); // wait for signals from wheel removal tech

	console.Wait();
	cout << "Replacing wheels, WheelReplaceTech" << endl;
	console.Signal();

	Sleep(500);
	frontWheelReplaceDone.Signal();
	frontWheelReplaceDone.Signal();

	console.Wait();
	cout << "Done replacing wheels, WheelRepaceTech" << endl;
	console.Signal();

	return 0;
}

WheelReplaceTech::~WheelReplaceTech() {}