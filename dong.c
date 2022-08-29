#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <unistd.h>

const int SCREEN_WIDTH = 512;
const int SCREEN_HEIGHT = 512;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

const int maxX = 1024;
const int minX = 0;
const int maxY = 1024;
const int minY = 0;

int score1 = 0;
int score2 = 0;

int ballX = 100;
int ballY = 500;
int ballXvelo = 1;
int ballYvelo = 1;

int bumper1Y = 512;
int bumper1X = 96;
int bumper1velo = 0;
int bumperSize = 176;
int bumper2Y = 512;
int bumper2X = 936;
int bumper2velo = 0;

int main(int argc, char* args[])
 {	
	//initialize SDL video, window, and renderer

	if( SDL_Init( SDL_INIT_VIDEO ) < 0)
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		exit(1);
	}
		SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);
		if( window == NULL )
		{ printf("Window could not be created! SDL_Error: %s\n", SDL_GetError() );
		}
	SDL_SetRenderDrawColor(renderer,0,0,0,255);

	int close_requested = 0;
	//main loop
	printf("start \n");
	for ( ; close_requested == 0; ) {
	
	//handle events SDL
	SDL_Event event;
	while (SDL_PollEvent(&event))
{
	switch (event.type)
	{
		case SDL_QUIT:
			close_requested = 1;
			break;
		
		case SDL_KEYDOWN:
			printf("\nkey press detected\n");
			switch(event.key.keysym.scancode)
		{
		case SDL_SCANCODE_W:
			printf("bumper1velo = %d", bumper1velo);
			bumper1velo = -1;
			printf("bumper1velo = %d", bumper1velo);
			break;
			
		case SDL_SCANCODE_S:
			bumper1velo = 1;
			break;
			
		case SDL_SCANCODE_UP:
			bumper2velo = -1;
			break;
		
		case SDL_SCANCODE_DOWN:
			bumper2velo = 1;
			break;
		
		}
		break;
		
		case SDL_KEYUP:
			printf("\nKey release detected\n");
			switch(event.key.keysym.scancode)
		{
		case SDL_SCANCODE_W:
			bumper1velo = 0;
			break;
		case SDL_SCANCODE_S:
			bumper1velo = 0;
			break;
		case SDL_SCANCODE_UP:
			bumper2velo = 0;
			break;
		case SDL_SCANCODE_DOWN:
			bumper2velo = 0;
			break;
		}
		break;
	}
}
	bumper1Y += bumper1velo;
	bumper2Y += bumper2velo;
	ballY += ballYvelo;
	ballX += ballXvelo;
	checkBumper();
	checkToScore();
	checkYbound();
	printf("Y: %04d", ballY );	
	printf(" X: %04d\r", ballX );
	fflush(stdout);
	drawRectangles();
	SDL_Delay(2);
	}
atexit(SDL_Quit);
	
	// exit program and clean resources
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	
	return EXIT_SUCCESS;
 }

int bounce(int velo)
{
	return -velo;
}

void drawRectangles()
{
	//convert internal distance units to equivalent screen placements
	SDL_SetRenderDrawColor(renderer,0,0,0,255);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer,255,255,255,255);
	
	
	//define rectangles, increment number to draw
	//#rectanglesize is defined here, width and height are the 3rd and 4th fields in SDL_Rect
	SDL_Rect ballRect = {ballX / 2, ballY / 2, 10, 10};
	int toDraw = 1;
	
	SDL_Rect bumper1Rect = {(bumper1X / 2) - 10, bumper1Y / 2, 10, bumperSize / 2};
	toDraw++;
	SDL_Rect bumper2Rect = {(bumper2X / 2), bumper2Y / 2, 10, bumperSize / 2};
	toDraw++;
	//draw and fill the rectangles
	struct SDL_Rect rectArray[] = {ballRect, bumper1Rect, bumper2Rect};
	SDL_RenderFillRects(renderer, rectArray, toDraw);
	SDL_RenderPresent(renderer);
}

int checkToScore()
{
	if (ballX >= maxX)
	{
		 score1++;
		printf("p1 score\n");
		ballX = 512;
		ballY = rand() % 1024;
		ballXvelo = 1;
		ballYvelo = 1;
		SDL_Delay(1000);
		return 0;
	 }
	if (ballX <= minX)
	{
		score2++;
		printf("p2 score\n");
		SDL_Delay(1000);
		ballXvelo = 1;
		ballYvelo = 1;
		ballX = 512;
		ballY = rand() % 1024;
		return 0;
	}
	if (score1 >= 3 || score2 >= 3)
	{
		printf("\n Game Set! p1: %d, p2: %d \n", score1, score2);
			SDL_DestroyWindow(window);
	SDL_Quit();

	exit( EXIT_SUCCESS);
	}
return 0;
}

int checkYbound() //Y range of object to check
{
	
	//to see the definition of the rectangles drawn to screen
	//search #rectanglesize
	if (bumper1velo + bumper1Y + bumperSize > maxY)
	{
		bumper1Y--;	
	}
	if (bumper1velo + bumper1Y < minY)
	{
		bumper1Y++;
	}
	if (bumper2velo + bumper2Y + bumperSize > maxY)
	{
		bumper2Y--;	
	}
	if (bumper2velo + bumper2Y < minY)
	{
		bumper2Y++;
	}
	if (ballY + 20 > maxY || ballY < minY)
	{
		ballYvelo *= -1;
	}
return 0;	
}

int checkBumper()
{
/* detect collision when called, conditionally
 call bounce with the degree of deflection
as argument */
	
//to see the definition of the rectangles drawn to screen
//search #rectanglesize
	if(ballX == bumper1X && (ballY + 20 > bumper1Y && ballY < bumper1Y + bumperSize))
		{
			ballXvelo = bounce(ballXvelo);
			return 0;
		}
	if(ballX == bumper2X - 20 && (ballY + 20 > bumper2Y && ballY < bumper2Y + bumperSize))
		{
			ballXvelo = bounce(ballXvelo);
			return 0;
		}
return 0;
}

