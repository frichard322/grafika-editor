#ifndef BIQUARTICARC3_H
#define BIQUARTICARC3_H

#include "../Core/LinearCombination3.h"
#include "../Basis/BasisFunctions.h"

namespace cagd
{
    class BiquarticArc3: public LinearCombination3
    {
    public:
        BiquarticArc3();

        // inherited pure virtual methods have to be redeclared and defined
        GLboolean BlendingFunctionValues(GLdouble u, RowMatrix<GLdouble> &values) const;
        GLboolean CalculateDerivatives(GLuint max_order_of_derivatives, GLdouble u, Derivatives& d) const;
    };
}
#endif // BIQUARTICARC3_H
