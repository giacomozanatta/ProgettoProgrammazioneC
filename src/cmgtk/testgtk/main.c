
int main() 
{
    
    initscr();
    WINDOW *finestra =newwin(40,40,10,10);
    box(finestra, '_', '|');
    curs_set(0);
    noecho();
    timeout(500);
}