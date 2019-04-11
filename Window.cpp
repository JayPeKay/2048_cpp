#include "Window.h"


Window::Window(int width, int height):
	savesFolder("saves/"), width(width), height(height), mouseX(0), mouseY(0), fieldSize(100), board(new Board(4, 4)),
	boardMargin(20), boradMarginTop(50), animationDuration(200) {
	if(initSDL()) {
		openStartView();

		SDL_FreeSurface(charset);
		SDL_FreeSurface(assets);
		SDL_DestroyTexture(texture);
		SDL_FreeSurface(gameSurface);
		SDL_FreeSurface(backgroundSurface);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}
}

Window::~Window() {
	delete board;
}

bool Window::initSDL() {
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		SDL_Quit();
		return false;
	}

	SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer);
	if(window == nullptr) {
		printf("SDL_CreateWindow error: %s\n", SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
		return false;
	}

	if(renderer == nullptr) {
		printf("SDL_CreateRenderer error: %s\n", SDL_GetError());
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return false;
	}

	backgroundSurface = SDL_CreateRGBSurface(0, height - 40, height - 40, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	if (backgroundSurface == NULL) {
		printf("SDL_CreateRGBSurface error: %s\n", SDL_GetError());
		SDL_FreeSurface(backgroundSurface);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return false;
	}

	gameSurface = SDL_CreateRGBSurface(0, width, height, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	if (gameSurface == NULL) {
		printf("SDL_CreateRGBSurface error: %s\n", SDL_GetError());
		SDL_FreeSurface(gameSurface);
		SDL_FreeSurface(backgroundSurface);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return false;
	}

	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
	if(texture == NULL) {
		printf("SDL_CreateTexture error: %s\n", SDL_GetError());
		SDL_DestroyTexture(texture);
		SDL_FreeSurface(gameSurface);
		SDL_FreeSurface(backgroundSurface);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return false;
	}

	assets = SDL_LoadBMP("./img/assets_2048.bmp");
	if(assets == NULL) {
		printf("SDL_LoadBMP(assets_2048.bmp) error: %s\n", SDL_GetError());
		SDL_DestroyTexture(texture);
		SDL_FreeSurface(gameSurface);
		SDL_FreeSurface(backgroundSurface);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return false;
	}
	SDL_SetColorKey(assets, true, 0xFFFFFF);

	gameBackground = SDL_MapRGBA(gameSurface->format, 250, 248, 239, 255);
	boardBackground = SDL_MapRGBA(backgroundSurface->format, 187, 173, 160, 255);
	SDL_SetWindowTitle(window, "Szablon do zdania drugiego 2017");

	charset = SDL_LoadBMP("./img/cs8x8.bmp");
	if (charset == NULL) {
		printf("SDL_LoadBMP(cs8x8.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(assets);
		SDL_DestroyTexture(texture);
		SDL_FreeSurface(gameSurface);
		SDL_FreeSurface(backgroundSurface);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return false;
	};
	SDL_SetColorKey(charset, true, 0x000000);

	gameRect = SDL_Rect{ width / 2 - backgroundSurface->w / 2, height / 2 - backgroundSurface->h / 2 };
	boardRect = SDL_Rect{ backgroundSurface->w / 2 - board->bSizeX * fieldSize / 2, (backgroundSurface->h + boradMarginTop) / 2 - board->bSizeY * fieldSize / 2 };

	return true;
}

void Window::startGameLoop() {
	char text[128];
	int t1 = SDL_GetTicks();
	int t2;
	double move;
	double delta;

	SDL_Rect dest;
	dest.x = 0;
	dest.y = 0;
	dest.w = fieldSize;
	dest.h = fieldSize;
	SDL_Rect src;
	src.x = 0;
	src.y = 0;
	src.w = fieldSize;
	src.h = fieldSize;

	char key;
	bool render = true;

	while(render) {
		SDL_FillRect(backgroundSurface, NULL, boardBackground);
		SDL_FillRect(gameSurface, NULL, gameBackground);
		SDL_FillRect(boardSurface, NULL, boardBackground);

		t2 = SDL_GetTicks();
		delta = (t2 - t1);
		t1 = t2;
		board->time += delta * 0.001;

		animObjects(delta);

		while(SDL_PollEvent(&event)) {
			switch(event.type) {
			case SDL_KEYDOWN:
				if(event.key.keysym.sym == SDLK_ESCAPE)
					return;
				else if(event.key.keysym.sym == SDLK_UP) {
					board->move(Board::UP);
				} else if(event.key.keysym.sym == SDLK_DOWN) {
					board->move(Board::DOWN);
				} else if(event.key.keysym.sym == SDLK_RIGHT) {
					board->move(Board::RIGHT);
				} else if(event.key.keysym.sym == SDLK_LEFT) {
					board->move(Board::LEFT);
				} else if(event.key.keysym.sym == SDLK_n) {
					key = 'n';
					render = false;
				} else if (event.key.keysym.sym == SDLK_l) {
					key = 'l';
					render = false;
				} else if (event.key.keysym.sym == SDLK_r) {
					key = 'r';
					render = false;
				} else if (event.key.keysym.sym == SDLK_u) {
					board->undoMove();
				} else if (event.key.keysym.sym == SDLK_s) {
					char str[12];
					sprintf(str, "%d", time(NULL));
					saveBoard(str);
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				mouseX = event.motion.x;
				mouseY = event.motion.y;
				break;
			case SDL_MOUSEBUTTONUP:
				if (abs(event.motion.x - mouseX) > abs(event.motion.y - mouseY)) {
					if (event.motion.x > mouseX) {
						board->move(Board::RIGHT);
					}
					else {
						board->move(Board::LEFT);
					}
				}
				else {
					if (event.motion.y > mouseY) {
						board->move(Board::DOWN);
					}
					else {
						board->move(Board::UP);
					}
				}
				break;
			case SDL_QUIT:
				return;
			}
		}

		sprintf(text, "2048, czas trwania = %.1f s, punkty = %d", board->time, board->score);
		drawString(backgroundSurface->w / 2, 10, text);

		src.x = 0;
		for(int i = 0; i < board->bSizeY; i++) {
			for(int j = 0; j < board->bSizeX; j++) {
				dest.x = j * fieldSize;
				dest.y = i * fieldSize;
				SDL_BlitSurface(assets, &src, boardSurface, &dest);
			}
		}

		for(int i = 0; i < board->entitiesLength; i++) {
			dest.x = board->entities[i]->gX * fieldSize;
			dest.y = board->entities[i]->gY * fieldSize;
			src.x = board->entities[i]->gTwoPower * fieldSize;
			SDL_BlitSurface(assets, &src, boardSurface, &dest);
		}

		for (int i = 0; i < board->entitiesToDeleteLength; i++) {
			if (board->entitiesToDelete[i]->gX != board->entitiesToDelete[i]->posX || board->entitiesToDelete[i]->gY != board->entitiesToDelete[i]->posY) {
				dest.x = board->entitiesToDelete[i]->gX * fieldSize;
				dest.y = board->entitiesToDelete[i]->gY * fieldSize;
				src.x = board->entitiesToDelete[i]->gTwoPower * fieldSize;
				SDL_BlitSurface(assets, &src, boardSurface, &dest);
			}
		}

		if(!board->canMove()) {
			key = 'e';
			render = false;
		}
		SDL_BlitSurface(boardSurface, NULL, backgroundSurface, &boardRect);
		SDL_BlitSurface(backgroundSurface, NULL, gameSurface, &gameRect);
		SDL_UpdateTexture(texture, NULL, gameSurface->pixels, gameSurface->pitch);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}

	switch (key) {
	case 'n':
		openStartView();
		break;
	case 'l':
		openSavesMenu();
		break;
	case 'r':
		openRecords();
		break;
	case 'e':
		openEndView();
	}
}

void Window::openStartView() {
	char *title = "Podaj rozmiar i nacisnij ENTER";
	char size[3];
	size[0] = '\0';
	int sizeLength = 1;
	SDL_Rect titleRect;
	SDL_Rect sizeRect;
	int inputY = 100;
	bool render = true;

	while (render) {
		SDL_FillRect(backgroundSurface, NULL, boardBackground);
		SDL_FillRect(gameSurface, NULL, gameBackground);

		titleRect = drawString(backgroundSurface->w / 2, inputY, title);

		if (sizeLength > 1) {
			sizeRect = drawString(backgroundSurface->w / 2, titleRect.y + titleRect.h + 10, size);
		}

		SDL_StartTextInput();
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
				if (event.key.keysym.sym >= SDLK_0 && event.key.keysym.sym <= SDLK_9) {
					size[sizeLength - 1] = event.key.keysym.sym;
					size[sizeLength++] = '\0';
				} else if (event.key.keysym.sym == SDLK_BACKSPACE) {
					if (sizeLength > 1) {
						--sizeLength;
						size[sizeLength - 1] = '\0';
					}
				} else if (event.key.keysym.sym == SDLK_ESCAPE) {
					return;
				} else if (event.key.keysym.sym == SDLK_RETURN) {
					render = false;
				}
				break;
			case SDL_QUIT:
				return;
			}
		}

		SDL_BlitSurface(backgroundSurface, NULL, gameSurface, &gameRect);
		SDL_UpdateTexture(texture, NULL, gameSurface->pixels, gameSurface->pitch);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}

	if (sizeLength > 1) {
		newGame(atoi(size), atoi(size));
		board->setNewBoard();
		startGameLoop();
	}
}

void Window::openSavesMenu() {
	char *title = "Podaj index i nacisnij ENTER";
	char id[3];
	id[0] = '\0';
	int idLength = 1;
	SDL_Rect titleRect;
	SDL_Rect idRect;
	int inputY = 100;
	bool render = true;

	int savesNumber;
	char **saves = getSaves(&savesNumber);

	while (render) {
		SDL_FillRect(backgroundSurface, NULL, boardBackground);
		SDL_FillRect(gameSurface, NULL, gameBackground);

		titleRect = drawString(backgroundSurface->w / 2, inputY, title);
		titleRect = drawString(backgroundSurface->w / 2, titleRect.y + titleRect.h + 10, id);
		idRect = SDL_Rect{ backgroundSurface->w / 2, titleRect.y + titleRect.h + 10 };

		for (int i = 0; i < savesNumber; i++) {
			idRect = drawString(backgroundSurface->w / 2, idRect.y + idRect.h + 10, saves[i]);
		}

		SDL_StartTextInput();
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
				if (event.key.keysym.sym >= SDLK_0 && event.key.keysym.sym <= SDLK_9) {
					id[idLength - 1] = event.key.keysym.sym;
					id[idLength++] = '\0';
				} else if (event.key.keysym.sym == SDLK_BACKSPACE) {
					if (idLength > 1) {
						--idLength;
						id[idLength - 1] = '\0';
					}
				} else if (event.key.keysym.sym == SDLK_ESCAPE) {
					return;
				} else if (event.key.keysym.sym == SDLK_RETURN) {
					render = false;
				}
				break;
			case SDL_QUIT:
				return;
			}
		}

		SDL_BlitSurface(backgroundSurface, NULL, gameSurface, &gameRect);
		SDL_UpdateTexture(texture, NULL, gameSurface->pixels, gameSurface->pitch);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}

	if (idLength > 1) {
		int i = atoi(id);
		if (i < savesNumber) {
			char tmp[64];
			sprintf(tmp, "%s%s.txt", savesFolder, saves[i]);

			createBoardFromFile(tmp);
			startGameLoop();
		}
	}

	for (int i = 0; i < savesNumber; i++) {
		delete[] saves[i];
	}
	delete[] saves;
}

void Window::openRecords() {
	char *sizeMessage = "Rozmiar planszy:";
	char size[3];
	size[0] = '\0';
	int sizeLength = 1;
	char *name = "Nazwa:";
	char *time = "Czas:";
	char *score = "Punkty:";
	char record[20];
	SDL_Rect idRect;
	SDL_Rect titleRect;
	int inputY = 20;
	bool render = true;

	int recordsNumber = 0;
	Record *records = nullptr;

	while (render) {
		SDL_FillRect(backgroundSurface, NULL, boardBackground);
		SDL_FillRect(gameSurface, NULL, gameBackground);

		titleRect = drawString(backgroundSurface->w / 2, inputY, sizeMessage);
		if (sizeLength > 1) {
			titleRect = drawString(titleRect.x + titleRect.w + 20, titleRect.y, size);
		}
		titleRect = drawString(backgroundSurface->w / 6, titleRect.y + titleRect.h + 10, name);
		titleRect = drawString(backgroundSurface->w / 6 * 3, titleRect.y, time);
		titleRect = drawString(backgroundSurface->w / 6 * 5, titleRect.y, score);
		idRect = SDL_Rect{ backgroundSurface->w / 6, titleRect.y + titleRect.h, 0, 0 };

		for (int i = 0; i < recordsNumber; i++) {
			idRect = drawString(backgroundSurface->w / 6, idRect.y + idRect.h + 10, records[i].name);
			sprintf(record, "%.3f", records[i].time);
			idRect = drawString(backgroundSurface->w / 6 * 3, idRect.y, record);
			sprintf(record, "%d", records[i].score);
			idRect = drawString(backgroundSurface->w / 6 * 5, idRect.y, record);
		}

		SDL_StartTextInput();
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_p) {
					for (int i = 1; i < recordsNumber; i++) {
						for (int j = 1; j < recordsNumber; j++) {
							if (records[j - 1].score < records[j].score) {
								Record tmp = records[j - 1];
								records[j - 1] = records[j];
								records[j] = tmp;
							}
						}
					}
				} else if (event.key.keysym.sym == SDLK_t) {
					for (int i = 1; i < recordsNumber; i++) {
						for (int j = 1; j < recordsNumber; j++) {
							if (records[j - 1].time > records[j].time) {
								Record tmp = records[j - 1];
								records[j - 1] = records[j];
								records[j] = tmp;
							}
						}
					}
				} else if (event.key.keysym.sym == SDLK_ESCAPE) {
					delete[] records;
					return;
				} else if (event.key.keysym.sym >= SDLK_0 && event.key.keysym.sym <= SDLK_9) {
					size[sizeLength - 1] = event.key.keysym.sym;
					size[sizeLength++] = '\0';

					delete[] records;
					records = getRecords(&recordsNumber, size);
				} else if (event.key.keysym.sym == SDLK_BACKSPACE) {
					if (sizeLength > 1) {
						--sizeLength;
						size[sizeLength - 1] = '\0';

						delete[] records;
						records = getRecords(&recordsNumber, size);
					}
				}
				break;
			case SDL_QUIT:
				delete[] records;
				return;
			}
		}

		SDL_BlitSurface(backgroundSurface, NULL, gameSurface, &gameRect);
		SDL_UpdateTexture(texture, NULL, gameSurface->pixels, gameSurface->pitch);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}
}

void Window::openEndView() {
	char *title = "Podaj nazwe i nacisnij ENTER";
	char name[32];
	name[0] = '\0';
	int nameLength = 1;
	SDL_Rect titleRect;
	int inputY = 100;
	bool render = true;

	while (render) {
		SDL_FillRect(backgroundSurface, NULL, boardBackground);
		SDL_FillRect(gameSurface, NULL, gameBackground);

		titleRect = drawString(backgroundSurface->w / 2, inputY, title);

		if (nameLength > 1) {
			drawString(backgroundSurface->w / 2, titleRect.y + titleRect.h + 10, name);
		}

		SDL_StartTextInput();
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_TEXTINPUT:
				name[nameLength - 1] = *event.text.text;
				name[nameLength++] = '\0';
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_BACKSPACE) {
					if (nameLength > 1) {
						--nameLength;
						name[nameLength - 1] = '\0';
					}
				} else if (event.key.keysym.sym == SDLK_ESCAPE) {
					return;
				} else if (event.key.keysym.sym == SDLK_RETURN) {
					render = false;
				}
				break;
			case SDL_QUIT:
				return;
			}
		}

		SDL_BlitSurface(backgroundSurface, NULL, gameSurface, &gameRect);
		SDL_UpdateTexture(texture, NULL, gameSurface->pixels, gameSurface->pitch);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}


	if (nameLength > 1) {
		addRecord(name);
	}
	openRecords();
}

char **Window::getSaves(int *n) {
	FILE *f = fopen("saves.txt", "r");
	FILE *fs;
	char c;

	*n = 0;
	char **saves = nullptr;
	if(f != NULL) {
		while((c = fgetc(f)) != EOF)
			if(c == '\n')
				(*n)++;
		rewind(f);

		saves = new char*[*n];
		for(int i = 0; i < *n; i++) {
			saves[i] = new char[12];
			fscanf(f, " %s ", saves[i]);
		}

		fclose(f);
	}

	return saves;
}

Window::Record *Window::getRecords(int *n, char *size) {
	char tmp[8];
	sprintf(tmp, "%s.txt", size);

	FILE *f = fopen(tmp, "r");
	Record *records = nullptr;
	*n = 0;
	char c;

	if(f != NULL) {
		while ((c = fgetc(f)) != EOF)
			if (c == '\n')
				(*n)++;
		rewind(f);

		records = new Record[*n];
		for (int i = 0; i < *n; i++) {
			fscanf(f, "%s %lf %d ", records[i].name, &records[i].time, &records[i].score);
		}

		fclose(f);
	}

	return records;
}

bool Window::addRecord(const char *name) {
	char file[8];
	sprintf(file, "%d.txt", board->bSizeX);
	FILE *f = fopen(file, "a");

	if(f != NULL) {
		fprintf(f, "%s %.3f %d\n", name, board->time, board->score);
		fclose(f);
		return true;
	}

	return false;
}

SDL_Rect Window::drawString(int x, int y, const char *text) {
	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = 8;
	d.h = 8;
	d.x = x - strlen(text) * 4;
	d.y = y;
	int counter = 0;
	while (text[counter]) {
		c = text[counter++] & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		SDL_BlitSurface(charset, &s, backgroundSurface, &d);
		d.x += 8;
		d.y = d.y;
	};

	d.w = strlen(text) * 8;
	d.x = x - d.w / 2;
	d.y = y;
	return d;
};

bool Window::saveBoard(char *fileName) {
	FILE *f = fopen("saves.txt", "a");

	char tmp[64];
	sprintf(tmp, "%s%s.txt", savesFolder, fileName);

	if (f != NULL) {
		fprintf(f, "%s\n", fileName);
		board->saveToFile(tmp);
		fclose(f);
		return true;
	}

	return false;
}

void Window::createBoardFromFile(char *fileName) {
	FILE *f = fopen(fileName, "r");
	char c;
	int size = 0;

	if(f != NULL) {
		while((c = fgetc(f)) != EOF)
			if(c == '\n')
				size++;
		size--;

		fclose(f);

		if (newGame(size, size)) {
			board->loadFromFile(fileName);
		}
	}
}

bool Window::newGame(int sizeX, int sizeY) {
	delete board;
	board = new Board(sizeX, sizeY);

	if (boardSurface != nullptr) {
		boardSurface = SDL_CreateRGBSurface(0, board->bSizeX * fieldSize, board->bSizeY * fieldSize, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	}
	if (boardSurface == NULL) {
		printf("SDL_CreateRGBSurface error: %s\n", SDL_GetError());
		SDL_FreeSurface(boardSurface);
		SDL_FreeSurface(backgroundSurface);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return false;
	}

	boardRect.x = backgroundSurface->w / 2 - boardSurface->w / 2;
	boardRect.y = (backgroundSurface->h + boradMarginTop) / 2 - boardSurface->h / 2;
	return true;
}

void Window::animObjects(double delta) {
	for(int i = 0; i < board->entitiesLength; i++) {
		if(board->entities[i]->posX != board->entities[i]->gX) {
			double r = (board->entities[i]->posX - board->entities[i]->lastX) * (delta / animationDuration);
			if(board->entities[i]->gX < board->entities[i]->posX) {
				if(board->entities[i]->gX + r > board->entities[i]->posX)
					board->entities[i]->gX = board->entities[i]->posX;
				else
					board->entities[i]->gX += r;
			} else {
				if(board->entities[i]->gX + r < board->entities[i]->posX)
					board->entities[i]->gX = board->entities[i]->posX;
				else
					board->entities[i]->gX += r;
			}
		} else if(board->entities[i]->posY != board->entities[i]->gY) {
			double r = (board->entities[i]->posY - board->entities[i]->lastY) * (delta / animationDuration);
			if(board->entities[i]->gY < board->entities[i]->posY) {
				if(board->entities[i]->gY + r > board->entities[i]->posY)
					board->entities[i]->gY = board->entities[i]->posY;
				else
					board->entities[i]->gY += r;
			} else {
				if(board->entities[i]->gY + r < board->entities[i]->posY)
					board->entities[i]->gY = board->entities[i]->posY;
				else
					board->entities[i]->gY += r;
			}
		} else if (board->entities[i]->gTwoPower != board->entities[i]->twoPower) {
			board->entities[i]->gTwoPower = board->entities[i]->twoPower;
		}
	}

	for (int i = 0; i < board->entitiesToDeleteLength; i++) {
		if (board->entitiesToDelete[i]->posX != board->entitiesToDelete[i]->gX) {
			double r = (board->entitiesToDelete[i]->posX - board->entitiesToDelete[i]->lastX) * (delta / animationDuration);
			if (board->entitiesToDelete[i]->gX < board->entitiesToDelete[i]->posX) {
				if (board->entitiesToDelete[i]->gX + r > board->entitiesToDelete[i]->posX)
					board->entitiesToDelete[i]->gX = board->entitiesToDelete[i]->posX;
				else
					board->entitiesToDelete[i]->gX += r;
			} else {
				if (board->entitiesToDelete[i]->gX + r < board->entitiesToDelete[i]->posX)
					board->entitiesToDelete[i]->gX = board->entitiesToDelete[i]->posX;
				else
					board->entitiesToDelete[i]->gX += r;
			}
		} else if (board->entitiesToDelete[i]->posY != board->entitiesToDelete[i]->gY) {
			double r = (board->entitiesToDelete[i]->posY - board->entitiesToDelete[i]->lastY) * (delta / animationDuration);
			if (board->entitiesToDelete[i]->gY < board->entitiesToDelete[i]->posY) {
				if (board->entitiesToDelete[i]->gY + r > board->entitiesToDelete[i]->posY)
					board->entitiesToDelete[i]->gY = board->entitiesToDelete[i]->posY;
				else
					board->entitiesToDelete[i]->gY += r;
			} else {
				if (board->entitiesToDelete[i]->gY + r < board->entitiesToDelete[i]->posY)
					board->entitiesToDelete[i]->gY = board->entitiesToDelete[i]->posY;
				else
					board->entitiesToDelete[i]->gY += r;
			}
		}
	}
}