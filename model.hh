#pragma once
#include <array>
#include <string>
#include <vector>

#include "mesh.hh"
#include "projector.hh"
#include "renderer.hh"
#include "vertex.hh"

namespace Renderer
{
    class Model
    {
    private:
        int _x;
        int _y;
        int _z;

        float _pitch;
        float _yaw;

        std::vector<Vertex*> _vertices;
        std::vector<Mesh> _meshes;

    public:
        Model(int x, int y, int z);
        ~Model();

        float getPitch();
        float getYaw();

        void setPitch(float p);
        void setYaw(float y);

        void addVertex(Vertex* v);
        void addMesh(const Mesh& m);

        void draw(SDL_Renderer* renderer, Projector& projector);

        std::vector<Vertex*>& getVertices();
        std::vector<Mesh>& getMeshes();

        void setPosition(int x, int y, int z);
        std::tuple<int, int, int> getPosition();

        bool _isWireframe;
    };
} // namespace Renderer