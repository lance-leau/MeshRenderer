#pragma once
#include <SDL2/SDL.h>
#include <vector>

#include "mesh.hh"
#include "model.hh"
#include "projector.hh"

namespace Renderer
{

    void drawWireframe(Mesh& mesh, std::vector<ProjectedPoint>& projected,
                       SDL_Renderer* renderer);

    void drawMesh(Mesh& mesh, std::vector<ProjectedPoint>& projected,
                  SDL_Renderer* renderer);

} // namespace Renderer
