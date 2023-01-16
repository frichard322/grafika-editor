#include "BiquarticCompositePatches3.h"

#include "Core/Exceptions.h"

using namespace cagd;
using namespace std;

BiquarticCompositeSurface3::PatchAttributes::PatchAttributes(): patch(new BiquarticPatch3())
{
    image = nullptr;
    material = -1;
    uLines = nullptr;
    vLines = nullptr;
    for(GLuint i = 0; i < 8; i++){
        neighbours[i] = nullptr;
        connection_type[i] = NONE;
    }
}

BiquarticCompositeSurface3::PatchAttributes::PatchAttributes(const PatchAttributes &a)
{
    *this = a;
}

BiquarticCompositeSurface3::PatchAttributes& BiquarticCompositeSurface3::PatchAttributes::operator =(const PatchAttributes &a)
{
    if (this != &a)
    {
        patch = new (nothrow) BiquarticPatch3();
        for(GLuint i = 0; i < 4; i++) {
            for(GLuint j = 0; j < 4; j++) {
                (*patch)(i,j) = (*a.patch)(i,j);
            }
        }
        material = a.material;
        std::copy(a.neighbours, a.neighbours+8, neighbours);
        std::copy(a.connection_type, a.connection_type+8, connection_type);
    }

    return *this;
}

BiquarticCompositeSurface3::PatchAttributes::~PatchAttributes()
{
    if(patch){
        delete patch; patch = nullptr;
    }
    if(image){
        delete image; image = nullptr;
    }
    for (GLuint i = 0; i < 8; i++){
        if(neighbours[i]){
            neighbours[i] = nullptr;
        }
    }
}

BiquarticCompositeSurface3::BiquarticCompositeSurface3(GLuint minimal_patch_count_to_reserved)
{
    _attributes.reserve(minimal_patch_count_to_reserved);
}

GLboolean BiquarticCompositeSurface3::InsertNewPatch()
{
    PatchAttributes newPatchAttr = PatchAttributes();

    newPatchAttr.patch->SetData(0, 0, -2.0, -2.0, 0.0);
    newPatchAttr.patch->SetData(0, 1, -2.0, -1.0, 0.0);
    newPatchAttr.patch->SetData(0, 2, -2.0, 1.0, 0.0);
    newPatchAttr.patch->SetData(0, 3, -2.0, 2.0, 0.0);

    newPatchAttr.patch->SetData(1, 0, -1.0, -2.0, 0.0);
    newPatchAttr.patch->SetData(1, 1, -1.0, -1.0, 0.0);
    newPatchAttr.patch->SetData(1, 2, -1.0, 1.0, 0.0);
    newPatchAttr.patch->SetData(1, 3, -1.0, 2.0, 0.0);

    newPatchAttr.patch->SetData(2, 0, 1.0, -2.0, 0.0);
    newPatchAttr.patch->SetData(2, 1, 1.0, -1.0, 0.0);
    newPatchAttr.patch->SetData(2, 2, 1.0, 1.0, 0.0);
    newPatchAttr.patch->SetData(2, 3, 1.0, 2.0, 0.0);

    newPatchAttr.patch->SetData(3, 0, 2.0, -2.0, 0.0);
    newPatchAttr.patch->SetData(3, 1, 2.0, -1.0, 0.0);
    newPatchAttr.patch->SetData(3, 2, 2.0, 1.0, 0.0);
    newPatchAttr.patch->SetData(3, 3, 2.0, 2.0, 0.0);
    _attributes.push_back(newPatchAttr);

    try {
        Update(_attributes.back());
    }
    catch (Exception& e) {
        e.what();
    }

    return GL_TRUE;
}

GLboolean BiquarticCompositeSurface3::DeleteExistingPatch(GLuint index)
{
    if (index < 0 || index > _attributes.size())
        return GL_FALSE;

    PatchAttributes* patchToDelete = &_attributes[index];

    for (GLuint i = 0; i < 8; i++)
    {
        if (PatchAttributes* patchNeighbour = patchToDelete->neighbours[i])
        {
            switch (patchToDelete->connection_type[i])
            {
            case N:
                patchNeighbour->neighbours[0] = nullptr;
                break;
            case NE:
                patchNeighbour->neighbours[1] = nullptr;
                break;
            case E:
                patchNeighbour->neighbours[2] = nullptr;
                break;
            case SE:
                patchNeighbour->neighbours[3] = nullptr;
                break;
            case S:
                patchNeighbour->neighbours[4] = nullptr;
                break;
            case SW:
                patchNeighbour->neighbours[5] = nullptr;
                break;
            case W:
                patchNeighbour->neighbours[6] = nullptr;
                break;
            case NW:
                patchNeighbour->neighbours[7] = nullptr;
                break;
            default:
                break;
            }
        }
    }

    if(&_attributes[index] != &_attributes.back()) {
        for (GLuint i = 0; i < 8; i++)
        {
            if (PatchAttributes* patchNeighbour = _attributes.back().neighbours[i])
            {
                switch (_attributes.back().connection_type[i])
                {
                case N:
                    patchNeighbour->neighbours[0] = &_attributes[index];
                    break;
                case NE:
                    patchNeighbour->neighbours[1] = &_attributes[index];
                    break;
                case E:
                    patchNeighbour->neighbours[2] = &_attributes[index];
                    break;
                case SE:
                    patchNeighbour->neighbours[3] = &_attributes[index];
                    break;
                case S:
                    patchNeighbour->neighbours[4] = &_attributes[index];
                    break;
                case SW:
                    patchNeighbour->neighbours[5] = &_attributes[index];
                    break;
                case W:
                    patchNeighbour->neighbours[6] = &_attributes[index];
                    break;
                case NW:
                    patchNeighbour->neighbours[7] = &_attributes[index];
                    break;
                default:
                    break;
                }
            }
        }
        _attributes[index] = _attributes.back();
        Update(_attributes[index]);
    }
    _attributes.pop_back();

    return GL_TRUE;
}

void  BiquarticCompositeSurface3::Update(PatchAttributes &patchAttr)
{
    patchAttr.image = patchAttr.patch->GenerateImage(30, 30, GL_STATIC_DRAW);
    if(!patchAttr.image){
        throw Exception("Could not render the patchAttr.image.");
    }
    if (!patchAttr.image->UpdateVertexBufferObjects()){
        throw Exception("Could not update vertex buffer objects for patchAttr.image");
    }
    if (!patchAttr.patch->UpdateVertexBufferObjectsOfData()){
        throw Exception("Could not update vertex buffer objects of data for patchAttr.patch");
    }
    patchAttr.uLines =  patchAttr.patch->GenerateUIsoparametricLines(40,1,50);
    patchAttr.vLines =  patchAttr.patch->GenerateVIsoparametricLines(10,1,50);
    for(GLuint i = 0; i < 40; i++){
        (*patchAttr.uLines)[i]->UpdateVertexBufferObjects(0.1);
    }
    for(GLuint i = 0; i < 10; i++){
        (*patchAttr.vLines)[i]->UpdateVertexBufferObjects(0.1);
    }
}

GLboolean BiquarticCompositeSurface3::RenderAllPatches(GLboolean _textured, QOpenGLTexture* texture, GLboolean _shader_enabled, ShaderProgram &_shader, GLboolean _patch_visibility, GLboolean _pcp_visibility, GLboolean _pcl_visibility, GLboolean _pud_visibility, GLboolean _pvd_visibility, GLboolean _p_isou_visibility, GLboolean _p_isov_visibility, GLboolean _p_normal_visibility) const
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);
    if(_textured){
        texture->bind();
    }
    if(!_textured && texture != nullptr){
        texture->release();
    }
    for (auto it = _attributes.begin(); it != _attributes.end(); ++it){
        if(_patch_visibility){
            if(_shader_enabled){
                _shader.Enable(GL_TRUE);
            }
            if((*it).material != -1) {
                Material material = _materials[(*it).material];
                material.Apply();
                (*it).image->Render();
            } else {
                Material::ApplyDefault();
                (*it).image->Render();
            }
            if(_shader_enabled){
                _shader.Disable();
            }
        }
        if(_pcp_visibility) {
            glColor3f(1.0f,0.0f,0.0f);
            glPointSize(5);
            (*it).patch->RenderData(GL_POINTS);
            glPointSize(1);
        }
        if(_pcl_visibility) {
            glColor3f(1.0f,0.0f,0.0f);
            glLineWidth(3);
            (*it).patch->RenderData(GL_LINE_STRIP);
            glLineWidth(1);
        }
        if(_p_isou_visibility) {
            glColor3f(0.0f,1.0f,0.0f);
            for(GLuint i = 0; i < 40; i++){
                (*(*it).uLines)[i]->RenderDerivatives(0,GL_LINE_STRIP);
                if(_pud_visibility){
                    (*(*it).uLines)[i]->RenderDerivatives(1,GL_LINES);
                }
            }
        }
        if(_p_isov_visibility) {
            glColor3f(0.0f,0.0f,1.0f);
            for(GLuint i = 0; i < 10; i++){
                (*(*it).vLines)[i]->RenderDerivatives(0,GL_LINE_STRIP);
                if(_pvd_visibility){
                    (*(*it).vLines)[i]->RenderDerivatives(1,GL_LINES);
                }
            }
        }
        if(_p_normal_visibility) {
            glColor3f(0.0f,1.0f,1.0f);
            (*it).image->RenderNormals((GLfloat) 0.1);
        }
        if((*it).highlighted){
            glLineWidth(5);
            glColor3f(1.0f,0.0f,0.0f);
            (*(*it).uLines)[0]->RenderDerivatives(0,GL_LINE_STRIP);
            glColor3f(0.0f,1.0f,0.0f);
            (*(*it).uLines)[39]->RenderDerivatives(0,GL_LINE_STRIP);
            glColor3f(1.0f,1.0f,0.0f);
            (*(*it).vLines)[0]->RenderDerivatives(0,GL_LINE_STRIP);
            glColor3f(0.0f,0.0f,1.0f);
            (*(*it).vLines)[9]->RenderDerivatives(0,GL_LINE_STRIP);
            glLineWidth(1);
        }
        glColor3f(1.0f,1.0f,1.0f);
    }
    return GL_TRUE;
}

GLboolean BiquarticCompositeSurface3::RenderSelectedPatch(GLuint index) const
{
    _attributes[index].image->Render();
    return GL_TRUE;
}

GLboolean BiquarticCompositeSurface3::ContinueExistingPatch(GLuint index, Direction direction){
    if(index < 0 || index>= _attributes.size()){
        return GL_FALSE;
    }
    PatchAttributes* patchPrev = &_attributes[index];
    InsertNewPatch();
    PatchAttributes* patchAttr = &_attributes.back();

    DCoordinate3 right_up = (*patchPrev->patch)(0,3)-(*patchPrev->patch)(0,2);
    DCoordinate3 up_right = (*patchPrev->patch)(0,3)-(*patchPrev->patch)(1,3);

    DCoordinate3 left_up = (*patchPrev->patch)(0,0)-(*patchPrev->patch)(0,1);
    DCoordinate3 up_left = (*patchPrev->patch)(0,0)-(*patchPrev->patch)(1,0);

    DCoordinate3 right_down = (*patchPrev->patch)(3,3)-(*patchPrev->patch)(3,2);
    DCoordinate3 down_right = (*patchPrev->patch)(3,3)-(*patchPrev->patch)(2,3);

    DCoordinate3 left_down = (*patchPrev->patch)(3,0)-(*patchPrev->patch)(3,1);
    DCoordinate3 down_left = (*patchPrev->patch)(3,0)-(*patchPrev->patch)(2,0);


    switch(direction){
        case N:
            if(patchPrev->neighbours[0]){
                _attributes.pop_back();
                return GL_FALSE;
            }
            for(GLuint i=0;i<4;++i){
                (*patchAttr->patch)(3-i,0)= (*patchPrev->patch)(0,0)+i*((*patchPrev->patch)(0,0)-(*patchPrev->patch)(1,0));
                (*patchAttr->patch)(3-i,1)= (*patchPrev->patch)(0,1)+i*((*patchPrev->patch)(0,1)-(*patchPrev->patch)(1,1));
                (*patchAttr->patch)(3-i,2)= (*patchPrev->patch)(0,2)+i*((*patchPrev->patch)(0,2)-(*patchPrev->patch)(1,2));
                (*patchAttr->patch)(3-i,3)= (*patchPrev->patch)(0,3)+i*((*patchPrev->patch)(0,3)-(*patchPrev->patch)(1,3));
            }
            patchAttr->connection_type[4]=N;
            patchPrev->connection_type[0]=S;
            patchPrev->neighbours[0]= patchAttr;
            patchAttr->neighbours[4]= patchPrev;
        break;
        case NE:

        if(patchPrev->neighbours[1]){
            _attributes.pop_back();
            return GL_FALSE;
        }

        for(GLuint i=0;i<4;++i){
            for(GLuint j=0;j<4;++j){
                (*patchAttr->patch)(3-i,j) = (*patchPrev->patch)(0,3) + j*right_up + i*up_right;
            }
        }
        patchAttr->connection_type[5]=NE;
        patchPrev->connection_type[1]=SW;
        patchPrev->neighbours[1]= patchAttr;
        patchAttr->neighbours[5]= patchPrev;
        break;
        case E:

        if( patchPrev->neighbours[2]){
            _attributes.pop_back();
            return GL_FALSE;
        }
        for(GLuint i=0;i<4;++i){
            (*patchAttr->patch)(3-i,0)= (*patchPrev->patch)(0,3)+i*((*patchPrev->patch)(0,3)-(*patchPrev->patch)(0,2));
            (*patchAttr->patch)(3-i,1)= (*patchPrev->patch)(1,3)+i*((*patchPrev->patch)(1,3)-(*patchPrev->patch)(1,2));
            (*patchAttr->patch)(3-i,2)= (*patchPrev->patch)(2,3)+i*((*patchPrev->patch)(2,3)-(*patchPrev->patch)(2,2));
            (*patchAttr->patch)(3-i,3)= (*patchPrev->patch)(3,3)+i*((*patchPrev->patch)(3,3)-(*patchPrev->patch)(3,2));
        }
        patchAttr->connection_type[4]=E;
        patchPrev->connection_type[2]=S;
        patchPrev->neighbours[2]= patchAttr;
        patchAttr->neighbours[4]= patchPrev;
        break;
        case SE:
        if(patchPrev->neighbours[3]){
            _attributes.pop_back();
            return GL_FALSE;
        }
        for(GLuint i=0;i<4;++i){
            for(GLuint j=0;j<4;++j){
                (*patchAttr->patch)(i,j) = (*patchPrev->patch)(3,3) + j*right_down + i*down_right;
            }
        }
        patchAttr->connection_type[7]=SE;
        patchPrev->connection_type[3]=NW;
        patchPrev->neighbours[3]= patchAttr;
        patchAttr->neighbours[7]= patchPrev;
        break;
        case S:
        if(patchPrev->neighbours[4]){
            _attributes.pop_back();
            return GL_FALSE;
        }
        for(GLuint i=0;i<4;++i){
            (*patchAttr->patch)(3-i,0)= (*patchPrev->patch)(3,3)+i*((*patchPrev->patch)(3,3)-(*patchPrev->patch)(2,3));
            (*patchAttr->patch)(3-i,1)= (*patchPrev->patch)(3,2)+i*((*patchPrev->patch)(3,2)-(*patchPrev->patch)(2,2));
            (*patchAttr->patch)(3-i,2)= (*patchPrev->patch)(3,1)+i*((*patchPrev->patch)(3,1)-(*patchPrev->patch)(2,1));
            (*patchAttr->patch)(3-i,3)= (*patchPrev->patch)(3,0)+i*((*patchPrev->patch)(3,0)-(*patchPrev->patch)(2,0));
        }
        patchAttr->connection_type[4]=S;
        patchPrev->connection_type[4]=S;
        patchPrev->neighbours[4]= patchAttr;
        patchAttr->neighbours[4]= patchPrev;
        break;
        case SW:
        if(patchPrev->neighbours[5]){
            _attributes.pop_back();
            return GL_FALSE;
        }
        for(GLuint i=0;i<4;++i){
            for(GLuint j=0;j<4;++j){
                (*patchAttr->patch)(i,3-j) = (*patchPrev->patch)(3,0) + j*left_down + i*down_left;
            }
        }
        patchAttr->connection_type[1]=SW;
        patchPrev->connection_type[5]=NE;
        patchPrev->neighbours[5]= patchAttr;
        patchAttr->neighbours[1]= patchPrev;
        break;
        case W:
        if(patchPrev->neighbours[6]){
            _attributes.pop_back();
            return GL_FALSE;
        }
        for(GLuint i=0;i<4;++i){
            (*patchAttr->patch)(3-i,0)= (*patchPrev->patch)(3,0)+i*((*patchPrev->patch)(3,0)-(*patchPrev->patch)(3,1));
            (*patchAttr->patch)(3-i,1)= (*patchPrev->patch)(2,0)+i*((*patchPrev->patch)(2,0)-(*patchPrev->patch)(2,1));
            (*patchAttr->patch)(3-i,2)= (*patchPrev->patch)(1,0)+i*((*patchPrev->patch)(1,0)-(*patchPrev->patch)(1,1));
            (*patchAttr->patch)(3-i,3)= (*patchPrev->patch)(0,0)+i*((*patchPrev->patch)(0,0)-(*patchPrev->patch)(0,1));
        }
        patchAttr->connection_type[4]=W;
        patchPrev->connection_type[6]=S;
        patchPrev->neighbours[6]= patchAttr;
        patchAttr->neighbours[4]= patchPrev;
        break;
        case NW:
        if(patchPrev->neighbours[7]){
            _attributes.pop_back();
            return GL_FALSE;
        }
        for(GLuint i=0;i<4;++i){
            for(GLuint j=0;j<4;++j){
                (*patchAttr->patch)(3-i,3-j) = (*patchPrev->patch)(0,0) + j*left_up + i*up_left;
            }
        }
        patchAttr->connection_type[2]=NW;
        patchPrev->connection_type[7]=SE;
        patchPrev->neighbours[7]= patchAttr;
        patchAttr->neighbours[2]= patchPrev;
        break;
        default:
            _attributes.pop_back();
            return GL_FALSE;
    }
    Update(*patchAttr);
    return GL_TRUE;
}
