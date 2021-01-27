#include <ncurses.h>
#include <time.h>
#include <stdlib.h>

char arr[9] =
{
    ' ', ' ', ' ',
    ' ', ' ', ' ',
    ' ', ' ', ' '
};

int selected = 4;
char lastx = 0;

int x = 0;
int o = 0;

int mode;
char me;

void show(int yoff, int xoff)
{    
    mvprintw(yoff, xoff, ".---.---.---.");
    for (unsigned int i = 0; i < 9; i++)
    {
	mvprintw(1 + yoff + i/3, xoff + i % 3 * 4, "|");
	if (i == selected)
	    attron(A_REVERSE);
	printw(" %c ", arr[i]);
	if (i == selected)
	    attroff(A_REVERSE);
	printw("|");
    }
    mvprintw(yoff + 4, xoff, "'---'---'---'");

    mvprintw(yoff + 2, xoff / 2 - 3, "X: %3d", x);
    mvprintw(yoff + 2, xoff + xoff / 2 - 3, "O: %3d", o);

    if (mode == 0)
    {
	mvprintw(yoff - 2, xoff + 6, "%c", me);
    }
    else if (mode == 1)
    {
	mvprintw(yoff - 2, xoff + 6, "%c", 'O' * lastx + 'X' * !lastx);
    }
}

int select_mode()
{
    clear();
    int xoff = (COLS - 12) / 2;
    int yoff = (LINES - 4) / 2;

    mvprintw(yoff, xoff, "Select  mode");

    
    int m = 0;


    while (1)
    {
	
	if (m == 0) attron(A_REVERSE);
	mvprintw(yoff + 2, xoff - 19, "Player vs Computer");
	if (m == 0) attroff(A_REVERSE);
	
	if (m == 1) attron(A_REVERSE);
	mvprintw(yoff + 2, xoff + 13, "Player vs Player");
	if (m == 1) attroff(A_REVERSE);

	int c = getch();

	if (c == KEY_LEFT)
	    m = 0;
	if (c == KEY_RIGHT)
	    m = 1;
	if (c == ' ' || c == '\n')
	    break;
    }
    clear();
    return m;
}

int detect_win(int yoff, int xoff, int steps, int _lastx)
{
    for (unsigned int l = 'X'; l == 'X' || l == 'O'; l = 'O' * (l == 'X'))
    {
	char wins = 0;
	if (arr[0] == l && arr[4] == l && arr[8] == l)
	{
	    arr[0] = '\\';
	    show(yoff, xoff);
	    refresh();

	    napms(500);
	    arr[4] = '\\';
	    show(yoff, xoff);
	    refresh();

	    napms(500);
	    arr[8] = '\\';
	    show(yoff, xoff);
	    refresh();

	    wins = 1;
	}
	if (!wins && arr[2] == l && arr[4] == l && arr[6] == l)
	{
	    arr[6] = '/';
	    show(yoff, xoff);
	    refresh();

	    napms(500);
	    arr[4] = '/';
	    show(yoff, xoff);
	    refresh();

	    napms(500);
	    arr[2] = '/';
	    show(yoff, xoff);
	    refresh();

	    wins = 1;
	}

	for (unsigned int i = 0; i < 3; i++)
	{
	    if (wins)
		break;
	    if (arr[i] == l && arr[i+3] == l && arr[i+6] == l)
	    {
		arr[i] = '|';
		show(yoff, xoff);
		refresh();
			
		napms(500);
		arr[i+3] = '|';
		show(yoff, xoff);
		refresh();
		    
		napms(500);
		arr[i+6] = '|';
		show(yoff, xoff);
		refresh();
		wins = 1;
	    }
	}

	for (unsigned int i = 0; i < 3; i++)
	{
	    if (wins)
		break;
	    if (arr[i*3] == l && arr[i*3+1] == l && arr[i*3+2] == l)
	    {
		arr[i*3] = '-';
		show(yoff, xoff);
		refresh();
		    
		napms(500);
		arr[i*3+1] = '-';
		show(yoff, xoff);
		refresh();
		    
		napms(500);
		arr[i*3+2] = '-';
		show(yoff, xoff);
		refresh();
		wins = 1;
	    }
	}

	if (wins)
	{
	    selected = -1;
	    for (unsigned int i = 0; i < 9; i++)
	    {
		arr[i] = '#';
		show(yoff, xoff);
		mvprintw(yoff + 6, xoff, "   %c wins!", l);
		refresh();
		napms(500);
	    }
	    selected = 4;

	    for (unsigned int i = 0; i < 9; i++)
		arr[i] = ' ';
	    clear();

	    if (l == 'X')
	    {
		x++;
		lastx = 0;
	    }
	    else if (l == 'O')
	    {
		o++;
		lastx = 1;
	    }

	    nodelay(stdscr, TRUE);

	    while (getch() != ERR);

	    nodelay(stdscr, FALSE);

	    steps = 0;
		
	    return 1;
	}
    }

	
    int draw = 1;
    for (unsigned int i = 0; i < 9; i++)
    {
	if (arr[i] == ' ')
	{
	    draw = 0;
	    break;
	}
    }
	
	
    if (draw)
    {
	selected = -1;
	for (unsigned int i = 0; i < 9; i++)
	{
	    arr[i] = '#';
	    show(yoff, xoff);
	    mvprintw(yoff + 6, xoff, " It's a draw");
	    refresh();
	    napms(500);
	}
	selected = 4;
	    
	for (unsigned int i = 0; i < 9; i++)
	    arr[i] = ' ';
	clear();

	lastx = _lastx;
	    
	nodelay(stdscr, TRUE);
	    
	while (getch() != ERR);
	    
	nodelay(stdscr, FALSE);

	steps = 0;
	return 1;
    }

    return 0;
}

char select_letter()
{
    clear();
    int xoff = (COLS - 14) / 2;
    int yoff = (LINES - 4) / 2;

    mvprintw(yoff, xoff, "Select  letter");

    
    int letter = 'X';


    while (1)
    {
	
	if (letter == 'X') attron(A_REVERSE);
	mvprintw(yoff + 2, xoff - 3, " X ");
	if (letter == 'X') attroff(A_REVERSE);
	
	if (letter == 'O') attron(A_REVERSE);
	mvprintw(yoff + 2, xoff + 14, " O ");
	if (letter == 'O') attroff(A_REVERSE);

	int c = getch();

	if (c == KEY_LEFT)
	    letter = 'X';
	if (c == KEY_RIGHT)
	    letter = 'O';
	if (c == ' ' || c == '\n')
	    break;
    }
    clear();
    return letter;
}

int main()
{
    initscr();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    mode = select_mode();
    
    if (mode == 0)
    {
	me = select_letter();
    }

    nodelay(stdscr, TRUE);

    srand(time(0));

    char xdid = 0;
    
    int _continue = 1;
    char _lastx = lastx;
    unsigned int steps = 0;
    unsigned int last_steps = me == 'O';
    while (_continue)
    {
	int xoff = (COLS - 13) / 2;
	int yoff = (LINES - 7) / 2;
	
        
	
        
	if (mode == 0 && me == 'O' && steps != last_steps)
	{
	    last_steps = steps;
	    int len = 0;
	    for (unsigned int i = 0; i < 9; i++)
	    {
		len += arr[i] == ' ';
	    }
	    
	    int availables[len];

	    unsigned int j = 0;
	    for (unsigned int i = 0; i < 9; i++)
	    {
		if (arr[i] == ' ')
		    availables[j++] = i;
	    }

	    arr[availables[rand() % len]] = 'X';
	    
	}

	show(yoff, xoff);
	refresh();


	


	int c = getch();

	
	switch (c)
	{
	case KEY_LEFT:
	    if (selected % 3 >= 1)
		selected--;
	    break;
	case KEY_RIGHT:
	    if (selected % 3 <= 1)
		selected++;
	    break;
	case KEY_UP:
	    if (selected >= 3)
		selected -= 3;
	    break;
	case KEY_DOWN:
	    if (selected <= 5)
		selected += 3;
	    break;
	case '\n':
	case ' ':
	    if (mode == 0)
	    {
		if (arr[selected] == ' ')
		{
		    arr[selected] = me;
		    steps++;
		    xdid = 0;
		}
	    }
	    else if (mode == 1)
	    {
		if (arr[selected] == ' ')
		{
		    arr[selected] = 'X' * !lastx + 'O' * lastx;
		    lastx = !lastx;
		}
	    }
	    break;
	}

	show(yoff, xoff);
	refresh();
	
	if (detect_win(yoff, xoff, steps, _lastx))
	{
	    steps = 0;
	    last_steps = me == 'O';
	    continue;
	}

	if (mode == 0 && me == 'X' && steps != last_steps)
	{
	    last_steps = steps;
	    int len = 0;
	    for (unsigned int i = 0; i < 9; i++)
	    {
		len += arr[i] == ' ';
	    }
	    
	    int availables[len];

	    unsigned int j = 0;
	    for (unsigned int i = 0; i < 9; i++)
	    {
		if (arr[i] == ' ')
		    availables[j++] = i;
	    }

	    arr[availables[rand() % len]] = 'O';
	    
	}

	show(yoff, xoff);
	refresh();
	

	if(detect_win(yoff, xoff, steps, _lastx))
	{
	    steps = 0;
	    last_steps = me == 'O';
	    continue;
	}

    }
    
    
    endwin();
    return 0;
}
