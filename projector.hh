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
        int _z;
        ProjectedPoint(int x, int y, int z)
            : _x(x)
            , _y(y)
            , _z(z){};
    };

    class Projector
    {
    private:
        float _focalLen; // 50 <=_focalLen <= 1000

    public:
        ProjectedPoint _offset;

        Projector(float focalLen = 300.0f,
                  ProjectedPoint offset = ProjectedPoint(0.0f, 0.0f, 0.0f));

        void setFocalLen(float newFocal);
        float getFocalLen();

        std::vector<ProjectedPoint>
        projectVertices(std::vector<Vertex*>& vertices);
        ProjectedPoint projectPoint(Vertex& v);
    };

} // namespace Renderer