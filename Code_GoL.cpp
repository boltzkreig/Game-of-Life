#include <locale.h>
#include <ncurses.h>
#include <unistd.h>
#include <random>
#include <cstring>

int n, m, sum, resx, resy, 
	delay = 83'333;
float r = 0.5;
char display[] = "\u2588";
char hold[20];

auto buff = new bool[150][300];
auto buf2 = new bool[150][300];
auto neig = new int [150][300];
WINDOW * win;

int mod(int a, int b) {
    return a >= 0 ? a % b : ((a % b) + b);
}
void print() {
    int iter = 0;
    for (int i = 0; i < n; i++) {
        wmove(win, ++iter, 1);
        for (int j = 0; j < m; j++) {
           buff[i][j] ? wprintw(win, display) : wprintw(win, " ");
        }
    }
}
void update() {
    //DO for every cell
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            //Sum the neighbours
            sum = -buff[i][j];
            for (int k = -1; k < 2; k++) {
                for (int l = -1; l < 2; l++) {
                    sum += buff[mod(i + k, n)][mod(j + l, m)];
                }
            }
            neig[i][j] = sum;
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (neig[i][j] < 2 || neig[i][j] > 3 ) buf2[i][j] = 0;
            if (neig[i][j] == 2 && buff[i][j] == 1) buf2[i][j] = 1;
           if (neig[i][j] == 3) buf2[i][j] = 1;
        }
    }
	buff = buf2;
}

int main(int argc, char * argv[]) {
    //Variable relations: (m,n) -> (x,y) -> (j,i)
	if((argc > 1) && !strcmp(strcpy(hold,argv[1]),"-h")){
		printf("Game-of-Life [Initial_Probability(0.5)] [Rest_Period(16)] [FPS(10)] [Probability_Diminish_Factor(50)] [Symbol(\u2588)]\n") ;
		exit(0);
	}
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
