#include "Board.h"

Board::Board(int bSizeX, int bSizeY)
	: bSizeX(bSizeX), bSizeY(bSizeY), entitiesLength(0), lastEntitiesLength(0), entitiesToDeleteLength(0), entitiesMaxLength(bSizeX * bSizeY), canUndo(false), time(0), score(0), lastScore(0) {
	entities = new Entity*[entitiesMaxLength];
	lastEntities = new Entity*[entitiesMaxLength];
	entitiesToDelete = new Entity*[entitiesMaxLength];

	board = new Entity**[bSizeY];
	for(int i = 0; i < bSizeY; i++) {
		board[i] = new Entity*[bSizeY];
	}
	for(int i = 0; i < bSizeY; i++) {
		for(int j = 0; j < bSizeX; j++) {
			board[i][j] = nullptr;
		}
	}
}

Board::~Board() {
	for(int i = 0; i < bSizeY; i++) {
		for(int j = 0; j < bSizeX; j++)
			delete board[i][j];
		delete[] board[i];
	}
	delete[] board;

	delete[] entities;

	deleteCopy();
	delete[] lastEntities;

	deleteEntities();
	delete[] entitiesToDelete;
}

void Board::setEntity(int x, int y, Entity *entity) {
	board[y][x] = entity;
}

Entity *Board::getEntity(int x, int y) {
	return board[y][x];
}

void Board::setNewBoard() {
	addEntity(randEntity());
	addEntity(randEntity());
}

Entity *Board::randEntity() {
	if (entitiesLength == entitiesMaxLength) {
		return nullptr;
	}

	int x, y;

	bool randNext = true;
	while (randNext) {
		x = rand() % bSizeX;
		y = rand() % bSizeY;
		for (int i = 0; i < entitiesLength; i++) {
			if (x == entities[i]->posX && y == entities[i]->posY) {
				break;
			}

			if (i == entitiesLength - 1) {
				randNext = false;
			}
		}

		if (entitiesLength == 0) {
			randNext = false;
		}
	}

	Entity *entity;
	int power = rand() % 10;
	if (power == 0) {
		entity = new Entity(x, y, 2);
	}
	else {
		entity = new Entity(x, y);
	}

	return entity;
}

void Board::addEntity(Entity *entity) {
	if (entitiesLength < entitiesMaxLength) {
		entities[entitiesLength++] = entity;
		setEntity(entity->posX, entity->posY, entity);
	}
}

void Board::move(Direction direction) {
	bool moved = false;
	deleteEntities();
	switch (direction) {
	case LEFT:
		if (canMoveLeft()) {
			createCopy();
			moveLeft();
			moved = true;
		}
		break;
	case RIGHT:
		if (canMoveRight()) {
			createCopy();
			moveRight();
			moved = true;
		}
		break;
	case UP:
		if (canMoveUp()) {
			createCopy();
			moveUp();
			moved = true;
		}
		break;
	case DOWN:
		if (canMoveDown()) {
			createCopy();
			moveDown();
			moved = true;
		}
		break;
	}

	if (moved) {
		addEntity(randEntity());
		canUndo = true;

		for (int i = 0; i < entitiesLength; i++) {
			entities[i]->wasJoined = false;
		}
	}
}

void Board::moveLeft() {
	Entity *entity = nullptr;
	Entity *entity2 = nullptr;

	for (int i = 0; i < bSizeY; i++) {
		for (int j = 0; j < bSizeX; j++) {
			if (entity != nullptr) {
				entity2 = getEntity(j, i);

				if (entity->canJoin(entity2)) {
					joinEntities(entity, entity2);
					score += pow(2, entity->twoPower);
				} else if (entity2 != nullptr) {
					if (entity->posX + 1 != entity2->posX) {
						changeEntityPos(entity2, entity->posX + 1, i);
					}
					entity = entity2;
				}
			} else {
				entity = getEntity(j, i);

				if (entity != nullptr && j != 0) {
					changeEntityPos(entity, 0, i);
				}
			}

			entity2 = nullptr;
		}

		entity = nullptr;
	}
}

void Board::moveRight() {
	Entity *entity = nullptr;
	Entity *entity2 = nullptr;

	for (int i = 0; i < bSizeY; i++) {
		for (int j = bSizeX - 1; j >= 0; j--) {
			if (entity != nullptr) {
				entity2 = getEntity(j, i);

				if (entity->canJoin(entity2)) {
					joinEntities(entity, entity2);
					score += pow(2, entity->twoPower);
				} else if (entity2 != nullptr) {
					if (entity->posX - 1 != entity2->posX) {
						changeEntityPos(entity2, entity->posX - 1, i);
					}
					entity = entity2;
				}
			} else {
				entity = getEntity(j, i);

				if (entity != nullptr && j != bSizeX - 1) {
					changeEntityPos(entity, bSizeX - 1, i);
				}
			}

			entity2 = nullptr;
		}

		entity = nullptr;
	}
}

void Board::moveUp() {
	Entity *entity = nullptr;
	Entity *entity2 = nullptr;

	for (int i = 0; i < bSizeX; i++) {
		for (int j = 0; j < bSizeY; j++) {
			if (entity != nullptr) {
				entity2 = getEntity(i, j);

				if (entity->canJoin(entity2)) {
					joinEntities(entity, entity2);
					score += pow(2, entity->twoPower);
				} else if (entity2 != nullptr) {
					if (entity->posY + 1 != entity2->posY) {
						changeEntityPos(entity2, i, entity->posY + 1);
					}
					entity = entity2;
				}
			} else {
				entity = getEntity(i, j);

				if (entity != nullptr && j != 0) {
					changeEntityPos(entity, i, 0);
				}
			}

			entity2 = nullptr;
		}

		entity = nullptr;
	}
}

void Board::moveDown() {
	Entity *entity = nullptr;
	Entity *entity2 = nullptr;

	for (int i = 0; i < bSizeX; i++) {
		for (int j = bSizeY - 1; j >= 0; j--) {
			if (entity != nullptr) {
				entity2 = getEntity(i, j);

				if (entity->canJoin(entity2)) {
					joinEntities(entity, entity2);
					score += pow(2, entity->twoPower);
				} else if(entity2 != nullptr) {
					if (entity->posY - 1 != entity2->posY) {
						changeEntityPos(entity2, i, entity->posY - 1);
					}
					entity = entity2;
				}
			} else {
				entity = getEntity(i, j);

				if (entity != nullptr && j != bSizeY - 1) {
					changeEntityPos(entity, i, bSizeY - 1);
				}
			}

			entity2 = nullptr;
		}

		entity = nullptr;
	}
}

bool Board::canMove() {
	return canMoveLeft() || canMoveRight() || canMoveUp() || canMoveDown();
}

bool Board::canMoveLeft() {
	Entity *entity = nullptr;
	Entity *entity2 = nullptr;

	for (int i = 0; i < bSizeY; i++) {
		for (int j = 0; j < bSizeX; j++) {
			if (entity != nullptr) {
				entity2 = getEntity(j, i);

				if (entity->canJoin(entity2)) {
					return true;
				} else if (entity2 != nullptr) {
					if (entity->posX + 1 != entity2->posX) {
						return true;
					}
					entity = entity2;
				}
			} else {
				entity = getEntity(j, i);

				if (entity != nullptr && j != 0) {
					return true;
				}
			}

			entity2 = nullptr;
		}

		entity = nullptr;
	}

	return false;
}

bool Board::canMoveRight() {
	Entity *entity = nullptr;
	Entity *entity2 = nullptr;

	for (int i = 0; i < bSizeY; i++) {
		for (int j = bSizeX - 1; j >= 0; j--) {
			if (entity != nullptr) {
				entity2 = getEntity(j, i);

				if (entity->canJoin(entity2)) {
					return true;
				} else if (entity2 != nullptr) {
					if (entity->posX - 1 != entity2->posX) {
						return true;
					}
					entity = entity2;
				}
			} else {
				entity = getEntity(j, i);

				if (entity != nullptr && j != bSizeX - 1) {
					return true;
				}
			}

			entity2 = nullptr;
		}

		entity = nullptr;
	}

	return false;
}

bool Board::canMoveUp() {
	Entity *entity = nullptr;
	Entity *entity2 = nullptr;

	for (int i = 0; i < bSizeX; i++) {
		for (int j = 0; j < bSizeY; j++) {
			if (entity != nullptr) {
				entity2 = getEntity(i, j);

				if (entity->canJoin(entity2)) {
					return true;
				} else if (entity2 != nullptr) {
					if (entity->posY + 1 != entity2->posY) {
						return true;
					}
					entity = entity2;
				}
			} else {
				entity = getEntity(i, j);

				if (entity != nullptr && j != 0) {
					return true;
				}
			}

			entity2 = nullptr;
		}

		entity = nullptr;
	}

	return false;
}

bool Board::canMoveDown() {
	Entity *entity = nullptr;
	Entity *entity2 = nullptr;

	for (int i = 0; i < bSizeX; i++) {
		for (int j = bSizeY - 1; j >= 0; j--) {
			if (entity != nullptr) {
				entity2 = getEntity(i, j);

				if (entity->canJoin(entity2)) {
					return true;
				} else if (entity2 != nullptr) {
					if (entity->posY - 1 != entity2->posY) {
						return true;
					}
					entity = entity2;
				}
			} else {
				entity = getEntity(i, j);

				if (entity != nullptr && j != bSizeY - 1) {
					return true;
				}
			}

			entity2 = nullptr;
		}

		entity = nullptr;
	}

	return false;
}

void Board::changeEntityPos(Entity *entity, int x, int y) {
	setEntity(entity->posX, entity->posY, nullptr);
	setEntity(x, y, entity);
	entity->changePos(x, y);
}

void Board::joinEntities(Entity *entity, Entity *entity2) {
	entity->changeValue();
	setEntity(entity2->posX, entity2->posY, nullptr);
	entitiesToDelete[entitiesToDeleteLength++] = entity2;
	entity2->changePos(entity->posX, entity->posY);
	for (int i = 0; i < entitiesLength; i++) {
		if (entities[i] == entity2) {
			entities[i] = entities[--entitiesLength];
			break;
		}
	}
}

void Board::undoMove() {
	if (!canUndo) {
		return;
	}

	score = lastScore;

	for (int i = 0; i < entitiesLength; i++) {
		setEntity(entities[i]->posX, entities[i]->posY, nullptr);
		delete entities[i];
	}
	entitiesLength = 0;

	for (int i = 0; i < lastEntitiesLength; i++) {
		entities[entitiesLength] = new Entity(*(lastEntities[i]));
		setEntity(entities[entitiesLength]->posX, entities[entitiesLength]->posY, entities[entitiesLength]);
		++entitiesLength;
	}

	canUndo = false;
}

void Board::createCopy() {
	deleteCopy();

	for (int i = 0; i < entitiesLength; i++) {
		lastEntities[lastEntitiesLength++] = new Entity(*(entities[i]));
	}
	lastScore = score;
}

void Board::deleteCopy() {
	for (int i = 0; i < lastEntitiesLength; i++) {
		delete lastEntities[i];
	}
	lastEntitiesLength = 0;
}

void Board::deleteEntities() {
	for (int i = 0; i < entitiesToDeleteLength; i++) {
		delete entitiesToDelete[i];
	}
	entitiesToDeleteLength = 0;
}

void Board::loadFromFile(char *fileName) {
	FILE *f = fopen(fileName, "r");
	char c;
	int counterX = 0;
	int counterY = 0;

	if(f != NULL) {
		fscanf(f, "%lf %d\n", &time, &score);
		while((c = fgetc(f)) != EOF) {
			if(c == '\n') {
				counterX = 0;
				counterY++;
			} else {
				if (c > '0') {
					setEntity(counterX, counterY, new Entity(counterX, counterY, c - '0'));
					entities[entitiesLength++] = getEntity(counterX, counterY);
				}

				counterX++;
			}
		}
		fclose(f);
	}
}

void Board::saveToFile(char *fileName) {
	FILE *f = fopen(fileName, "w");
	Entity *entity = nullptr;
	
	if (f != NULL) {
		fprintf(f, "%.3f %d\n", time, score);

		for (int i = 0; i < bSizeY; i++) {
			for (int j = 0; j < bSizeX; j++) {
				entity = getEntity(j, i);
				if (entity == nullptr) {
					fprintf(f, "0");
				} else {
					fprintf(f, "%c", getEntity(j, i)->twoPower + '0');
				}
			}
			fprintf(f, "\n");
		}

		fclose(f);
	}
}