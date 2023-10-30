#include <SDL.h>
#include <iostream>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGTH = 480;

const int PLAYER_WIDTH = 7;
const int PLAYER_HEIGHT = 50;
const int PLAYER_DEPTH = 50;

const float PLAYER_SPEED = 0.2f;

const int BALL_SIZE = 10;
const float STARTING_BALL_SPEED = 0.1f;

// Ball class that is basically a rectangle that moves
class Ball {
public:
	float velocity[2] = {STARTING_BALL_SPEED, STARTING_BALL_SPEED};
	float fractionalMove[2] = { 0.0f, 0.0f };
	SDL_Rect rect = { SCREEN_WIDTH / 2, 100, BALL_SIZE, BALL_SIZE };
};

// Player class that is also basically a rectangle that moves
class Player {
public:
	float velocity = 0.0f;
	float fractionalMove = 0.0f;
	SDL_Rect rect = { 100, SCREEN_HEIGTH / 2, PLAYER_WIDTH, PLAYER_HEIGHT };
};

SDL_Texture* LoadTexture(SDL_Surface* surface, SDL_Renderer* renderer) {
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	return texture;
}

int main(int argc, char* args[])
{
	int prevTime = 0;
	int currentTime = 0;
	float deltaTime = 0.0f;
	int framesElapsed = 0;

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	Player players[2];

	Ball ball;
	float ballSpeed = STARTING_BALL_SPEED;

	players[0].rect.x = PLAYER_DEPTH;
	players[1].rect.x = SCREEN_WIDTH - PLAYER_DEPTH;

	window = SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGTH, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	bool running = true;
	SDL_Event ev;


	while (running) {

		// Outputting FPS every 1 second
		framesElapsed++;

		prevTime = currentTime;
		currentTime = SDL_GetTicks();
		deltaTime += (currentTime - prevTime) / 1000.0f;

		if (deltaTime > 1.0f) {
			std::cout << int(framesElapsed / deltaTime)  << std::endl;
			deltaTime = 0;
			framesElapsed = 0;
		}

		// Event Managment
		while (SDL_PollEvent(&ev) != 0) {

			if (ev.type == SDL_QUIT) {
				running = false;
			}
			else if (ev.type == SDL_KEYDOWN) {
				switch (ev.key.keysym.sym) {
					case SDLK_w:
						players[0].velocity = -PLAYER_SPEED;
						break;
					case SDLK_s:
						players[0].velocity = +PLAYER_SPEED;
						break;
					case SDLK_UP:
						players[1].velocity = -PLAYER_SPEED;
						break;
					case SDLK_DOWN:
						players[1].velocity = +PLAYER_SPEED;
						break;
				}
			}
			else if (ev.type == SDL_KEYUP) {
				switch (ev.key.keysym.sym) {
				case SDLK_w:
					players[0].velocity = 0;
					players[0].fractionalMove = 0;
					break;
				case SDLK_s:
					players[0].velocity = 0;
					players[0].fractionalMove = 0;
					break;
				case SDLK_UP:
					players[1].velocity = 0;
					players[1].fractionalMove = 0;
					break;
				case SDLK_DOWN:
					players[1].velocity = 0;
					players[1].fractionalMove = 0;
					break;
				}
			}
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		// Drawing and updating players
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

		for (int i = 0; i < 2; i++) {
			players[i].fractionalMove += players[i].velocity;
			if (players[i].fractionalMove > 1.0f) {
				players[i].rect.y += 1;
				players[i].fractionalMove = 0.0f;

				if (players[i].rect.y > SCREEN_HEIGTH - PLAYER_HEIGHT)
					players[i].rect.y = SCREEN_HEIGTH - PLAYER_HEIGHT;
			}
			else if (players[i].fractionalMove < -1.0f) {
				players[i].rect.y -= 1;
				players[i].fractionalMove = 0.0f; 
				
				if (players[i].rect.y < 0)
					players[i].rect.y = 0;
			}

			SDL_RenderFillRect(renderer, &players[i].rect);
		}

		// Drawing and updating ball
		ball.fractionalMove[0] += ball.velocity[0];
		ball.fractionalMove[1] += ball.velocity[1];
		if (ball.fractionalMove[0] > 1.0f) {
			ball.rect.x += 1;
			ball.fractionalMove[0] = 0;
		}
		else if (ball.fractionalMove[0] < -1.0f) {
			ball.rect.x -= 1;
			ball.fractionalMove[0] = 0;
		}
		if (ball.fractionalMove[1] > 1.0f) {
			ball.rect.y += 1;
			ball.fractionalMove[1] = 0;
		}
		else if (ball.fractionalMove[1] < -1.0f) {
			ball.rect.y -= 1;
			ball.fractionalMove[1] = 0;
		}

		SDL_RenderFillRect(renderer, &ball.rect);

		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	window = nullptr;
	renderer = nullptr;

	SDL_Quit();

	return 0;
}
