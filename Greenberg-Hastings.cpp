#include <locale.h>
#include <ncurses.h>
#include <unistd.h>
#include <random>
#include <cstring>

#define LIVE 1
#define DEAD 0

int n, m, sum, resx, resy, 
	factor = 50,
	delay = 100'000,
	rest = 16;
float r = 0.001;
char display[] = "\u2588";
char hold[20];

auto buff = new int[150][300];
auto buf2 = new int[150][300];
auto neig = new bool[150][300];
WINDOW * win;

    std::random_device rd;
    //std::mt19937_64 gen(rd());
    std::ranlux48_base gen(rd());

int mod(int a, int b) {
    return a >= 0 ? a % b : ((a % b) + b);
}
void print() {
    int iter = 0;
    for (int i = 0; i < n; i++) {
        wmove(win, ++iter, 1);
        for (int j = 0; j < m; j++) {
			switch(buff[i][j]){
				case DEAD:
					wprintw(win, ".");
					break;

				case LIVE:
					wprintw(win, display);
					break;

				default:
					wprintw(win, " ");
					break;
			}
        }
    }
}
void update() ;
int main(int argc, char * argv[]) {
    //Variable relations: (m,n) -> (x,y) -> (j,i)
	if((argc > 1) && !strcmp(strcpy(hold,argv[1]),"-h")){
		printf("green [Initial_Probability(0.001)] [Rest_Period(16)] [FPS(10)] [Probability_Diminish_Factor(50)] [Symbol(\u2588)]\n") ;
		exit(0);
	}
	if(argc > 1){r = strtod(argv[1], NULL);}
	if(argc > 2){rest = std::stoi(argv[2], NULL);}
	if(argc > 3){delay = (int)(1'000'000/strtod(argv[3], NULL));}
	if(argc > 4){factor = std::stoi(argv[4], NULL);}
   	if(argc > 5){strcpy(display, argv[5]);}


    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    curs_set(0);

    std::bernoulli_distribution d(r);
    getmaxyx(stdscr, resy, resx);
    m = resx - 4;
    n = resy - 4;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            buff[i][j] = d(gen);
        }
    }

    printw("resolution is %d X %d", resy, resx);
    win = newwin(n + 2, m + 2, (resy / 2) - (n / 2), (resx / 2) - (m / 2));
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
void update() {
    std::bernoulli_distribution dd(r/factor);
    //DO for every cell
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            //Sum the neighbours
			neig[i][j] = DEAD;
            for (int k = -1; k < 2; k+=2) {
				if(buff[mod(i+k,n)][mod(j,m)] == LIVE){
					neig[i][j] = LIVE;
					break;
				}
            }
            for (int l = -1; l < 2; l+=2) {
				if(buff[mod(i,n)][mod(j+l,m)] == LIVE){
					neig[i][j] = LIVE;
					break;
				}
            }
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
			if (buff[i][j] == LIVE){buf2[i][j] = 2;}
			if (buff[i][j] ==  rest){buf2[i][j] = DEAD;}
			//if (buff[i][j] ==  rest && neig[i][j] == DEAD){buf2[i][j] = d(gen); }
			else if(buff[i][j] > 1 && buff[i][j] < rest){buf2[i][j]++;}
            if (buff[i][j] == DEAD && neig[i][j] == LIVE) buf2[i][j] = LIVE;
			else if (buff[i][j] ==  DEAD && neig[i][j] == DEAD){buf2[i][j] = dd(gen); }
        }
    }
	buff = buf2;
}

