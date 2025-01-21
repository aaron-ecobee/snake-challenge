#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <unistd.h>
#include <termios.h>
#include <sys/select.h>
#endif

// The challenge here is to make the traditional "snake" game (aka nibbles) where a snake
// travels around (always moving), and can be turned using keys (of your choosing). There will
// be a certain time allotted for the challenge, but there is no expectation to finish
// the entire project within the alotted time. Instead, the expectation is to approach the
// development process iteratively, gradually increasing the complexity of the game within
// the alotted time period.
//
// The developer is allowed to develop in c or c++ (whichever they prefer). Some helper
// functions are given in c below to allow for both c and c++ support.
// 
// The Rules:
//
// Stage 1:
// 	- The snake is always in motion. Keys are used to change its direction.
// 	- The snake traverses a 2-D grid.
// 	- The snake is of size 1.
//  - The game is over if the snake goes out-of-bounds
//	
// Stage 2:
// 	- The snake grows in length by size 1 whenever it eats food.
// 	- The food randomly appears on the grid (except where the snake is located).
// 	- The snake cannot touch itself, otherwise the game is over.

// Stage 3:
// 	- After a period of time, obstacles start appearing on the board at fixed locations and specified intervals. 
// 	- The obstacles are contiguous, linear, and of random length (up to 5).
// 	- When the snake hits and obstable it immediately bounces back and dies.

// Stage 4:
// 	- After a period of time, some of the obstacles turn into snakes themselves and start moving around the board.

// gcc build command: "gcc main.c -o snake_game"
// or
// g++ build command: "gcc main.c -o snake_game"

// Cross platform sleep function
void sleep_seconds(int seconds) {
#ifdef _WIN32
    Sleep(seconds * 1000);  // Windows Sleep takes milliseconds
#else
    sleep(seconds);
#endif
}

static void print_matrix(const char* map, size_t width, size_t height);

// Returns -1 in timeout scenario
char getchar_with_timeout(int timeout_seconds);


int main(int argc, char* argv[])
{
	// TOOD: starting point, 3, 2, 1, Go!
	return 0;
}


static void print_matrix(const char* map, size_t width, size_t height)
{
	printf("\033c");

	for (size_t i = 0; i < height; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			printf("%c", map[i*height + j]);
		}

		printf("\n");
	}
}

#ifdef _WIN32

char getchar_with_timeout(int timeout_seconds) {
	int elapsed = 0;
	while (elapsed < timeout_seconds) {
		if (_kbhit()) {   // Check if a key is pressed
			return _getch();  // Return the character if a key is pressed
		}

		// Sleep for 1 second
		sleep_seconds(1);

		elapsed++;
	}
	return -1;  // Return -1 if timeout reached without any key press
}

#else   // Linux, Mac

char getchar_with_timeout(int timeout_seconds) {
	struct termios oldt, newt;
	char ch;

	// Get the current terminal settings
	tcgetattr(STDIN_FILENO, &oldt);    // Save the current terminal settings
	newt = oldt;                       // Copy current settings to modify
	newt.c_lflag &= ~(ICANON | ECHO);   // Disable canonical mode and echo
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);  // Apply the new settings

											  // Set up the timeout for select()
	fd_set readfds;
	struct timeval timeout;
	FD_ZERO(&readfds);  // Clear the fd_set
	FD_SET(STDIN_FILENO, &readfds);  // Add stdin (standard input) to the fd_set

	timeout.tv_sec = timeout_seconds;   // Timeout in seconds
	timeout.tv_usec = 0;                // Timeout in microseconds

										// Use select to wait for input or timeout
	int result = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &timeout);

	if (result > 0) {
		// Input is ready, read a character
		ch = getchar();
	}
	else if (result == 0) {
		// Timeout occurred
		ch = -1;  // Indicate timeout by returning -1
	}
	else {
		// Error
		perror("select");
		ch = -1;
	}

	// Restore the terminal settings
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

	return ch;
}

#endif

