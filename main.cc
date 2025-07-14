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

    Projector projector(400, ProjectedPoint(WIDTH / 2.0f, HEIGHT / 2.0f, 0.0f));

    // Model model(0, 0, 100); // global position in front of camera

    // // define 8 vertices centered at (0, 0, 300) for the model
    // model.addVertex(new Vertex(-50, -50, -50)); // 0
    // model.addVertex(new Vertex(50, -50, -50)); // 1
    // model.addVertex(new Vertex(50, 50, -50)); // 2
    // model.addVertex(new Vertex(-50, 50, -50)); // 3
    // model.addVertex(new Vertex(-50, -50, 50)); // 4
    // model.addVertex(new Vertex(50, -50, 50)); // 5
    // model.addVertex(new Vertex(50, 50, 50)); // 6
    // model.addVertex(new Vertex(-50, 50, 50)); // 7

    // // define the model meshes:
    // // 2 mesh per model side
    // model.addMesh(Mesh(0, 1, 2, 110, 190, 150));
    // model.addMesh(Mesh(0, 2, 3, 120, 180, 160)); // front
    // model.addMesh(Mesh(4, 5, 6, 130, 170, 170));
    // model.addMesh(Mesh(4, 6, 7, 140, 160, 180)); // back
    // model.addMesh(Mesh(1, 5, 6, 190, 110, 120));
    // model.addMesh(Mesh(1, 6, 2, 100, 100, 130)); // right
    // model.addMesh(Mesh(0, 4, 7, 110, 190, 140));
    // model.addMesh(Mesh(0, 7, 3, 120, 180, 150)); // left
    // model.addMesh(Mesh(0, 4, 5, 150, 130, 190));
    // model.addMesh(Mesh(0, 5, 1, 160, 140, 100)); // bottom
    // model.addMesh(Mesh(3, 7, 6, 170, 110, 110));
    // model.addMesh(Mesh(3, 6, 2, 180, 120, 120)); // top

    Renderer::Model model = Renderer::parseOBJ("cocorico.obj");
    std::cout << model.getMeshes().size() << "\n";

    bool running = true;
    SDL_Event event;

    float yaw = 0.0f;
    float pitch = 0.0f;

    bool pitchUp, pitchDown, yawRight, yawLeft;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                running = false;
                break;

            case SDL_KEYDOWN:
                if (event.key.repeat == 0)
                {
                    switch (event.key.keysym.scancode)
                    {
                    case SDL_SCANCODE_RIGHT:
                        yawRight = true;
                        break;
                    case SDL_SCANCODE_LEFT:
                        yawLeft = true;
                        break;
                    case SDL_SCANCODE_UP:
                        pitchUp = true;
                        break;
                    case SDL_SCANCODE_DOWN:
                        pitchDown = true;
                        break;
                    case SDL_SCANCODE_SPACE:
                        model._isWireframe = !model._isWireframe;
                        break;
                    default:
                        break;
                    }
                }
                break;
            case SDL_KEYUP:
                if (event.key.repeat == 0)
                {
                    switch (event.key.keysym.scancode)
                    {
                    case SDL_SCANCODE_RIGHT:
                        yawRight = false;
                        break;
                    case SDL_SCANCODE_LEFT:
                        yawLeft = false;
                        break;
                    case SDL_SCANCODE_UP:
                        pitchUp = false;
                        break;
                    case SDL_SCANCODE_DOWN:
                        pitchDown = false;
                        break;
                    default:
                        break;
                    }
                }
                break;
            }
        }
        if (yawLeft)
            yaw += 0.1f;
        if (yawRight)
            yaw -= 0.1f;
        if (pitchUp)
            pitch -= 0.1f;
        if (pitchDown)
            pitch += 0.1f;
        model.setYaw(yaw);
        model.setPitch(pitch);

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