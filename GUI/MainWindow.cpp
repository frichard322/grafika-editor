#include "MainWindow.h"

namespace cagd
{
    MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
    {
        setupUi(this);

    /*

      the structure of the main window's central widget

     *---------------------------------------------------*
     |                 central widget                    |
     |                                                   |
     |  *---------------------------*-----------------*  |
     |  |     rendering context     |   scroll area   |  |
     |  |       OpenGL widget       | *-------------* |  |
     |  |                           | | side widget | |  |
     |  |                           | |             | |  |
     |  |                           | |             | |  |
     |  |                           | *-------------* |  |
     |  *---------------------------*-----------------*  |
     |                                                   |
     *---------------------------------------------------*

    */
        _side_widget = new SideWidget(this);

        _scroll_area = new QScrollArea(this);
        _scroll_area->setWidget(_side_widget);
        _scroll_area->setSizePolicy(_side_widget->sizePolicy());
        _scroll_area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

        _gl_widget = new GLWidget(this);

        centralWidget()->setLayout(new QHBoxLayout());
        centralWidget()->layout()->addWidget(_gl_widget);
        centralWidget()->layout()->addWidget(_scroll_area);

        // creating a signal slot mechanism between the rendering context and the side widget
        // rotations
        connect(_side_widget->rotate_x_slider, SIGNAL(valueChanged(int)), _gl_widget, SLOT(set_angle_x(int)));
        connect(_side_widget->rotate_y_slider, SIGNAL(valueChanged(int)), _gl_widget, SLOT(set_angle_y(int)));
        connect(_side_widget->rotate_z_slider, SIGNAL(valueChanged(int)), _gl_widget, SLOT(set_angle_z(int)));
        // zoom
        connect(_side_widget->zoom_factor_spin_box, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_zoom_factor(double)));
        // transitions
        connect(_side_widget->trans_x_spin_box, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_trans_x(double)));
        connect(_side_widget->trans_y_spin_box, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_trans_y(double)));
        connect(_side_widget->trans_z_spin_box, SIGNAL(valueChanged(double)), _gl_widget, SLOT(set_trans_z(double)));
        // arc related signal-slot mechanizm
        // arc count
        connect(_gl_widget, SIGNAL(updateArcCount(int)), _side_widget, SLOT(slot_set_maximum_arc_count(int)));
        // insert arc
        connect(_side_widget->insertNewArc, SIGNAL(clicked()), _gl_widget, SLOT(slot_insert_new_arc()));
        // delete arc
        connect(_side_widget->deleteButton, SIGNAL(clicked()), _side_widget, SLOT(slot_clicked_delete()));
        connect(_side_widget, SIGNAL(clickedDelete(int)), _gl_widget, SLOT(slot_delete_arc(int)));
        // color arc
        connect(_side_widget->chooseColorArc, SIGNAL(valueChanged(int)), _gl_widget, SLOT(slot_set_color_arc_index(int)));
        connect(_gl_widget, SIGNAL(redColorChanged(double)), _side_widget->chooseColorRed, SLOT(setValue(double)));
        connect(_gl_widget, SIGNAL(greenColorChanged(double)), _side_widget->chooseColorGreen, SLOT(setValue(double)));
        connect(_gl_widget, SIGNAL(blueColorChanged(double)), _side_widget->chooseColorBlue, SLOT(setValue(double)));
        connect(_side_widget->chooseColorRed, SIGNAL(valueChanged(double)), _side_widget, SLOT(slot_changed_red(double)));
        connect(_side_widget->chooseColorGreen, SIGNAL(valueChanged(double)), _side_widget, SLOT(slot_changed_green(double)));
        connect(_side_widget->chooseColorBlue, SIGNAL(valueChanged(double)), _side_widget, SLOT(slot_changed_blue(double)));
        connect(_side_widget, SIGNAL(changedRed(int, double)), _gl_widget, SLOT(slot_set_red_arc(int, double)));
        connect(_side_widget, SIGNAL(changedGreen(int, double)), _gl_widget, SLOT(slot_set_green_arc(int, double)));
        connect(_side_widget, SIGNAL(changedBlue(int, double)), _gl_widget, SLOT(slot_set_blue_arc(int, double)));
        // visibility arc
        connect(_side_widget->checkBoxControlPoints, SIGNAL(stateChanged(int)), _gl_widget, SLOT(SetControlPointVisibility()));
        connect(_side_widget->checkBoxControlLines, SIGNAL(stateChanged(int)), _gl_widget, SLOT(SetControlLineVisibility()));
        connect(_side_widget->checkBoxDerivate1, SIGNAL(stateChanged(int)), _gl_widget, SLOT(SetFirstOrderDerivateVisibility()));
        connect(_side_widget->checkBoxDerivate2, SIGNAL(stateChanged(int)), _gl_widget, SLOT(SetSecondOrderDerivateVisibility()));
        // movement arc
        connect(_side_widget->chooseMoveArc, SIGNAL(valueChanged(int)), _side_widget, SLOT(slot_changed_move_arc_index(int)));
        connect(_side_widget->chooseMoveCtrlPoint, SIGNAL(valueChanged(int)), _side_widget, SLOT(slot_changed_move_cp_index(int)));
        connect(_side_widget, SIGNAL(changedMoveArcIndex(int, int)), _gl_widget, SLOT(slot_set_position_arc_index(int, int)));
        connect(_side_widget, SIGNAL(changedMoveCpIndex(int, int)), _gl_widget, SLOT(slot_set_position_arc_index(int, int)));
        connect(_gl_widget, SIGNAL(XcoordChanged(double)), _side_widget->chooseMoveX, SLOT(setValue(double)));
        connect(_gl_widget, SIGNAL(YcoordChanged(double)), _side_widget->chooseMoveY, SLOT(setValue(double)));
        connect(_gl_widget, SIGNAL(ZcoordChanged(double)), _side_widget->chooseMoveZ, SLOT(setValue(double)));
        connect(_side_widget->chooseMoveX, SIGNAL(valueChanged(double)), _side_widget, SLOT(slot_changed_X(double)));
        connect(_side_widget->chooseMoveY, SIGNAL(valueChanged(double)), _side_widget, SLOT(slot_changed_Y(double)));
        connect(_side_widget->chooseMoveZ, SIGNAL(valueChanged(double)), _side_widget, SLOT(slot_changed_Z(double)));
        connect(_side_widget, SIGNAL(changedX(int, int, double)), _gl_widget, SLOT(slot_set_x_arc(int, int, double)));
        connect(_side_widget, SIGNAL(changedY(int, int, double)), _gl_widget, SLOT(slot_set_y_arc(int, int, double)));
        connect(_side_widget, SIGNAL(changedZ(int, int, double)), _gl_widget, SLOT(slot_set_z_arc(int, int, double)));
        // continue arc
        connect(_side_widget->continueButton, SIGNAL(clicked()), _side_widget, SLOT(slot_continue_arc()));
        connect(_side_widget, SIGNAL(continueArc(int, int)), _gl_widget, SLOT(slot_continue_arc(int, int)));
        // join arcs
        connect(_side_widget->joinButton, SIGNAL(clicked()), _side_widget, SLOT(slot_join_arcs()));
        connect(_side_widget, SIGNAL(joinArcs(int, int, int, int)), _gl_widget, SLOT(slot_join_arcs(int, int, int, int)));
        // merge arcs
        connect(_side_widget->mergeButton, SIGNAL(clicked()), _side_widget, SLOT(slot_merge_arcs()));
        connect(_side_widget, SIGNAL(mergeArcs(int, int, int, int)), _gl_widget, SLOT(slot_merge_arcs(int, int, int, int)));
        // highlighting arcs
        connect(_side_widget->chooseDeleteArc, SIGNAL(valueChanged(int)), _gl_widget, SLOT(slot_show_highlightingArc(int)));
        connect(_side_widget->chooseColorArc, SIGNAL(valueChanged(int)), _gl_widget, SLOT(slot_show_highlightingArc(int)));
        connect(_side_widget->chooseMoveArc, SIGNAL(valueChanged(int)), _gl_widget, SLOT(slot_show_highlightingArc(int)));
        connect(_side_widget->chooseContinueArc, SIGNAL(valueChanged(int)), _gl_widget, SLOT(slot_show_highlightingArc(int)));
        connect(_side_widget->chooseJoinArc_1, SIGNAL(valueChanged(int)), _side_widget, SLOT(slot_changed_join_indexArc()));
        connect(_side_widget, SIGNAL(changedJoinIndexArc(int, int)), _gl_widget, SLOT(slot_show_highlightingArc(int, int)));
        connect(_side_widget->chooseJoinArc_2, SIGNAL(valueChanged(int)), _side_widget, SLOT(slot_changed_join_indexArc()));
        connect(_side_widget, SIGNAL(changedJoinIndexArc(int, int)), _gl_widget, SLOT(slot_show_highlightingArc(int, int)));
        connect(_side_widget->chooseMergeArc_1, SIGNAL(valueChanged(int)), _side_widget, SLOT(slot_changed_merge_indexArc()));
        connect(_side_widget, SIGNAL(changedMergeIndexArc(int, int)), _gl_widget, SLOT(slot_show_highlightingArc(int, int)));
        connect(_side_widget->chooseMergeArc_2, SIGNAL(valueChanged(int)), _side_widget, SLOT(slot_changed_merge_indexArc()));
        connect(_side_widget, SIGNAL(changedMergeIndexArc(int, int)), _gl_widget, SLOT(slot_show_highlightingArc(int, int)));
        // saving arcs
        connect(_side_widget->saveArcButton, SIGNAL(clicked()), _gl_widget, SLOT(slot_save_arcs()));
        // loading arcs
        connect(_side_widget->loadArcButton, SIGNAL(clicked()), _gl_widget, SLOT(slot_load_arcs()));
        // -----------------------------------------------------------------------------------------------------------------
        // patch related signal-slot mechanizm
        // patch count
        connect(_gl_widget, SIGNAL(updatePatchCount(int)), _side_widget, SLOT(slot_set_maximum_patch_count(int)));
        // insert patch
        connect(_side_widget->insertNewPatch, SIGNAL(clicked()), _gl_widget, SLOT(slot_insert_new_patch()));
        // delete patch
        connect(_side_widget->deleteButton_Patch, SIGNAL(clicked()), _side_widget, SLOT(slot_clicked_delete_patch()));
        connect(_side_widget, SIGNAL(clickedDeletePatch(int)), _gl_widget, SLOT(slot_delete_patch(int)));
        // visibility patch
        connect(_side_widget->checkBoxControlPoints_Patch, SIGNAL(stateChanged(int)), _gl_widget, SLOT(SetControlPointVisibilityPatch()));
        connect(_side_widget->checkBoxControlLines_Patch, SIGNAL(stateChanged(int)), _gl_widget, SLOT(SetControlLineVisibilityPatch()));
        connect(_side_widget->checkBoxUDerivates_Patch, SIGNAL(stateChanged(int)), _gl_widget, SLOT(SetUDerivateVisibilityPatch()));
        connect(_side_widget->checkBoxVDerivates_Patch, SIGNAL(stateChanged(int)), _gl_widget, SLOT(SetVDerivateVisibilityPatch()));
        connect(_side_widget->checkBoxISO_U_Patch, SIGNAL(stateChanged(int)), _gl_widget, SLOT(SetUISOVisibilityPatch()));
        connect(_side_widget->checkBoxISO_V_Patch, SIGNAL(stateChanged(int)), _gl_widget, SLOT(SetVISOVisibilityPatch()));
        connect(_side_widget->checkBoxNormalVectors_Patch, SIGNAL(stateChanged(int)), _gl_widget, SLOT(SetNormalsVisibilityPatch()));
        connect(_side_widget->checkBoxPatch, SIGNAL(stateChanged(int)), _gl_widget, SLOT(SetVisibilityPatch()));
        // material patch
        connect(_side_widget->chooseMaterial_Patch, SIGNAL(valueChanged(int)), _gl_widget, SLOT(slot_set_material_patch_index(int)));
        connect(_gl_widget, SIGNAL(materialChanged(int)), _side_widget->chooseMaterial_Index_Patch, SLOT(setCurrentIndex(int)));
        connect(_side_widget->chooseMaterial_Index_Patch, SIGNAL(currentIndexChanged(int)), _side_widget, SLOT(slot_changed_material(int)));
        connect(_side_widget, SIGNAL(changedMaterial(int, int)), _gl_widget, SLOT(slot_set_material_patch(int, int)));
        // continue patch
        connect(_side_widget->continueButton_Patch, SIGNAL(clicked()), _side_widget, SLOT(slot_continue_patch()));
        connect(_side_widget, SIGNAL(continuePatch(int, int)), _gl_widget, SLOT(slot_continue_patch(int, int)));
        // join patch
        connect(_side_widget->joinButton_Patch, SIGNAL(clicked()), _side_widget, SLOT(slot_join_patches()));
        connect(_side_widget, SIGNAL(joinPatches(int, int, int, int)), _gl_widget, SLOT(slot_join_patches(int, int, int, int)));
        // merge patch
        connect(_side_widget->mergeButton_Patch, SIGNAL(clicked()), _side_widget, SLOT(slot_merge_patches()));
        connect(_side_widget, SIGNAL(mergePatches(int, int, int, int)), _gl_widget, SLOT(slot_merge_patches(int, int, int, int)));
        // movement patch
        connect(_side_widget->chooseMove_Patch, SIGNAL(valueChanged(int)), _side_widget, SLOT(slot_changed_move_patch_index(int)));
        connect(_side_widget->chooseMoveCtrlPoint_Patch, SIGNAL(valueChanged(int)), _side_widget, SLOT(slot_changed_move_patch_cp_index(int)));
        connect(_side_widget, SIGNAL(changedMovePatchIndex(int, int)), _gl_widget, SLOT(slot_set_position_patch_index(int, int)));
        connect(_side_widget, SIGNAL(changedMovePatchCpIndex(int, int)), _gl_widget, SLOT(slot_set_position_patch_index(int, int)));
        connect(_gl_widget, SIGNAL(XcoordChangedPatch(double)), _side_widget->chooseMoveX_Patch, SLOT(setValue(double)));
        connect(_gl_widget, SIGNAL(YcoordChangedPatch(double)), _side_widget->chooseMoveY_Patch, SLOT(setValue(double)));
        connect(_gl_widget, SIGNAL(ZcoordChangedPatch(double)), _side_widget->chooseMoveZ_Patch, SLOT(setValue(double)));
        connect(_side_widget->chooseMoveX_Patch, SIGNAL(valueChanged(double)), _side_widget, SLOT(slot_changed_X_patch(double)));
        connect(_side_widget->chooseMoveY_Patch, SIGNAL(valueChanged(double)), _side_widget, SLOT(slot_changed_Y_patch(double)));
        connect(_side_widget->chooseMoveZ_Patch, SIGNAL(valueChanged(double)), _side_widget, SLOT(slot_changed_Z_patch(double)));
        connect(_side_widget, SIGNAL(changedXPatch(int, int, double)), _gl_widget, SLOT(slot_set_x_patch(int, int, double)));
        connect(_side_widget, SIGNAL(changedYPatch(int, int, double)), _gl_widget, SLOT(slot_set_y_patch(int, int, double)));
        connect(_side_widget, SIGNAL(changedZPatch(int, int, double)), _gl_widget, SLOT(slot_set_z_patch(int, int, double)));
        // highlighting patches
        connect(_side_widget->chooseDelete_Patch, SIGNAL(valueChanged(int)), _gl_widget, SLOT(slot_show_highlightingPatch(int)));
        connect(_side_widget->chooseMaterial_Patch, SIGNAL(valueChanged(int)), _gl_widget, SLOT(slot_show_highlightingPatch(int)));
        connect(_side_widget->chooseMove_Patch, SIGNAL(valueChanged(int)), _gl_widget, SLOT(slot_show_highlightingPatch(int)));
        connect(_side_widget->chooseContinue_Patch, SIGNAL(valueChanged(int)), _gl_widget, SLOT(slot_show_highlightingPatch(int)));
        connect(_side_widget->chooseJoin_Patch_1, SIGNAL(valueChanged(int)), _side_widget, SLOT(slot_changed_join_indexPatch()));
        connect(_side_widget, SIGNAL(changedJoinIndexPatch(int, int)), _gl_widget, SLOT(slot_show_highlightingPatch(int, int)));
        connect(_side_widget->chooseJoin_Patch_2, SIGNAL(valueChanged(int)), _side_widget, SLOT(slot_changed_join_indexPatch()));
        connect(_side_widget, SIGNAL(changedJoinIndexPatch(int, int)), _gl_widget, SLOT(slot_show_highlightingPatch(int, int)));
        connect(_side_widget->chooseMerge_Patch_1, SIGNAL(valueChanged(int)), _side_widget, SLOT(slot_changed_merge_indexPatch()));
        connect(_side_widget, SIGNAL(changedMergeIndexPatch(int, int)), _gl_widget, SLOT(slot_show_highlightingPatch(int, int)));
        connect(_side_widget->chooseMerge_Patch_2, SIGNAL(valueChanged(int)), _side_widget, SLOT(slot_changed_merge_indexPatch()));
        connect(_side_widget, SIGNAL(changedMergeIndexPatch(int, int)), _gl_widget, SLOT(slot_show_highlightingPatch(int, int)));
        // saving patches
        connect(_side_widget->savePatchButton, SIGNAL(clicked()), _gl_widget, SLOT(slot_save_patches()));
        // loading patches
        connect(_side_widget->loadPatchButton, SIGNAL(clicked()), _gl_widget, SLOT(slot_load_patches()));
        // light
        connect(_side_widget->lightButton, SIGNAL(clicked()), _side_widget, SLOT(slot_lighting_changed()));
        connect(_side_widget, SIGNAL(changedPosition(int, double, double, double, double)), _gl_widget, SLOT(slot_change_light_position(int, double, double, double, double)));
        connect(_side_widget, SIGNAL(changedAmbient(int, double, double, double, double)), _gl_widget, SLOT(slot_change_light_ambient(int, double, double, double, double)));
        connect(_side_widget, SIGNAL(changedDiffuse(int, double, double, double, double)), _gl_widget, SLOT(slot_change_light_diffuse(int, double, double, double, double)));
        connect(_side_widget, SIGNAL(changedSpecular(int, double, double, double, double)), _gl_widget, SLOT(slot_change_light_specular(int, double, double, double, double)));
        connect(_side_widget->chooseLight, SIGNAL(valueChanged(int)), _gl_widget, SLOT(slot_lighting_index_changed(int)));
        connect(_gl_widget, SIGNAL(positionXChanged(double)), _side_widget->light_position_x, SLOT(setValue(double)));
        connect(_gl_widget, SIGNAL(positionYChanged(double)), _side_widget->light_position_y, SLOT(setValue(double)));
        connect(_gl_widget, SIGNAL(positionZChanged(double)), _side_widget->light_position_z, SLOT(setValue(double)));
        connect(_gl_widget, SIGNAL(positionWChanged(double)), _side_widget->light_position_w, SLOT(setValue(double)));
        connect(_gl_widget, SIGNAL(ambientRChanged(double)), _side_widget->light_ambient_r, SLOT(setValue(double)));
        connect(_gl_widget, SIGNAL(ambientGChanged(double)), _side_widget->light_ambient_g, SLOT(setValue(double)));
        connect(_gl_widget, SIGNAL(ambientBChanged(double)), _side_widget->light_ambient_b, SLOT(setValue(double)));
        connect(_gl_widget, SIGNAL(ambientAChanged(double)), _side_widget->light_ambient_a, SLOT(setValue(double)));
        connect(_gl_widget, SIGNAL(diffuseRChanged(double)), _side_widget->light_diffuse_r, SLOT(setValue(double)));
        connect(_gl_widget, SIGNAL(diffuseGChanged(double)), _side_widget->light_diffuse_g, SLOT(setValue(double)));
        connect(_gl_widget, SIGNAL(diffuseBChanged(double)), _side_widget->light_diffuse_b, SLOT(setValue(double)));
        connect(_gl_widget, SIGNAL(diffuseAChanged(double)), _side_widget->light_diffuse_a, SLOT(setValue(double)));
        connect(_gl_widget, SIGNAL(specularRChanged(double)), _side_widget->light_specular_r, SLOT(setValue(double)));
        connect(_gl_widget, SIGNAL(specularGChanged(double)), _side_widget->light_specular_g, SLOT(setValue(double)));
        connect(_gl_widget, SIGNAL(specularBChanged(double)), _side_widget->light_specular_b, SLOT(setValue(double)));
        connect(_gl_widget, SIGNAL(specularAChanged(double)), _side_widget->light_specular_a, SLOT(setValue(double)));
        // shader
        connect(_side_widget->disableShader, SIGNAL(stateChanged(int)), _gl_widget, SLOT(slot_disable_shader()));
        // texture
        connect(_side_widget->chooseTexture_Patch, SIGNAL(valueChanged(int)), _gl_widget, SLOT(slot_change_texture(int)));
    }

    //--------------------------------
    // implementation of private slots
    //--------------------------------
    void MainWindow::on_action_Quit_triggered()
    {
        qApp->exit(0);
    }
}
