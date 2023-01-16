#include "BiquarticCompositePatches3.h"
#include "Core/Matrices.h"
#include "Core/Exceptions.h"

using namespace cagd;
using namespace std;

void BiquarticCompositeSurface3::Connect(PatchAttributes* good, PatchAttributes* needsChange){

    std::vector<int> indexes;
    for(GLuint i=0;i<8;++i){
        if(needsChange->neighbours[i] == good){
            indexes.push_back(i);
        }
    }
    for(auto index : indexes){
        Direction neighbourDir = needsChange->connection_type[index];
        Matrix<DCoordinate3> newPoints(2,4);
        switch(neighbourDir){
        case N:
            for(GLuint i=0;i<4;++i){
                newPoints(0,i)=(*good->patch)(0,i);
                newPoints(1,i)=2*(*good->patch)(0,i)-(*good->patch)(1,i);
            }
            break;
        case E:
            for(GLuint i=0;i<4;++i){
                newPoints(0,i)=(*good->patch)(i,3);
                newPoints(1,i)=2*(*good->patch)(i,3)-(*good->patch)(i,2);
            }
            break;
        case S:
            for(GLuint i=0;i<4;++i){
                newPoints(0,3-i)=(*good->patch)(3,i);
                newPoints(1,3-i)=2*(*good->patch)(3,i)-(*good->patch)(2,i);
            }
            break;
        case W:
            for(GLuint i=0;i<4;++i){
                newPoints(0,3-i)=(*good->patch)(i,0);
                newPoints(1,3-i)=2*(*good->patch)(i,0)-(*good->patch)(i,1);
            }
            break;
        case NE:
                newPoints(0,0)=(*good->patch)(0,3);
                newPoints(1,0)=2*(*good->patch)(0,3)-(*good->patch)(0,2);
                newPoints(1,1)=2*(*good->patch)(0,3)-(*good->patch)(1,3);
            break;
        case SE:
                newPoints(0,0)=(*good->patch)(3,3);
                newPoints(1,0)=2*(*good->patch)(3,3)-(*good->patch)(2,3);
                newPoints(1,1)=2*(*good->patch)(3,3)-(*good->patch)(3,2);
            break;
        case SW:
                newPoints(0,0)=(*good->patch)(3,0);
                newPoints(1,0)=2*(*good->patch)(3,0)-(*good->patch)(3,1);
                newPoints(1,1)=2*(*good->patch)(3,0)-(*good->patch)(2,0);
            break;
        case NW:
                newPoints(0,0)=(*good->patch)(0,0);
                newPoints(1,0)=2*(*good->patch)(0,0)-(*good->patch)(1,0);
                newPoints(1,1)=2*(*good->patch)(0,0)-(*good->patch)(0,1);
            break;
        default:
            break;
        }

        switch(index){
        case 0:
            for(GLuint i=0;i<4;++i){
                (*needsChange->patch)(0,3-i)=newPoints(0,i);
                (*needsChange->patch)(1,3-i)=newPoints(1,i);
            }
            break;
        case 2:
            for(GLuint i=0;i<4;++i){
               (*needsChange->patch)(3-i,3)=newPoints(0,i);
               (*needsChange->patch)(3-i,2)=newPoints(1,i);
            }
            break;
        case 4:
            for(GLuint i=0;i<4;++i){
               (*needsChange->patch)(3,3-i)=newPoints(0,3-i);
               (*needsChange->patch)(2,3-i)=newPoints(1,3-i);
            }
            break;
        case 6:
            for(GLuint i=0;i<4;++i){
               (*needsChange->patch)(3-i,0)=newPoints(0,3-i);
               (*needsChange->patch)(3-i,1)=newPoints(1,3-i);
            }
            break;
        case 1:
            (*needsChange->patch)(0,3)=newPoints(0,0);
            (*needsChange->patch)(0,2)=newPoints(1,0);
            (*needsChange->patch)(1,3)=newPoints(1,1);
            break;
        case 3:
            (*needsChange->patch)(3,3)=newPoints(0,0);
            (*needsChange->patch)(2,3)=newPoints(1,0);
            (*needsChange->patch)(3,2)=newPoints(1,1);
            break;
        case 5:
            (*needsChange->patch)(3,0)=newPoints(0,0);
            (*needsChange->patch)(3,1)=newPoints(1,0);
            (*needsChange->patch)(2,0)=newPoints(1,1);
            break;
        case 7:
            (*needsChange->patch)(0,0)=newPoints(0,0);
            (*needsChange->patch)(1,0)=newPoints(1,0);
            (*needsChange->patch)(0,1)=newPoints(1,1);
            break;
        }
    }
    Update(*needsChange);
}

void BiquarticCompositeSurface3::UpdateControlPoint(GLuint index, GLuint row, GLuint col, GLdouble x, GLdouble y, GLdouble z){

    if(index < 0 || index > _attributes.size() - 1){
        return;
    }

    PatchAttributes* patchAttr = &_attributes[index];
    std::vector<PatchAttributes*> neighbours_to_update;
    DCoordinate3 sub(x-(*patchAttr->patch)(row,col).x(),y-(*patchAttr->patch)(row,col).y(),z-(*patchAttr->patch)(row,col).z());//Mennyivel mozgott a pont

    //Vectorba berakod a frissitendo szomszedokat

    if((row == 1 && col == 0)){
        neighbours_to_update.push_back((*patchAttr).neighbours[6]);
        neighbours_to_update.push_back((*patchAttr).neighbours[7]);
        neighbours_to_update.push_back((*patchAttr).neighbours[0]);
    }
    if((row == 2 && col == 0) || (row == 3 && col == 1)) {
        neighbours_to_update.push_back((*patchAttr).neighbours[6]);
        neighbours_to_update.push_back((*patchAttr).neighbours[5]);
        neighbours_to_update.push_back((*patchAttr).neighbours[4]);
    }
    if((row == 0 && col == 1)){
        neighbours_to_update.push_back((*patchAttr).neighbours[0]);
        neighbours_to_update.push_back((*patchAttr).neighbours[7]);
        neighbours_to_update.push_back((*patchAttr).neighbours[6]);
    }
    if((row == 0 && col == 2) || (row == 1 && col == 3)){
        neighbours_to_update.push_back((*patchAttr).neighbours[0]);
        neighbours_to_update.push_back((*patchAttr).neighbours[1]);
        neighbours_to_update.push_back((*patchAttr).neighbours[2]);
    }
    if((row == 2 && col == 3) || (row == 3 && col == 2)){
        neighbours_to_update.push_back((*patchAttr).neighbours[2]);
        neighbours_to_update.push_back((*patchAttr).neighbours[3]);
        neighbours_to_update.push_back((*patchAttr).neighbours[4]);
    }

    if((row == 1 && col == 1)){
        neighbours_to_update.push_back((*patchAttr).neighbours[0]);
        neighbours_to_update.push_back((*patchAttr).neighbours[6]);
    }

    if((row == 1 && col == 2)){
        neighbours_to_update.push_back((*patchAttr).neighbours[0]);
        neighbours_to_update.push_back((*patchAttr).neighbours[2]);
    }

    if((row == 2 && col == 2)){
        neighbours_to_update.push_back((*patchAttr).neighbours[2]);
        neighbours_to_update.push_back((*patchAttr).neighbours[4]);
    }

    if((row == 2 && col == 1)){
        neighbours_to_update.push_back((*patchAttr).neighbours[5]);
        neighbours_to_update.push_back((*patchAttr).neighbours[4]);
    }

    if((row == 0 && col == 0)){
        neighbours_to_update.push_back((*patchAttr).neighbours[6]);
        neighbours_to_update.push_back((*patchAttr).neighbours[7]);
        neighbours_to_update.push_back((*patchAttr).neighbours[0]);
    }

    if((row == 0 && col == 3)){
        neighbours_to_update.push_back((*patchAttr).neighbours[2]);
        neighbours_to_update.push_back((*patchAttr).neighbours[1]);
        neighbours_to_update.push_back((*patchAttr).neighbours[0]);
    }

    if((row == 3 && col == 3)){
        neighbours_to_update.push_back((*patchAttr).neighbours[2]);
        neighbours_to_update.push_back((*patchAttr).neighbours[3]);
        neighbours_to_update.push_back((*patchAttr).neighbours[4]);
    }

    if((row == 3 && col == 0)){
        neighbours_to_update.push_back((*patchAttr).neighbours[6]);
        neighbours_to_update.push_back((*patchAttr).neighbours[5]);
        neighbours_to_update.push_back((*patchAttr).neighbours[4]);
    }

    (*patchAttr->patch)(row,col).x() = x;
    (*patchAttr->patch)(row,col).y() = y;
    (*patchAttr->patch)(row,col).z() = z;

    if((row == 1 && col == 0) || (row == 2 && col == 0)){
        (*patchAttr->patch)(row,col+1) += sub;
    }
    if((row == 0 && col == 1) || (row == 0 && col == 2)){
        (*patchAttr->patch)(row+1,col) += sub;
    }
    if((row == 1 && col == 3) || (row == 2 && col == 3)){
        (*patchAttr->patch)(row,col-1) += sub;
    }
    if((row == 3 && col == 1) || (row == 3 && col == 2)){
        (*patchAttr->patch)(row-1,col) += sub;
    }

    if((row == 0 && col == 0)){
        (*patchAttr->patch)(row+1,col) += sub;
        (*patchAttr->patch)(row,col+1) += sub;
    }

    if((row == 0 && col == 3)){
        (*patchAttr->patch)(row+1,col) += sub;
        (*patchAttr->patch)(row,col-1) += sub;
    }

    if((row == 3 && col == 3)){
        (*patchAttr->patch)(row-1,col) += sub;
        (*patchAttr->patch)(row,col-1) += sub;
    }

    if((row == 3 && col == 0)){
        (*patchAttr->patch)(row-1,col) += sub;
        (*patchAttr->patch)(row,col+1) += sub;
    }

    Update(*patchAttr);

    for(auto it : neighbours_to_update){
        if(it){
            Connect(patchAttr,it);
        }
    }
}
