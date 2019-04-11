#pragma once

class Entity {
public:
	int posX;
	int posY;
	int lastX;
	int lastY;
	double gX;
	double gY;
	int twoPower;
	int gTwoPower;
	bool lastMoveChangeValue;
	bool wasJoined;

	Entity(int posX, int posY, int twoPower = 1);
	Entity(const Entity &entity);
	~Entity();

	void changePos(int x, int y);
	void changeValue(bool up = true);
	bool canJoin(Entity *entity);
};