#include "BiquarticPatch3.h"
using namespace cagd;

BiquarticPatch3::BiquarticPatch3():
    TensorProductSurface3(0.0, 1.0, 0.0, 1.0, 4, 4)
{
}

GLboolean BiquarticPatch3::UBlendingFunctionValues(GLdouble u_knot, RowMatrix<GLdouble>& blending_values) const
{
    if(u_knot < 0.0 || u_knot > 1.0){
        return GL_FALSE;
    }

    blending_values.ResizeColumns(4);
    blending_values[0] = F0(u_knot);
    blending_values[1] = F1(u_knot);
    blending_values[2] = F2(u_knot);
    blending_values[3] = F3(u_knot);

    return GL_TRUE;
}

GLboolean BiquarticPatch3::VBlendingFunctionValues(GLdouble v_knot, RowMatrix<GLdouble>& blending_values) const
{
    if(v_knot < 0.0 || v_knot > 1.0){
        return GL_FALSE;
    }

    blending_values.ResizeColumns(4);
    blending_values[0] = F0(v_knot);
    blending_values[1] = F1(v_knot);
    blending_values[2] = F2(v_knot);
    blending_values[3] = F3(v_knot);

    return GL_TRUE;
}

GLboolean BiquarticPatch3::CalculatePartialDerivatives(GLuint maximum_order_of_partial_derivatives, GLdouble u, GLdouble v, PartialDerivatives& pd) const
{
    if(u < 0.0 || u > 1.0 || v < 0.0 || v > 1.0 || maximum_order_of_partial_derivatives > 1){
        return GL_FALSE;
    }

    Matrix<GLdouble> blending_values(2,4), d1_blending_values(2,4);
    // 0 row index for u
    blending_values(0,0) = F0(u); d1_blending_values(0,0) = d1F0(u);
    blending_values(0,1) = F1(u); d1_blending_values(0,1) = d1F1(u);
    blending_values(0,2) = F2(u); d1_blending_values(0,2) = d1F2(u);
    blending_values(0,3) = F3(u); d1_blending_values(0,3) = d1F3(u);
    // 1 row index for v
    blending_values(1,0) = F0(v); d1_blending_values(1,0) = d1F0(v);
    blending_values(1,1) = F1(v); d1_blending_values(1,1) = d1F1(v);
    blending_values(1,2) = F2(v); d1_blending_values(1,2) = d1F2(v);
    blending_values(1,3) = F3(v); d1_blending_values(1,3) = d1F3(v);

    pd.ResizeRows(2);
    pd.LoadNullVectors();

    for(GLuint i = 0; i < 4; i++){
        DCoordinate3 d0_v, d1_v;
        for(GLuint j = 0; j < 4; j++){
            d0_v += _data(i,j) * blending_values(1,j);
            d1_v += _data(i,j) * d1_blending_values(1,j);
        }

        pd(0,0) += d0_v * blending_values(0,i);
        pd(1,0) += d0_v * d1_blending_values(0,i);
        pd(1,1) += d1_v * blending_values(0,i);
    }

    return GL_TRUE;
}
