#include "defines.h"

//use t_char
static void printDigit(t_game* g, t_char d)
{
	for (uint32_t i = 0U; i < DIGIT_HEIGHT; i++)
	{
		for (uint32_t j = 0U; j < DIGIT_WIDTH; j++)
		{
			if (g->digit[d.c][i][j])
				g->screen[(d.pos_y + i) * g->screen_w + d.pos_x + j] = d.colour;
			else if (d.background)
				g->screen[(d.pos_y + i) * g->screen_w + d.pos_x + j] = d.background;
		}
	}
}

static void printPlayerScore(t_game* g, unsigned int player, int start_x, int start_y)
{
	t_char d;
	t_char e;

	d.colour = CLR_WHITE;
	d.background = CLR_TRANSPARENT;
	d.pos_x = start_x;
	d.pos_y = start_y;

	e = d;
	e.colour = CLR_GRAY_3;
	e.c = 8;

	int score = g->tetris[player].score;
	for (int i = 10000; i >= 1; i /= 10)
	{
		d.c = (score / i) % 10;
		printDigit(g, e);
		printDigit(g, d);
		d.pos_x += 32;
		e.pos_x += 32;
	}
	d.pos_x = start_x + 64;
	d.pos_y = start_y + 72;

	e = d;
	e.colour = CLR_GRAY_3;
	e.c = 8;
	int level = g->tetris[player].level;
	for (int i = 100; i >= 1; i /= 10)
	{
		d.c = (level / i) % 10;
		printDigit(g, e);
		printDigit(g, d);
		d.pos_x += 32;
		e.pos_x += 32;
	}
	d.pos_x = start_x + 32;
	d.pos_y = start_y + 72 * 2;

	e = d;
	e.colour = CLR_GRAY_3;
	e.c = 8;
	int lines = g->tetris[player].lines;
	for (int i = 1000; i >= 1; i /= 10)
	{
		d.c = (lines / i) % 10;
		printDigit(g, e);
		printDigit(g, d);
		d.pos_x += 32;
		e.pos_x += 32;
	}
}

void printScore(t_game* g)
{
	printPlayerScore(g, PLAYER_1, g->offset_x0 - 256, g->offset_y0);
	printPlayerScore(g, PLAYER_2, g->offset_x1 + 96 + BOARD_WIDTH * SQUARE_SIZE, g->offset_y1);
}

static void drawHorizontalSegment(uint8_t digit[DIGIT_HEIGHT][DIGIT_WIDTH], uint8_t y)
{
	for (uint8_t i = 0; i < DIGIT_THICKNESS; i++)
	{
		for (uint8_t j = 2; j < DIGIT_WIDTH - 2; j++)
		{
			digit[y + i][j] = 1;
		}
	}
}

static void drawVerticalSegment(uint8_t digit[DIGIT_HEIGHT][DIGIT_WIDTH], uint8_t x, uint8_t yStart, uint8_t yEnd)
{
	for (uint8_t i = 0; i < DIGIT_THICKNESS; i++)
	{
		for (uint8_t j = yStart; j < yEnd; j++)
		{
			digit[j][x + i] = 1;
		}
	}
}

void generateDigits(t_game* g)
{
	uint8_t num = 0;

	while (num < 10)
	{
		// Define which segments should be filled for each digit
		int segments[10][7] = {
			{1, 1, 1, 0, 1, 1, 1}, // 0
			{0, 1, 0, 0, 1, 0, 0}, // 1
			{1, 1, 0, 1, 0, 1, 1}, // 2
			{1, 1, 0, 1, 1, 0, 1}, // 3
			{0, 1, 1, 1, 1, 0, 0}, // 4
			{1, 0, 1, 1, 1, 0, 1}, // 5
			{1, 0, 1, 1, 1, 1, 1}, // 6
			{1, 1, 0, 0, 1, 0, 0}, // 7
			{1, 1, 1, 1, 1, 1, 1}, // 8
			{1, 1, 1, 1, 1, 0, 1}  // 9
		};

		int mid = DIGIT_HEIGHT / 2;
		int bottom = DIGIT_HEIGHT - DIGIT_THICKNESS - 2;

		if (segments[num][0])
			drawHorizontalSegment(g->digit[num], 2); // Top
		if (segments[num][1])
			drawVerticalSegment(g->digit[num], DIGIT_WIDTH - DIGIT_THICKNESS - 2, 2,
								mid); // Upper Right
		if (segments[num][2])
			drawVerticalSegment(g->digit[num], 2, 2, mid); // Upper Left
		if (segments[num][3])
			drawHorizontalSegment(g->digit[num], mid - DIGIT_THICKNESS / 2); // Middle
		if (segments[num][4])
			drawVerticalSegment(g->digit[num], DIGIT_WIDTH - DIGIT_THICKNESS - 2, mid,
								bottom); // Lower Right
		if (segments[num][5])
			drawVerticalSegment(g->digit[num], 2, mid, bottom); // Lower Left
		if (segments[num][6])
			drawHorizontalSegment(g->digit[num], bottom); // Bottom
		num++;
	}
}
