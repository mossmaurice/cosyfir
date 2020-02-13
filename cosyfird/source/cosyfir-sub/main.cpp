#include <ncurses.h>
#include <mosquittopp.h>


int main()
{
    initscr();
    printw("Connecting to TTN server..");
    refresh();

    // Connect to TTN server

    getch();
    endwin();

    return 0;
}
