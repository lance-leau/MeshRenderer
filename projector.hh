#pragma once
#include <SDL2/SDL.h>
#include <vector>

#include "vertex.hh"

namespace Renderer
{
    class ProjectedPoint
    {
    public:
        int _x;
        int _y;
        ProjectedPoint(int x, int y)
            : _x(x)
            , _y(y){};
    };

    class Projector
    {
    private:
        float _focalLen; // 50 <=_focalLen <= 1000
        ProjectedPoint _offset;

    public:
        Projector(float focalLen = 300.0f,
                  ProjectedPoint offset = ProjectedPoint(0.0f, 0.0f));

        void setFocalLen(float newFocal);
        float getFocalLen();

        std::vector<ProjectedPoint>
        projectVertices(std::vector<Vertex*>& vertices);
        ProjectedPoint projectPoint(Vertex& v);
    };

} // namespace Renderer