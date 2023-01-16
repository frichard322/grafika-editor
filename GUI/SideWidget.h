#pragma once

#include <QWidget>
#include "ui_SideWidget.h"

namespace cagd
{
    class SideWidget: public QWidget, public Ui::SideWidget
    {
        Q_OBJECT
    public:
        // special and default constructor
        SideWidget(QWidget *parent = 0);

    public slots:
        inline void slot_set_maximum_arc_count(int size) {
            this->chooseDeleteArc->setMaximum(size);
            this->chooseColorArc->setMaximum(size);
            this->chooseMoveArc->setMaximum(size);
            this->chooseJoinArc_1->setMaximum(size);
            this->chooseJoinArc_2->setMaximum(size);
            this->chooseMergeArc_1->setMaximum(size);
            this->chooseMergeArc_2->setMaximum(size);
            this->chooseContinueArc->setMaximum(size);
        }
        inline void slot_clicked_delete() {
            emit clickedDelete(this->chooseDeleteArc->value());
        }
        inline void slot_changed_red(double r) {
            emit changedRed(this->chooseColorArc->value(), r);
        }
        inline void slot_changed_green(double g) {
            emit changedGreen(this->chooseColorArc->value(), g);
        }
        inline void slot_changed_blue(double b) {
            emit changedBlue(this->chooseColorArc->value(), b);
        }
        inline void slot_changed_move_arc_index(int arc_index) {
            emit changedMoveArcIndex(arc_index, this->chooseMoveCtrlPoint->value());
        }
        inline void slot_changed_move_cp_index(int cp_index) {
            emit changedMoveCpIndex(this->chooseMoveArc->value(), cp_index);
        }
        inline void slot_changed_X(double x) {
            emit changedX(this->chooseMoveArc->value(), this->chooseMoveCtrlPoint->value(), x);
        }
        inline void slot_changed_Y(double y) {
            emit changedY(this->chooseMoveArc->value(), this->chooseMoveCtrlPoint->value(), y);
        }
        inline void slot_changed_Z(double z) {
            emit changedZ(this->chooseMoveArc->value(), this->chooseMoveCtrlPoint->value(), z);
        }
        inline void slot_continue_arc() {
            emit continueArc(this->chooseContinueArc->value(), this->chooseContinueDirection->currentIndex());
        }
        inline void slot_join_arcs() {
            emit joinArcs(this->chooseJoinArc_1->value(), this->chooseJoinDirection_1->currentIndex(), this->chooseJoinArc_2->value(), this->chooseJoinDirection_2->currentIndex());
        }
        inline void slot_merge_arcs() {
            emit mergeArcs(this->chooseMergeArc_1->value(), this->chooseMergeDirection_1->currentIndex(), this->chooseMergeArc_2->value(), this->chooseMergeDirection_2->currentIndex());
        }
        inline void slot_changed_join_indexArc() {
            emit changedJoinIndexArc(this->chooseJoinArc_1->value(), this->chooseJoinArc_2->value());
        }
        inline void slot_changed_merge_indexArc() {
            emit changedMergeIndexArc(this->chooseMergeArc_1->value(), this->chooseMergeArc_2->value());
        }
        //-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        // Patch
        inline void slot_set_maximum_patch_count(int size) {
            this->chooseDelete_Patch->setMaximum(size);
            this->chooseMaterial_Patch->setMaximum(size);
            this->chooseMove_Patch->setMaximum(size);
            this->chooseJoin_Patch_1->setMaximum(size);
            this->chooseJoin_Patch_2->setMaximum(size);
            this->chooseMerge_Patch_1->setMaximum(size);
            this->chooseMerge_Patch_2->setMaximum(size);
            this->chooseContinue_Patch->setMaximum(size);
        }

        inline void slot_clicked_delete_patch() {
            emit clickedDeletePatch(this->chooseDelete_Patch->value());
        }

        inline void slot_changed_material(int material_index) {
            emit changedMaterial(this->chooseMaterial_Patch->value(), material_index);
        }

        inline void slot_continue_patch() {
            emit continuePatch(this->chooseContinue_Patch->value(), this->chooseContinueDirection_Patch->currentIndex());
        }

        inline void slot_join_patches() {
            emit joinPatches(this->chooseJoin_Patch_1->value(), this->chooseJoinDirection_Patch_1->currentIndex(), this->chooseJoin_Patch_2->value(), this->chooseJoinDirection_Patch_2->currentIndex());
        }

        inline void slot_merge_patches() {
            emit mergePatches(this->chooseMerge_Patch_1->value(), this->chooseMergeDirection_Patch_1->currentIndex(), this->chooseMerge_Patch_2->value(), this->chooseMergeDirection_Patch_2->currentIndex());
        }

        inline void slot_changed_move_patch_index(int patch_index) {
            emit changedMovePatchIndex(patch_index, this->chooseMoveCtrlPoint_Patch->value());
        }

        inline void slot_changed_move_patch_cp_index(int patch_cp_index) {
            emit changedMovePatchCpIndex(this->chooseMove_Patch->value(), patch_cp_index);
        }

        inline void slot_changed_X_patch(double x) {
            emit changedXPatch(this->chooseMove_Patch->value(), this->chooseMoveCtrlPoint_Patch->value(), x);
        }
        inline void slot_changed_Y_patch(double y) {
            emit changedYPatch(this->chooseMove_Patch->value(), this->chooseMoveCtrlPoint_Patch->value(), y);
        }
        inline void slot_changed_Z_patch(double z) {
            emit changedZPatch(this->chooseMove_Patch->value(), this->chooseMoveCtrlPoint_Patch->value(), z);
        }
        inline void slot_changed_join_indexPatch() {
            emit changedJoinIndexPatch(this->chooseJoin_Patch_1->value(), this->chooseJoin_Patch_2->value());
        }
        inline void slot_changed_merge_indexPatch() {
            emit changedMergeIndexPatch(this->chooseMerge_Patch_1->value(), this->chooseMerge_Patch_2->value());
        }
        // Light
        inline void slot_lighting_changed() {
            emit changedPosition(this->chooseLight->value(),
                                 this->light_position_x->value(),
                                 this->light_position_y->value(),
                                 this->light_position_z->value(),
                                 this->light_position_w->value()
                                 );
            emit changedAmbient(this->chooseLight->value(),
                                this->light_ambient_r->value(),
                                this->light_ambient_g->value(),
                                this->light_ambient_b->value(),
                                this->light_ambient_a->value()
                                );
            emit changedDiffuse(this->chooseLight->value(),
                                this->light_diffuse_r->value(),
                                this->light_diffuse_g->value(),
                                this->light_diffuse_b->value(),
                                this->light_diffuse_a->value()
                                );
            emit changedSpecular(this->chooseLight->value(),
                                 this->light_specular_r->value(),
                                 this->light_specular_g->value(),
                                 this->light_specular_b->value(),
                                 this->light_specular_a->value()
                                 );
        }
    signals:
        // Arc
        void clickedDelete(int);
        void changedRed(int, double);
        void changedGreen(int, double);
        void changedBlue(int, double);
        void changedX(int, int, double);
        void changedY(int, int, double);
        void changedZ(int, int, double);
        void changedMoveArcIndex(int, int);
        void changedMoveCpIndex(int, int);
        void continueArc(int, int);
        void joinArcs(int, int, int, int);
        void mergeArcs(int, int, int, int);
        void changedJoinIndexArc(int, int);
        void changedMergeIndexArc(int, int);
        //---------------------------------
        // Patch
        void clickedDeletePatch(int);
        void changedMaterial(int, int);
        void continuePatch(int, int);
        void joinPatches(int, int, int, int);
        void mergePatches(int, int, int, int);
        void changedMovePatchIndex(int, int);
        void changedMovePatchCpIndex(int, int);
        void changedXPatch(int, int, double);
        void changedYPatch(int, int, double);
        void changedZPatch(int, int, double);
        void changedJoinIndexPatch(int, int);
        void changedMergeIndexPatch(int, int);
        //---------------------------------
        // Light
        void changedPosition(int, double, double, double, double);
        void changedAmbient(int, double, double, double, double);
        void changedDiffuse(int, double, double, double, double);
        void changedSpecular(int, double, double, double, double);
    };
}
