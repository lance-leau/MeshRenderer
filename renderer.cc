#include "renderer.hh"

#include <algorithm>
#include <cmath>

namespace Renderer
{

    void drawWireframe(Mesh& mesh, std::vector<ProjectedPoint>& projected,
                       SDL_Renderer* renderer)
    {
        auto indices = mesh.getVertices();

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // black

        SDL_RenderDrawLine(renderer, projected[indices[0]]._x,
                           projected[indices[0]]._y, projected[indices[1]]._x,
                           projected[indices[1]]._y);

        SDL_RenderDrawLine(renderer, projected[indices[1]]._x,
                           projected[indices[1]]._y, projected[indices[2]]._x,
                           projected[indices[2]]._y);

        SDL_RenderDrawLine(renderer, projected[indices[2]]._x,
                           projected[indices[2]]._y, projected[indices[0]]._x,
                           projected[indices[0]]._y);
    }

    bool isInsideTriangle(int px, int py, ProjectedPoint& A, ProjectedPoint& B,
                          ProjectedPoint& C)
    {
        int ax = A._x, ay = A._y;
        int bx = B._x, by = B._y;
        int cx = C._x, cy = C._y;

        int v0x = cx - ax, v0y = cy - ay;
        int v1x = bx - ax, v1y = by - ay;
        int v2x = px - ax, v2y = py - ay;

        int dot00 = v0x * v0x + v0y * v0y;
        int dot01 = v0x * v1x + v0y * v1y;
        int dot02 = v0x * v2x + v0y * v2y;
        int dot11 = v1x * v1x + v1y * v1y;
        int dot12 = v1x * v2x + v1y * v2y;

        float denom = static_cast<float>(dot00 * dot11 - dot01 * dot01);
        if (denom == 0.0f)
            return false; // degenerate

        float u = static_cast<float>(dot11 * dot02 - dot01 * dot12) / denom;
        float v = static_cast<float>(dot00 * dot12 - dot01 * dot02) / denom;

        return (u >= 0) && (v >= 0) && (u + v <= 1);
    }

    static inline int edgeFunc(int x0, int y0, int x1, int y1, int x, int y)
    {
        return (y0 - y1) * x + (x1 - x0) * y + x0 * y1 - x1 * y0;
    }

    inline float orientationFactor(float x0, float y0, float z0, float x1,
                                   float y1, float z1, float x2, float y2,
                                   float z2, float ambient = 0.2f)
    {
        float nx = (y1 - y0) * (z2 - z0) - (z1 - z0) * (y2 - y0);
        float ny = (z1 - z0) * (x2 - x0) - (x1 - x0) * (z2 - z0);
        float nz = (x1 - x0) * (y2 - y0) - (y1 - y0) * (x2 - x0);

        float len = std::sqrt(nx * nx + ny * ny + nz * nz);
        if (len > 1e-6f)
        {
            nx /= len;
            ny /= len;
            nz /= len;
        }
        else
        {
            nx = 0;
            ny = 0;
            nz = 1;
        }

        float cosTheta = -nz;
        float diff = (cosTheta > 0.0f ? cosTheta : 0.0f);

        // mix with ambient
        return ambient + (1.0f - ambient) * diff;
    }

    void drawMesh(Mesh& mesh, std::vector<ProjectedPoint>& projected,
                  SDL_Renderer* renderer, std::vector<Vertex>& rotatedWorld)
    {
        auto idx = mesh.getVertices();
        const auto& A = projected[idx[0]];
        const auto& B = projected[idx[1]];
        const auto& C = projected[idx[2]];

        if (A._z <= 0 || B._z <= 0 || C._z <= 0)
            return;

        int x0 = A._x, y0 = A._y;
        int x1 = B._x, y1 = B._y;
        int x2 = C._x, y2 = C._y;

        int area = edgeFunc(x0, y0, x1, y1, x2, y2);
        if (area >= 0)
            return;

        int minX = std::min({ x0, x1, x2 });
        int maxX = std::max({ x0, x1, x2 });
        int minY = std::min({ y0, y1, y2 });
        int maxY = std::max({ y0, y1, y2 });

        auto& world0 = rotatedWorld[idx[0]];
        Vertex& world1 = rotatedWorld[idx[1]];
        Vertex& world2 = rotatedWorld[idx[2]];

        float _x0 = world0.getX(), _y0 = world0.getY(), _z0 = world0.getZ();
        float _x1 = world1.getX(), _y1 = world1.getY(), _z1 = world1.getZ();
        float _x2 = world2.getX(), _y2 = world2.getY(), _z2 = world2.getZ();

        float orient = orientationFactor(_x0, _y0, _z0, _x1, _y1, _z1, _x2, _y2,
                                         _z2, 0.2f);

        Uint8 r = mesh.getColor()[0] * orient;
        Uint8 g = mesh.getColor()[1] * orient;
        Uint8 b = mesh.getColor()[2] * orient;
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);

        for (int y = minY; y <= maxY; ++y)
        {
            for (int x = minX; x <= maxX; ++x)
            {
                int w0 = edgeFunc(x1, y1, x2, y2, x, y);
                int w1 = edgeFunc(x2, y2, x0, y0, x, y);
                int w2 = edgeFunc(x0, y0, x1, y1, x, y);
                if (w0 <= 0 && w1 <= 0 && w2 <= 0)
                {
                    SDL_RenderDrawPoint(renderer, x, y);
                }
            }
        }
    }

} // namespace Renderer
