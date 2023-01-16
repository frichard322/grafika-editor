#include "BiquarticArc3.h"
using namespace cagd;

BiquarticArc3::BiquarticArc3():
    LinearCombination3(0.0, 1.0, 4)
{
}

GLboolean BiquarticArc3::BlendingFunctionValues(GLdouble u, RowMatrix<GLdouble> &values) const
{
    if(u < 0.0 || u > 1.0){
        return GL_FALSE;
    }

    values.ResizeColumns(4);
    values[0] = F0(u);
    values[1] = F1(u);
    values[2] = F2(u);
    values[3] = F3(u);

    return GL_TRUE;
}

GLboolean BiquarticArc3::CalculateDerivatives(GLuint max_order_of_derivatives, GLdouble u, Derivatives& d) const
{
    if(u < 0.0 || u > 1.0){
        return GL_FALSE;
    }

    d.ResizeRows(max_order_of_derivatives + 1);
    d.LoadNullVectors();

    Matrix<GLdouble> d_values(max_order_of_derivatives + 1, 4);
    d_values(0,0) = F0(u);
    d_values(0,1) = F1(u);
    d_values(0,2) = F2(u);
    d_values(0,3) = F3(u);

    if(max_order_of_derivatives >= 1){
        d_values(1,0) = d1F0(u);
        d_values(1,1) = d1F1(u);
        d_values(1,2) = d1F2(u);
        d_values(1,3) = d1F3(u);
    }

    if(max_order_of_derivatives >= 2){
        d_values(2,0) = d2F0(u);
        d_values(2,1) = d2F1(u);
        d_values(2,2) = d2F2(u);
        d_values(2,3) = d2F3(u);
    }

    for(GLuint i = 0; i <= max_order_of_derivatives; i++){
        for(GLuint j = 0; j < _data.GetRowCount(); j++){
            d[i] += _data[j] * d_values(i,j);
        }
    }

    return GL_TRUE;
}
