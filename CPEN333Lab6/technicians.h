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
	int data;
	int main(void);

	int Refueler(void* args);
	int RearJack(void* args);
	int FrontJack(void* args);

public:
	Supervisor(int num); // default constructor, needs to be public to be access outside
	~Supervisor();
};

class Refueler : public ActiveClass
{
private:
	int data;
	int main(void);

public:
	Refueler(int num);
	~Refueler();
};

class Jacker : public ActiveClass
{
private:
	int data;
	int main(void);

public:
	Jacker(int num);
	~Jacker();
};

class Wheeler : public ActiveClass
{
private:
	int data;
	int main(void);

public:
	Wheeler(int num);
	~Wheeler();
};

#endif