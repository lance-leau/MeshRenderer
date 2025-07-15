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

        // to overlay the wireframe with the mesh
        // drawWireframe(mesh, projected, renderer);
    }

    void clearWithLayeredGradient(SDL_Renderer* renderer, SDL_Window* window)
    {
        int width, height;
        SDL_GetWindowSize(window, &width, &height);

        int skyEnd = (2 * height) / 3;
        int fadeH = height / 40;
        int fadeEnd = std::min(skyEnd + fadeH, height);

        // #0077FF #A6FBFF #607894
        const SDL_Color top = { 0x00, 0x77, 0xFF, 255 };
        const SDL_Color mid = { 0xA6, 0xFB, 0xFF, 255 };
        const SDL_Color ground = { 0x60, 0x78, 0x94, 255 };

        SDL_SetRenderDrawColor(renderer, ground.r, ground.g, ground.b,
                               ground.a);
        SDL_RenderClear(renderer);

        for (int y = 0; y < skyEnd; ++y)
        {
            float t = float(y) / float(skyEnd - 1);
            Uint8 r = Uint8(top.r + t * (mid.r - top.r));
            Uint8 g = Uint8(top.g + t * (mid.g - top.g));
            Uint8 b = Uint8(top.b + t * (mid.b - top.b));
            SDL_SetRenderDrawColor(renderer, r, g, b, 255);
            SDL_RenderDrawLine(renderer, 0, y, width, y);
        }

        for (int y = skyEnd; y < fadeEnd; ++y)
        {
            float t = float(y - skyEnd) / float((fadeEnd - skyEnd) - 1);
            Uint8 r = Uint8(mid.r + t * (ground.r - mid.r));
            Uint8 g = Uint8(mid.g + t * (ground.g - mid.g));
            Uint8 b = Uint8(mid.b + t * (ground.b - mid.b));
            SDL_SetRenderDrawColor(renderer, r, g, b, 255);
            SDL_RenderDrawLine(renderer, 0, y, width, y);
        }
    }
} // namespace Renderer
