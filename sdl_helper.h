#include <SDL2/SDL.h>
#include "mesh.h"

void cap_framerate(Uint32 starting_tick, int fps)
{
	double frameTime = SDL_GetTicks() - starting_tick;
	if ((1000 / fps) > frameTime)
	{
		SDL_Delay(1000 / fps - (frameTime));
		if (100 > frameTime)
		{
			// std::cout << std::string(100, ' ') << "\r" << std::flush;
			// std::cout << "Frame Time: " << frameTime << "ms. " << "FPS: " << 1000.0/frameTime << "\r" << std::flush;
		}
	}
}

void renderTriangle(SDL_Renderer* renderer, triangle& t)
{
	Uint8 lum = t.getLum();
	lum = 200;
	std::vector<SDL_Vertex> verts = 
	{
		{ SDL_FPoint{(float)t[0].x(), (float)t[0].y()}, SDL_Color{lum, lum, lum, 255}, SDL_FPoint{ 0 } },
		{ SDL_FPoint{(float)t[1].x(), (float)t[1].y()}, SDL_Color{lum, lum, lum, 255}, SDL_FPoint{ 0 } },
		{ SDL_FPoint{(float)t[2].x(), (float)t[2].y()}, SDL_Color{lum, lum, lum, 255}, SDL_FPoint{ 0 } },
	};
	SDL_RenderGeometry(renderer, nullptr, verts.data(), verts.size(), nullptr, 0);
}

void renderWireframe(SDL_Renderer* renderer, triangle& t)
{
	SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );
	SDL_RenderDrawLineF( renderer, t[0].x(), t[0].y(), t[1].x(), t[1].y() );
	SDL_RenderDrawLineF( renderer, t[1].x(), t[1].y(), t[2].x(), t[2].y() );
	SDL_RenderDrawLineF( renderer, t[0].x(), t[0].y(), t[2].x(), t[2].y() );

}