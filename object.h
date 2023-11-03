#ifndef OBJECT_H
#define OBJECT_H

#include "matrix.h"
#include "vec3.h"
#include "mesh.h"
#include "matrices.h"
#include "sdl_helper.h"
#include "vec_mat.h"

#include <string>

class object
{
public:
    object();
    object(std::string filename) : _filename(filename) 
    { 
        _mesh.LoadFromObjectFile(filename); 
        _position = vec3(0,0,0);
        _rotation = {0, 0, 0};
    };
    object(std::string filename, vec3 position) : _filename(filename), _position(position) 
    { 
        _mesh.LoadFromObjectFile(filename); 
        _rotation = {0, 0, 0};
    };
    object(std::string filename, vec3 position, double rotx, double roty, double rotz) : _filename(filename), _position(position), _rotation({rotx, roty, rotz}) 
    { _mesh.LoadFromObjectFile(filename); };

    ~object() {};

    // House keeping
    void setID(int id) { _id = id; };
    int getID() const { return _id; };

    void setName(std::string name) { _name = name; };
    std::string getName() const { return _name; };

    void setFilename(std::string filename) { _filename = filename; _mesh.LoadFromObjectFile(filename);};
    std::string getFilename() const { return _filename; };

    void move(vec3 position) { _position = position; for ( triangle &t : _mesh.getTris() ) { tranTri(t, _position); } };
    vec3 getPos() const { return _position; };

    mesh getMesh() { return _mesh; };
    mesh inView( vec3 &camera, vec3 &lightDirection );


private:
    int _id;
    std::string _name;
    std::string _filename;
    mesh _mesh;
    vec3 _position;
    std::vector<double> _rotation;
    vec3 _scale;
};

mesh object::inView(vec3 &camera, vec3 &lightDirection)
{
    
    double screen_height = 720/2;
    double screen_width = 1280/2;
    projMat proj(0.1, 1000.0, 90.0, screen_height/screen_width);
    mesh toRaster;

    for (int triIndex = 0; triIndex < _mesh.getTris().size(); triIndex++)
    {
        triangle tri = _mesh[triIndex];

        tranTri(tri, _position);

        vec3 normal = tri.normal().fnorm();
        double facingCam = normal.dot(tri[0] - camera);

        if (facingCam < 0.0)
        {
            // Translation vector
            Uint8 lum = normal.dot(lightDirection) * 255;
            tri.setLum(lum);

            // Project the triangle from 3D to 2D
            tXm(tri, proj);

            // Scale into view
            scaleTri(tri, screen_width, screen_height);

            toRaster.addTriangle(tri);
        }
    }
    toRaster.sort();
    return toRaster;
}

#endif // OBJECT_H