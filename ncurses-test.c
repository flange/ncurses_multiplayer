#include <errno.h>
#include <inttypes.h>
#include <ncurses.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PLAYER1 1
#define PLAYER2 2
#define PLAYER_CLEAR 0


void die(const char *msg)
{
	endwin();

	if (msg) {
		if (errno)
			perror(msg);
		else
			printf("%s\n", msg);

		fflush(stdout);
	}

	exit(EXIT_FAILURE);
}

void ncurses_init(void)
{
	WINDOW *stdscr = initscr();
	if (!stdscr)
		die("init: initscr() failed");

	if (has_colors() == FALSE)
		die("init: terminal doesn't support colors");

	if (cbreak() == ERR)
		die("init: cbreak() failed");

	if (noecho() == ERR)
		die("init: noecho() failed");

	if (keypad(stdscr, TRUE) == ERR)
		die("init: keypad() failed");

	if (start_color() == ERR)
		die("init: start_color() failed");

	if (init_pair(1, COLOR_BLACK, COLOR_WHITE) == ERR)
		die("init: init_pair(1) failed");
}

WINDOW *create_world(const uint8_t height,
                     const uint8_t width,
                     const uint8_t startx, /* top left corner x */
                     const uint8_t starty) /* top left corner y */
{
	WINDOW *w = newwin(height, width, startx, starty);
	if (!w)
		die("create_world: Couldn't create window");

	if (wbkgd(w, COLOR_PAIR(1)) == ERR)
		die("create_world: Couldn't set world color");

	return w;
}

void draw_player(const uint8_t player_id,
                 WINDOW * const w,
                 const uint8_t x,
                 const uint8_t y)
{
	if (!w)
		die("draw_player: Window pointer was NULL");

	switch(player_id) {
	case PLAYER1:
		mvwaddch(w, y, x, '#');
		break;
	case PLAYER2:
		mvwaddch(w, y, x, '@');
		break;
	case PLAYER_CLEAR:
		mvwaddch(w, y, x, ' ');
		break;
	}

	if (refresh() == ERR)
		die("draw_player: Couldn't refresh stdscr");

	if (wrefresh(w) == ERR)
		die("draw_player: Couldn't refresh world");
}

void handle_input(const uint16_t input,
		  const WINDOW * const world,
		  uint8_t * const x,
		  uint8_t * const y)
{
	uint32_t max_x = world->_maxx; /* == width  - 1 */
	uint32_t max_y = world->_maxy; /* == height - 1 */

	switch(input) {
	case KEY_UP:
		*y > 0 ? (*y)-- : (*y = max_y);
		break;
	case KEY_DOWN:
		*y = ((*y) + 1) % (max_y + 1);
		break;
	case KEY_LEFT:
		*x > 0 ? (*x)-- : (*x = max_x);
		break;
	case KEY_RIGHT:
		*x = ((*x) + 1) % (max_x + 1);
		break;
	case 'q':
		die(NULL);
	}
}

int main(void)
{
	ncurses_init();

	printw("'q' to quit\n");

	WINDOW *world = create_world(10, 20, 5, 10);
	draw_player(PLAYER1, world, 0, 0);

	uint16_t input;
	uint8_t x = 0;
	uint8_t y = 0;


	while (1) {
		input = getch();
		draw_player(PLAYER_CLEAR, world, x, y);
		handle_input(input, world, &x, &y);
		draw_player(PLAYER1, world, x, y);
	}


	endwin();

	return 0;
}







