#include <SDL2/SDL.h>
#include <iostream>
#include <fstream>
#include <strstream>
#include <string>
#include <algorithm>

#include "vec3.h"
#include "matrix.h"
#include "mesh.h"
#include "comp_funcs.h"

int main(int argc, char* argv[])
{
    // Create Window
	float screen_width = 1280;
	float screen_height = 720;
    int fps = 60;
	SDL_Init( SDL_INIT_EVERYTHING );
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;

	// Render Black Screen
	window = SDL_CreateWindow("engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, (SDL_WINDOW_SHOWN, SDL_WINDOW_RESIZABLE));
	renderer = SDL_CreateRenderer(window, -1, (SDL_RENDERER_ACCELERATED, SDL_RENDERER_PRESENTVSYNC));
	SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
	SDL_RenderClear( renderer );
    SDL_RenderPresent( renderer );

    const auto startTime = std::chrono::steady_clock::now();
    std::chrono::duration<double> fElapstedTime;
    rotX rotx(0.0);
    rotY roty(0.0);
    rotZ rotz(0.0);
    projMat proj(0.1, 1000.0, 90.0, screen_height/screen_width);

    mesh meshCube({
		// SOUTH
		triangle({ vec3(0.0f, 0.0f, 0.0f),    vec3(0.0f, 1.0f, 0.0f),    vec3(1.0f, 1.0f, 0.0f) }),
		triangle({ vec3(0.0f, 0.0f, 0.0f),    vec3(1.0f, 1.0f, 0.0f),    vec3(1.0f, 0.0f, 0.0f) }),

		// EAST                                                      
		triangle({ vec3(1.0f, 0.0f, 0.0f),    vec3(1.0f, 1.0f, 0.0f),    vec3(1.0f, 1.0f, 1.0f) }),
		triangle({ vec3(1.0f, 0.0f, 0.0f),    vec3(1.0f, 1.0f, 1.0f),    vec3(1.0f, 0.0f, 1.0f) }),

		// NORTH                                                     
		triangle({ vec3(1.0f, 0.0f, 1.0f),    vec3(1.0f, 1.0f, 1.0f),    vec3(0.0f, 1.0f, 1.0f) }),
		triangle({ vec3(1.0f, 0.0f, 1.0f),    vec3(0.0f, 1.0f, 1.0f),    vec3(0.0f, 0.0f, 1.0f) }),

		// WEST                                                      
		triangle({ vec3(0.0f, 0.0f, 1.0f),    vec3(0.0f, 1.0f, 1.0f),    vec3(0.0f, 1.0f, 0.0f) }),
		triangle({ vec3(0.0f, 0.0f, 1.0f),    vec3(0.0f, 1.0f, 0.0f),    vec3(0.0f, 0.0f, 0.0f) }),

		// TOP                                                       
		triangle({ vec3(0.0f, 1.0f, 0.0f),    vec3(0.0f, 1.0f, 1.0f),    vec3(1.0f, 1.0f, 1.0f) }),
		triangle({ vec3(0.0f, 1.0f, 0.0f),    vec3(1.0f, 1.0f, 1.0f),    vec3(1.0f, 1.0f, 0.0f) }),

		// BOTTOM                                                    
		triangle({ vec3(1.0f, 0.0f, 1.0f),    vec3(0.0f, 0.0f, 1.0f),    vec3(0.0f, 0.0f, 0.0f) }),
		triangle({ vec3(1.0f, 0.0f, 1.0f),    vec3(0.0f, 0.0f, 0.0f),    vec3(1.0f, 0.0f, 0.0f) }), 
    });

    vec3 camera(0.0, 0.0, 0.0);
    vec3 lightDirection(0.0,0.0,-1.0);

    Uint32 starting_tick;
    SDL_Event event;
    bool running = true;

    while (running) 
    {
        starting_tick = SDL_GetTicks();
        fElapstedTime = std::chrono::steady_clock::now() - startTime;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
                break;
            }
        }
        rotx.updateTheta(fElapstedTime,1);
        roty.updateTheta(fElapstedTime,1);
        rotz.updateTheta(fElapstedTime,1);

        // Once the scene is sorted, the triangles that will be rasterized will be put here
        mesh toRaster;

        // Loop through all the triangles of the mesh
        for (int triIndex = 0; triIndex < meshCube.getTris().size(); triIndex++)
        {
            triangle tri = meshCube[triIndex];
            for (int pointIndex = 0; pointIndex < tri.getP().size(); pointIndex++)
            {
                vec3 point = tri[pointIndex];
                // point.out();
            }
        }

        cap_framerate(starting_tick, fps);
    }

    return 0;
}