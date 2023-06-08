all:
	g++ Code_GoL.cpp -lncursesw -std=c++20 -O3 -o Game-of-Life
	g++ General-Moore-Neighbour.cpp -lncursesw -std=c++20 -O3 -o Birth-Survive
	g++ Greenberg-Hastings.cpp -lncursesw -std=c++20 -O3 -o Waves
	g++ Code_110.cpp -lncursesw -std=c++20 -O3 -o Rule-110
	g++ Wolfram.cpp -lncursesw -std=c++20 -O3 -o Carpet
