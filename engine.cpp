#include <SDL2/SDL.h>
#include <iostream>
#include <fstream>
#include <strstream>
#include <string>
#include <algorithm>
#include <vector>
#include <omp.h>

#include "includes.h"

// To Do:
// - Add a camera movement system.
// ~ Add a lighting system.
// - Add a texture system.
// + Add a model loader.
// - Add scene (model) organiser.
//     - create a scene class.
//     - let the class handle all the properties of the objects.
//     - pass the class to the renderer.  


int main(int argc, char* argv[])
{
    // Create Window
	double screen_width = 1280; //640
	double screen_height = 720; //480
    int fps = 60;
	SDL_Init( SDL_INIT_VIDEO );
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;

	// Render Black Screen
	window = SDL_CreateWindow("engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, (SDL_WINDOW_SHOWN, SDL_WINDOW_RESIZABLE, SDL_VIDEO_OPENGL, SDL_WINDOW_OPENGL));
	renderer = SDL_CreateRenderer(window, -1, (SDL_RENDERER_ACCELERATED, SDL_RENDERER_PRESENTVSYNC, SDL_WINDOW_OPENGL, SDL_VIDEO_OPENGL));
	SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
	SDL_RenderClear( renderer );
    SDL_RenderPresent( renderer );

    const auto startTime = std::chrono::steady_clock::now();
    std::chrono::duration<double> fElapstedTime;

    rotX rotx(0.0);
    rotY roty(0.0);
    rotZ rotz(0.0);
    projMat proj(0.1, 1000.0, 90.0, screen_height/screen_width);

    vec3 camera(0.0, 0.0, 0.0); // 
    // vec3 tVec(0.0,0.0,3.0);
    vec3 lightDirection(0.0, 0.0, -1.0); // camera position

    Uint32 starting_tick;
    SDL_Event event;
    bool running = true;
    int count = 0;

    bool wireframe = false;
    bool shade = true;
    bool randColor = false;

    std::vector<mesh> object_array;

    mesh teapot;
    teapot.LoadFromObjectFile( "objects/teapot.obj" );
    teapot.setPos(vec3(2.0,1.0,6.0));
    object_array.push_back(teapot);

    mesh teapot2;
    teapot2.LoadFromObjectFile( "objects/teapot.obj" );
    teapot2.setPos(vec3(-3.0,-1.0,8.0));
    object_array.push_back(teapot2);

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
            // check if event is key pressed
            if (event.type == SDL_KEYDOWN)
            {
                // check which key was pressed
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    running = false;
                    break;
                }
                if (event.key.keysym.sym == SDLK_t)
                {
                    if (wireframe) { wireframe = false; } else { wireframe = true; }
                }
                if (event.key.keysym.sym == SDLK_y)
                {
                    if (shade) { shade = false; } else { shade = true; }
                }
                if (event.key.keysym.sym == SDLK_r)
                {
                    if (randColor) { randColor = false; } else { randColor = true; }
                }
            }
        }
        SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255);
        SDL_RenderClear( renderer );

        rotx.updateTheta( fElapstedTime, 0.5 );
        roty.updateTheta( fElapstedTime, 0.3 );
        rotz.updateTheta( fElapstedTime, 0.7 );

        // Once the scene is sorted, the triangles that will be rasterized will be put here
        mesh toRaster;

        // Loop through all the triangles of the mesh 
        // set OMP threads to 4

        #pragma omp parallel for shared(toRaster) OMP_DYNAMIC num_threads(4)
        for ( mesh obj : object_array)
        {
            vec3 tVec = obj.getPos();
            for ( triangle tri : obj.getTris() )
            {

                // triangle tri = object[triIndex];
                // Rotate the triangles
                tXm(tri, rotz);     tXm(tri, rotx);     tXm(tri, roty);

                // Translation vector
                // vec3 tVec = vec3(0.0,0.0,7.0);
                tranTri(tri, tVec);

                // Calculate the normal of the triangle
                vec3 normal = tri.normal().fnorm();
                
                // Check if the triangle is visible
                double facingCam = normal.dot(tri[0] - camera);

                // Only draw the triangle if it's visible
                if (facingCam < 0.0)
                {
                    // Project triangles from 3D to 2D
                    tXm(tri, proj);

                    // Scale into view
                    scaleTri( tri, screen_width, screen_height );

                    // Illumination
                    tri.setLum( normal.dot(lightDirection) * 255 );

                    // Add triangle to the list of triangles to be rasterized
                    #pragma omp critical
                    toRaster.addTriangle(tri);
                }
            }
        }
        #pragma omp barrier
        toRaster.sort();

        #pragma omp critical
        for (triangle t : toRaster.getTris()) 
        { 
            if (shade) { renderTriangle(renderer, t, randColor); }
            if (wireframe) { renderWireframe(renderer, t); }
        }
        SDL_RenderPresent(renderer);

        if (count > 50) { fpsCounter(starting_tick); count = 0; } else { count++; }
        cap_framerate( starting_tick, fps );
    }

    return 0;
}