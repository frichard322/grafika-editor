#ifndef BIQUARTICCOMPOSITEPATCHES_H
#define BIQUARTICCOMPOSITEPATCHES_H

#include <Core/Materials.h>
#include "BiquarticPatch3.h"
#include "../Core/ShaderPrograms.h"
#include <QOpenGLTexture>
#include "../Core/Lights.h"

namespace cagd
{
    class BiquarticCompositeSurface3
    {
    public:
        enum Direction{N, NW, W, SW, S, SE, E, NE, NONE};

        class PatchAttributes
        {
        public:
            BiquarticPatch3     *patch;
            TriangulatedMesh3   *image;
            GLint            material;
            GLboolean           highlighted = false;

            RowMatrix<GenericCurve3*> *uLines, *vLines;

            PatchAttributes   *neighbours[8];
            Direction         connection_type[8];

            PatchAttributes();
            PatchAttributes(const PatchAttributes &a);
            PatchAttributes& operator =(const PatchAttributes &a);
            ~PatchAttributes();
            inline void setHighlighting(GLboolean state){
                highlighted = state;
            }
        };

    public:
        std::vector<PatchAttributes> _attributes;
        Material _materials[7] = {MatFBBrass, MatFBEmerald, MatFBGold, MatFBPearl, MatFBRuby, MatFBSilver, MatFBTurquoise};

        // special/default ctor
        BiquarticCompositeSurface3(GLuint minimal_patch_count_to_reserved = 1000);

        // operations
        GLboolean InsertNewPatch();
        GLboolean DeleteExistingPatch(GLuint index);
        GLboolean ContinueExistingPatch(GLuint index, Direction direction);
        GLboolean JoinExistingPatches(GLuint index_0, Direction direction_0, GLuint index_1, Direction direction_1);
        GLboolean MergeExistingPatches(GLuint index_0, Direction direction_0, GLuint index_1, Direction direction_1);

        GLboolean RenderAllPatches(GLboolean _textured, QOpenGLTexture* texture, GLboolean _shader_enabled, ShaderProgram &_shader, GLboolean _patch_visibility, GLboolean _pcp_visibility, GLboolean _pcl_visibility, GLboolean _pud_visibility, GLboolean _pvd_visibility, GLboolean _p_isou_visibility, GLboolean _p_isov_visibility, GLboolean _p_normal_visibility) const;
        GLboolean RenderSelectedPatch(GLuint index) const;

        void Update(PatchAttributes &patchAttr);

        void UpdateControlPoint(GLuint index, GLuint row, GLuint col, GLdouble x, GLdouble y, GLdouble z);
        void Connect(PatchAttributes* good, PatchAttributes* needsChange);

        inline void disableAllHighlighting(){
            for(GLuint i = 0; i < _attributes.size(); i++){
                _attributes[i].setHighlighting(GL_FALSE);
            }
        }

        inline GLint GetIndex(const PatchAttributes& patchAttr) const {
            for(GLuint i=0;i<_attributes.size();++i){
                if(&_attributes[i] == &patchAttr){
                    return i;
                }
            }
            return -1;
        }

        inline friend std::ostream& operator <<(std::ostream& lhs, const BiquarticCompositeSurface3& rhs)
        {
            GLint n = rhs._attributes.size();
            lhs << n << '\n';
            for(GLint i=0;i<n;++i){
                lhs << rhs._attributes[i].material << '\n';
                for(GLint r=0;r<4;++r){
                    for(GLint c=0;c<4;++c){
                        DCoordinate3 tmp;
                        rhs._attributes[i].patch->GetData(r,c,tmp);
                        lhs << tmp <<" ";
                    }
                    lhs << '\n';
                }
            }
            for(GLint i=0;i<n;++i){
                for(GLint j=0;j<8;++j){
                    if(rhs._attributes[i].neighbours[j]){
                        lhs << rhs.GetIndex(*rhs._attributes[i].neighbours[j])<<" ";
                        GLuint dir;
                        switch(rhs._attributes[i].connection_type[j]){
                        case N:
                            dir = 1;
                            break;
                        case NW:
                            dir = 2;
                            break;
                        case W:
                            dir = 3;
                            break;
                        case SW:
                            dir = 4;
                            break;
                        case S:
                            dir = 5;
                            break;
                        case SE:
                            dir = 5;
                            break;
                        case E:
                            dir = 7;
                            break;
                        case NE:
                            dir = 8;
                            break;
                        }

                        lhs << dir << " ";
                        continue;
                    }
                    lhs << -1 << " ";
                }
                lhs << '\n';
            }
            return lhs;
        }

        inline friend std::istream& operator >>(std::istream& lhs, BiquarticCompositeSurface3& rhs)
        {
           GLint n;
           lhs >> n;
           for(GLint i=0;i<n;++i){
               rhs.InsertNewPatch();
               PatchAttributes* patchAttr = &rhs._attributes.back();
               lhs >> patchAttr->material;
               for(GLint r=0;r<4;++r){
                   for(GLint c=0;c<4;++c){
                       lhs >> (*patchAttr->patch)(r,c);
                   }
               }
               rhs.Update(*patchAttr);
           }
           for(GLint i=0;i<n;++i){
               PatchAttributes* patchAttr = &rhs._attributes[i];
               for(GLint j=0;j<8;++j){
                   GLint neighbourIndex;
                   GLint dirInt;
                   lhs >> neighbourIndex;
                   if(neighbourIndex != -1){
                       lhs >> dirInt;
                       Direction dir;
                       switch(dirInt){
                       case 1:
                           dir = N;
                           break;
                       case 2:
                           dir = NW;
                           break;
                       case 3:
                           dir = W;
                           break;
                       case 4:
                           dir = SW;
                           break;
                       case 5:
                           dir = S;
                           break;
                       case 6:
                           dir = SE;
                           break;
                       case 7:
                           dir = E;
                           break;
                       case 8:
                           dir = NE;
                           break;
                       }

                       patchAttr->neighbours[j]=&rhs._attributes[neighbourIndex];
                       patchAttr->connection_type[j]=dir;
                   }
               }
               rhs.Update(*patchAttr);
           }
           return lhs;
        }
    };
}

#endif // BIQUARTICCOMPOSITEPATCHES_H
