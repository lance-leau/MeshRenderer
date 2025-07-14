#include "model.hh"

#include <algorithm>

#include "renderer.hh"

namespace Renderer
{

    Model::Model(int x, int y, int z)
        : _x(x)
        , _y(y)
        , _z(z)
    {
        _isWireframe = false;
    }

    Model::~Model()
    {
        for (Vertex* v : _vertices)
            delete v;
    }

    void Model::draw(SDL_Renderer* renderer, Projector& projector)
    {
        std::vector<ProjectedPoint> projected;
        projected.reserve(_vertices.size());

        // FIXME rotate vertex

        std::vector<Vertex> rotated;
        rotated.reserve(_vertices.size());
        projected.reserve(_vertices.size());

        for (Vertex* v : _vertices)
        {
            Vertex worldV(v->getX(), v->getY(), v->getZ());
            worldV.rotate(this->getPitch(), this->getYaw());

            worldV.setX(worldV.getX() + _x);
            worldV.setY(worldV.getY() + _y);
            worldV.setZ(worldV.getZ() + _z);

            projected.push_back(projector.projectPoint(worldV));
            rotated.push_back(worldV);
        }
        std::vector<Mesh> sortedMeshes = _meshes;

        std::sort(sortedMeshes.begin(), sortedMeshes.end(),
                  [&](Mesh& a, Mesh& b) {
                      float za = (rotated[a.getVertices()[0]].getZ()
                                  + rotated[a.getVertices()[1]].getZ()
                                  + rotated[a.getVertices()[2]].getZ())
                          / 3.0f;

                      float zb = (rotated[b.getVertices()[0]].getZ()
                                  + rotated[b.getVertices()[1]].getZ()
                                  + rotated[b.getVertices()[2]].getZ())
                          / 3.0f;

                      return za > zb;
                  });
        if (_isWireframe)
        {
            for (auto& mesh : sortedMeshes)
                drawWireframe(mesh, projected, renderer);
        }
        else
        {
            for (auto& mesh : sortedMeshes)
                drawMesh(mesh, projected, renderer, rotated);
        }
    }

    std::vector<Vertex*>& Model::getVertices()
    {
        return _vertices;
    }

    std::vector<Mesh>& Model::getMeshes()
    {
        return _meshes;
    }

    void Model::setPosition(int x, int y, int z)
    {
        _x = x;
        _y = y;
        _z = z;
    }

    std::tuple<int, int, int> Model::getPosition()
    {
        return { _x, _y, _z };
    }

    void Model::addVertex(Vertex* v)
    {
        _vertices.push_back(v);
    }

    void Model::addMesh(const Mesh& m)
    {
        _meshes.push_back(m);
    }

    float Model::getPitch()
    {
        return _pitch;
    }

    float Model::getYaw()
    {
        return _yaw;
    }

    void Model::setPitch(float p)
    {
        _pitch = p;
    }

    void Model::setYaw(float y)
    {
        _yaw = y;
    }

} // namespace Renderer
