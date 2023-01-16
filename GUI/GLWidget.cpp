#include "GLWidget.h"
#include <QTimer>

#if !defined(__APPLE__)
#include <GL/glu.h>
#endif

#include <iostream>
#include <fstream>
using namespace std;

namespace cagd
{
    //--------------------------------
    // special and default constructor
    //--------------------------------
    GLWidget::GLWidget(QWidget *parent): QOpenGLWidget(parent)
    {
    }

    //--------------------------------
    // destructor
    //--------------------------------
    GLWidget::~GLWidget()
    {
        _deleteExistingDynamicallyAllocatedGeometricObject();
    }

    //--------------------------------
    // declaration of my own methods
    //--------------------------------
    bool GLWidget::_createSomeGeometricObjectByDynamicalMemoryAllocation()
    {
        _createLight();
        _shader_enabled = true;
        _shader = ShaderProgram();
        installShader();

        emit updateArcCount((int)bcc._attributes.size() - 1);
        emit updatePatchCount((int)bcs._attributes.size() - 1);
        return true;
    }

    bool GLWidget::_renderExistingGeometricObject()
    {
        bcc.RenderAllArcs(_cp_visibility, _cl_visibility, _d1_visibility, _d2_visibility);
        bcs.RenderAllPatches(_textured, texture, _shader_enabled, _shader, _patch_visibility, _pcp_visibility, _pcl_visibility, _pud_visibility, _pvd_visibility, _p_isou_visibility, _p_isov_visibility, _p_normal_visibility);
        return true;
    }

    void GLWidget::_deleteExistingDynamicallyAllocatedGeometricObject()
    {
    }

    void GLWidget::SetControlPointVisibility(){
        _cp_visibility = 1 - _cp_visibility;
        update();
    }

    void GLWidget::SetControlLineVisibility(){
        _cl_visibility = 1 - _cl_visibility;
        update();
    }

    void GLWidget::SetFirstOrderDerivateVisibility(){
        _d1_visibility = 1 - _d1_visibility;
        update();
    }

    void GLWidget::SetSecondOrderDerivateVisibility(){
        _d2_visibility = 1 - _d2_visibility;
        update();
    }

    void GLWidget::SetControlPointVisibilityPatch(){
        _pcp_visibility = 1 - _pcp_visibility;
        update();
    }

    void GLWidget::SetControlLineVisibilityPatch(){
        _pcl_visibility = 1 - _pcl_visibility;
        update();
    }

    void GLWidget::SetUDerivateVisibilityPatch(){
        _pud_visibility = 1 - _pud_visibility;
        update();
    }

    void GLWidget::SetVDerivateVisibilityPatch(){
        _pvd_visibility = 1 - _pvd_visibility;
        update();
    }

    void GLWidget::SetUISOVisibilityPatch(){
        _p_isou_visibility = 1 - _p_isou_visibility;
        update();
    }

    void GLWidget::SetVISOVisibilityPatch(){
        _p_isov_visibility = 1 - _p_isov_visibility;
        update();
    }

    void GLWidget::SetNormalsVisibilityPatch(){
        _p_normal_visibility = 1 - _p_normal_visibility;
        update();
    }

    void GLWidget::SetVisibilityPatch(){
        _patch_visibility = 1 - _patch_visibility;
        update();
    }

    //--------------------------------------------------------------------------------------
    // this virtual function is called once before the first call to paintGL() or resizeGL()
    //--------------------------------------------------------------------------------------
    void GLWidget::initializeGL()
    {
        // creating a perspective projection matrix
        _aspect = static_cast<double>(width()) / static_cast<double>(height());
        _z_near = 1.0;
        _z_far  = 1000.0;
        _fovy   = 45.0;
        setProjection();

        // setting the model view matrix
        _eye[0] = _eye[1] = 0.0; _eye[2] = 6.0;
        _center[0] = _center[1] = _center[2] = 0.0;
        _up[0] = _up[2] = 0.0; _up[1] = 1.0;
        setModelview();

        // enabling the depth test
        glEnable(GL_DEPTH_TEST);

        // setting the background color
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        // initial values of transformation parameters
        _angle_x = _angle_y = _angle_z = 0.0;
        _trans_x = _trans_y = _trans_z = 0.0;
        _zoom = 1.0;

        // ...

        try
        {
            // initializing the OpenGL Extension Wrangler library
            GLenum error = glewInit();

            if (error != GLEW_OK)
            {
                throw Exception("Could not initialize the OpenGL Extension Wrangler Library!");
            }

            if (!glewIsSupported("GL_VERSION_2_0"))
            {
                throw Exception("Your graphics card is not compatible with OpenGL 2.0+! "
                                "Try to update your driver or buy a new graphics adapter!");
            }

            // create and store your geometry in display lists or vertex buffer objects
            // ...
            if(!_createSomeGeometricObjectByDynamicalMemoryAllocation()){
                throw Exception("Could not create renderable geometry!");
            }
        }
        catch (Exception &e)
        {
            e.what();
        }
    }

    //-----------------------
    // the rendering function
    //-----------------------
    void GLWidget::paintGL()
    {
        // clears the color and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // stores/duplicates the original model view matrix
        glPushMatrix();

        // applying transformations
        glRotatef(_angle_x, 1.0, 0.0, 0.0);
        glRotatef(_angle_y, 0.0, 1.0, 0.0);
        glRotatef(_angle_z, 0.0, 0.0, 1.0);
        glTranslated(_trans_x, _trans_y, _trans_z);
        glScaled(_zoom, _zoom, _zoom);

        // render your geometry
        try{
            if(!_renderExistingGeometricObject()){
                throw Exception("Could not render existing geometry object!");
            }
        }
        catch (Exception &e)
        {
            e.what();
        }

        // pops the current matrix stack, replacing the current matrix with the one below it on the stack,
        // i.e., the original model view matrix is restored
        glPopMatrix();
    }

    //----------------------------------------------------------------------------
    // when the main window is resized one needs to redefine the projection matrix
    //----------------------------------------------------------------------------
    void GLWidget::resizeGL(int w, int h)
    {
        // setting the new size of the rendering context
        glViewport(0, 0, w, h);

        // redefining the projection matrix
        _aspect = static_cast<double>(w) / static_cast<double>(h);
        setProjection();

        update();
    }

    void GLWidget::setProjection()
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(_fovy, _aspect, _z_near, _z_far);
        glMatrixMode(GL_MODELVIEW);
    }

    void GLWidget::setModelview()
    {
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(_eye[0], _eye[1], _eye[2], _center[0], _center[1], _center[2], _up[0], _up[1], _up[2]);
    }

    //-----------------------------------
    // implementation of the private slots
    //-----------------------------------

    //-----------------------------------
    // implementation of the public slots
    //-----------------------------------

    void GLWidget::set_angle_x(int value)
    {
        if (_angle_x != value)
        {
            _angle_x = value;
            update();
        }
    }

    void GLWidget::set_angle_y(int value)
    {
        if (_angle_y != value)
        {
            _angle_y = value;
            update();
        }
    }

    void GLWidget::set_angle_z(int value)
    {
        if (_angle_z != value)
        {
            _angle_z = value;
            update();
        }
    }

    void GLWidget::set_zoom_factor(double value)
    {
        if (_zoom != value)
        {
            _zoom = value;
            update();
        }
    }

    void GLWidget::set_trans_x(double value)
    {
        if (_trans_x != value)
        {
            _trans_x = value;
            update();
        }
    }

    void GLWidget::set_trans_y(double value)
    {
        if (_trans_y != value)
        {
            _trans_y = value;
            update();
        }
    }

    void GLWidget::set_trans_z(double value)
    {
        if (_trans_z != value)
        {
            _trans_z = value;
            update();
        }
    }

    void GLWidget::slot_show_highlightingArc(int index1, int index2) {
        bcc.disableAllHighlighting();
        if(index1 >= 0 && index1 < (int) bcc._attributes.size()) {
            bcc._attributes[index1].setHighlighting(GL_TRUE);
        }
        if(index1 >= 0 && index1 < (int) bcc._attributes.size()) {
            bcc._attributes[index2].setHighlighting(GL_TRUE);
        }
        update();
    }

    void GLWidget::slot_insert_new_arc() {
        bcc.InsertNewArc();
        emit updateArcCount((int)bcc._attributes.size() - 1);
        update();
    }

    void GLWidget::slot_delete_arc(int index) {
        if(index >= 0 && index < (int) bcc._attributes.size()) {
            bcc.DeleteExistingArc(index);
            emit updateArcCount((int)bcc._attributes.size() - 1);
            update();
        }
    }

    void GLWidget::slot_set_red_arc(int index, double r) {
        if(index >= 0 && index < (int) bcc._attributes.size()) {
            (*bcc._attributes[index].color)[0] = r;
            update();
        }
    }

    void GLWidget::slot_set_green_arc(int index, double g) {
        if(index >= 0 && index < (int) bcc._attributes.size()) {
            (*bcc._attributes[index].color)[1] = g;
            update();
        }
    }

    void GLWidget::slot_set_blue_arc(int index, double b) {
        if(index >= 0 && index < (int) bcc._attributes.size()) {
            (*bcc._attributes[index].color)[2] = b;
            update();
        }
    }

    void GLWidget::slot_set_color_arc_index(int index) {
        if(index >= 0 && index < (int) bcc._attributes.size()) {
            emit redColorChanged(bcc._attributes[index].color->r());
            emit greenColorChanged(bcc._attributes[index].color->g());
            emit blueColorChanged(bcc._attributes[index].color->b());
        }
    }

    void GLWidget::slot_set_x_arc(int arc_index, int cp_index, double x){
        if(arc_index >= 0 && arc_index < (int) bcc._attributes.size() && cp_index < 4) {
            bcc.UpdateControlPoint(arc_index, cp_index, x, (*bcc._attributes[arc_index].arc)[cp_index].y(), (*bcc._attributes[arc_index].arc)[cp_index].z());
            update();
        }
    }

    void GLWidget::slot_set_y_arc(int arc_index, int cp_index, double y){
        if(arc_index >= 0 && arc_index < (int) bcc._attributes.size() && cp_index < 4) {
            bcc.UpdateControlPoint(arc_index, cp_index, (*bcc._attributes[arc_index].arc)[cp_index].x(), y, (*bcc._attributes[arc_index].arc)[cp_index].z());
            update();
        }
    }

    void GLWidget::slot_set_z_arc(int arc_index, int cp_index, double z){
        if(arc_index >= 0 && arc_index < (int) bcc._attributes.size() && cp_index < 4) {
            bcc.UpdateControlPoint(arc_index, cp_index, (*bcc._attributes[arc_index].arc)[cp_index].x(), (*bcc._attributes[arc_index].arc)[cp_index].y(), z);
            update();
        }
    }

    void GLWidget::slot_set_position_arc_index(int arc_index, int cp_index){
        if(arc_index >= 0 && arc_index < (int) bcc._attributes.size() && cp_index < 4) {
            emit XcoordChanged((*bcc._attributes[arc_index].arc)[cp_index].x());
            emit YcoordChanged((*bcc._attributes[arc_index].arc)[cp_index].y());
            emit ZcoordChanged((*bcc._attributes[arc_index].arc)[cp_index].z());
        }
    }

    void GLWidget::slot_continue_arc(int arc_index, int side) {
        if(arc_index >= 0 && arc_index < (int) bcc._attributes.size()) {
            BiquarticCompositeCurve3::Direction dir = (!side) ? BiquarticCompositeCurve3::Direction::LEFT : BiquarticCompositeCurve3::Direction::RIGHT;
            GLboolean result = bcc.ContinueExisitingArc(arc_index, dir);
            if(result){
                emit updateArcCount((int)bcc._attributes.size() - 1);
            }
            update();
        }
    }

    void GLWidget::slot_join_arcs(int index_0, int side_0, int index_1, int side_1) {
        if(index_0 >= 0 && index_0 < (int) bcc._attributes.size() && index_1 >= 0 && index_1 < (int) bcc._attributes.size()) {
            BiquarticCompositeCurve3::Direction dir0 = (!side_0) ? BiquarticCompositeCurve3::Direction::LEFT : BiquarticCompositeCurve3::Direction::RIGHT;
            BiquarticCompositeCurve3::Direction dir1 = (!side_1) ? BiquarticCompositeCurve3::Direction::LEFT : BiquarticCompositeCurve3::Direction::RIGHT;
            GLboolean result = bcc.JoinExistingArcs(index_0, dir0, index_1, dir1);
            if(result){
                emit updateArcCount((int)bcc._attributes.size() - 1);
            }
            update();
        }
    }

    void GLWidget::slot_merge_arcs(int index_0, int side_0, int index_1, int side_1) {
        if(index_0 >= 0 && index_0 < (int) bcc._attributes.size() && index_1 >= 0 && index_1 < (int) bcc._attributes.size()) {
            BiquarticCompositeCurve3::Direction dir0 = (!side_0) ? BiquarticCompositeCurve3::Direction::LEFT : BiquarticCompositeCurve3::Direction::RIGHT;
            BiquarticCompositeCurve3::Direction dir1 = (!side_1) ? BiquarticCompositeCurve3::Direction::LEFT : BiquarticCompositeCurve3::Direction::RIGHT;
            GLboolean result = bcc.MergeExistingArcs(index_0, dir0, index_1, dir1);
            if(result){
                emit updateArcCount((int)bcc._attributes.size() - 1);
            }
            update();
        }
    }

    void GLWidget::slot_save_arcs(){
        try{
            if(bcc._attributes.size() > 0) {
                fstream f("arc.data", ios_base::out);
                f << bcc;
                f.close();
                for(GLint i = bcc._attributes.size() - 1; i >= 0; i--){
                    bcc.DeleteExistingArc(i);
                }
                emit updateArcCount((int)bcc._attributes.size() - 1);
                update();
            } else {
                throw Exception("No arcs to be saved!");
            }
        } catch(Exception e){
            e.what();
        }
    }

    void GLWidget::slot_load_arcs(){
        try{
            if(bcc._attributes.size() == 0) {
                fstream f("arc.data", ios_base::in);
                f >> bcc;
                f.close();
                emit updateArcCount((int)bcc._attributes.size() - 1);
                update();
            } else {
                throw Exception("BCC should be empty!");
            }
        } catch(Exception e){
            e.what();
        }
    }

    //---------------------------------------------------------------------------------------------------------------------------------------------------
    void GLWidget::slot_show_highlightingPatch(int index1, int index2) {
        bcs.disableAllHighlighting();
        if(index1 >= 0 && index1 < (int) bcs._attributes.size()) {
            bcs._attributes[index1].setHighlighting(GL_TRUE);
        }
        if(index1 >= 0 && index1 < (int) bcs._attributes.size()) {
            bcs._attributes[index2].setHighlighting(GL_TRUE);
        }
        update();
    }

    void GLWidget::slot_insert_new_patch() {
        bcs.InsertNewPatch();
        emit updatePatchCount((int)bcs._attributes.size() - 1);
        update();
    }

    void GLWidget::slot_delete_patch(int index) {
        if(index >= 0 && index < (int) bcs._attributes.size()) {
            bcs.DeleteExistingPatch(index);
            emit updatePatchCount((int)bcs._attributes.size() - 1);
            update();
        }
    }
    void GLWidget::slot_set_material_patch(int index, int material_index) {
        if(index >= 0 && index < (int) bcs._attributes.size()) {
            bcs._attributes[index].material = material_index - 1;
            update();
        }
    }

    void GLWidget::slot_set_material_patch_index(int index) {
        if(index >= 0 && index < (int) bcs._attributes.size()) {
            emit materialChanged(bcs._attributes[index].material + 1);
        }
    }

    void GLWidget::slot_continue_patch(int patch_index, int side) {
        if(patch_index >= 0 && patch_index < (int) bcs._attributes.size()) {
            BiquarticCompositeSurface3::Direction dir;
            switch (side) {
            case 0:
                dir = BiquarticCompositeSurface3::Direction::N;
                break;
            case 1:
                dir = BiquarticCompositeSurface3::Direction::NE;
                break;
            case 2:
                dir = BiquarticCompositeSurface3::Direction::E;
                break;
            case 3:
                dir = BiquarticCompositeSurface3::Direction::SE;
                break;
            case 4:
                dir = BiquarticCompositeSurface3::Direction::S;
                break;
            case 5:
                dir = BiquarticCompositeSurface3::Direction::SW;
                break;
            case 6:
                dir = BiquarticCompositeSurface3::Direction::W;
                break;
            case 7:
                dir = BiquarticCompositeSurface3::Direction::NW;
                break;
            }

            GLboolean result = bcs.ContinueExistingPatch(patch_index, dir);
            if(result){
                emit updatePatchCount((int)bcs._attributes.size() - 1);
            }
            update();
        }
    }

    void GLWidget::slot_join_patches(int index_0, int side_0, int index_1, int side_1) {
        if(index_0 >= 0 && index_0 < (int) bcs._attributes.size() && index_1 >= 0 && index_1 < (int) bcs._attributes.size()) {
            BiquarticCompositeSurface3::Direction dir0, dir1;

            switch (side_0) {
            case 0:
                dir0 = BiquarticCompositeSurface3::Direction::N;
                break;
            case 1:
                dir0 = BiquarticCompositeSurface3::Direction::NE;
                break;
            case 2:
                dir0 = BiquarticCompositeSurface3::Direction::E;
                break;
            case 3:
                dir0 = BiquarticCompositeSurface3::Direction::SE;
                break;
            case 4:
                dir0 = BiquarticCompositeSurface3::Direction::S;
                break;
            case 5:
                dir0 = BiquarticCompositeSurface3::Direction::SW;
                break;
            case 6:
                dir0 = BiquarticCompositeSurface3::Direction::W;
                break;
            case 7:
                dir0 = BiquarticCompositeSurface3::Direction::NW;
                break;
            }

            switch (side_1) {
            case 0:
                dir1 = BiquarticCompositeSurface3::Direction::N;
                break;
            case 1:
                dir1 = BiquarticCompositeSurface3::Direction::NE;
                break;
            case 2:
                dir1 = BiquarticCompositeSurface3::Direction::E;
                break;
            case 3:
                dir1 = BiquarticCompositeSurface3::Direction::SE;
                break;
            case 4:
                dir1 = BiquarticCompositeSurface3::Direction::S;
                break;
            case 5:
                dir1 = BiquarticCompositeSurface3::Direction::SW;
                break;
            case 6:
                dir1 = BiquarticCompositeSurface3::Direction::W;
                break;
            case 7:
                dir1 = BiquarticCompositeSurface3::Direction::NW;
                break;
            }

            GLboolean result = bcs.JoinExistingPatches(index_0, dir0, index_1, dir1);
            if(result){
                emit updatePatchCount((int)bcs._attributes.size() - 1);
            }
            update();
        }
    }

    void GLWidget::slot_merge_patches(int index_0, int side_0, int index_1, int side_1) {
        if(index_0 >= 0 && index_0 < (int) bcs._attributes.size() && index_1 >= 0 && index_1 < (int) bcs._attributes.size()) {
            BiquarticCompositeSurface3::Direction dir0, dir1;

            switch (side_0) {
            case 0:
                dir0 = BiquarticCompositeSurface3::Direction::N;
                break;
            case 1:
                dir0 = BiquarticCompositeSurface3::Direction::NE;
                break;
            case 2:
                dir0 = BiquarticCompositeSurface3::Direction::E;
                break;
            case 3:
                dir0 = BiquarticCompositeSurface3::Direction::SE;
                break;
            case 4:
                dir0 = BiquarticCompositeSurface3::Direction::S;
                break;
            case 5:
                dir0 = BiquarticCompositeSurface3::Direction::SW;
                break;
            case 6:
                dir0 = BiquarticCompositeSurface3::Direction::W;
                break;
            case 7:
                dir0 = BiquarticCompositeSurface3::Direction::NW;
                break;
            }

            switch (side_1) {
            case 0:
                dir1 = BiquarticCompositeSurface3::Direction::N;
                break;
            case 1:
                dir1 = BiquarticCompositeSurface3::Direction::NE;
                break;
            case 2:
                dir1 = BiquarticCompositeSurface3::Direction::E;
                break;
            case 3:
                dir1 = BiquarticCompositeSurface3::Direction::SE;
                break;
            case 4:
                dir1 = BiquarticCompositeSurface3::Direction::S;
                break;
            case 5:
                dir1 = BiquarticCompositeSurface3::Direction::SW;
                break;
            case 6:
                dir1 = BiquarticCompositeSurface3::Direction::W;
                break;
            case 7:
                dir1 = BiquarticCompositeSurface3::Direction::NW;
                break;
            }

            GLboolean result = bcs.MergeExistingPatches(index_0, dir0, index_1, dir1);
            if(result){
                emit updatePatchCount((int)bcs._attributes.size() - 1);
            }
            update();
        }
    }

    void GLWidget::slot_set_position_patch_index(int patch_index, int patch_cp_index){
        if(patch_index >= 0 && patch_index < (int) bcs._attributes.size() && patch_cp_index < 16) {
            int i = patch_cp_index / 4, j = patch_cp_index % 4;
            emit XcoordChangedPatch((*bcs._attributes[patch_index].patch)(i,j).x());
            emit YcoordChangedPatch((*bcs._attributes[patch_index].patch)(i,j).y());
            emit ZcoordChangedPatch((*bcs._attributes[patch_index].patch)(i,j).z());
        }
    }

    void GLWidget::slot_set_x_patch(int patch_index, int patch_cp_index, double x){
        if(patch_index >= 0 && patch_index < (int) bcs._attributes.size() && patch_cp_index < 16) {
            int i = patch_cp_index / 4, j = patch_cp_index % 4;
            bcs.UpdateControlPoint(patch_index, i, j, x, (*bcs._attributes[patch_index].patch)(i,j).y(), (*bcs._attributes[patch_index].patch)(i,j).z());
            update();
        }
    }

    void GLWidget::slot_set_y_patch(int patch_index, int patch_cp_index, double y){
        if(patch_index >= 0 && patch_index < (int) bcs._attributes.size() && patch_cp_index < 16) {
            int i = patch_cp_index / 4, j = patch_cp_index % 4;
            bcs.UpdateControlPoint(patch_index, i, j, (*bcs._attributes[patch_index].patch)(i,j).x(), y, (*bcs._attributes[patch_index].patch)(i,j).z());
            update();
        }
    }

    void GLWidget::slot_set_z_patch(int patch_index, int patch_cp_index, double z){
        if(patch_index >= 0 && patch_index < (int) bcs._attributes.size() && patch_cp_index < 16) {
            int i = patch_cp_index / 4, j = patch_cp_index % 4;
            bcs.UpdateControlPoint(patch_index, i, j, (*bcs._attributes[patch_index].patch)(i,j).x(), (*bcs._attributes[patch_index].patch)(i,j).y(), z);
            update();
        }
    }

    void GLWidget::slot_save_patches(){
        try{
            if(bcs._attributes.size() > 0) {
                fstream f("patch.data", ios_base::out);
                f << bcs;
                f.close();
                for(GLint i = bcs._attributes.size() - 1; i >= 0; i--){
                    bcs.DeleteExistingPatch(i);
                }
                emit updatePatchCount((int)bcs._attributes.size() - 1);
                update();
            } else {
                throw Exception("No arcs to be saved!");
            }
        } catch(Exception e){
            e.what();
        }
    }

    void GLWidget::slot_load_patches(){
        try{
            if(bcs._attributes.size() == 0) {
                fstream f("patch.data", ios_base::in);
                f >> bcs;
                f.close();
                emit updatePatchCount((int)bcs._attributes.size() - 1);
                update();
            } else {
                throw Exception("BCC should be empty!");
            }
        } catch(Exception e){
            e.what();
        }
    }
    //---------------------------------------------------------------------------------------------------------------------------------------------------
    // Light
    void GLWidget::_createLight(){
        DirectionalLight _dl = DirectionalLight(GL_LIGHT0,position1,ambient_intensity1,diffuse_intensity1,specular_intensity1);
        DirectionalLight _dl1 = DirectionalLight(GL_LIGHT1,position2,ambient_intensity2,diffuse_intensity2,specular_intensity2);
    }

    void GLWidget::installShader(){
        /*if(!_shader.InstallShaders("Shaders/directional_light.vert", "Shaders/directional_light.frag", GL_FALSE)){
            throw Exception("Could not install directional light shader!");
        }*/
        if(!_shader.InstallShaders("Shaders/two_sided_lighting.vert", "Shaders/two_sided_lighting.frag", GL_FALSE)){
            throw Exception("Could not install two sided lighting shader!");
        }
    }

    void GLWidget::slot_change_light_position(int light_index,double p_1,double p_2,double p_3,double p_4){
        if(light_index == 0){
            position1=HCoordinate3(p_1,p_2,p_3,p_4);
            DirectionalLight _dl = DirectionalLight(GL_LIGHT0,position1,ambient_intensity1,diffuse_intensity1,specular_intensity1);
        }
        if(light_index == 1){
            position2=HCoordinate3(p_1,p_2,p_3,p_4);
            DirectionalLight _dl2 = DirectionalLight(GL_LIGHT1,position2,ambient_intensity2,diffuse_intensity2,specular_intensity2);
        }
        installShader();
    }

    void GLWidget::slot_change_light_ambient(int light_index,double p_1,double p_2,double p_3,double p_4){
        if(light_index == 0){
            ambient_intensity1 = Color4(p_1,p_2,p_3,p_4);
            DirectionalLight _dl = DirectionalLight(GL_LIGHT0,position1,ambient_intensity1,diffuse_intensity1,specular_intensity1);
        }
        if(light_index == 1){
            ambient_intensity2 = Color4(p_1,p_2,p_3,p_4);
            DirectionalLight _dl2 = DirectionalLight(GL_LIGHT1,position2,ambient_intensity2,diffuse_intensity2,specular_intensity2);
        }
        installShader();
    }

    void GLWidget::slot_change_light_diffuse(int light_index,double p_1,double p_2,double p_3,double p_4){
        if(light_index == 0){
            diffuse_intensity1 = Color4(p_1,p_2,p_3,p_4);
            DirectionalLight _dl = DirectionalLight(GL_LIGHT0,position1,ambient_intensity1,diffuse_intensity1,specular_intensity1);
        }
        if(light_index == 1){
            diffuse_intensity2 = Color4(p_1,p_2,p_3,p_4);
            DirectionalLight _dl2 = DirectionalLight(GL_LIGHT1,position2,ambient_intensity2,diffuse_intensity2,specular_intensity2);
        }
        installShader();
    }

    void GLWidget::slot_change_light_specular(int light_index,double p_1,double p_2,double p_3,double p_4){
        if(light_index == 0){
            specular_intensity1 = Color4(p_1,p_2,p_3,p_4);
            DirectionalLight _dl = DirectionalLight(GL_LIGHT0,position1,ambient_intensity1,diffuse_intensity1,specular_intensity1);
        }
        if(light_index == 1){
            specular_intensity2 = Color4(p_1,p_2,p_3,p_4);
            DirectionalLight _dl2 = DirectionalLight(GL_LIGHT1,position2,ambient_intensity2,diffuse_intensity2,specular_intensity2);
        }
        installShader();
    }

    void GLWidget::slot_lighting_index_changed(int index){
        if(index == -1){
            return;
        }
        HCoordinate3* position;
        Color4 *ambient, *diffuse, *specular;
        if(index == 0){
            position = &position1;
            ambient = &ambient_intensity1;
            diffuse = &diffuse_intensity1;
            specular = &specular_intensity1;
        } else {
            position = &position2;
            ambient = &ambient_intensity2;
            diffuse = &diffuse_intensity2;
            specular = &specular_intensity2;
        }
        emit positionXChanged(position->x());
        emit positionYChanged(position->y());
        emit positionZChanged(position->z());
        emit positionWChanged(position->w());
        emit ambientRChanged(ambient->r());
        emit ambientGChanged(ambient->g());
        emit ambientBChanged(ambient->b());
        emit ambientAChanged(ambient->a());
        emit diffuseRChanged(diffuse->r());
        emit diffuseGChanged(diffuse->g());
        emit diffuseBChanged(diffuse->b());
        emit diffuseAChanged(diffuse->a());
        emit specularRChanged(specular->r());
        emit specularGChanged(specular->b());
        emit specularBChanged(specular->g());
        emit specularAChanged(specular->a());
    }

    void GLWidget::slot_disable_shader(){
        _shader_enabled = 1 - _shader_enabled;
        update();
    }

    void GLWidget::_createTexture(){
        texture = new QOpenGLTexture(QImage(_texture_name).mirrored());
        texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
        texture->setMagnificationFilter(QOpenGLTexture::Linear);
    }

    void GLWidget::slot_change_texture(int index){
        if(index == -1){
            _textured = false;
            update();
            return;
        }
        if(index >= 0 && index <= 9){
            sprintf(_texture_name,"./Textures/%d.jpg",index+1);
            _textured = true;
            _createTexture();
            update();
        }
    }
}
