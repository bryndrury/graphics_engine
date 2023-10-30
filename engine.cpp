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
#include "cube.h"

int main(int argc, char* argv[])
{
    // Create Window
	float screen_width = 1280;
	float screen_height = 720;
    int fps = 30;
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

        rotx.updateTheta(fElapstedTime,0.5);
        roty.updateTheta(fElapstedTime,0.5);
        rotz.updateTheta(fElapstedTime,0.5);

        // Once the scene is sorted, the triangles that will be rasterized will be put here
        std::vector<triangle> toRaster;

        // Loop through all the triangles of the mesh
        // #pragma omp parallel for num_threads(4) shared(toRaster)
        for (int triIndex = 0; triIndex < meshCube.getTris().size(); triIndex++)
        {
            triangle tri = meshCube[triIndex];
            // Rotate the triangles
            tXm(tri, rotz);     tXm(tri, rotx);     tXm(tri, roty);

            // Translation vector
            vec3 tVec(0.0,0.0,3.0);
            tranTri(tri, tVec);

            // Calculate the normal of the triangle
            vec3 normal = tri.normal().fnorm();
            
            // Check if the triangle is visible
            double facingCam = normal.dot(tri[0] - camera);

            // Only draw the triangle if it's visible
            if (facingCam < 0.0)
            {
                // Illumination
                Uint8 luminance = normal.dot(lightDirection) * 255;
                tri.setLum( luminance );

                // Project triangles from 3D to 2D
                tXm(tri, proj);

                // Scale into view
                scaleTri(tri, screen_width, screen_height);

                // Add triangle to the list of triangles to be rasterized
                // #pragma omp critical
                toRaster.push_back(tri);
            }
        }
        sort(toRaster.begin(), toRaster.end(), [](triangle& t1, triangle& t2) 
            {   double z1 = (t1[0].z() + t1[1].z() + t1[2].z()) / 3.0;
                double z2 = (t2[0].z() + t2[1].z() + t2[2].z()) / 3.0;
                return z1 > z2;     });

        // #pragma omp critical
        for (triangle t : toRaster) 
        {   
            renderTriangle(renderer, t);
        }
        SDL_RenderPresent(renderer);

        cap_framerate(starting_tick, fps);
    }

    return 0;
}