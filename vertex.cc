#include "vertex.hh"

namespace Renderer
{
    Vertex::Vertex(long x, long y, long z)
        : _x(x)
        , _y(y)
        , _z(z)
    {}

    long Vertex::getX()
    {
        return _x;
    }

    long Vertex::getY()
    {
        return _y;
    }

    long Vertex::getZ()
    {
        return _z;
    }

    void Vertex::setX(long x)
    {
        _x = x;
    }

    void Vertex::setY(long y)
    {
        _y = y;
    }

    void Vertex::setZ(long z)
    {
        _z = z;
    }

    void Vertex::print()
    {
        std::cout << "(" << _x << ", " << _y << ", " << _z << ")";
    }
} // namespace Renderer