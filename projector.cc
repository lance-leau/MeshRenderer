#include "projector.hh"

namespace Renderer
{

    Projector::Projector(float focalLen, ProjectedPoint offset)
        : _focalLen(focalLen)
        , _offset(offset)
    {}

    void Projector::setFocalLen(float newFocal)
    {
        _focalLen = newFocal;
    }

    float Projector::getFocalLen()
    {
        return _focalLen;
    }

    ProjectedPoint Projector::projectPoint(Vertex& v)
    {
        float z = static_cast<float>(v.getZ());
        float x = static_cast<float>(v.getX());
        float y = static_cast<float>(v.getY());

        float denom = (_focalLen + z);
        float xProj = (_focalLen * x) / denom + _offset._x;
        float yProj = (_focalLen * y) / denom + _offset._y;

        return ProjectedPoint(xProj, yProj);
    }

    std::vector<ProjectedPoint>
    Projector::projectVertices(std::vector<Vertex*>& vertices)
    {
        std::vector<ProjectedPoint> projected;
        projected.reserve(vertices.size());

        for (Vertex* v : vertices)
            projected.push_back(projectPoint(*v));

        return projected;
    }

} // namespace Renderer
