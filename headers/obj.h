#ifndef OBJECT_H
#define OBJECT_H

#include "vec3.h"
#include "mesh.h"

class object
{
public:
    object() {};
    object(mesh mesh, vec3 v) : _mesh(mesh), _pos(v) {};
    mesh _mesh;
    vec3 _pos;
    unsigned int id;
};

#endif // OBJECT_H