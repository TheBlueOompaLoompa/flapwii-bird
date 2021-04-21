#include "pipe.hpp"

Pipe::Pipe() {
	Pipe::y = rand() % 240 + 1 + (240/2);
	Pipe::x = 640;
}

Pipe::~Pipe() {
	
}

void Pipe::move() {
	Pipe::x -= Pipe::speed;
}

void Pipe::reset() {
	Pipe::y = rand() % 240 + 1 + (240/2);
	Pipe::x = 640;
}