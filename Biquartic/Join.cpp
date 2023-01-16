#include "BiquarticCompositePatches3.h"
#include "Core/Matrices.h"
#include "Core/Exceptions.h"

using namespace cagd;
using namespace std;

GLboolean BiquarticCompositeSurface3::JoinExistingPatches(GLuint index_0, Direction direction_0, GLuint index_1, Direction direction_1){
    if(index_0 < 0 || index_0 > _attributes.size() || index_1 < 0 || index_1 > _attributes.size()){
        return GL_FALSE;
    }

    InsertNewPatch();
    PatchAttributes* patchAttr = &_attributes.back();
    PatchAttributes* patch0 = &_attributes[index_0];
    PatchAttributes* patch1 = &_attributes[index_1];

    GLuint _dir0;
    GLuint _dir1;

    Matrix<DCoordinate3> vectors_0(4,4);
    Matrix<DCoordinate3> vectors_1(4,4);

    for(GLuint i = 0;i<4;++i){
        vectors_0(0,i)=(*patch0->patch)(0,i)-(*patch0->patch)(1,i);
        vectors_1(0,i)=(*patch1->patch)(0,i)-(*patch1->patch)(1,i);
    }

    for(GLuint i = 0;i<4;++i){
        vectors_0(1,i)=(*patch0->patch)(i,0)-(*patch0->patch)(i,1);
        vectors_1(1,i)=(*patch1->patch)(i,0)-(*patch1->patch)(i,1);
    }

    for(GLuint i = 0;i<4;++i){
        vectors_0(2,i)=(*patch0->patch)(i,3)-(*patch0->patch)(i,2);
        vectors_1(2,i)=(*patch1->patch)(i,3)-(*patch1->patch)(i,2);
    }

    for(GLuint i = 0;i<4;++i){
        vectors_0(3,i)=(*patch0->patch)(3,i)-(*patch0->patch)(2,i);
        vectors_1(3,i)=(*patch1->patch)(3,i)-(*patch1->patch)(2,i);
    }

    switch(direction_0){
    case S:
        _dir0 = 4;
        for(GLuint i=0;i<4;++i){
            (*patchAttr->patch)(0,i) = (*patch0->patch)(3,3-i);
            (*patchAttr->patch)(1,i) = (*patch0->patch)(3,3-i)+vectors_0(3,3-i);
        }
        break;
    case N:
        _dir0 = 0;
        for(GLuint i=0;i<4;++i){
            (*patchAttr->patch)(0,i) = (*patch0->patch)(0,i);
            (*patchAttr->patch)(1,i) = (*patch0->patch)(0,i)+vectors_0(0,i);
        }
        break;
    case W:
        _dir0 = 6;
        for(GLuint i=0;i<4;++i){
            (*patchAttr->patch)(0,i) = (*patch0->patch)(3-i,0);
            (*patchAttr->patch)(1,i) = (*patch0->patch)(3-i,0)+vectors_0(1,3-i);
        }
        break;
    case E:
        _dir0 = 2;
        for(GLuint i=0;i<4;++i){
            (*patchAttr->patch)(0,i) = (*patch0->patch)(i,3);
            (*patchAttr->patch)(1,i) = (*patch0->patch)(i,3)+vectors_0(2,i);
        }
        break;
    case NE:
        _dir0 = 1;
        (*patchAttr->patch)(3,0) = (*patch0->patch)(0,3);
        (*patchAttr->patch)(2,0) = (*patch0->patch)(0,3)+vectors_0(0,3);
        (*patchAttr->patch)(3,1) = (*patch0->patch)(0,3)+vectors_0(2,0);
        break;
    case SE:
        _dir0 = 3;
        (*patchAttr->patch)(3,0) = (*patch0->patch)(3,3);
            (*patchAttr->patch)(2,0) = (*patch0->patch)(3,3)+vectors_0(2,3);
            (*patchAttr->patch)(3,1) = (*patch0->patch)(3,3)+vectors_0(3,3);
        break;
    case SW:
        _dir0 = 5;
        (*patchAttr->patch)(3,0) =(*patch0->patch)(3,0);
            (*patchAttr->patch)(2,0) = (*patch0->patch)(3,0)+vectors_0(3,0);
            (*patchAttr->patch)(3,1) = (*patch0->patch)(3,0)+vectors_0(1,3);
        break;
    case NW:
        _dir0 = 7;
        (*patchAttr->patch)(3,0) = (*patch0->patch)(0,0);
            (*patchAttr->patch)(2,0) = (*patch0->patch)(0,0)+vectors_0(1,0);
            (*patchAttr->patch)(3,1) = (*patch0->patch)(0,0)+vectors_0(0,3);
        break;
    default:
        break;
    }
    if(patch0->neighbours[_dir0]){
        _attributes.pop_back();
        return GL_FALSE;
    }

    switch(direction_1){
    case S:
        _dir1 = 4;
        for(GLuint i=0;i<4;++i){
            (*patchAttr->patch)(3,3-i) = (*patch1->patch)(3,3-i);
            (*patchAttr->patch)(2,3-i) = (*patch1->patch)(3,3-i)+vectors_1(3,3-i);
        }
        break;
    case N:
        _dir1 = 0;
        for(GLuint i=0;i<4;++i){
            (*patchAttr->patch)(3,3-i) = (*patch1->patch)(0,i);
            (*patchAttr->patch)(2,3-i) = (*patch1->patch)(0,i)+vectors_1(0,i);
        }
        break;
    case W:
        _dir1 = 6;
        for(GLuint i=0;i<4;++i){
            (*patchAttr->patch)(3,3-i) = (*patch1->patch)(3-i,0);
            (*patchAttr->patch)(2,3-i) = (*patch1->patch)(3-i,0)+vectors_1(1,3-i);
        }
        break;
    case E:
        _dir1 = 2;
        for(GLuint i=0;i<4;++i){
            (*patchAttr->patch)(3,3-i) = (*patch1->patch)(i,3);
            (*patchAttr->patch)(2,3-i) = (*patch1->patch)(i,3)+vectors_1(2,i);
        }
        break;
    case NE:
        _dir1 = 1;
        (*patchAttr->patch)(0,3) = (*patch1->patch)(0,3);
            (*patchAttr->patch)(1,3) = (*patch1->patch)(0,3)+vectors_1(0,3);
            (*patchAttr->patch)(0,2) = (*patch1->patch)(0,3)+vectors_1(2,0);
        break;
    case SE:
        _dir1 = 3;
        (*patchAttr->patch)(0,3) = (*patch1->patch)(3,3);
            (*patchAttr->patch)(1,3) = (*patch1->patch)(3,3)+vectors_1(2,3);
            (*patchAttr->patch)(0,2) = (*patch1->patch)(3,3)+vectors_1(3,3);
        break;
    case SW:
        _dir1 = 5;
        (*patchAttr->patch)(0,3) = (*patch1->patch)(3,0);
            (*patchAttr->patch)(1,3) = (*patch1->patch)(3,0)+vectors_1(3,0);
            (*patchAttr->patch)(0,2) = (*patch1->patch)(3,0)+vectors_1(1,3);
        break;
    case NW:
        _dir1 = 7;
        (*patchAttr->patch)(0,3) = (*patch1->patch)(0,0);
            (*patchAttr->patch)(1,3) = (*patch1->patch)(0,0)+vectors_1(1,0);
            (*patchAttr->patch)(0,2) = (*patch1->patch)(0,0)+vectors_1(0,3);
        break;
    default:
        break;
    }

    if(patch1->neighbours[_dir1]){
        _attributes.pop_back();
        return GL_FALSE;
    }
    if((_dir1 + _dir0)%2 == 1){
        _attributes.pop_back();
        return GL_FALSE;
    }
    if((_dir1 % 2 == 0) && (_dir0 % 2 == 0) ){
        patch0->neighbours[_dir0]=patchAttr;
        patch1->neighbours[_dir1]=patchAttr;

        patchAttr->neighbours[0]=patch0;
        patchAttr->neighbours[4]=patch1;

        patch0->connection_type[_dir0]=N;
        patch1->connection_type[_dir1]=S;

        patchAttr->connection_type[0]=direction_0;
        patchAttr->connection_type[4]=direction_1;
    }
    if((_dir1 % 2 == 1) && (_dir0 % 2 == 1) ){
        DCoordinate3 up = (*patchAttr->patch)(2,0) - (*patchAttr->patch)(3,0);
        DCoordinate3 right = (*patchAttr->patch)(3,1) - (*patchAttr->patch)(3,0);
        DCoordinate3 down = (*patchAttr->patch)(1,3) - (*patchAttr->patch)(0,3);
        DCoordinate3 left = (*patchAttr->patch)(0,2) - (*patchAttr->patch)(0,3);

        (*patchAttr->patch)(0,0) = ((*patchAttr->patch)(3,0) + 3*up + (*patchAttr->patch)(0,3) + 3*left)/2;
        (*patchAttr->patch)(0,1) = ((*patchAttr->patch)(3,0) + 3*up + 1*right + (*patchAttr->patch)(0,3) + 2*left)/2;
        (*patchAttr->patch)(1,0) = ((*patchAttr->patch)(3,0) + 2*up + 1*down + (*patchAttr->patch)(0,3) + 3*left)/2;
        (*patchAttr->patch)(1,1) = ((*patchAttr->patch)(3,0) + 2*up + 1*right + 1*down + (*patchAttr->patch)(0,3) + 2*left)/2;
        (*patchAttr->patch)(1,2) = ((*patchAttr->patch)(3,0) + 2*up + 2*right + 1*down + (*patchAttr->patch)(0,3) + 1*left)/2;
        (*patchAttr->patch)(2,1) = ((*patchAttr->patch)(3,0) + 1*up + 1*right + 2*down + (*patchAttr->patch)(0,3) + 2*left)/2;
        (*patchAttr->patch)(2,2) = ((*patchAttr->patch)(3,0) + 1*up + 2*right + 2*down + (*patchAttr->patch)(0,3) + 1*left)/2;
        (*patchAttr->patch)(2,3) = ((*patchAttr->patch)(3,0) + 1*up + 3*right + 2*down + (*patchAttr->patch)(0,3))/2;
        (*patchAttr->patch)(3,2) = ((*patchAttr->patch)(3,0) + 2*right + 3*down + (*patchAttr->patch)(0,3) +1*left)/2;
        (*patchAttr->patch)(3,3) = ((*patchAttr->patch)(3,0) + 3*down + (*patchAttr->patch)(0,3) + 3*right)/2;


        patch0->neighbours[_dir0]=patchAttr;
        patch1->neighbours[_dir1]=patchAttr;

        patchAttr->neighbours[5]=patch0;
        patchAttr->neighbours[1]=patch1;

        patch0->connection_type[_dir0]=SW;
        patch1->connection_type[_dir1]=NE;

        patchAttr->connection_type[5]=direction_0;
        patchAttr->connection_type[1]=direction_1;
    }

    Update(*patchAttr);

    return GL_TRUE;
}
