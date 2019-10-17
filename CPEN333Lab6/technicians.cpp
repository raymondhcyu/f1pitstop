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
	cout << "Pit stop has a vehicle inside, supervisor" << endl;

	// Do pit stop stuff
	RefuelerThread.WaitForThread();
	FrontJackThread.WaitForThread();
	//RearJackThread.WaitForThread();

	cout << "Pit stop vehicle serviced and releasing, supervisor" << endl;
	exitLight.Signal(); // wait for exit light
	pitEmpty.Wait(); // signal that pit empty

	return 0;
}

int Supervisor::Refueler(void* args) {
	CSemaphore refuelStop("Refuel Stop", 0, 1);
	CSemaphore refuelStart("Refuel Start", 0, 1);

	refuelStart.Signal();
	cout << "Instruction to start refuelling, supervisor" << endl;
	refuelStop.Wait();

	return 0;
}

int Supervisor::FrontJack(void* args) {
	CSemaphore frontJackUp("Front Jack Up", 0, 1);
	CSemaphore frontJackDown("Front Jack Down", 0, 1);

	frontJackUp.Signal(); // start front jack up
	cout << "Instruction to jack front up, supervisor" << endl;
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
	CSemaphore refuelStop("Refuel Stop", 0, 1);
	CSemaphore refuelStart("Refuel Start", 0, 1);

	refuelStart.Wait(); // wait for refuel start signal from supervisor
	cout << "Vehicle being refueled, refueller" << endl;
	Sleep(1000);
	refuelStop.Signal(); // wait for refuel stop signal from supervisor (or should it send complete status?)
	cout << "Vehicle done refuelling, refueller" << endl;
	
	return 0;
}

Refueler::~Refueler() {}

JackTech::JackTech(void) {}

int JackTech::main(void) {
	CSemaphore frontJackUp("Front Jack Up", 0, 1);
	CSemaphore frontJackDone("Front Jack Done", 0, 1);
	CSemaphore frontNutInstallDone("Front Nuts Install Done", 0, 2);
	CSemaphore frontJackDown("Front Jack Down", 0, 1);

	frontJackUp.Wait(); // wait for jack up signal from supervisor
	cout << "Vehicle front being jacked up, JackTech" << endl;
	Sleep(750);
	frontJackDone.Signal(); // front jack done jacking up, signal to NutTech
	cout << "Vehicle front done jacking up, JackTech" << endl;

	frontNutInstallDone.Wait(); // wait twice for nuts to reinstall
	frontNutInstallDone.Wait();
	cout << "Vehicle front being jacked down, JackTech" << endl;
	Sleep(500);
	frontJackDown.Signal(); // signal completion, tires on asphalt
	cout << "Vehicle done jacking down, JackTech" << endl;

	return 0;
}

JackTech::~JackTech() {}

NutTech::NutTech() {}

int NutTech::main(void) {
	CSemaphore frontJackDone("Front Jack Done", 0, 1);
	CSemaphore frontNutRemovalDone("Front Nuts Removal Done", 0, 2);
	CSemaphore frontWheelReplaceDone("Front Wheels Replace Done", 0, 2);
	CSemaphore frontNutInstallDone("Front Nuts Install Done", 0, 2);

	frontJackDone.Wait(); // wait for signal from JackTech
	cout << "Removing nuts, NutTech" << endl;
	Sleep(750);
	frontNutRemovalDone.Signal(); // signal nut removal process done
	frontNutRemovalDone.Signal();
	cout << "Done removing nuts, NutTech" << endl;

	frontWheelReplaceDone.Wait(); // wait for signal from WheelReplaceTech
	frontWheelReplaceDone.Wait();
	cout << "Replacing nuts, NutTech" << endl;
	Sleep(500);
	frontNutInstallDone.Signal();
	frontNutInstallDone.Signal();
	cout << "Done replacing nuts, NutTech" << endl;

	return 0;
}

NutTech::~NutTech() {}

WheelRemoveTech::WheelRemoveTech() {}

int WheelRemoveTech::main(void) {
	CSemaphore frontNutRemovalDone("Front Nuts Removal Done", 0, 2);
	CSemaphore frontWheelRemovalDone("Front Wheels Removal Done", 0, 2); // one for each left and right wheel

	frontNutRemovalDone.Wait(); // wait for signals from nut techs
	frontNutRemovalDone.Wait(); // wait for signals from nut techs
	cout << "Removing wheels, WheelRemoveTech" << endl;
	Sleep(1000);
	frontWheelRemovalDone.Signal();
	frontWheelRemovalDone.Signal();
	cout << "Done removing wheel, WheelRemoveTech" << endl;

	return 0;
}

WheelRemoveTech::~WheelRemoveTech() {}

WheelReplaceTech::WheelReplaceTech() {}

int WheelReplaceTech::main(void) {
	CSemaphore frontWheelRemovalDone("Front Wheels Removal Done", 0, 2);
	CSemaphore frontWheelReplaceDone("Front Wheels Replace Done", 0, 2);

	frontWheelRemovalDone.Wait(); // wait for signals from wheel removal tech
	frontWheelRemovalDone.Wait(); // wait for signals from wheel removal tech
	cout << "Replacing wheels, WheelReplaceTech" << endl;
	Sleep(500);
	frontWheelReplaceDone.Signal();
	frontWheelReplaceDone.Signal();
	cout << "Done replacing wheels, WheelRepaceTech" << endl;

	return 0;
}

WheelReplaceTech::~WheelReplaceTech() {}