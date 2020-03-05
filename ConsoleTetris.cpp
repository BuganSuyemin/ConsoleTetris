﻿#include <iostream>
#include <windows.h>
#include <time.h>
#include <conio.h>


using namespace std;

const int TILE_SIZE = 1, WIDTH = 10, HEIGHT = 13,
SCREEN_WIDTH = WIDTH * TILE_SIZE, SCREEN_HEIGHT = HEIGHT * TILE_SIZE;
bool screenBuffer[SCREEN_WIDTH * SCREEN_HEIGHT];
bool blocks[WIDTH][HEIGHT];
bool activeBlock[4][4];
bool configurations[7][4][4];
int activeX, activeY;
const int SLEEP_TICKS = 2;
short action;

void clearBuffer()
{
	for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
	{
		screenBuffer[i] = 0;
	}
}

void draw(int x, int y)
{
	if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT)
	{
		screenBuffer[y * SCREEN_WIDTH + x] = 1;
	}
}

void drawBlock(int x, int y)
{
	for (int i = 0; i < TILE_SIZE; i++)
		for (int j = 0; j < TILE_SIZE; j++)
			draw(x * TILE_SIZE + i, y * TILE_SIZE + j);
}

void actuallyDraw()
{
	for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
	{
		if (i % SCREEN_WIDTH == 0 && i != 0)
			cout << endl;
		char a = screenBuffer[i] ? '█' : ' ';
		cout << a;
	}
}

void rotateBlock(bool block[][4])
{
	bool t[4][4];

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			t[i][j] = block[j][3 - i];
		}
	}

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			block[i][j] = t[i][j];
		}
	}
}

void normalizeBlock(bool block[][4])
{
	while (1)
	{
		for (int i = 0; i < 4; i++)
			if (block[i][0])
				goto second;
		for (int j = 0; j < 3; j++)
			for (int i = 0; i < 4; i++)
				block[i][j] = block[i][j + 1];
		for (int i = 0; i < 4; i++)
			block[i][3] = 0;
	}
second:
	while (1)
	{
		for (int j = 0; j < 4; j++)
			if (block[0][j])
				goto outer;

		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 4; j++)
				block[i][j] = block[i + 1][j];
		for (int j = 0; j < 4; j++)
			block[3][j] = 0;
	}
outer:
	return;
}

void addToList(bool list[][4][4], int index, bool block[4][4])
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			list[index][i][j] = block[i][j];
}

void generatePossibleBlockConfigurations(bool list[4][4][4])
{
	bool a[4][4] = { 1, 1, 1, 1 };
	rotateBlock(a);
	normalizeBlock(a);
	addToList(list, 0, a);

	bool b[4][4] = { 1, 1, 0, 0, 1, 1 };
	rotateBlock(b);
	normalizeBlock(b);
	addToList(list, 1, b);

	bool c[4][4] = { 1, 1, 1, 0, 0, 1 };
	rotateBlock(c);
	normalizeBlock(c);
	addToList(list, 2, c);

	bool d[4][4] = { 1, 1, 1, 0, 0, 0, 1 };
	rotateBlock(d);
	normalizeBlock(d);
	addToList(list, 3, d);

	bool e[4][4] = { 1, 1, 1, 0, 1 };
	rotateBlock(e);
	normalizeBlock(e);
	addToList(list, 4, e);

	bool f[4][4] = { 0, 1, 1, 0, 1, 1 };
	rotateBlock(f);
	normalizeBlock(f);
	addToList(list, 5, f);

	bool g[4][4] = { 1, 1, 0, 0, 0, 1, 1 };
	rotateBlock(g);
	normalizeBlock(g);
	addToList(list, 6, g);
}

bool blockCantFall()
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if ((activeBlock[i][j] && j + 1 + activeY == HEIGHT) || (activeBlock[i][j] && blocks[i + activeX][j + activeY + 1]))
				return true;
	return false;
}

void handleFall()
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (activeBlock[i][j])
				blocks[i + activeX][j + activeY] = 1;

	activeX = 0;
	activeY = 0;
	int l = rand() % 7;

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			activeBlock[i][j] = configurations[l][i][j];
}

bool canMove(short dir)
{
	dir = dir == 1 ? -1 : (dir == 2 ? 1 : 0);
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (blocks[i + activeX + dir][j + activeY] && activeBlock[i][j]
				|| (activeBlock[i][j] && i + dir + activeX == WIDTH)
				|| (activeBlock[i][j] && i + dir + activeX == -1))
				return 0;
	return 1;
}

void clearLines()
{
	for (int j = 0; j < HEIGHT; j++)
	{
		bool f = true;
		for (int i = 0; i < WIDTH; i++)
			if (!blocks[i][j])
				f = false;
		if (f)
		{
			for (int i1 = 0; i1 < WIDTH; i1++)
			{
				for (int j1 = j; j1 > 0; j1--)
					blocks[i1][j1] = blocks[i1][j - 1];
				
				blocks[i1][0] = 0;
			}
		}
	}
}

int tick = 0;
void update()
{
	tick++;

	/*for (int l = 0; l < 7; l++)
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				if (configurations[l][i][j])
					drawBlock(i, 5*l + j);*/


	if (action == 1 && canMove(action))
		activeX--;
	if (action == 2 && canMove(action))
		activeX++;

	if (tick == SLEEP_TICKS)
	{
		tick = 1;

		if (blockCantFall())
		{
			handleFall();
			clearLines();
		}
		else
		{
			activeY++;
		}
	}


	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			activeBlock[i][j] ? draw(i + activeX, j + activeY) : (void)0;

	for (int i = 0; i < WIDTH; i++)
	{
		for (int j = 0; j < HEIGHT; j++)
		{
			if (blocks[i][j])
				drawBlock(i, j);
		}
	}
}


int main()
{
	generatePossibleBlockConfigurations(configurations);

	srand(time(NULL));
	int l = rand() % 7;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			activeBlock[i][j] = configurations[l][i][j];

	while (1)
	{
		system("CLS");
		clearBuffer();

		if (GetKeyState('A') & 0x8000)
			action = 1;
		if (GetKeyState('D') & 0x8000)
			action = 2;

		update();

		action = 0;
		actuallyDraw();
		Sleep(300);
	}
}