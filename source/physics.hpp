#pragma once

#include "vec2.hpp"
#include "pipe.hpp"

class Physics {
private:
	const float gravity = 0.5;
	const float flap_height = -6.5;

	Vec2 position;

	void reset();
	bool is_colliding(Pipe pipe_1, Pipe pipe_2);
	void update_score(Pipe pipe_1, Pipe pipe_2);

public:
	Physics();
	~Physics();

	Vec2 update_bird(bool flap, Pipe pipe_1, Pipe pipe_2);

	float velocity;
	bool pipe_iter = false;
	bool dead = false;
	int score = 0;
};