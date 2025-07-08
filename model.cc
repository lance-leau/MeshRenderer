#include "model.hh"

#include <algorithm>

#include "renderer.hh"

namespace Renderer
{

    Model::Model(int x, int y, int z)
        : _x(x)
        , _y(y)
        , _z(z)
    {}

    Model::~Model()
    {
        for (Vertex* v : _vertices)
            delete v;
    }

    void Model::draw(SDL_Renderer* renderer, Projector& projector)
    {
        std::vector<ProjectedPoint> projected;
        projected.reserve(_vertices.size());

        for (Vertex* v : _vertices)
        {
            Vertex worldV(v->getX() + _x, v->getY() + _y, v->getZ() + _z);
            projected.push_back(projector.projectPoint(worldV));
        }
        std::vector<Mesh> sortedMeshes = _meshes;

        std::sort(sortedMeshes.begin(), sortedMeshes.end(),
                  [&](Mesh& a, Mesh& b) {
                      float za = (_vertices[a.getVertices()[0]]->getZ()
                                  + _vertices[a.getVertices()[1]]->getZ()
                                  + _vertices[a.getVertices()[2]]->getZ())
                          / 3.0f;

                      float zb = (_vertices[b.getVertices()[0]]->getZ()
                                  + _vertices[b.getVertices()[1]]->getZ()
                                  + _vertices[b.getVertices()[2]]->getZ())
                          / 3.0f;

                      return za > zb; // back to front (higher Z is farther)
                  });

        for (auto& mesh : sortedMeshes)
            drawMesh(mesh, projected, renderer);
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

} // namespace Renderer
