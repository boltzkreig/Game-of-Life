#include <locale.h>
#include <ncurses.h>
#include <unistd.h>
#include <random>
#include <cstring>

int n, m, sum, resx, resy, 
	delay = 83'333;
float r = 0.5;
char display[] = "\u2588";

bool buff[150][600] ={0} ;
bool buf2[600] ={0} ;
WINDOW * win;

int mod(int a, int b) {
    return a >= 0 ? a % b : ((a % b) + b);
}
void print() {
    int iter = 0;
    for (int i = 0; i < m; i++) {
        wmove(win, ++iter, 1);
        for (int j = 0; j < n; j++) {
           buff[i][j] ? wprintw(win, display) : wprintw(win, " ");
        }
    }
}
void update() {
    //DO for every cell
	for( int i = m-1; i > 0; i--){
	   for( int j = 0; j < n ; j++){
			buff[i][j] = buff[i-1][j];
	   }
	}	   

    for (int i = 0; i < n; i++) {
		int mask = 0;
		for (int j = -1; j < 2; j++){
			mask <<= 1;
			if ( buff[0][mod(i+j,n)] ) mask ++;
		}

		switch(mask){
			case 0:
				buf2[i] = 0;
				break;

			case 1:
				buf2[i] = 1;
				break;

			case 2:
				buf2[i] = 1;
				break;

			case 3:
				buf2[i] = 1;
				break;

			case 4:
				buf2[i] = 0;
				break;

			case 5:
				buf2[i] = 1;
				break;

			case 6:
				buf2[i] = 1;
				break;

			case 7:
				buf2[i] = 0;
				break;
		}

	}

	for (int i = 0; i < n; i++) {
            buff[0][i] = buf2[i];
    }
}

int main(int argc, char * argv[]) {
    //Variable relations: (m,n) -> (x,y) -> (j,i)
	if(argc > 1){r = strtod(argv[1], NULL);}
	if(argc > 2){delay = (int)(1'000'000/strtod(argv[2], NULL));}
   	if(argc > 3){strcpy(display, argv[3]);}

    std::random_device rd;
    //std::mt19937_64 gen(rd());
    std::ranlux48_base gen(rd());
    std::bernoulli_distribution d(r);

    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    curs_set(0);

    getmaxyx(stdscr, resy, resx);
    n = resx - 4;
    m = resy - 4;

    for (int i = 0; i < n; i++) {
            buff[0][i] = d(gen);
    }

    printw("resolution is %d X %d", resy, resx);
    win = newwin(m + 2, n + 2, (resy / 2) - (m / 2), (resx / 2) - (n / 2));
    refresh();
    box(win, 0, 0);

    while (1) {
        print();
        update();
        wrefresh(win);
        usleep(delay);
    }

    refresh();
    //getch();
    endwin();
    return 0;
}
