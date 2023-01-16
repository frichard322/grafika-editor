#ifndef BIQUARTICPATCH3_H
#define BIQUARTICPATCH3_H

#include "../Core/TensorProductSurfaces3.h"
#include "../Basis/BasisFunctions.h"

namespace cagd
{
    class BiquarticPatch3: public TensorProductSurface3
    {
    public:
        BiquarticPatch3();
        GLboolean UBlendingFunctionValues(GLdouble u_knot, RowMatrix<GLdouble>& blending_values) const;
        GLboolean VBlendingFunctionValues(GLdouble v_knot, RowMatrix<GLdouble>& blending_values) const;
        GLboolean CalculatePartialDerivatives(GLuint maximum_order_of_partial_derivatives, GLdouble u, GLdouble v, PartialDerivatives& pd) const;
    };
}

#endif // BIQUARTICPATCH3_H
