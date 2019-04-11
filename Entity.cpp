#include "Entity.h"

Entity::Entity(int posX, int posY, int twoPower)
	: posX(posX), posY(posY), lastX(posX), lastY(posY), gX(posX), gY(posY), twoPower(twoPower), gTwoPower(twoPower), wasJoined(false) {
}

Entity::Entity(const Entity &entity) : Entity(entity.posX, entity.posY, entity.twoPower) {
}

Entity::~Entity() {}

void Entity::changePos(int x, int y) {
	this->lastX = this->posX;
	this->lastY = this->posY;
	this->posX = x;
	this->posY = y;
}

void Entity::changeValue(bool up) {
	if (up) {
		wasJoined = true;
		++twoPower;
	} else {
		--twoPower;
	}
}

bool Entity::canJoin(Entity *entity) {
	return entity != nullptr && entity->twoPower == this->twoPower && !this->wasJoined;
}