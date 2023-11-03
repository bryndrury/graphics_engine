#include "includes.h"

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

    vec3 camera(0.0, 0.0, 0.0);
    vec3 lightDirection(0.0,0.0,-1.0);

    // Object properties:
    mesh object;
    object.LoadFromObjectFile("objects/teapot.obj");
    // Position: 
    // vec3 tVec(0.0,0.0,3.0);

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

        // rotx.updateTheta(fElapstedTime,0.5);
        roty.updateTheta(fElapstedTime,0.3);
        // rotz.updateTheta(fElapstedTime,0.7);

        // Once the scene is sorted, the triangles that will be rasterized will be put here
        mesh toRaster;

        // Loop through all the triangles of the mesh
        int loopend = object.getTris().size();
        #pragma omp parallel for shared(toRaster) OMP_DYNAMIC
        for (int triIndex = 0; triIndex < loopend; triIndex++)
        {

            triangle tri = object[triIndex];
            // Rotate the triangles
            tXm(tri, rotz);     tXm(tri, rotx);     tXm(tri, roty);

            // Translation vector
            vec3 tVec = vec3(0.0,0.0,6.0);
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
                tri.lum = luminance ;

                // Project triangles from 3D to 2D
                tXm(tri, proj);

                // Scale into view
                scaleTri(tri, screen_width, screen_height);

                // Add triangle to the list of triangles to be rasterized
                #pragma omp critical
                toRaster.addTriangle (tri);
            }
        }
        #pragma omp barrier
        toRaster.sort();

        // #pragma omp critical
        for (triangle t : toRaster.getTris()) 
        { 
            // renderTriangle(renderer, t);
            renderWireframe(renderer, t);
        }
        SDL_RenderPresent(renderer);
        fpsCounter(starting_tick);
        // std::cout << std::string(100, ' ') << "\r" << "Frame Time: " << (SDL_GetTicks() - starting_tick) << "ms. " << "FPS: " << 1000.0/(SDL_GetTicks() - starting_tick) << "\r" << std::flush;
        // cap_framerate(starting_tick, fps);
    }

    return 0;
}