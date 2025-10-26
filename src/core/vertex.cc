#include "vertex.hh"

#include <cmath>

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

    void Vertex::rotate(float pitch, float yaw)
    {
        float x = static_cast<float>(_x);
        float y = static_cast<float>(_y);
        float z = static_cast<float>(_z);

        // Yaw (Y axis)
        float x1 = cos(yaw) * x + sin(yaw) * z;
        float z1 = -sin(yaw) * x + cos(yaw) * z;

        // Pitch (X axis)
        float y1 = cos(pitch) * y - sin(pitch) * z1;
        float z2 = sin(pitch) * y + cos(pitch) * z1;

        _x = static_cast<int>(x1);
        _y = static_cast<int>(y1);
        _z = static_cast<int>(z2);
    }

    void Vertex::print()
    {
        std::cout << "(" << _x << ", " << _y << ", " << _z << ")";
    }
} // namespace Renderer