#include <SDL2/SDL.h>

void cap_framerate(Uint32 starting_tick, int fps)
{
	double frameTime = SDL_GetTicks() - starting_tick;
	if ((1000 / fps) > frameTime)
	{
		SDL_Delay(1000 / fps - (SDL_GetTicks() - starting_tick));
		if ((1000 / 1) > frameTime)
		{
			std::cout << "Frame Time: " << frameTime << "ms. " << "FPS: " << 1000.0/frameTime << "\r" << std::flush;
		}
	}
}