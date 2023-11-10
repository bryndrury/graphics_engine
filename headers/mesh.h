#ifndef MESH_H
#define MESH_H

#include <vector>
#include <SDL2/SDL.h>
#include <fstream>
#include <strstream>
#include <algorithm>
#include "vec3.h"

class triangle
{
public:
    triangle() {}
    triangle(vec3 v0, vec3 v1, vec3 v2) : p{v0, v1, v2}, lum(0) {}
    triangle(vec3 v0, vec3 v1, vec3 v2, Uint8 l) : p{v0, v1, v2}, lum(l) {}
    triangle(std::vector<vec3> v) : p(v), lum(0) {}

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

    vec3 norm() const { return (p[1] - p[0]).cross(p[2] - p[0]); }

    void setP(std::vector<vec3> v) { p = v; }
    std::vector<vec3> getP() const { return p; }
    void setLum(Uint8 l) { lum = l; }
    Uint8 getLum() const { return lum; }

    vec3 normal() const { return (p[1] - p[0]).cross(p[2] - p[0]); }
    int size() const { return p.size(); }
    void out() const { p[0].out(); p[1].out(); p[2].out(); }

    void tranTri(triangle &t , vec3 &v) { for (int i = 0; i < t.size(); i++) { t[i] += v; } }

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
    void sort();

    void setPos(vec3 v) { pos = v; }
    vec3 getPos() const { return pos; }

private:
    std::vector<triangle> tris;
    vec3 pos;
};

bool mesh::LoadFromObjectFile(std::string filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cout << "Error opening file: " << filename << std::endl;
        return false;
    }
    std::vector<vec3> verts;
    while (!file.eof())
    {
        char line[128];
        file.getline(line, 128);
        std::strstream s;
        s << line;
        char junk;
        if (line[0] == 'v')
        {
            double x, y, z;
            s >> junk >> x >> y >> z;
            vec3 v(x, y, z);
            verts.push_back(v);
        }
        if (line[0] == 'f')
        {
            int f[3];
            s >> junk >> f[0] >> f[1] >> f[2];
            addTriangle(triangle(verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1]));
        }
    }
    return true;
}

void mesh::sort() 
{
    // sort triangles by z value
    std::sort(tris.begin(), tris.end(), [](triangle& t1, triangle& t2)
			{
				float z1 = (t1[0].z() + t1[1].z() + t1[2].z()) / 3.0f;
				float z2 = (t2[0].z() + t2[1].z() + t2[2].z()) / 3.0f;
				return z1 > z2;
			});
}

#endif // MESH_H