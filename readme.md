Game of Life
===================================
**An Implemention of Conway's Game of Life using `ncurses` written in C++**

***INSTALLATION***

Clone the Repository:

    git clone https://github.com/boltzkreig/Game-of-Life
    
Change working directory to `Game-of-Life`

     cd Game-of-Life
     
Run make (Make sure ncurses library, GCC and Makefile are installed) 

     make
	 
Run the `Game-of-Life` executable. Optional argument can be used

	./Game-of-Life [Probability] [FPS] [Alive] 
     
- Probability	= Initial Alive/Dead Ratio		( 0.5 )
- FPS		= Frame per Second			( 12 )
- Alive		= Character to Display Alive Cells	( █ )

***NOTE***
1. Design for Unix-like system.
2. Requires C++20
3. The world size is defined by ternimal-window size. 100x300 is hard-coded, in case of larger screens repeation of cell occures. Increse as per requirement.
4. Use Ctrl-C to Exit; Ctrl-S/Ctrl-Q to Pause/Unpause
