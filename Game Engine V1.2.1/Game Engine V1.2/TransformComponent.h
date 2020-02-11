#pragma once
#include "ECS.h"
#include "Vector2D.h"

// In struct, everything default is public
// In class, everything default is private

class TransformComponent : public Component {

public:

	Vector2D position;
	Vector2D velocity;

	int height = 600;
	int width = 800;
	double scale = 1.0;

	int speed = 3;

	TransformComponent() {
		position.Zero();
	}

	TransformComponent(double sc) {
		position.Zero();
		scale = sc;
	}

	TransformComponent(float x, float y) {
		position.x = x;
		position.y = y;
	}

	TransformComponent(float x, float y, int h, int w, int sc) {
		position.x = x;
		position.y = y;
		height = h;
		width = w;
		scale = sc;
	}


	// The override makes sure that you are overriding a function and parent function is a virtual
	// Not needed, but good programming practice, much like const function this is override specifier
	void init() override {
		velocity.Zero();
	}
	
	void update() override {
		position.x += velocity.x * speed;
		position.y += velocity.y * speed;
	}
};