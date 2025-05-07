#pragma once

#include "Entity.h"

class Predator : public Entity {
public:
	Predator();

	void update(float dt);
};