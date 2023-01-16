#ifndef BIQUADRATICCOMPOSITEARC3_H
#define BIQUADRATICCOMPOSITEARC3_H

#include <Core/Colors4.h>
#include "BiquarticArc3.h"
#include "../Core/Exceptions.h"
#include <vector>

using namespace std;

namespace cagd
{
    class BiquarticCompositeCurve3
    {
    public:
        enum Direction{LEFT, RIGHT, NONE};

        class ArcAttributes
        {
        public:
            BiquarticArc3   *arc;
            GenericCurve3   *image;
            Color4          *color;
            GLboolean       highlighted = false;

            ArcAttributes   *previous, *next; // pointers to neighboring arc attributes
            Direction       previous_connection_type, next_connection_type;

            ArcAttributes();
            ArcAttributes(const ArcAttributes &a);
            ArcAttributes& operator =(const ArcAttributes &a);
            ~ArcAttributes();
            inline void setHighlighting(GLboolean state){
                highlighted = state;
            }
        };

    public:
        std::vector<ArcAttributes> _attributes;          // sequence of arc attributes
    public:
        // special/default ctor
        BiquarticCompositeCurve3(GLuint minimal_arc_count_to_reserved = 1000);

        GLboolean InsertNewArc();
        GLboolean DeleteExistingArc(GLuint index);
        GLboolean ContinueExisitingArc(GLuint index, Direction direction);
        GLboolean JoinExistingArcs(GLuint index_0, Direction direction_0, GLuint index_1, Direction direction_1);
        GLboolean MergeExistingArcs(GLuint index_0, Direction direction_0, GLuint index_1, Direction direction_1);

        GLboolean RenderAllArcs(GLboolean cp, GLboolean cl, GLboolean d1, GLboolean d2, GLenum const render_mode = GL_LINE_STRIP) const;
        GLboolean RenderSelectedArc(GLuint index, GLboolean cp, GLboolean cl, GLboolean d1, GLboolean d2, GLenum const render_mode = GL_LINE_STRIP) const;

        void Update(ArcAttributes &arc_attr);
        GLboolean setColor(GLuint index, GLdouble r, GLdouble g, GLdouble b);
        GLboolean setData(GLuint index, GLuint i, DCoordinate3 const coord);
        GLboolean setData(GLuint index, GLuint i, GLdouble x, GLdouble y, GLdouble z);

        void UpdateControlPoint(GLuint index, GLuint pointIndex, GLdouble x, GLdouble y, GLdouble z);

        inline void disableAllHighlighting(){
            for(GLuint i = 0; i < _attributes.size(); i++){
                _attributes[i].setHighlighting(GL_FALSE);
            }
        }

        inline GLint GetIndex(const ArcAttributes& arcAttr) const {
            for(GLuint i=0;i<_attributes.size();++i){
                if(&_attributes[i] == &arcAttr){
                    return i;
                }
            }
            return -1;
        }

        inline friend std::ostream& operator <<(std::ostream& lhs, const BiquarticCompositeCurve3& rhs)
        {
            lhs << rhs._attributes.size() << endl;
            for(GLuint i = 0; i < rhs._attributes.size(); i++){
                lhs << rhs._attributes[i].arc->GetData();
                lhs << rhs._attributes[i].color->r() << " " << rhs._attributes[i].color->g() << " " << rhs._attributes[i].color->b() << " " << rhs._attributes[i].color->a() << endl;
                lhs << rhs.GetIndex(*rhs._attributes[i].previous) << " " << rhs.GetIndex(*rhs._attributes[i].next) << endl;
                lhs << rhs._attributes[i].previous_connection_type << " " << rhs._attributes[i].next_connection_type << endl;
            }
            return lhs;
        }

        friend std::istream& operator >>(std::istream& lhs, BiquarticCompositeCurve3& rhs)
        {
            GLint n, prev_index, next_index;
            lhs >> n;
            vector<pair<int,int>> neighbours;
            for(GLint i = 0; i < n; i++){
                rhs.InsertNewArc();
                ArcAttributes* newArcAttr = &rhs._attributes.back();
                ColumnMatrix<DCoordinate3> coords;
                lhs >> coords;
                newArcAttr->arc->SetData(coords);
                lhs >> newArcAttr->color->r() >> newArcAttr->color->g() >> newArcAttr->color->b() >> newArcAttr->color->a();
                lhs >> prev_index >> next_index;
                neighbours.push_back(make_pair(prev_index, next_index));
                GLuint tmp;
                lhs >> tmp;
                newArcAttr->previous_connection_type = (Direction) tmp;
                lhs >> tmp;
                newArcAttr->next_connection_type = (Direction) tmp;
                rhs.Update(*newArcAttr);
            }
            cout << "1";
            for(GLint i = 0; i < n; i++){
                prev_index = neighbours[i].first;
                next_index = neighbours[i].second;
                if(prev_index != -1){
                    rhs._attributes[i].previous = &rhs._attributes[prev_index];
                }
                if(next_index != -1){
                    rhs._attributes[i].next = &rhs._attributes[next_index];
                }
            }
            cout << "1";
            return lhs;
        }
    };
}

#endif // BIQUADRATICCOMPOSITEARC3_H
