#include "mesh.hh"

#include <iostream>

namespace Renderer
{
    Mesh::Mesh(size_t v1, size_t v2, size_t v3, unsigned char r,
               unsigned char g, unsigned char b, unsigned char a)
        : _vertices{ v1, v2, v3 }
        , _color{ r, g, b, a }
    {}

    std::array<size_t, 3>& Mesh::getVertices()
    {
        return _vertices;
    }

    std::array<unsigned char, 4>& Mesh::getColor()
    {
        return _color;
    }

    void Mesh::setColor(unsigned char r, unsigned char g, unsigned char b,
                        unsigned char a)
    {
        _color = { r, g, b, a };
    }

    void Mesh::print(std::vector<Vertex*>& vertices)
    {
        std::cout << "(" << static_cast<int>(_color.at(0)) << ", "
                  << static_cast<int>(_color.at(1)) << ", "
                  << static_cast<int>(_color.at(2)) << ", "
                  << static_cast<int>(_color.at(3)) << ")\n";
        for (auto v : _vertices)
        {
            std::cout << "\t";
            vertices[v]->print();
            std::cout << ",\n";
        }
        std::cout << "}\n";
    }

} // namespace Renderer
