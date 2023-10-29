#ifndef MESH_H
#define MESH_H

#include <vector>
#include <SDL2/SDL.h>
#include "vec3.h"

class triangle
{
public:
    triangle() {}
    triangle(vec3 v0, vec3 v1, vec3 v2) : p{v0, v1, v2} {}
    triangle(vec3 v0, vec3 v1, vec3 v2, Uint8 l) : p{v0, v1, v2}, lum(l) {}

    vec3& operator[](int i) { return p[i]; }
    vec3& begin() { return p[0]; }
    vec3& end() { return p[2]; }

    triangle& operator=(const triangle &t) 
    {
        if (this != &t) 
        {
            p = t.getP();
            lum = t.getLum();
        }
    return *this;
    }

    void setP(std::vector<vec3> v) { p = v; }
    std::vector<vec3> getP() const { return p; }
    void setLum(Uint8 l) { lum = l; }
    Uint8 getLum() const { return lum; }

    vec3 normal() const { return (p[1] - p[0]).cross(p[2] - p[0]); }
    int size() const { return p.size(); }

private:
    std::vector<vec3> p{3}; // Vertices
    Uint8 lum; // Luminosity
};

class mesh
{
public:
    mesh() {}
    mesh(std::vector<triangle> t) : tris(t) {}
    ~mesh() {}

    triangle& operator[](int i) { return tris[i]; }
    triangle& begin() { return tris[0]; }
    triangle& end() { return tris[tris.size() - 1]; }

    mesh& operator=(const mesh &m) const 
    {
        mesh *n = new mesh(m.getTris()); 
        return *n;
    }

    std::vector<triangle> getTris() const { return tris; }
    void setTris(std::vector<triangle> t) { tris = t; }
    bool LoadFromObjectFile(std::string filename);

    void addTriangle(triangle t) { tris.push_back(t); }
    int size() const { return tris.size(); }

private:
    std::vector<triangle> tris;
};

bool mesh::LoadFromObjectFile(std::string filename)
{

}

#endif // MESH_H