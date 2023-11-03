vec3 mXv(vec3 &v, Matrix &m)
{
    std::vector<double> a;
    for (int i = 0; i < 3; i++)
    {
        a.push_back(v[0] * m(0, i) + v[1] * m(1, i) + v[2] * m(2, i) + m(3, i));
    }
    
    double w = v[0] * m(0, 3) + v[1] * m(1, 3) + v[2] * m(2, 3) + m(3, 3);
    if (w != 0)
    {
        a[0] /= w;
        a[1] /= w;
        a[2] /= w;
    }
    vec3 result(a);
    return result;
}

void tXm(triangle &t, Matrix &m)
{
    std::vector<vec3> a;
    for (int i = 0; i < 3; i++)
    {
        a.push_back(mXv(t[i], m));
    }
    triangle result(a);
    t = result;
}

void tranTri(triangle &t , vec3 &v)
{
    for (int i = 0; i < t.size(); i++)
    {
        t[i] += v;
    }
}

void scaleTri(triangle &t, double screen_width, double screen_height)
{
    double scaleX = screen_width * 0.5;
    double scaleY = screen_height * 0.5;

    t[0].setX( ( t[0].x() + 1.0 ) );    t[0].setY( ( t[0].y() + 1.0 ) );
    t[1].setX( ( t[1].x() + 1.0 ) );    t[1].setY( ( t[1].y() + 1.0 ) );
    t[2].setX( ( t[2].x() + 1.0 ) );    t[2].setY( ( t[2].y() + 1.0 ) );

    t[0].setX( ( t[0].x() * scaleX ) );    t[0].setY( ( t[0].y() * scaleY ) );
    t[1].setX( ( t[1].x() * scaleX ) );    t[1].setY( ( t[1].y() * scaleY ) );
    t[2].setX( ( t[2].x() * scaleX ) );    t[2].setY( ( t[2].y() * scaleY ) );
}