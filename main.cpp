#include "main.h"
#include "Model.h"
#include "QDebug"
#include "Animation.h"
#include "Skeleton.h"
#include <QtMath>

void exportTestStuff() {
    QString path = "C:/Users/Typhox/Desktop/exp/";

    Model m = Model::fromFile(path + "TOWER_DRAGON.MESH.xml");
    Skeleton bindPose = Skeleton::fromFile(path + "TOWER_DRAGON.SKELETON.xml");
    m.addSkeleton(bindPose);

    m.exportMDL("dragon.mdl");

    Animation anim = Animation::fromFile(path + "IDLE.SKELETON.xml");
    anim.applyBindPose(bindPose);
    anim.exportAnm("dragon_idle.anm");
}

void exportAnim(Skeleton bindPose, QString name, ExtraData extraData = ExtraData()) {
    Animation anim = Animation::fromFile("animations/" + name.toUpper() + ".SKELETON.xml");
    anim.applyBindPose(bindPose);
    anim.setExtraData(extraData);
    anim.exportAnm("dragon/anm/dragon_" + name.toLower() + ".anm");
}

void exportRealStuff() {
    Model m = Model::fromFile("TOWER_DRAGON.MESH.xml");
    Skeleton bindPose = Skeleton::fromFile("TOWER_DRAGON.SKELETON.xml");
    m.addSkeleton(bindPose);

    m.exportMDL("dragon.mdl");

    Model mDummy = Model::dummy();
    mDummy.exportMDL("dummy.mdl");

    /* BOSSDRAGONATTACK         -> SwipeRight
     * BOSSDRAGONSNORT          -> genericSound2
     * HUGESWING                -> genericSound3
     *
     * SHOCKEXPLODE             -> genericSound1
     * DRAGONFLAP               -> genericSound4
     * BOSSDRAGONATTACK2        -> specialAttackSound1
     * WHIPBIG                  -> specialAttackSound2
     * WHIP                     -> specialAttackSound3
     * FLAPBIGDRAGON            -> specialAttackSound4
     * BOSSDRAGONFIRERELEASE    -> voiceSound1
     * BREATHINTAKE             -> voiceSound2
     * BOSSDRAGONLAND           -> voiceSound3
     * NETHBOSSCAST             -> voxSound
     * NETHBOSSROAR
     * NETHBRUTELONGATTACK
     * DESERTGUST
     * BODYLANDBIG              -> bodyFallSound
     * BOSSDRAGONDEATH          -> deathSound1
     *
     * FOOTSTEP -> L Footstep
     */

    exportAnim(bindPose, "idle", { {
                                       {"genericSound2", 4},
                                       {"genericSound2", 9},
                                   } });

    exportAnim(bindPose, "attack1", { {
                                          {"RightHandHit", 15},
                                          {"SwipeRight", 7},
                                          {"genericSound2", 33},
                                          {"genericSound3", 11},
                                          {"R Footstep", 10},
                                          {"AllowInterrupt", 20},
                                      }, {
                                          {"Records\\creatures\\dragon\\skills\\clawswipe_helper_spawn.dbr", "ClawAttack", 16},
                                      } });
    exportAnim(bindPose, "attack2", { {
                                          {"RightHandHit", 14},
                                          {"SwipeRight", 6},
                                          {"genericSound2", 32},
                                          {"genericSound3", 9},
                                          {"AllowInterrupt", 23},
                                      }, {
                                          {"Records\\creatures\\dragon\\skills\\clawswipe_helper_spawn.dbr", "ClawAttack", 16},
                                      } });
//    exportAnim(bindPose, "attack3");
//    exportAnim(bindPose, "die1");
//    exportAnim(bindPose, "dragon_flyby");
//    exportAnim(bindPose, "land_spawn");

    exportAnim(bindPose, "run", { {
                                      {"L Footstep", 20},
                                      {"R Footstep", 4},
                                      {"genericSound2", 2},
                                      {"genericSound2", 15},
                                  } });

//    exportAnim(bindPose, "soar");
//    exportAnim(bindPose, "special_breath");
//    exportAnim(bindPose, "special_fireball");
//    exportAnim(bindPose, "special_fly_slam");
    //    exportAnim(bindPose, "special_flyby");
    //    exportAnim(bindPose, "special_hover_fire");
    exportAnim(bindPose, "special_tailwhip", { {
                                                   {"RightHandHit", 6},
                                                   {"specialAttackSound2", 14},
                                                   {"specialAttackSound3", 23},
                                               } });
//    exportAnim(bindPose, "special_takeoff");
//    exportAnim(bindPose, "special_wingbuff_end");
//    exportAnim(bindPose, "special_wingbuff_loop");
//    exportAnim(bindPose, "special_wingbuff_start");
//    exportAnim(bindPose, "walk");
}

void exportDummy() {
    Model m = Model::dummy();
    m.exportMDL("dragon.mdl");

    Animation anim = Animation::dummy();
    anim.exportAnm("dragon_idle.anm");
}

void test() {


    // From Skeleton:
    float skRotAngle = 2.98361f;
    QVector3D skRotAxis = {-0.678327f, -0.734582f, -0.0161608f};
    QVector3D skTranslate = {-0.64363f, 0.0666824f, 0.608228f};
    QQuaternion qskRot = QQuaternion::fromAxisAndAngle(skRotAxis, qRadiansToDegrees(skRotAngle));

    // From AnimationSkeleton:
    float anskRotAngle = 2.76351f;
    QVector3D anskRotAxis = {-0.500949f, -0.843208f, -0.195062f};
    QVector3D anskTranslate = {-0.642333f, -0.0427256f, 0.611621f};
    QQuaternion qanskRot = QQuaternion::fromAxisAndAngle(anskRotAxis, qRadiansToDegrees(anskRotAngle));

    // From Animation Keyframe
    float animRotAngle = 0.278387f;
    QVector3D animRotAxis = {0.112537f, 0.139021f, 0.983874f};
    QVector3D animTranslate = {0.0514671f, 0.116855f, 0.0471957f};
    QQuaternion qanimRot = QQuaternion::fromAxisAndAngle(animRotAxis, qRadiansToDegrees(animRotAngle));


    // The 3D point
    QVector3D point = {0, 0, 0};

    // The Control Group - This is how it should end up like.
    QVector3D convertedTrans = qanskRot.inverted() * animTranslate;

    QMatrix4x4 resultOnlyAnim;
    resultOnlyAnim.translate(anskTranslate);
    resultOnlyAnim.rotate(qanskRot);
    resultOnlyAnim.translate(convertedTrans);
    resultOnlyAnim.rotate(qanimRot);

    qDebug() << "GOAL:" << resultOnlyAnim * point;

    // The simulation - This is how it currently ends up.

    QQuaternion qRotationChange = qskRot.inverted() * qanskRot;
    QVector3D translationChange = qskRot.inverted() * (anskTranslate - skTranslate);


    animTranslate = qanskRot * (qskRot.inverted() * animTranslate + translationChange);
    qanimRot = qRotationChange * qanimRot;


    convertedTrans = qanskRot.inverted() * animTranslate;

    QMatrix4x4 resultAll;
    resultAll.translate(skTranslate);
    resultAll.rotate(qskRot);
    resultAll.translate(convertedTrans);
    resultAll.rotate(qanimRot);


    qDebug() << "HAVE:" << resultAll * point;
}

int main(int , char *[])
{
    exportRealStuff();
//    exportTestStuff();
//    test();
//    exportDummy();

    return 0;
}
