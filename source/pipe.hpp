#pragma once

#include <grrlib.h>
#include <gccore.h>

#include <stdlib.h>

class Pipe {
private:
	const float speed = 1;
public:
	float x, y;

	Pipe();
	~Pipe();
	void move();
	void reset();
};