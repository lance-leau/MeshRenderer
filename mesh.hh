#pragma once
#include <array>
#include <string>
#include <vector>

#include "vertex.hh"

namespace Renderer
{
    class Mesh
    {
    private:
        std::array<size_t, 3> _vertices;
        std::array<unsigned char, 4> _color;

    public:
        Mesh(size_t v1, size_t v2, size_t v3, unsigned char r = 222,
             unsigned char g = 222, unsigned char b = 222,
             unsigned char a = 255);

        std::array<size_t, 3>& getVertices();
        std::array<unsigned char, 4>& getColor();

        void setColor(unsigned char r, unsigned char g, unsigned char b,
                      unsigned char a);
        void print(std::vector<Vertex*>& vertices);
    };

} // namespace Renderer