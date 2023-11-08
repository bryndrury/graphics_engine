// Multiply matrix by vector
vec3 mXv(vec3 &v, Matrix &m)
{
    vec3 result(    (v[0] * m(0, 0) + v[1] * m(1, 0) + v[2] * m(2, 0) + m(3, 0))    ,
                    (v[0] * m(0, 1) + v[1] * m(1, 1) + v[2] * m(2, 1) + m(3, 1))    ,
                    (v[0] * m(0, 2) + v[1] * m(1, 2) + v[2] * m(2, 2) + m(3, 2))    );

    double w = v[0] * m(0, 3) + v[1] * m(1, 3) + v[2] * m(2, 3) + m(3, 3);
    
    if (w != 0) { result /= w; }

    return result;
}

// Multiply triangle by matrix
void tXm(triangle &t, Matrix &m)
{
    t = triangle(
        mXv(t[0], m),
        mXv(t[1], m),
        mXv(t[2], m)
        );
}

// Translate triangle by vector 
void tranTri(triangle &t , vec3 &v)
{
    t[0] += v; 
    t[1] += v; 
    t[2] += v;
}

void scaleTri(triangle &t, const double &screen_width, const double &screen_height)
{
    double scaleX = screen_width * 0.5;
    double scaleY = screen_height * 0.5;

    // triangle temp(  vec3( (t[0].x() + 1.0)*scaleX, (t[0].y() + 1.0)*scaleY, t[0].z() ),
    //                 vec3( (t[1].x() + 1.0)*scaleY, (t[1].y() + 1.0)*scaleY, t[1].z() ),
    //                 vec3( (t[2].x() + 1.0)*scaleY, (t[2].y() + 1.0)*scaleY, t[2].z() )
    //             );
    // t = temp;

    t[0].setX( ( t[0].x() + 1.0 ) );    t[0].setY( ( t[0].y() + 1.0 ) );
    t[1].setX( ( t[1].x() + 1.0 ) );    t[1].setY( ( t[1].y() + 1.0 ) );
    t[2].setX( ( t[2].x() + 1.0 ) );    t[2].setY( ( t[2].y() + 1.0 ) );

    t[0].setX( ( t[0].x() * scaleX ) );    t[0].setY( ( t[0].y() * scaleY ) );
    t[1].setX( ( t[1].x() * scaleX ) );    t[1].setY( ( t[1].y() * scaleY ) );
    t[2].setX( ( t[2].x() * scaleX ) );    t[2].setY( ( t[2].y() * scaleY ) );
}