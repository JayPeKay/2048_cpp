#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Entity.h"

class Board {
public:
	enum Direction {
		RIGHT,
		LEFT,
		UP,
		DOWN
	};

	int bSizeX;
	int bSizeY;
	Entity **entities;
	int entitiesLength;
	Entity **entitiesToDelete;
	int entitiesToDeleteLength;
	Entity **lastEntities;
	double time;
	int score;
	int lastScore;

	Board(int bSizeX, int bSizeY);
	~Board();

	void setEntity(int x, int y, Entity *entity);
	Entity *randEntity();
	void addEntity(Entity *entity);
	Entity *getEntity(int x, int y);
	void loadFromFile(char *fileName);
	void saveToFile(char *fileName);

	bool canMove();
	void move(Direction direction);
	void undoMove();
	void setNewBoard();

private:
	Entity ***board;
	int entitiesMaxLength;
	int lastEntitiesLength;
	bool canUndo;

	bool canMoveLeft();
	bool canMoveRight();
	bool canMoveDown();
	bool canMoveUp();
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	void changeEntityPos(Entity *entity, int x, int y);
	void joinEntities(Entity *entity, Entity *entity2);
	void createCopy();
	void deleteCopy();
	void deleteEntities();
};