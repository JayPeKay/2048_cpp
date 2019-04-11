#pragma once
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include"./sdl-2.0.7/include/SDL.h"

#include "Board.h"

class Window {
public:
	struct Record {
		char name[32];
		double time;
		int score;
	};

	Window(int width, int height);
	~Window();

private:
	int width;
	int height;
	int boardMargin, boradMarginTop;
	int boardBackground, gameBackground;
	int mouseX;
	int mouseY;
	int fieldSize;
	int animationDuration;

	Board *board;
	char *savesFolder;

	SDL_Rect gameRect, boardRect;
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Surface *boardSurface, *gameSurface, *backgroundSurface;
	SDL_Texture *texture;
	SDL_Event event;
	SDL_Surface *assets, *charset;

	bool initSDL();

	void openStartView();
	bool newGame(int sizeX, int sizeY);
	void startGameLoop();
	void openSavesMenu();
	char **getSaves(int *n);
	void openRecords();
	void openEndView();

	SDL_Rect drawString(int x, int y, const char *text);
	bool saveBoard(char *fileName);
	void createBoardFromFile(char *fileName);
	void animObjects(double move);

	Record *getRecords(int *n, char *size);
	bool addRecord(const char *name);
};