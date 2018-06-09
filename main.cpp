#include <iostream>
#include "SDL.h"

using namespace std;

const int screen_width = 800;
const int screen_height = 600;
int cpuscore = 0;
int playerscore = 0;

struct PaddleData
{
	double x;
	double y;
	double width;
	double height;
	double speed_y;
};

void init_playerpaddle(PaddleData *leftpaddle)
{
	leftpaddle->x = 10;
	leftpaddle->y = screen_height / 2 - 30;
	leftpaddle->width = 10;
	leftpaddle->height = 60;
	leftpaddle->speed_y = 15;
}

void init_cpupaddle(PaddleData *rightpaddle)
{
	rightpaddle->x = 780;
	rightpaddle->y = screen_height / 2 - 30;
	rightpaddle->width = 10;
	rightpaddle->height = 60;
	rightpaddle->speed_y = 1;
}


struct BallData
{
	double x;
	double y;
	double width;
	double height;
	double speed_y;
	double speed_x;
};

void init_ball(BallData *ball)
{
	ball->x = screen_width / 2;
	ball->y = screen_height / 2;
	ball->width = 10;
	ball->height = 10;
	ball->speed_y = 0.01;
	ball->speed_x = 0.01;
}

void Ballupdate(BallData *balldata)
{
	balldata->x -= balldata->speed_x;
	balldata->y -= balldata->speed_y;

	if (balldata->y + balldata->height >= screen_height  || balldata->y < 0)
	{
		balldata->speed_y*= -1 ;
	}

	if (balldata->x + balldata->width >= screen_width)
	{
		//balldata->speed_x *= -1;
		balldata->x = screen_width / 2;
		balldata->y = screen_height / 2;
		playerscore++;
		cout << "Player Has scored " << playerscore <<"\n";
	}
	if (balldata->x < 0)
	{
		balldata->x = screen_width / 2;
		balldata->y = screen_height / 2;
		cpuscore++;
		cout << "Computer Has scored " << cpuscore << "\n";
		balldata->speed_x = -0.01;
		balldata->speed_y = -0.01;
	}
	
}

void CPU_Paddle_AI(BallData *ball, PaddleData *paddle)
{
	if (ball->x < screen_width * 3 / 3 && ball->speed_x < 0)
	{
		// Follow the ball
		if (ball->y < paddle->y + paddle->height)
		{
			 
			paddle->y -= paddle->speed_y;

			if (paddle->y < 0)
			{
			   paddle->y = 0;
			}
		}
	    if (ball->y > paddle->y)
		{
			paddle->y += paddle->speed_y;
			if (paddle->y + paddle->height > 600)
			{
				paddle->y = 590;
			}
		}
	}
}



//MAIN PROGRAM

int main(int argv, char **argc)
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window *window = SDL_CreateWindow("SDL 1", 450, 150, screen_width, screen_height, SDL_WINDOW_SHOWN);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);


	//Initializing Dashed Middle Line Rects

	SDL_Rect Middle_Dashed_Line;

	int Number_Of_Rects = 9;
	int gap = 70;
	double *x = new double[Number_Of_Rects];
	double *y = new double[Number_Of_Rects];
	double *w = new double[Number_Of_Rects];
	double *h = new double[Number_Of_Rects];

    //loop for giving gap between dashed rects

	for (int i = 0; i< Number_Of_Rects; i++)
	{
		x[i] = 400;
		y[i] = 15;
		
		w[i] = 10;
		h[i] = 40;
		if (i > 0)
		{
			y[i] += gap;
			gap += 65;	
		}
	}

	//Initializing Paddles

	PaddleData *playerpaddle = new PaddleData;
	init_playerpaddle(playerpaddle);
	SDL_Rect paddle_rectangle;

	PaddleData *cpupaddle = new PaddleData;
	init_cpupaddle(cpupaddle);
	SDL_Rect paddle_rectangle2;


	//Initializing Ball

	BallData *ball = new BallData;
	init_ball(ball);
	SDL_Rect ball_rectangle;



	bool quit = false;

	//Game Loop

	while (quit == false)
	{

		//Event Loop
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				cout << "SomeOne Clicked the X button" << "\n";
				quit = true;
			}

			else if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_UP:
					playerpaddle->y -= playerpaddle->speed_y;
					if (playerpaddle->y < 0)
					{
						playerpaddle->y = 0;
					}
					break;
				case SDLK_DOWN:
					playerpaddle->y += playerpaddle->speed_y;
					if (playerpaddle->y + playerpaddle->height >= 600)
					{
						playerpaddle->y = 540;
					}
					break;
				}
			}
			
		}


		//Game Stuff
		Ballupdate(ball);
		CPU_Paddle_AI(ball, cpupaddle);

		if (ball->x < playerpaddle->x + playerpaddle->width && ball->x > playerpaddle->x && ball->y < playerpaddle->y + playerpaddle->height && ball->y > playerpaddle->y)
		{
			ball->speed_x = - 0.01;
			ball->speed_y = - 0.01;
		}
		
		//CPU_Paddle_AI(ball, cpupaddle);

		if (ball->x < cpupaddle->x + cpupaddle->width && ball->x > cpupaddle->x && ball->y < cpupaddle->y + cpupaddle->height && ball->y < cpupaddle->y)
		{
			ball->speed_x = 0.01;
			ball->speed_y = 0.01;
		}



		//Draw

		//Draw Game Window
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);


		//Drawing the Middle Dash Line on Screen
		for (int i = 0; i < Number_Of_Rects; i++)
		{
			Middle_Dashed_Line.x = x[i];
			Middle_Dashed_Line.y = y[i];
			Middle_Dashed_Line.w = w[i];
			Middle_Dashed_Line.h = h[i];
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			SDL_RenderFillRect(renderer, &Middle_Dashed_Line);
		}

		//Drawing Player Paddle on Screen
		paddle_rectangle.x = playerpaddle->x;
		paddle_rectangle.y = playerpaddle->y;
		paddle_rectangle.w = playerpaddle->width;
		paddle_rectangle.h = playerpaddle->height;

		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		SDL_RenderFillRect(renderer, &paddle_rectangle);

		//Drawing CPU Paddle on Screen
		paddle_rectangle2.x = cpupaddle->x;
		paddle_rectangle2.y = cpupaddle->y;
		paddle_rectangle2.w = cpupaddle->width;
		paddle_rectangle2.h = cpupaddle->height;

		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderFillRect(renderer, &paddle_rectangle2);

		//Drawing Ball on Screen
		ball_rectangle.x = ball->x;
		ball_rectangle.y = ball->y;
		ball_rectangle.w = ball->width;
		ball_rectangle.h = ball->height;

		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		SDL_RenderFillRect(renderer, &ball_rectangle);


		SDL_RenderPresent(renderer);


	}



	//system("Pause");
	return 0;
}