#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <GL/glew.h>
#include "TriangulatedMeshes3.h"

using namespace std;

namespace cagd
{
    class Properties
    {
    public:
        DCoordinate3 angle, scale, trans, color; // rotation, scaling, translation
        GLuint type;

    public:
        Properties();
        Properties(DCoordinate3 angle, DCoordinate3 scale, DCoordinate3 trans, DCoordinate3 color, GLuint type);
        friend ostream& operator <<(ostream& lhs, const Properties& rhs);
        friend istream& operator >>(istream& lhs, Properties& rhs);
    };

    inline Properties::Properties()
    {}

    inline Properties::Properties(DCoordinate3 angle, DCoordinate3 scale, DCoordinate3 trans, DCoordinate3 color, GLuint type)
    {
        this->angle = angle;
        this->scale = scale;
        this->trans = trans;
        this->color = color;
        this->type = type;
    }

    // output to stream
    inline ostream& operator <<(ostream& lhs, const Properties& rhs)
    {
        return lhs << "Rotation: " << rhs.angle << endl
                   << "Scaling: " << rhs.scale << endl
                   << "Translation: " << rhs.trans << endl
                   << "Color: " << rhs.color << endl
                   << "Type: " << rhs.type << endl;
    }

    // input from stream
    inline istream& operator >>(istream& lhs, Properties& rhs)
    {
        return lhs >> rhs.angle >> rhs.scale >> rhs.trans >> rhs.color >> rhs.type;
    }
}

#endif // PROPERTIES_H
