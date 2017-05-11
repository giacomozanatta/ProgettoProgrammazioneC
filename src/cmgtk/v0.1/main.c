#include <ncurses.h>
static void screen_init(int a[][N], struct values dim, struct values *fixed_space)
{
    int rows, cols;
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_BLUE, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(6, COLOR_CYAN, COLOR_BLACK);
    raw();
    noecho();
    /* Magic numbers explanation:
     * (dim.a - 6) : 6 -> 4 lines of borders + 2 lines of score WIN
     * (dim.b - 2) : only 2 lines of borders
     * rows and cols -> + 3 : real dimensions of the field subwin,
     * calculated as N - 1 spaces between N elements of the int (both vertical
     * and horizontal, multiplied fixed_space (either vertical or horizontal)
     * + 3: 2 for the borders and 1 for the first elem
     Graphicallly explained: |O O O O O| -> 5 elements, with a total number
     of 4 spaces between them, plus 2 for the borders, plus 1 for the first elem. */
    fixed_space->a = (dim.a - 6) / N;
    fixed_space->b = (dim.b - 2) / N;
    rows = ((N - 1) * fixed_space->a) + 3;
    cols = ((N - 1) * fixed_space->b) + 3;
    /* create sub windows centered */
    field = subwin(stdscr, rows, cols, (dim.a - 4 - rows) / 2, (dim.b - cols) / 2);
    score = subwin(stdscr, 2 + 2, dim.b, dim.a - 4, 0);
    keypad(field, TRUE);
    wborder(field, '|', '|', '-', '-', '+', '+', '+', '+');
    wborder(score, '|', '|', '-', '-', '+', '+', '+', '+');
    mvwprintw(score, 2, 1, "Enter to put a bomb (*). Space to uncover. q anytime to *rage* quit.");
    mvwprintw(score, 1, 1, "Still %d bombs.", ps.bombs);
    wrefresh(score);
}


int main(){
	screeninit()
	initscr();



}