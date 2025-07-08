#include <algorithm>
#include <cmath>
#include <fstream>
#include <sstream>

#include "model.hh"
#include "renderer.hh"

namespace Renderer
{
    Model parseOBJ(const std::string& path)
    {
        Model model(0, 200, 100); // placeholder position

        std::ifstream file(path);
        if (!file.is_open())
            return model;

        std::string line;
        std::vector<Vertex*> vertices;
        std::vector<std::array<int, 3>> faces;

        while (std::getline(file, line))
        {
            std::istringstream iss(line);
            std::string prefix;
            iss >> prefix;

            if (prefix == "v")
            {
                float x, y, z;
                iss >> x >> y >> z;
                float xNew = -y * 10;
                float yNew = -z * 10;
                float zNew = x * 10;
                vertices.push_back(new Vertex(xNew, yNew, zNew));
            }
            else if (prefix == "f")
            {
                std::array<int, 3> face;
                std::string v1, v2, v3;
                iss >> v1 >> v2 >> v3;

                auto parseIndex = [](const std::string& token) -> int {
                    size_t pos = token.find('/');
                    return std::stoi(token.substr(0, pos))
                        - 1; // OBJ is 1-indexed
                };

                face[0] = parseIndex(v1);
                face[1] = parseIndex(v2);
                face[2] = parseIndex(v3);

                faces.push_back(face);
            }
        }

        for (Vertex* v : vertices)
            model.addVertex(v);
        for (const auto& f : faces)
            model.addMesh(Mesh(f[0], f[1], f[2], 255, 255, 255, 255));

        return model;
    }
} // namespace Renderer