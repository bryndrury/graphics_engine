#include <SDL2/SDL.h>
#include <iostream>
#include <fstream>
#include <strstream>
#include <string>
#include <algorithm>
#include <omp.h>

#include "sdl_helper.h"
#include "vec3.h"
#include "matrix.h"
#include "mesh.h"
#include "matricies.h"
#include "vec_mat.h"

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
		triangle( vec3(0.0f, 0.0f, 0.0f),    vec3(0.0f, 1.0f, 0.0f),    vec3(1.0f, 1.0f, 0.0f) ),
		triangle( vec3(0.0f, 0.0f, 0.0f),    vec3(1.0f, 1.0f, 0.0f),    vec3(1.0f, 0.0f, 0.0f) ),

		// EAST                                                      
		triangle( vec3(1.0f, 0.0f, 0.0f),    vec3(1.0f, 1.0f, 0.0f),    vec3(1.0f, 1.0f, 1.0f) ),
		triangle( vec3(1.0f, 0.0f, 0.0f),    vec3(1.0f, 1.0f, 1.0f),    vec3(1.0f, 0.0f, 1.0f) ),

		// NORTH                                                     
		triangle( vec3(1.0f, 0.0f, 1.0f),    vec3(1.0f, 1.0f, 1.0f),    vec3(0.0f, 1.0f, 1.0f) ),
		triangle( vec3(1.0f, 0.0f, 1.0f),    vec3(0.0f, 1.0f, 1.0f),    vec3(0.0f, 0.0f, 1.0f) ),

		// WEST                                                      
		triangle( vec3(0.0f, 0.0f, 1.0f),    vec3(0.0f, 1.0f, 1.0f),    vec3(0.0f, 1.0f, 0.0f) ),
		triangle( vec3(0.0f, 0.0f, 1.0f),    vec3(0.0f, 1.0f, 0.0f),    vec3(0.0f, 0.0f, 0.0f) ),

		// TOP                                                       
		triangle( vec3(0.0f, 1.0f, 0.0f),    vec3(0.0f, 1.0f, 1.0f),    vec3(1.0f, 1.0f, 1.0f) ),
		triangle( vec3(0.0f, 1.0f, 0.0f),    vec3(1.0f, 1.0f, 1.0f),    vec3(1.0f, 1.0f, 0.0f) ),

		// BOTTOM                                                    
		triangle( vec3(1.0f, 0.0f, 1.0f),    vec3(0.0f, 0.0f, 1.0f),    vec3(0.0f, 0.0f, 0.0f) ),
		triangle( vec3(1.0f, 0.0f, 1.0f),    vec3(0.0f, 0.0f, 0.0f),    vec3(1.0f, 0.0f, 0.0f) ), 
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
        SDL_SetRenderDrawColor(renderer, 0,0,0,0);
        SDL_RenderClear(renderer);


        rotx.updateTheta(fElapstedTime,1);
        roty.updateTheta(fElapstedTime,1);
        rotz.updateTheta(fElapstedTime,1);

        // Once the scene is sorted, the triangles that will be rasterized will be put here
        std::vector<triangle> toRaster;

        // Loop through all the triangles of the mesh
        #pragma omp parallel for num_threads(4) shared(toRaster)
        for (int triIndex = 0; triIndex < meshCube.getTris().size(); triIndex++)
        {
            triangle tri = meshCube[triIndex];
            // tXm(tri, rotz);
            // tXm(tri, rotx);

            // Translation vector
            vec3 tVec(8.0,0.0,8.0);
            tranTri(tri, tVec);

            vec3 normal = tri.normal();
            normal = normal.norm();

            double facingCam = normal.dot(tri[0] - camera);

            if (facingCam < 0.0)
            {
                // Illumination
                double lightIntensity = normal.dot(lightDirection);
                tri.setLum(lightIntensity * 255.0);

                // Project triangles from 3D --> 2D
                tXm(tri, proj);

                // Scale into view
                // vec3 scaleVec(screen_width*0.5, screen_height*0.5, 1.0);
                scaleTri(tri, screen_width, screen_height);

                // Add triangle to the list of triangles to be rasterized
                #pragma omp critical
                toRaster.push_back(tri);
            }
        }
        for (triangle t : toRaster) 
        {
            Uint8 lum = t.getLum();
            std::vector<SDL_Vertex> verts = 
            {
                { SDL_FPoint{(float)t[0].x(), (float)t[0].y()}, SDL_Color{lum, lum, lum, 255}, SDL_FPoint{ 0 } },
                { SDL_FPoint{(float)t[1].x(), (float)t[1].y()}, SDL_Color{lum, lum, lum, 255}, SDL_FPoint{ 0 } },
                { SDL_FPoint{(float)t[2].x(), (float)t[2].y()}, SDL_Color{lum, lum, lum, 255}, SDL_FPoint{ 0 } },
            };
            SDL_RenderGeometry(renderer, nullptr, verts.data(), verts.size(), nullptr, 0);
        }
        SDL_RenderPresent(renderer);

        cap_framerate(starting_tick, fps);
    }

    return 0;
}