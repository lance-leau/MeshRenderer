#pragma once
#include <SDL2/SDL.h>
#include <vector>

#include "mesh.hh"
#include "model.hh"
#include "projector.hh"

namespace Renderer
{
    Model parseOBJ(const std::string& path);
}