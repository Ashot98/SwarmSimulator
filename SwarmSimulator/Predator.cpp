#include "Predator.h"
#include "Globals.h"

Predator::Predator()
	: Entity(Globals::predatorColor) {}

void Predator::update(float dt)
{
	move(dt);
}
