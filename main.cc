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

const int WIDTH = 1600;
const int HEIGHT = 900;

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
            case SDL_WINDOWEVENT:
                int width, height;
                SDL_GetWindowSize(window, &width, &height);
                projector._offset =
                    ProjectedPoint(width / 2, (height / 2) + 200, 0);
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

        Renderer::clearWithLayeredGradient(renderer, window);

        model.draw(renderer, projector);

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 FPS
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}