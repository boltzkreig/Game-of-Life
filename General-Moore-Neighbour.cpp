// Birth Survival Probability FPS Symbol
#include <locale.h>
#include <ncurses.h>
#include <unistd.h>
#include <random>
#include <cstring>
#include <vector>

int n, m, sum, resx, resy, 
	delay = 83'333;
float r = 0.07;
char display[] = "\u2588";
char hold[20];

std::vector<int> birth;
std::vector<int> survive;

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
			for (auto iter : survive){
				if(neig[i][j] == iter && buff[i][j] == 1){
				   	buf2[i][j] = 1;
				   	break;
				}else{
					buf2[i][j] = 0;
				}
			}
			for (auto iter : birth){
				if(neig[i][j] == iter && buff[i][j] == 0){ 
					buf2[i][j] = 1;
				   	break;
				}
			}
        }
    }
	buff = buf2;
}

int main(int argc, char * argv[]) {
    //Variable relations: (m,n) -> (x,y) -> (j,i)
	if((argc > 1) && !strcmp(strcpy(hold,argv[1]),"-h")){
		printf("gen [Birth(*)] [Survive(*)] [Initial_Probability(0.07)] [FPS(12)] [Symbol(\u2588)]\n") ;
		exit(0);
	}
if( argc > 2){
	int len_b = strlen(argv[1]);
	int len_s = strlen(argv[2]);
			
	for( int i=0; i < len_b; i++){
		birth.push_back((int)argv[1][i] - (int)'0');
	}
	for( int i=0; i < len_s; i++){
		survive.push_back((int)argv[2][i] - (int)'0');
	}
}else{
	survive.push_back(2); survive.push_back(3);
	birth.push_back(3);
}
	if(argc > 3){r = strtod(argv[3], NULL);}
	if(argc > 4){delay = (int)(1'000'000/strtod(argv[4], NULL));}
   	if(argc > 5){strcpy(display, argv[5]);}

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
