#ifndef BASISFUNCTIONS_H
#define BASISFUNCTIONS_H

#include <GL/glew.h>
#include <cmath>

namespace cagd
{
    // Project type II.

    // zeroth-order derivatives
    GLdouble F0(GLdouble u);
    GLdouble F1(GLdouble u);
    GLdouble F2(GLdouble u);
    GLdouble F3(GLdouble u);

    // first-order derivatives
    GLdouble d1F0(GLdouble u);
    GLdouble d1F1(GLdouble u);
    GLdouble d1F2(GLdouble u);
    GLdouble d1F3(GLdouble u);

    // second-order derivatives
    GLdouble d2F0(GLdouble u);
    GLdouble d2F1(GLdouble u);
    GLdouble d2F2(GLdouble u);
    GLdouble d2F3(GLdouble u);
}

#endif // BASISFUNCTIONS_H
