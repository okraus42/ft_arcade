#ifndef SNAKE_CLIENT_H
#define SNAKE_CLIENT_H
#pragma once

#define FOOD_COLOR		 "\033[38:5:231m"
#define BACKGROUND_COLOR "\033[48:5:16m"
#define SNAKE1_COLOR	 "\033[38:5:33m"
#define SNAKE2_COLOR	 "\033[38:5:28m"
#define SNAKE3_COLOR	 "\033[38:5:214m"
#define SNAKE4_COLOR	 "\033[38:5:219m"
#define SNAKE5_COLOR	 "\033[38:5:51m"
#define SNAKE6_COLOR	 "\033[38:5:46m"
#define SNAKE7_COLOR	 "\033[38:5:226m"
#define SNAKE8_COLOR	 "\033[38:5:207m"
#define COLOR_RESET		 "\033[0m"

#define SNAKE_HEAD "██"
#define SNAKE_BODY "▓▓"
#define SNAKE_TAIL "▒▒"
#define FOOD	   "░░"

#define SCREEN_WIDTH  80
#define SCREEN_HEIGHT 8

// ANSI Colours and Borders
#define BORDER_COLOUR	 "\033[30:107m"
#define HIGHLIGHT_COLOUR "\033[1:38:5:254:48:5:233m"

#define CLEAR_SCREEN	 "\033[2J\033[3J\033[H"
#define HIDE_CURSOR		 "\033[?25l"
#define SHOW_CURSOR		 "\033[?25h"

#define ENTER_ALT_SCREEN "\033[?1049h"
#define EXIT_ALT_SCREEN	 "\033[?1049l"

#endif /*SNAKE_CLIENT_H*/
