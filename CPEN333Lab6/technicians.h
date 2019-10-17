/*	Raymond Yu
	14 October 2019
	11298149
*/

#pragma once
#include "..\rt.h"

#ifndef __technicians__
#define __technicians__

// Active Class objects are like a mini program with a main() and multiple threads
class Supervisor : public ActiveClass
{
private:
	int main(void);

	int Refueler(void* args);
	int RearJack(void* args);
	int FrontJack(void* args);

public:
	Supervisor(void); // default constructor, needs to be public to be access outside
	~Supervisor();
};

class Refueler : public ActiveClass
{
private:
	int main(void);

public:
	Refueler();
	~Refueler();
};

class JackTech : public ActiveClass
{
private:
	int main(void);

public:
	JackTech();
	~JackTech();
};

class NutTech : public ActiveClass
{
private:
	int main(void);

public:
	NutTech();
	~NutTech();
};

class WheelRemoveTech : public ActiveClass
{
private:
	int main(void);

public:
	WheelRemoveTech();
	~WheelRemoveTech();
};

class WheelReplaceTech : public ActiveClass
{
private:
	int main(void);

public:
	WheelReplaceTech();
	~WheelReplaceTech();
};

#endif