#include <SDL2/SDL.h>
#include <iostream>
#include <string>

#include "mesh.hh"
#include "model.hh"
#include "parseOBJ.hh"
#include "renderer.hh"
#include "vertex.hh"

using Renderer::ProjectedPoint;
using Renderer::Projector;
using Renderer::Vertex;
using Renderer::Model;
using Renderer::Mesh;

const int WIDTH = 800;
const int HEIGHT = 600;

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "WSL SDL2 Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (!window)
    {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError()
                  << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    Projector projector(400, ProjectedPoint(WIDTH / 2.0f, HEIGHT / 2.0f));

    // Model cube(0, cube_y, 100); // global position in front of camera

    // // define 8 vertices centered at (0, 0, 300) for the cube
    // cube.addVertex(new Vertex(-50, -50, -50)); // 0
    // cube.addVertex(new Vertex(50, -50, -50)); // 1
    // cube.addVertex(new Vertex(50, 50, -50)); // 2
    // cube.addVertex(new Vertex(-50, 50, -50)); // 3
    // cube.addVertex(new Vertex(-50, -50, 50)); // 4
    // cube.addVertex(new Vertex(50, -50, 50)); // 5
    // cube.addVertex(new Vertex(50, 50, 50)); // 6
    // cube.addVertex(new Vertex(-50, 50, 50)); // 7

    // // define the cube meshes:
    // // 2 mesh per cube side
    // cube.addMesh(Mesh(0, 1, 2, 110, 190, 150));
    // cube.addMesh(Mesh(0, 2, 3, 120, 180, 160)); // front
    // cube.addMesh(Mesh(4, 5, 6, 130, 170, 170));
    // cube.addMesh(Mesh(4, 6, 7, 140, 160, 180)); // back
    // cube.addMesh(Mesh(1, 5, 6, 190, 110, 120));
    // cube.addMesh(Mesh(1, 6, 2, 100, 100, 130)); // right
    // cube.addMesh(Mesh(0, 4, 7, 110, 190, 140));
    // cube.addMesh(Mesh(0, 7, 3, 120, 180, 150)); // left
    // cube.addMesh(Mesh(0, 4, 5, 150, 130, 190));
    // cube.addMesh(Mesh(0, 5, 1, 160, 140, 100)); // bottom
    // cube.addMesh(Mesh(3, 7, 6, 170, 110, 110));
    // cube.addMesh(Mesh(3, 6, 2, 180, 120, 120)); // top

    // Model model = parseOBJ("cat.obj");
    Renderer::Model model = Renderer::parseOBJ("cat.obj");

    bool running = true;
    SDL_Event event;

    float yaw = 0.0f;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = false;
            const Uint8* keystates = SDL_GetKeyboardState(NULL);
            if (keystates[SDL_SCANCODE_RIGHT])
            {
                yaw += 0.1f;
                model.setYaw(yaw);
            }
        }

        SDL_SetRenderDrawColor(renderer, 91, 183, 238,
                               255); // light blue background
        SDL_RenderClear(renderer);

        model.draw(renderer, projector);

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 FPS
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}