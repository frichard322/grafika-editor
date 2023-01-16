#pragma once

#include <GL/glew.h>
#include <QOpenGLWidget>
#include <QOpenGLTexture>
#include "../Core/TriangulatedMeshes3.h"
#include "../Core/Exceptions.h"
#include "../Core/DCoordinates3.h"
#include "../Core/HCoordinates3.h"
#include "../Core/Matrices.h"
#include "../Core/RealSquareMatrices.h"
#include "../Core/Constants.h"
#include "../Core/Colors4.h"
#include "../Core/Materials.h"
#include "../Core/Lights.h"
#include "../Core/Properties.h"
#include "../Biquartic/BiquarticArc3.h"
#include "../Biquartic/BiquarticCompositeCurve3.h"
#include "../Biquartic/BiquarticPatch3.h"
#include "../Biquartic/BiquarticCompositePatches3.h"
#include <vector>
#include "../Core/ShaderPrograms.h"

using namespace std;

namespace cagd
{
    class GLWidget: public QOpenGLWidget
    {
        Q_OBJECT

    private:

        // variables defining the projection matrix
        double       _aspect;            // aspect ratio of the rendering window
        double       _fovy;              // field of view in direction y
        double       _z_near, _z_far;    // distance of near and far clipping planes

        // variables defining the model-view matrix
        double       _eye[3], _center[3], _up[3];

        // variables needed by transformations
        int         _angle_x, _angle_y, _angle_z;
        double      _zoom;
        double      _trans_x, _trans_y, _trans_z;

        // Arc Variables
        GLboolean _cp_visibility = 0, _cl_visibility = 0, _d1_visibility = 0, _d2_visibility = 0;
        BiquarticCompositeCurve3 bcc;

        //Patch Variables
        GLint _shader_enabled;
        ShaderProgram _shader;

        GLboolean _patch_visibility = 1, _pcp_visibility = 0, _pcl_visibility = 0, _pud_visibility = 0, _pvd_visibility = 0, _p_isou_visibility = 0, _p_isov_visibility = 0, _p_normal_visibility = 0;
        BiquarticCompositeSurface3 bcs;

        HCoordinate3 position1= HCoordinate3(0.0f,0.0f,1.0f,0.0f);
        Color4       ambient_intensity1= Color4(0.4f,0.4f,0.4f,1.0f);
        Color4       diffuse_intensity1= Color4(0.8f,0.8f,0.8f,1.0f);
        Color4       specular_intensity1= Color4(1.0f,1.0f,1.0f,1.0f);

        HCoordinate3 position2= HCoordinate3(0.0f,0.0f,1.0f,0.0f);
        Color4       ambient_intensity2= Color4(0.4f,0.4f,0.4f,1.0f);
        Color4       diffuse_intensity2= Color4(0.8f,0.8f,0.8f,1.0f);
        Color4       specular_intensity2= Color4(1.0f,1.0f,1.0f,1.0f);

        char*  _texture_name = new char[30];
        QOpenGLTexture *texture = nullptr;
        bool _textured = false;

        // Default Methods
        bool _createSomeGeometricObjectByDynamicalMemoryAllocation();
        bool _renderExistingGeometricObject();
        void _deleteExistingDynamicallyAllocatedGeometricObject();

    public:
        // special and default constructor
        // the format specifies the properties of the rendering window
        GLWidget(QWidget* parent = 0);
        virtual ~GLWidget();

        // redeclared virtual functions
        void initializeGL();
        void paintGL();
        void resizeGL(int w, int h);

        void setProjection();
        void setModelview();

        void _createLight();
        void installShader();
        void _createTexture();

    public slots:
        // public event handling methods/slots
        // Arc
        void set_angle_x(int value);
        void set_angle_y(int value);
        void set_angle_z(int value);

        void set_zoom_factor(double value);

        void set_trans_x(double value);
        void set_trans_y(double value);
        void set_trans_z(double value);

        void slot_show_highlightingArc(int index1, int index2 = -1);
        void slot_insert_new_arc();
        void slot_delete_arc(int index);

        void slot_set_red_arc(int index, double r);
        void slot_set_green_arc(int index, double g);
        void slot_set_blue_arc(int index, double b);
        void slot_set_color_arc_index(int index);

        void SetControlPointVisibility();
        void SetControlLineVisibility();
        void SetFirstOrderDerivateVisibility();
        void SetSecondOrderDerivateVisibility();

        void slot_set_x_arc(int arc_index, int cp_index, double x);
        void slot_set_y_arc(int arc_index, int cp_index, double y);
        void slot_set_z_arc(int arc_index, int cp_index, double z);
        void slot_set_position_arc_index(int arc_index, int cp_index);

        void slot_continue_arc(int arc_index, int side);
        void slot_join_arcs(int index_0, int side_0, int index_1, int side_1);
        void slot_merge_arcs(int index_0, int side_0, int index_1, int side_1);

        void slot_save_arcs();
        void slot_load_arcs();

        //---------------------------------------------------------------------
        // Patch
        void slot_show_highlightingPatch(int index1, int index2 = -1);
        void slot_insert_new_patch();
        void slot_delete_patch(int index);

        void SetControlPointVisibilityPatch();
        void SetControlLineVisibilityPatch();
        void SetUDerivateVisibilityPatch();
        void SetVDerivateVisibilityPatch();
        void SetUISOVisibilityPatch();
        void SetVISOVisibilityPatch();
        void SetNormalsVisibilityPatch();
        void SetVisibilityPatch();

        void slot_set_material_patch(int index, int material_index);
        void slot_set_material_patch_index(int index);

        void slot_continue_patch(int patch_index, int side);
        void slot_join_patches(int index_0, int side_0, int index_1, int side_1);
        void slot_merge_patches(int index0, int side_0, int index_1, int side_1);

        void slot_set_position_patch_index(int patch_index, int patch_cp_index);
        void slot_set_x_patch(int patch_index, int patch_cp_index, double x);
        void slot_set_y_patch(int patch_index, int patch_cp_index, double y);
        void slot_set_z_patch(int patch_index, int patch_cp_index, double z);

        void slot_save_patches();
        void slot_load_patches();

        // Light
        void slot_change_light_position(int light_index,double p_1,double p_2,double p_3,double p_4);
        void slot_change_light_ambient(int light_index,double p_1,double p_2,double p_3,double p_4);
        void slot_change_light_diffuse(int light_index,double p_1,double p_2,double p_3,double p_4);
        void slot_change_light_specular(int light_index,double p_1,double p_2,double p_3,double p_4);
        void slot_lighting_index_changed(int index);

        // Shader
        void slot_disable_shader();

        // Texture
        void slot_change_texture(int index);

    signals:
        // Arc
        void updateArcCount(int);
        void redColorChanged(double);
        void greenColorChanged(double);
        void blueColorChanged(double);
        void XcoordChanged(double);
        void YcoordChanged(double);
        void ZcoordChanged(double);
        //------------------------------
        // Patch
        void updatePatchCount(int);
        void materialChanged(int);
        void XcoordChangedPatch(double);
        void YcoordChangedPatch(double);
        void ZcoordChangedPatch(double);
        //------------------------------
        // Light
        void positionXChanged(double);
        void positionYChanged(double);
        void positionZChanged(double);
        void positionWChanged(double);
        void ambientRChanged(double);
        void ambientGChanged(double);
        void ambientBChanged(double);
        void ambientAChanged(double);
        void diffuseRChanged(double);
        void diffuseGChanged(double);
        void diffuseBChanged(double);
        void diffuseAChanged(double);
        void specularRChanged(double);
        void specularGChanged(double);
        void specularBChanged(double);
        void specularAChanged(double);
    };
}
