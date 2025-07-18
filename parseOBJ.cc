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
        Model model(0, 0, 0); // placeholder position

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

                // // cat
                // iss >> x >> y >> z;
                // float xNew = -y * 10;
                // float yNew = -z * 10;
                // float zNew = x * 10;

                // chest
                iss >> x >> y >> z;
                float xNew = x * 0.2f;
                float yNew = -y * 0.2f;
                float zNew = -z * 0.2f;

                // // cocorico
                // iss >> x >> y >> z;
                // float xNew = x * 100;
                // float yNew = -y * 100;
                // float zNew = -z * 100;

                vertices.push_back(new Vertex(xNew, yNew, zNew));
            }
            else if (prefix == "f")
            {
                std::array<int, 3> face1;
                std::array<int, 3> face2;
                std::string v1, v2, v3, v4;
                iss >> v1 >> v2 >> v3 >> v4;

                auto parseIndex = [](const std::string& token) -> int {
                    size_t pos = token.find('/');
                    return std::stoi(token.substr(0, pos)) - 1;
                };

                face1[0] = parseIndex(v1);
                face1[1] = parseIndex(v2);
                face1[2] = parseIndex(v3);

                faces.push_back(face1);

                if (v4 != "")
                {
                    face2[0] = parseIndex(v1);
                    face2[1] = parseIndex(v3);
                    face2[2] = parseIndex(v4);

                    faces.push_back(face2);
                }
            }
        }

        for (Vertex* v : vertices)
            model.addVertex(v);
        for (const auto& f : faces)
            // model.addMesh(Mesh(f[0], f[1], f[2], 255, 255, 255, 255));
            model.addMesh(Mesh(f[0], f[1], f[2], 86, 50, 19, 255)); // brown
        // model.addMesh(Mesh(f[0], f[1], f[2], 255, 141, 31, 255)); // orange

        return model;
    }
} // namespace Renderer