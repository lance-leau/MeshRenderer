#include "renderer.hh"

#include <algorithm>
#include <cmath>

namespace Renderer
{
    float Mesh::closest = 100000;
    float Mesh::furthest = 0;
    float Mesh::buffed_closest = 100000;
    float Mesh::buffed_furthest = 0;

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

    void drawMesh(Mesh& mesh, std::vector<ProjectedPoint>& projected,
                  SDL_Renderer* renderer)
    {
        auto indices = mesh.getVertices();
        ProjectedPoint& A = projected[indices[0]];
        ProjectedPoint& B = projected[indices[1]];
        ProjectedPoint& C = projected[indices[2]];

        float dist = (float)A._z;

        if (dist < 0)
            dist *= -1;

        if (dist < mesh.buffed_closest)
            mesh.buffed_closest = dist;
        if (dist > mesh.buffed_furthest)
            mesh.buffed_furthest = dist;

        // std::cout << dist << " : " << mesh.closest << " : "
        //           << (-dist + mesh.furthest) / (mesh.furthest - mesh.closest)
        //           << '\n';

        float max = mesh.furthest * 1.1f;
        float min = mesh.closest * 0.8f;

        Uint8 r = mesh.getColor()[0] * ((-dist + max) / (max - min));
        Uint8 g = mesh.getColor()[1] * (-dist + max) / (max - min);
        Uint8 b = mesh.getColor()[2] * (-dist + max) / (max - min);
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);

        // FIXME this is horrible, there is one bounding box and iteration for
        // every mesh !!! optimize by only placin each pixel once per freame
        int minX = std::min({ A._x, B._x, C._x });
        int maxX = std::max({ A._x, B._x, C._x });
        int minY = std::min({ A._y, B._y, C._y });
        int maxY = std::max({ A._y, B._y, C._y });

        for (int y = minY; y <= maxY; ++y)
        {
            for (int x = minX; x <= maxX; ++x)
            {
                if (isInsideTriangle(x, y, A, B, C))
                {
                    SDL_RenderDrawPoint(renderer, x, y);
                }
            }
        }
    }

} // namespace Renderer
