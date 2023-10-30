#include <cmath>
#include <chrono>
#include "matrix.h"

// Rotation Matrices that inherit from Matrix
class rotX : public Matrix
{
public:
    rotX(double theta) : Matrix(4, 4)
    {
        std::vector<double> temp {1, 0, 0, 0, 0, cos(theta), -sin(theta), 0, 0, sin(theta), cos(theta), 0, 0, 0, 0, 1};
        setValues(temp);
    }
    void updateTheta(std::chrono::duration<double> fElapstedTime, double rate) 
    {
        theta += rate * fElapstedTime.count();
        setValues({1, 0, 0, 0, 0, cos(theta), -sin(theta), 0, 0, sin(theta), cos(theta), 0, 0, 0, 0, 1});
    }
private:
    double theta = 0.0;
};

class rotY : public Matrix
{
public:
    rotY(double theta) : Matrix(4, 4)
    {
        std::vector<double> temp {cos(theta), 0, sin(theta), 0, 0, 1, 0, 0, -sin(theta), 0, cos(theta), 0, 0, 0, 0, 1};
        setValues(temp);
    }
    void updateTheta(std::chrono::duration<double> fElapstedTime, double rate) 
    {
        theta += rate * fElapstedTime.count(); 
        setValues({cos(theta), 0, sin(theta), 0, 0, 1, 0, 0, -sin(theta), 0, cos(theta), 0, 0, 0, 0, 1});
    }
private:
    double theta = 0.0;
};

class rotZ : public Matrix
{
public:
    rotZ(double theta) : Matrix(4, 4)
    {
        _theta = theta;
        setValues({cos(_theta), -sin(_theta), 0, 0, sin(_theta), cos(_theta), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1});
    }
    void updateTheta(std::chrono::duration<double> fElapstedTime, double rate)
    { 
        _theta += rate * fElapstedTime.count();
        setValues({cos(_theta), -sin(_theta), 0, 0, sin(_theta), cos(_theta), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1});
    }
private:
    double _theta = 0.0;
};

class projMat : public Matrix
{
public:
    projMat(double fNear, double fFar, double fFov, double fAspectRatio) : Matrix(4, 4)
    {
        double fFovRad = 1.0 / tan(fFov * 0.5 / 180.0 * 3.14159);
        _near = fNear;  _far = fFar;    _fovRad = fFovRad;  _aspectRatio = fAspectRatio;
        setValues({_aspectRatio * _fovRad, 0, 0, 0, 0, _fovRad, 0, 0, 0, 0, _far / (_far - _near), 1, 0, 0, (-_far * _near) / (_far - _near), 0});
    }
    void changeFov(double fFov)
    {
        double fFovRad = 1.0 / tan(fFov * 0.5 / 180.0 * 3.14159);
        _fovRad = fFovRad;
        setValues({_aspectRatio * _fovRad, 0, 0, 0, 0, _fovRad, 0, 0, 0, 0, _far / (_far - _near), 1, 0, 0, (-_far * _near) / (_far - _near), 0});
    }
    void changeNear(double fNear)
    {
        _near = fNear;
        setValues({_aspectRatio * _fovRad, 0, 0, 0, 0, _fovRad, 0, 0, 0, 0, _far / (_far - _near), 1, 0, 0, (-_far * _near) / (_far - _near), 0});
    }
    void changeFar(double fFar)
    {
        _far = fFar;
        setValues({_aspectRatio * _fovRad, 0, 0, 0, 0, _fovRad, 0, 0, 0, 0, _far / (_far - _near), 1, 0, 0, (-_far * _near) / (_far - _near), 0});
    }
    void changeAspectRatio(double fAspectRatio)
    {
        _aspectRatio = fAspectRatio;
        setValues({_aspectRatio * _fovRad, 0, 0, 0, 0, _fovRad, 0, 0, 0, 0, _far / (_far - _near), 1, 0, 0, (-_far * _near) / (_far - _near), 0});
    }
    void changeAspectRatio(double screen_height, double screen_width)
    {
        double fAspectRatio = screen_width / screen_height;
        _aspectRatio = fAspectRatio;
        setValues({_aspectRatio * _fovRad, 0, 0, 0, 0, _fovRad, 0, 0, 0, 0, _far / (_far - _near), 1, 0, 0, (-_far * _near) / (_far - _near), 0});
    
    }
private:
    double _near;
    double _far;
    double _fovRad;
    double _aspectRatio;
};