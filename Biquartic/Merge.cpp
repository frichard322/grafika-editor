#include "BiquarticCompositePatches3.h"
#include "Core/Matrices.h"
#include "Core/Exceptions.h"

using namespace cagd;
using namespace std;

GLboolean BiquarticCompositeSurface3::MergeExistingPatches(GLuint index_0, Direction direction_0, GLuint index_1, Direction direction_1){
    if(index_0 < 0 || index_0 > _attributes.size() || index_1 < 0 || index_1 > _attributes.size()){
        return GL_FALSE;
    }

    PatchAttributes* patch0 = &_attributes[index_0];
    PatchAttributes* patch1 = &_attributes[index_1];

    RowMatrix<DCoordinate3> points_0(4);
    RowMatrix<DCoordinate3> points_1(4);

    GLint _dir0;
    GLint _dir1;

    switch(direction_0){
    case N:
        _dir0=0;
        for(GLuint i=0;i<4;++i){
            points_0[i]=(*patch0->patch)(1,i);
        }
        break;
    case E:
        _dir0=2;
        for(GLuint i=0;i<4;++i){
            points_0[i]=(*patch0->patch)(i,2);
        }
        break;
    case W:
        _dir0=6;
        for(GLuint i=0;i<4;++i){
            points_0[i]=(*patch0->patch)(3-i,1);
        }
        break;
    case S:
        _dir0=4;
        for(GLuint i=0;i<4;++i){
            points_0[i]=(*patch0->patch)(2,3-i);
        }
        break;
    case NE:
        _dir0=1;
        points_0[0]=((*patch0->patch)(0,2)+(*patch0->patch)(1,3))/2;
        break;
    case SE:
        _dir0=3;
        points_0[0]=((*patch0->patch)(2,3)+(*patch0->patch)(3,2))/2;
        break;
    case SW:
        _dir0=5;
        points_0[0]=((*patch0->patch)(3,1)+(*patch0->patch)(2,0))/2;
        break;
    case NW:
        _dir0=5;
        points_0[0]=((*patch0->patch)(0,1)+(*patch0->patch)(1,0))/2;
        break;
    default:
        break;
    }

    switch(direction_1){
    case N:
        _dir1=0;
        for(GLuint i=0;i<4;++i){
            points_1[i]=(*patch1->patch)(1,i);
        }
        break;
    case E:
        _dir1=2;
        for(GLuint i=0;i<4;++i){
            points_1[i]=(*patch1->patch)(i,2);
        }
        break;
    case W:
        _dir1=6;
        for(GLuint i=0;i<4;++i){
            points_1[i]=(*patch1->patch)(3-i,1);
        }
        break;
    case S:
        _dir1=4;
        for(GLuint i=0;i<4;++i){
            points_1[i]=(*patch1->patch)(2,3-i);
        }
        break;
    case NE:
        _dir1=1;
        points_1[0]=((*patch1->patch)(0,2)+(*patch1->patch)(1,3))/2;
        break;
    case SE:
        _dir1=3;
        points_1[0]=((*patch1->patch)(2,3)+(*patch1->patch)(3,2))/2;
        break;
    case SW:
        _dir1=5;
        points_1[0]=((*patch1->patch)(3,1)+(*patch1->patch)(2,0))/2;
        break;
    case NW:
        _dir1=5;
        points_1[0]=((*patch1->patch)(0,1)+(*patch1->patch)(1,0))/2;
        break;
    default:
        break;
    }

    //---------
    if((_dir1+_dir0)%2 == 1 || (patch0->neighbours[_dir0]) || (patch1->neighbours[_dir1]) ){
        return GL_FALSE;
    }
    //----------

    switch(direction_1){
    case N:
        for(GLuint i=0;i<4;++i){
            (*patch1->patch)(0,3-i)=(points_0[i]+points_1[3-i])/2;
        }
        break;
    case E:
        for(GLuint i=0;i<4;++i){
           (*patch1->patch)(3-i,3)=(points_0[i]+points_1[3-i])/2;
        }
        break;
    case W:
        for(GLuint i=0;i<4;++i){

           (*patch1->patch)(i,0)=(points_0[i]+points_1[3-i])/2;
        }
        break;
    case S:
        for(GLuint i=0;i<4;++i){
            (*patch1->patch)(3,i)=(points_0[i]+points_1[3-i])/2;

        }
        break;
    case NE:
        (*patch1->patch)(0,3)=(points_0[0]+points_1[0])/2;
        break;
    case SE:
        (*patch1->patch)(3,3)=(points_0[0]+points_1[0])/2;
        break;
    case SW:
        (*patch1->patch)(3,0)=(points_0[0]+points_1[0])/2;
        break;
    case NW:
        (*patch1->patch)(0,0)=(points_0[0]+points_1[0])/2;
        break;
    default:
        break;
    }

    switch(direction_0){
    case N:
        for(GLuint i=0;i<4;++i){
            (*patch0->patch)(0,i)=(points_0[i]+points_1[3-i])/2;
        }
        break;
    case E:
        for(GLuint i=0;i<4;++i){
            (*patch0->patch)(i,3)=(points_0[i]+points_1[3-i])/2;
        }
        break;
    case W:
        for(GLuint i=0;i<4;++i){
            (*patch0->patch)(3-i,0)=(points_0[i]+points_1[3-i])/2;
        }
        break;
    case S:
        for(GLuint i=0;i<4;++i){
            (*patch0->patch)(3,3-i)=(points_0[i]+points_1[3-i])/2;
        }
        break;
    case NE:
        (*patch0->patch)(0,3)=(points_0[0]+points_1[0])/2;
        break;
    case SE:
        (*patch0->patch)(3,3)=(points_0[0]+points_1[0])/2;
        break;
    case SW:
        (*patch0->patch)(3,0)=(points_0[0]+points_1[0])/2;
        break;
    case NW:
        (*patch0->patch)(0,0)=(points_0[0]+points_1[0])/2;
        break;
    default:
        break;
    }

    patch0->neighbours[_dir0]=patch1;
    patch1->neighbours[_dir1]=patch0;

    patch0->connection_type[_dir0]=direction_1;
    patch1->connection_type[_dir1]=direction_0;

    Update(*patch0);
    Update(*patch1);

    for(GLuint i=0;i<8;++i){
        if(patch0->neighbours[i]){
            Connect(patch0,patch0->neighbours[i]);
        }
    }

    for(GLuint i=0;i<8;++i){
        if(patch1->neighbours[i]){
            Connect(patch1,patch1->neighbours[i]);
        }
    }

    return GL_TRUE;
}
