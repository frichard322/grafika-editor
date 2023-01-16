#include "BiquarticCompositeCurve3.h"

using namespace cagd;
using namespace std;


BiquarticCompositeCurve3::ArcAttributes::ArcAttributes(): arc(new BiquarticArc3())
{
    image = nullptr;
    color = new Color4(1.0f, 1.0f, 1.0f);
    previous = nullptr;
    next = nullptr;
    previous_connection_type = next_connection_type = NONE;
}

BiquarticCompositeCurve3::ArcAttributes::ArcAttributes(const ArcAttributes &a):
    previous(a.previous),
    next(a.next),
    previous_connection_type(a.previous_connection_type),
    next_connection_type(a.next_connection_type)
{
   *this = a;
}

BiquarticCompositeCurve3::ArcAttributes& BiquarticCompositeCurve3::ArcAttributes::operator =(const ArcAttributes &a)
{
    if (this != &a)
    {
        arc = new (nothrow) BiquarticArc3();
        for(GLuint i = 0; i < 4; i++) {
            (*arc)[i] = (*a.arc)[i];
        }
        color = new Color4(*a.color);
        previous = a.previous;
        next = a.next;
        previous_connection_type = a.previous_connection_type;
        next_connection_type = a.next_connection_type;
    }
    return *this;
}

BiquarticCompositeCurve3::ArcAttributes::~ArcAttributes()
{
    if(arc){
        delete arc; arc = nullptr;
    }
    if(image){
        delete image; image = nullptr;
    }
    if(color){
        delete color; color = nullptr;
    }
}

BiquarticCompositeCurve3::BiquarticCompositeCurve3(GLuint minimal_arc_count_to_reserved)
{
    _attributes.reserve(minimal_arc_count_to_reserved);
}

GLboolean BiquarticCompositeCurve3::InsertNewArc()
{
    ArcAttributes arcAttr = ArcAttributes();
   (*arcAttr.arc)[0] = DCoordinate3(0, 0, 1);
   (*arcAttr.arc)[1] = DCoordinate3(1, 1, 0);
   (*arcAttr.arc)[2] = DCoordinate3(1, 2, 0);
   (*arcAttr.arc)[3] = DCoordinate3(0, 3, 0);
    _attributes.push_back(arcAttr);

    try{
        Update(_attributes.back());
    }
    catch(Exception &e){
        e.what();
    }

    return GL_TRUE;
}

GLboolean BiquarticCompositeCurve3::DeleteExistingArc(GLuint index)
{
    if (index < 0 || index >= _attributes.size())
        return GL_FALSE;

    if(_attributes[index].previous)
        _attributes[index].previous->next = nullptr;

    if(_attributes[index].next)
        _attributes[index].next->previous = nullptr;

    if(_attributes[index].arc) {
        delete _attributes[index].arc;
        _attributes[index].arc = nullptr;
    }
    if(_attributes[index].image) {
        delete _attributes[index].image;
        _attributes[index].image = nullptr;
    }
    if(_attributes[index].color) {
        delete _attributes[index].color;
        _attributes[index].color = nullptr;
    }

    if(&_attributes[index] != &_attributes.back()) {
        if(_attributes.back().next) {
            _attributes.back().next->previous = &_attributes[index];
        }
        if(_attributes.back().previous) {
            _attributes.back().previous->next = &_attributes[index];
        }
        _attributes[index] = _attributes.back();
        Update(_attributes[index]);
    }
    _attributes.pop_back();

    return GL_TRUE;
}

GLboolean BiquarticCompositeCurve3::ContinueExisitingArc(GLuint index, Direction direction)
{
    if(index < 0 || index >= _attributes.size()) {
        return GL_FALSE;
    }
    InsertNewArc();
    ArcAttributes *arcNew = &_attributes.back();
    ArcAttributes *arcToContinue = &_attributes[index];

    if (direction == LEFT) {
        if (arcToContinue->previous)
        {
            _attributes.pop_back();
            return GL_FALSE;
        }

        DCoordinate3 newCoords = (*arcToContinue->arc)[0] - (*arcToContinue->arc)[1];

        (*arcNew->arc)[3] = (*arcToContinue->arc)[0];
        (*arcNew->arc)[2] = (*arcToContinue->arc)[0] + newCoords;
        (*arcNew->arc)[1] = (*arcToContinue->arc)[0] + 2*newCoords;
        (*arcNew->arc)[0] = (*arcToContinue->arc)[0] + 3*newCoords;

        arcNew->next = arcToContinue;
        arcNew->next_connection_type = LEFT;
        arcToContinue->previous = arcNew;
        arcToContinue->previous_connection_type = RIGHT;
    } else if (direction == RIGHT) {
        if (arcToContinue->next)
        {
            _attributes.pop_back();
            return GL_FALSE;
        }

        DCoordinate3 newCoords = (*arcToContinue->arc)[3] - (*arcToContinue->arc)[2];

        (*arcNew->arc)[0] = (*arcToContinue->arc)[3];
        (*arcNew->arc)[1] = (*arcToContinue->arc)[3] + newCoords;
        (*arcNew->arc)[2] = (*arcToContinue->arc)[3] + 2*newCoords;
        (*arcNew->arc)[3] = (*arcToContinue->arc)[3] + 3*newCoords;

        arcNew->previous = arcToContinue;
        arcNew->previous_connection_type = RIGHT;
        arcToContinue->next = arcNew;
        arcToContinue->next_connection_type = LEFT;
    }

    Update(*arcNew);

    return GL_TRUE;
}

GLboolean BiquarticCompositeCurve3::RenderAllArcs(GLboolean cp, GLboolean cl, GLboolean d1, GLboolean d2, GLenum const render_mode) const{
    if (render_mode == GL_LINE_STRIP ||
        render_mode == GL_LINE_LOOP  ||
        render_mode == GL_POINTS ||
        render_mode == GL_LINES)
    {
        for (auto it = _attributes.begin(); it != _attributes.end(); ++it){
            glColor3f((*it).color->r(),(*it).color->g(),(*it).color->b());
            (*it).image->RenderDerivatives(0,render_mode);
            if(cp){
                glPointSize(5);
                (*it).arc->RenderData(GL_POINTS);
                glPointSize(1);
            }
            if(cl){
                glPointSize(5);
                (*it).arc->RenderData(GL_LINE_STRIP);
                glPointSize(1);
            }
            if(d1){
                (*it).image->RenderDerivatives(1, GL_LINES);
            }
            if(d2){
                (*it).image->RenderDerivatives(2, GL_LINES);
            }
            if((*it).highlighted){
                glPointSize(5);
                glBegin(GL_POINTS);
                glColor3f(1.0f,0.0f,0.0f);
                glVertex3f((*it).arc[0][0].x(), (*it).arc[0][0].y(), (*it).arc[0][0].z());
                glColor3f(0.0f,0.0f,1.0f);
                glVertex3f((*it).arc[0][3].x(), (*it).arc[0][3].y(), (*it).arc[0][3].z());
                glEnd();
                glPointSize(1);
            }
        }
        return GL_TRUE;
    }
    return GL_FALSE;
}

GLboolean BiquarticCompositeCurve3::RenderSelectedArc(GLuint index , GLboolean cp, GLboolean cl, GLboolean d1, GLboolean d2, GLenum const render_mode) const{
    if (render_mode == GL_LINE_STRIP ||
        render_mode == GL_LINE_LOOP  ||
        render_mode == GL_POINTS ||
        render_mode == GL_LINES)
    {
        glColor3f(_attributes[index].color->r(),_attributes[index].color->g(),_attributes[index].color->b());
        _attributes[index].image->RenderDerivatives(0,render_mode);
        if(cp){
            glPointSize(5);
            _attributes[index].arc->RenderData(GL_POINTS);
            glPointSize(1);
        }
        if(cl){
            glPointSize(5);
            _attributes[index].arc->RenderData(GL_LINE_STRIP);
            glPointSize(1);
        }
        if(d1){
            _attributes[index].image->RenderDerivatives(1, GL_LINES);
        }
        if(d2){
            _attributes[index].image->RenderDerivatives(2, GL_LINES);
        }
        return GL_TRUE;
    }
    return GL_FALSE;
}


GLboolean BiquarticCompositeCurve3::JoinExistingArcs(GLuint index_0, Direction direction_0, GLuint index_1, Direction direction_1){

    if( index_0 < 0 || index_0 >= _attributes.size() ||  index_1 < 0 || index_1 >= _attributes.size()){
        return GL_FALSE;
    }

    if(direction_0 == LEFT){

        if(_attributes[index_0].previous){
            return GL_FALSE;
        }

        if(direction_1 == LEFT){
            if(_attributes[index_1].previous){
                return GL_FALSE;
            }

            InsertNewArc();
            ArcAttributes* arcAttr = &_attributes.back();
            ArcAttributes* arc0 = &_attributes[index_0];
            ArcAttributes* arc1 = &_attributes[index_1];
            (*arcAttr->arc)[0]=(*arc1->arc)[0];
            (*arcAttr->arc)[3]=(*arc0->arc)[0];
            (*arcAttr->arc)[1]=2*(*arc1->arc)[0] - (*arc1->arc)[1];
            (*arcAttr->arc)[2]=2*(*arc0->arc)[0] - (*arc0->arc)[1];

            arc0->previous = arcAttr;
            arc1->previous = arcAttr;
            arcAttr->previous = arc1;
            arcAttr->next = arc0;
            arc0->previous_connection_type = RIGHT;
            arc1->previous_connection_type = LEFT;
            arcAttr->previous_connection_type = LEFT;
            arcAttr->next_connection_type = LEFT;

            try{
                Update(*arcAttr);
            }  catch (Exception e) {
                e.what();
            }

            return GL_TRUE;
        }

        if(direction_1 == RIGHT){
            if(_attributes[index_1].next){
                return GL_FALSE;
            }

            InsertNewArc();
            ArcAttributes* arcAttr = &_attributes.back();
            ArcAttributes* arc0 = &_attributes[index_0];
            ArcAttributes* arc1 = &_attributes[index_1];
            (*arcAttr->arc)[0]=(*arc1->arc)[3];
            (*arcAttr->arc)[3]=(*arc0->arc)[0];
            (*arcAttr->arc)[1]=2*(*arc1->arc)[3] - (*arc1->arc)[2];
            (*arcAttr->arc)[2]=2*(*arc0->arc)[0] - (*arc0->arc)[1];

            arc0->previous = arcAttr;
            arc1->next = arcAttr;
            arcAttr->previous = arc1;
            arcAttr->next = arc0;
            arc0->previous_connection_type = RIGHT;
            arc1->next_connection_type = LEFT;
            arcAttr->previous_connection_type = RIGHT;
            arcAttr->next_connection_type = LEFT;

            try{
                Update(*arcAttr);
            }  catch (Exception e) {
                e.what();
            }

            return GL_TRUE;
        }
    }
    else{
        if(_attributes[index_0].next){
            return GL_FALSE;
        }

        if(direction_1 == LEFT){
            if(_attributes[index_1].previous){
                return GL_FALSE;
            }

            InsertNewArc();
            ArcAttributes* arcAttr = &_attributes.back();
            ArcAttributes* arc0 = &_attributes[index_0];
            ArcAttributes* arc1 = &_attributes[index_1];
            (*arcAttr->arc)[0]=(*arc1->arc)[0];
            (*arcAttr->arc)[3]=(*arc0->arc)[3];
            (*arcAttr->arc)[1]=2*(*arc1->arc)[0] - (*arc1->arc)[1];
            (*arcAttr->arc)[2]=2*(*arc0->arc)[3] - (*arc0->arc)[2];

            arc0->next = arcAttr;
            arc1->previous = arcAttr;
            arcAttr->previous = arc1;
            arcAttr->next = arc0;
            arc0->next_connection_type = RIGHT;
            arc1->previous_connection_type = LEFT;
            arcAttr->previous_connection_type = LEFT;
            arcAttr->next_connection_type = RIGHT;

            try{
                Update(*arcAttr);
            }  catch (Exception e) {
                e.what();
            }

            return GL_TRUE;
        }

        if(direction_1 == RIGHT){
            if(_attributes[index_1].next){
                return GL_FALSE;
            }

            InsertNewArc();
            ArcAttributes* arcAttr = &_attributes.back();
            ArcAttributes* arc0 = &_attributes[index_0];
            ArcAttributes* arc1 = &_attributes[index_1];
            (*arcAttr->arc)[0]=(*arc1->arc)[3];
            (*arcAttr->arc)[3]=(*arc0->arc)[3];
            (*arcAttr->arc)[1]=2*(*arc1->arc)[3] - (*arc1->arc)[2];
            (*arcAttr->arc)[2]=2*(*arc0->arc)[3] - (*arc0->arc)[2];

            arc0->next = arcAttr;
            arc1->next = arcAttr;
            arcAttr->previous = arc1;
            arcAttr->next = arc0;
            arc0->next_connection_type = RIGHT;
            arc1->next_connection_type = LEFT;
            arcAttr->previous_connection_type = RIGHT;
            arcAttr->next_connection_type = RIGHT;

            try{
                Update(*arcAttr);
            }  catch (Exception &e) {
                e.what();
            }

            return GL_TRUE;
        }
    }

    return GL_FALSE;
}



GLboolean BiquarticCompositeCurve3::MergeExistingArcs(GLuint index_0, Direction direction_0, GLuint index_1, Direction direction_1)
{
    if(index_0 < 0 || index_0 > _attributes.size() || index_1 < 0 || index_1 > _attributes.size()){
        return GL_FALSE;
    }

    ArcAttributes* arc_0 = &_attributes[index_0];
    ArcAttributes* arc_1 = &_attributes[index_1];

    if((direction_0 == Direction::LEFT && arc_0->previous != nullptr)
            || (direction_0 == Direction::RIGHT && arc_0->next != nullptr)
            || (direction_1 == Direction::LEFT && arc_1->previous != nullptr)
            || (direction_1 == Direction::RIGHT && arc_1->next != nullptr)
    ){
        return GL_FALSE;
    }

    if(direction_0 == Direction::LEFT && direction_1 == Direction::LEFT){
        (*arc_0->arc)[0] = (*arc_1->arc)[0] = ((*arc_0->arc)[1] + (*arc_1->arc)[1]) / 2;
        arc_0->previous = arc_1;
        arc_1->previous = arc_0;
        arc_0->previous_connection_type = arc_1->previous_connection_type = LEFT;
    }
    else if(direction_0 == Direction::LEFT && direction_1 == Direction::RIGHT){
        (*arc_0->arc)[0] = (*arc_1->arc)[3] = ((*arc_0->arc)[1] + (*arc_1->arc)[2]) / 2;
        arc_0->previous = arc_1;
        arc_1->next = arc_0;
        arc_0->previous_connection_type = RIGHT;
        arc_1->next_connection_type = LEFT;
    }
    else if(direction_0 == Direction::RIGHT && direction_1 == Direction::LEFT){
        (*arc_0->arc)[3] = (*arc_1->arc)[0] = ((*arc_0->arc)[2] + (*arc_1->arc)[1]) / 2;
        arc_0->next = arc_1;
        arc_1->previous = arc_0;
        arc_0->next_connection_type = LEFT;
        arc_1->previous_connection_type = RIGHT;
    }
    else if(direction_0 == Direction::RIGHT && direction_1 == Direction::RIGHT){
        (*arc_0->arc)[3] = (*arc_1->arc)[3] = ((*arc_0->arc)[2] + (*arc_1->arc)[2]) / 2;
        arc_0->next = arc_1;
        arc_1->next = arc_0;
        arc_0->next_connection_type = arc_1->next_connection_type = RIGHT;
    }
    else {
        return GL_FALSE;
    }

    try {
        Update(*arc_0);
        Update(*arc_1);
    }
    catch(Exception &e){
        e.what();
    }

    return GL_TRUE;
}

void BiquarticCompositeCurve3::Update(ArcAttributes& arc_attr)
{
    arc_attr.image = arc_attr.arc->GenerateImage(2,50);
    if(!arc_attr.image) {
        throw Exception("Could not generate image of arc!");
    }
    arc_attr.arc->DeleteVertexBufferObjectsOfData();
    if(!arc_attr.arc->UpdateVertexBufferObjectsOfData()) {
        throw Exception("Could not update vertex buffer object of data of arc!");
    }
    arc_attr.image->DeleteVertexBufferObjects();
    if(!arc_attr.image->UpdateVertexBufferObjects(0.2)) {
        throw Exception("Could not update vertex buffer objects of image arc!");
    }
}

GLboolean BiquarticCompositeCurve3::setData(GLuint index, GLuint i, DCoordinate3 const coord)
{
    if(index >= 0 && index < _attributes.size() && i >= 0 && i < 4){
        (*_attributes[index].arc)[i] = coord;
        Update(_attributes[index]);
        return GL_TRUE;
    }
    else{
        return GL_FALSE;
    }
}

GLboolean BiquarticCompositeCurve3::setData(GLuint index, GLuint i, GLdouble x, GLdouble y, GLdouble z)
{
    if(index >= 0 && index < _attributes.size() && i >= 0 && i < 4){
        (*_attributes[index].arc)[i].x() = x;
        (*_attributes[index].arc)[i].y() = y;
        (*_attributes[index].arc)[i].z() = z;
        Update(_attributes[index]);
        return GL_TRUE;
    }
    else{
        return GL_FALSE;
    }
}

GLboolean BiquarticCompositeCurve3::setColor(GLuint index, GLdouble r, GLdouble g, GLdouble b)
{
    if(index >= 0 && index < _attributes.size()){
        (*_attributes[index].color)[0] = r;
        (*_attributes[index].color)[1] = g;
        (*_attributes[index].color)[2] = b;
        return GL_TRUE;
    }
    else{
        return GL_FALSE;
    }
}

void BiquarticCompositeCurve3::UpdateControlPoint(GLuint index, GLuint pointIndex, GLdouble x, GLdouble y, GLdouble z){

    if(index < 0 || index > _attributes.size() - 1){
        return;
    }

    ArcAttributes* arcAttr = &_attributes[index];
    ArcAttributes* neighbour;
    Direction neighbourDir;
    DCoordinate3 sub(x-(*arcAttr->arc)[pointIndex].x(),y-(*arcAttr->arc)[pointIndex].y(),z-(*arcAttr->arc)[pointIndex].z());
    (*arcAttr->arc)[pointIndex].x()=x;
    (*arcAttr->arc)[pointIndex].y()=y;
    (*arcAttr->arc)[pointIndex].z()=z;
    Update(*arcAttr);

        if(pointIndex <= 1){
            neighbour = arcAttr->previous;
            neighbourDir = arcAttr->previous_connection_type;
        } else {
            neighbour = arcAttr->next;
            neighbourDir = arcAttr->next_connection_type;
        }
        if (neighbour != nullptr){
        switch(pointIndex){
        case 1:
            if(neighbourDir == LEFT){
                (*neighbour->arc)[1]=2*(*arcAttr->arc)[0] - (*arcAttr->arc)[1];
                Update(*neighbour);
            }
            if(neighbourDir == RIGHT){
                (*neighbour->arc)[2]=2*(*arcAttr->arc)[0] - (*arcAttr->arc)[1];
                Update(*neighbour);
            }
            break;
        case 2:
            if(neighbourDir == LEFT){
                (*neighbour->arc)[1]=2*(*arcAttr->arc)[3] - (*arcAttr->arc)[2];
                Update(*neighbour);
            }
            if(neighbourDir == RIGHT){
                (*neighbour->arc)[2]=2*(*arcAttr->arc)[3] - (*arcAttr->arc)[2];
                Update(*neighbour);
            }
            break;
        default:
            if(pointIndex == 0){
                (*arcAttr->arc)[pointIndex+1] += sub;
            }
            if(pointIndex == 3){
                (*arcAttr->arc)[pointIndex-1] += sub;
            }
            Update(*arcAttr);
            if(neighbourDir == LEFT){
                (*neighbour->arc)[0]=(*arcAttr->arc)[pointIndex];
                if(pointIndex == 3){
                    (*neighbour->arc)[1]=2*(*arcAttr->arc)[3] - (*arcAttr->arc)[2];
                }
                else if(pointIndex == 0){
                    (*neighbour->arc)[1]=2*(*arcAttr->arc)[0] - (*arcAttr->arc)[1];
                }
                Update(*neighbour);
            }
            if(neighbourDir == RIGHT){
                (*neighbour->arc)[3]=(*arcAttr->arc)[pointIndex];
                if(pointIndex == 3){
                    (*neighbour->arc)[2]=2*(*arcAttr->arc)[3] - (*arcAttr->arc)[2];
                } else if(pointIndex == 0){
                    (*neighbour->arc)[2]=2*(*arcAttr->arc)[0] - (*arcAttr->arc)[1];
                }
                Update(*neighbour);
            }
            break;
        }
    }
}
