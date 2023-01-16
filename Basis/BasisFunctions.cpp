#include "BasisFunctions.h"

// Project type II.

// zeroth-order derivatives
GLdouble cagd::F0(GLdouble u)
{
    return F3(1-u);
}

GLdouble cagd::F1(GLdouble u)
{
    return F2(1-u);
}

GLdouble cagd::F2(GLdouble u)
{
    return 4 * pow(u,3) * (1-u) + 3 * pow(u,2) * pow(1-u,2);
}

GLdouble cagd::F3(GLdouble u)
{
    return pow(u,4);
}


// first-order derivatives
GLdouble cagd::d1F0(GLdouble u)
{
    return -d1F3(1-u);
}

GLdouble cagd::d1F1(GLdouble u)
{
    return -d1F2(1-u);
}

GLdouble cagd::d1F2(GLdouble u)
{
    return -2 * u * (2 * u * u + 3 * u - 3);
}

GLdouble cagd::d1F3(GLdouble u)
{
    return 4 * pow(u,3);
}


// second-order derivatives
GLdouble cagd::d2F0(GLdouble u)
{
    return d2F3(1-u);
}

GLdouble cagd::d2F1(GLdouble u)
{
    return d2F2(1-u);
}

GLdouble cagd::d2F2(GLdouble u)
{
    return -12 * u * u - 12 * u + 6;
}

GLdouble cagd::d2F3(GLdouble u)
{
    return 12 * pow(u,2);
}

