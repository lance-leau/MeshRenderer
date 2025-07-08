#pragma once
#include <iostream>
#include <string>

namespace Renderer
{

    class Vertex
    {
    private:
        long _x;
        long _y;
        long _z;

    public:
        Vertex(long x, long y, long z);

        long getX();
        long getY();
        long getZ();

        void setX(long x);
        void setY(long y);
        void setZ(long z);

        void print();
    };
} // namespace Renderer