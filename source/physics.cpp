#include "physics.hpp"

Physics::Physics() {
	Physics::position.y = 640/3;
	Physics::position.x = 480/3;
}
Physics::~Physics() {}

Vec2 Physics::update_bird(bool flap, Pipe pipe_1, Pipe pipe_2){
	if(flap){
		velocity = Physics::flap_height;
	}else{
		velocity += Physics::gravity;
	}

	Physics::position.y += velocity;

	Physics::dead = is_colliding(pipe_1, pipe_2);

	Physics::update_score(pipe_1, pipe_2);

	if(Physics::dead) reset();

	return Physics::position;
}

void Physics::reset(){
	Physics::position.y = 640/3;
	Physics::velocity = 0;
	Physics::score = 0;
	Physics::pipe_iter = false;
}

bool Physics::is_colliding(Pipe pipe_1, Pipe pipe_2){
	// Pipe 1
	return (((pipe_1.x - (144/4) <= Physics::position.x || pipe_1.x <= Physics::position.x) && pipe_1.x + 52 >= Physics::position.x) &&
	(pipe_1.y <= Physics::position.y || pipe_1.y - 100 >= Physics::position.y )) ||

	// Pipe 2
	(((pipe_2.x - (144/4) <= Physics::position.x || pipe_2.x <= Physics::position.x) && pipe_2.x + 52 >= Physics::position.x) &&
	(pipe_2.y <= Physics::position.y || pipe_2.y - 100 >= Physics::position.y )) ||

	// Screen top bottom
	Physics::position.y > 480 || Physics::position.y < 0;
}

void Physics::update_score(Pipe pipe_1, Pipe pipe_2){
	if(
		(!Physics::pipe_iter && pipe_1.x + 52 <= 640/3 && pipe_1.x + 52 + 20 >= 640/3) ||
		(Physics::pipe_iter && pipe_2.x + 52 <= 640/3 && pipe_2.x + 52 + 20 >= 640/3))
	{
		Physics::pipe_iter = !Physics::pipe_iter;
		Physics::score++;
	}
}