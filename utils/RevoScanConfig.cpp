#include "RevoScanConfig.h"
#include "JsonConfig.h"

// #include "log/RvDebug.h"

#include <QCoreApplication>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include <QDebug>
#include <QApplication>
#include <QJsonParseError>

#include <string>
#include <fstream>
#include <iostream>

#define KEY_COMMON  "common"
#define KEY_PROJECTLIST  "projectList"
#define KEY_LANGUAGE    "language"
#define KEY_HOTKEYS    "hotkeys"
#define KEY_COURSEURL   "courseUrl"
#define KEY_EMAIL       "email"
#define KEY_SAVEPATH     "savePath"
#define KEY_OPERATORRECORDNUM     "operatorRecordNum"
#define KEY_GUIDE     "guide"
#define KEY_GUIDELIST   "guideList"
//#define KEY_ENTER_STARTPAGE     "enterStartpageFirst"
#define KEY_SCANNER     "scanner"
#define KEY_WIFIMODE     "wifiMode"
#define KEY_WIFICHANNEL     "wifiChannel"
#define KEY_LOG     "log"
#define KEY_FILE     "file"
#define KEY_LEVEL     "level"
#define KEY_PARTS     "parts"
#define KEY_TWOAXISTURNTABLE    "twoAxisTurntable"
#define KEY_ZAXISSPEED  "zAxisSpeed"
#define KEY_XAXIS   "xAxis"
#define KEY_ANGLE   "angle"
#define KEY_EXPERIENCE_DIALOG "experienceDialog"
#define KEY_SURVEY_BTN "surveyBtn"
#define KEY_TEMP_UNIT_TYPE    "temperatureUnitType"
#define KEY_SHOW_REVOPAL_TIPS   "showRevoPalTips"

#define KEY_SCAN    "scan"
#define KEY_AUDIOWARN   "audioWarn"
#define KEY_SCANGUIDE   "scanGuide"
#define KEY_LASTSCENEPARAM  "lastSceneParam"
#define KEY_SCANSCENELIST   "scanSceneList"
#define KEY_NAME    "name"
#define KEY_ACCURACY    "accuracy"
#define KEY_MODE    "mode"
#define KEY_OBJECT  "object"
#define KEY_TEXTURE "texture"

#define KEY_EDIT    "edit"
#define KEY_MODELSETTING    "modelSetting"
#define KEY_VIEWSETTING     "viewSetting"
#define KEY_MODELBACKCOLOR  "modelBackColor"

#define KEY_POINTCLOUD  "pointCloud"
#define KEY_FUSE    "fuse"
#define KEY_SPACING "spacing"

#define KEY_SIMPLIFY "simplify"
#define KEY_WEIGHT "weight"

#define KEY_MAX "max"
#define KEY_MIN "min"
#define KEY_DEFAULT "default"
#define KEY_CURRENT "current"

#define KEY_OVERLAPDETECTION    "overlapDetection"
#define KEY_SMOOTH  "smooth"
#define KEY_ISOLATEDOBJECTS "isolatedObjects"
#define KEY_RADIUS  "radius"
#define KEY_ITERATIONS  "iterations"
#define KEY_PROPORTIONOFISOLATEDOBJECT  "proportionOfIsolatedObject"
#define KEY_MESH    "mesh"
#define KEY_MESHING    "meshing"
#define KEY_LEVEL "level"
#define KEY_PROPORTION "proportion"

#define KEY_PROPORTIONOFTHEHOLEAREA "proportionOfTheHoleArea"
#define KEY_SMOOTHNESSLEVEL "smoothnessLevel"
#define KEY_SMOOTHNESSITERATIONS "smoothnessIterations"

#define KEY_ISOLATEDOBJECTS "isolatedObjects"
#define KEY_PROPORTIONOFISOLATEDOBJECT  "proportionOfIsolatedObject"
#define KEY_SHARP   "sharp"
#define KEY_STRENGTH    "strength"

#define KEY_FULL_DUMP   "fullDump"

#define KEY_SCAN_SCENE  "scanScene"

#define KEY_FIRMWARE    "firmware"
#define KEY_FIRMWARE_LASTEST_CONNECTED    "lastestConnectedfirmware"
#define KEY_FIRMWARE_PACKAGE_NAME   "firmwarePackageName"
#define KEY_FIRMWARE_VERSION        "firmwareVersion"
#define KEY_FIRMWARE_SKIP_VERSION   "firmwareSkipVersion"

#define SCREEN_HD_WIDTH 1920     //高清屏幕宽
#define SCREEN_HD_HEIGHT 1080    //高清屏幕高
#define SCREEN_2K_WIDTH 2560     //2K屏幕宽
#define SCREEN_2K_HEIGHT 1440    //2K屏幕高
#define SCREEN_4K_WIDTH 3840     //4K屏幕宽
#define SCREEN_4K_HEIGHT 2160    //4K屏幕高

#define MAINWINDOW_WIDTH  1600   //主窗口宽
#define MAINWINDOW_HEIGHT 900    //主窗口高

JsonConfig& getConfig()
{
    static JsonConfig config(QStandardPaths::standardLocations(QStandardPaths::AppConfigLocation).first()+"/config/revoscan.cfg");
    return config;
}

QStringList toList(const QJsonArray &array)
{
    QStringList list;
    for (const auto &v : array) {
        // list.append(v.toString());
        list.insert(0, QDir::toNativeSeparators(v.toString()));
    }
    list.removeDuplicates();
    return list;
}

RevoScanConfig::RevoScanConfig(QObject *parent) : QObject(parent)
{

}


QStringList RevoScanConfig::getProjectList()
{
    JsonConfig& config = getConfig();
    QJsonValue value = config.getValue(KEY_COMMON, KEY_PROJECTLIST);
    if(value.isArray()){
        return toList(value.toArray());
    }
    return QStringList();
}

void RevoScanConfig::addToProjectList(QString value)
{
    value = QDir::toNativeSeparators(value);
    JsonConfig& config = getConfig();
    QJsonArray oldValue = config.getValue(KEY_COMMON, KEY_PROJECTLIST).toArray();
    for (int i = 0; i < oldValue.size(); i++)
    {
        if (oldValue[i] == value)
        {
            oldValue.removeAt(i);
            break;
            /* code */
        }
        
    }
    oldValue.append(value);
    config.setValue(KEY_COMMON, KEY_PROJECTLIST, oldValue);   
    // if(!oldValue.contains(value)){
    //     oldValue.append(value);
    //     config.setValue(KEY_COMMON, KEY_PROJECTLIST, oldValue);
    // }
}


void RevoScanConfig::removeFromProjectList(QString value)
{
    value = QDir::toNativeSeparators(value);
    JsonConfig& config = getConfig();
    QJsonArray oldValue = config.getValue(KEY_COMMON, KEY_PROJECTLIST).toArray();
    for(int i=0; i<oldValue.size(); i++){
        if(oldValue[i].toString() == value){
            oldValue.removeAt(i);
            config.setValue(KEY_COMMON, KEY_PROJECTLIST, oldValue);
            break;
        }
    }
}


void RevoScanConfig::clearProjectList()
{
    JsonConfig& config = getConfig();
    config.setValue(KEY_COMMON, KEY_PROJECTLIST, QJsonValue());

}

QString RevoScanConfig::getLanguage(const QString& defaultValue)
{
    JsonConfig& config = getConfig();
    QJsonValue oldValue = config.getValue(KEY_COMMON, KEY_LANGUAGE);
    return oldValue.toString(defaultValue);
}

void RevoScanConfig::setLangeuage(const QString& lang)
{
    JsonConfig& config = getConfig();
    config.setValue(KEY_COMMON, KEY_LANGUAGE, lang);
}

QMap<QString, QString> RevoScanConfig::getShortcutList()
{
    QMap<QString, QString> res;
    JsonConfig& config = getConfig();
    QJsonObject value = config.getValue(KEY_COMMON, KEY_HOTKEYS).toObject();
    QJsonObject::const_iterator it = value.constBegin();
    QJsonObject::const_iterator end = value.constEnd();
    while(it != end)
    {
        res.insert(it.key(), it.value().toString());
        it++;
    }
    return res;
}

QString RevoScanConfig::getShortcut(const QString& name)
{
    JsonConfig& config = getConfig();
    QJsonValue oldValue = config.getValue(QString("%1/%2").arg(KEY_COMMON,KEY_HOTKEYS), name);
    return oldValue.toString();

}
void RevoScanConfig::setShortcut(const QString& name, const QString& shortcut)
{
    JsonConfig& config = getConfig();
    config.setValue(QString("%1/%2").arg(KEY_COMMON,KEY_HOTKEYS), name, shortcut);
}

QString RevoScanConfig::getCourseUrl(const QString& defaultValue)
{
    JsonConfig& config = getConfig();
    QJsonValue oldValue = config.getValue(KEY_COMMON, KEY_COURSEURL);
    return oldValue.toString(defaultValue);
}
void RevoScanConfig::setCourseUrl(const QString& url)
{
    JsonConfig& config = getConfig();
    config.setValue(KEY_COMMON, KEY_COURSEURL, url);
}


QString RevoScanConfig::getEmail(const QString& defaultValue)
{
    JsonConfig& config = getConfig();
    QJsonValue oldValue = config.getValue(KEY_COMMON, KEY_EMAIL);
    return oldValue.toString(defaultValue);
}
void RevoScanConfig::setEmail(const QString& value)
{
    JsonConfig& config = getConfig();
    config.setValue(KEY_COMMON, KEY_EMAIL, value);
}

QString RevoScanConfig::getSavePath(const QString& defaultValue)
{
    JsonConfig& config = getConfig();
    QJsonValue oldValue = config.getValue(KEY_COMMON, KEY_SAVEPATH);
    return oldValue.toString(defaultValue);
}
void RevoScanConfig::setSavePath(const QString& value)
{
    JsonConfig& config = getConfig();
    config.setValue(KEY_COMMON, KEY_SAVEPATH, value);
}

int RevoScanConfig::getOperatorRecordNum(int defaultValue)
{
    JsonConfig& config = getConfig();
    QJsonValue oldValue = config.getValue(KEY_COMMON, KEY_OPERATORRECORDNUM);
    return oldValue.toInt(defaultValue);
}

void RevoScanConfig::setOperatorRecordNum(int value)
{
    JsonConfig& config = getConfig();
    config.setValue(KEY_COMMON, KEY_OPERATORRECORDNUM, value);
}

bool RevoScanConfig::getGuide(bool defaultValue)
{
    JsonConfig& config = getConfig();
    QJsonValue oldValue = config.getValue(KEY_COMMON, KEY_GUIDE);
    return oldValue.toBool(defaultValue);
}

void RevoScanConfig::setGuide(bool value)
{
    JsonConfig& config = getConfig();
    config.setValue(KEY_COMMON, KEY_GUIDE, value);
}

void RevoScanConfig::addToGuideList(const QString &value)
{
    JsonConfig& config = getConfig();
    QJsonArray oldValue = config.getValue(KEY_COMMON, KEY_GUIDELIST).toArray();
    if(!oldValue.contains(value)){
        oldValue.append(value);
        config.setValue(KEY_COMMON, KEY_GUIDELIST, oldValue);
    }
}
    
bool RevoScanConfig::getGuideItem(const QString &value)
{    
    JsonConfig& config = getConfig();
    QJsonArray oldValue = config.getValue(KEY_COMMON, KEY_GUIDELIST).toArray();
    if(oldValue.contains(value)){
        return true;
    }

    return false;
}

void RevoScanConfig::clearGuideList()
{
    JsonConfig& config = getConfig();
    config.setValue(KEY_COMMON, KEY_GUIDELIST, QJsonValue());
}

int RevoScanConfig::getWifiMode()
{
    JsonConfig& config = getConfig();
    QJsonValue oldValue = config.getValue(KEY_SCANNER, KEY_WIFIMODE);
    return oldValue.toInt();
}

void RevoScanConfig::setWifiMode(int value)
{
    JsonConfig& config = getConfig();
    config.setValue(KEY_SCANNER, KEY_WIFIMODE, value);
}


int RevoScanConfig::getWifiChannel(int defaultValue)
{
    JsonConfig& config = getConfig();
    QJsonValue oldValue = config.getValue(KEY_SCANNER, KEY_WIFICHANNEL);
    return oldValue.toInt(defaultValue);
}

void RevoScanConfig::setWifiChannel(int value)
{
    JsonConfig& config = getConfig();
    config.setValue(KEY_SCANNER, KEY_WIFICHANNEL, value);
}


QString RevoScanConfig::getLogFileName(const QString& defaultValue)
{
    JsonConfig& config = getConfig();
    QJsonValue oldValue = config.getValue(KEY_LOG, KEY_FILE);
    return oldValue.toString(defaultValue);
}
void RevoScanConfig::setLogFileName(const QString& value)
{
    JsonConfig& config = getConfig();
    config.setValue(KEY_LOG, KEY_FILE, value);
}


void RevoScanConfig::setLogLevel(int value)
{
    JsonConfig& config = getConfig();
    config.setValue(KEY_LOG, KEY_LEVEL, value);
}


int RevoScanConfig::getLogLevel(int defaultValue)
{
    JsonConfig& config = getConfig();
    QJsonValue oldValue = config.getValue(KEY_LOG, KEY_LEVEL);
    return oldValue.toInt(defaultValue);
}



int RevoScanConfig::getZAxisSpeed(int defaultValue)
{
    JsonConfig& config = getConfig();
    QJsonValue oldValue = config.getValue(QString("%1/%2").arg(KEY_PARTS, KEY_TWOAXISTURNTABLE), KEY_ZAXISSPEED);
    return oldValue.toInt(defaultValue);
}

void RevoScanConfig::setZAxisSpeed(int value)
{
    JsonConfig& config = getConfig();
    config.setValue(QString("%1/%2").arg(KEY_PARTS, KEY_TWOAXISTURNTABLE), KEY_ZAXISSPEED, value);
}

int RevoScanConfig::getAngle(int index, int defaultValue)
{
    JsonConfig& config = getConfig();
    QJsonValue oldValue = config.getValue(QString("%1/%2/%3").arg(KEY_PARTS, KEY_TWOAXISTURNTABLE, KEY_XAXIS), QString::number(index));
    return oldValue.toInt(defaultValue);
}

void RevoScanConfig::setAngle(int index, int value)
{
    JsonConfig& config = getConfig();
    config.setValue(QString("%1/%2/%3").arg(KEY_PARTS, KEY_TWOAXISTURNTABLE, KEY_XAXIS, KEY_ANGLE),  QString::number(index), value);

}


bool RevoScanConfig::getAudioWarn(bool defaultValue)
{
    JsonConfig& config = getConfig();
    QJsonValue oldValue = config.getValue(KEY_SCAN, KEY_AUDIOWARN);
    return oldValue.toBool(defaultValue);
}

void RevoScanConfig::setAudioWarn(bool value)
{
    JsonConfig& config = getConfig();
    config.setValue(KEY_SCAN, KEY_AUDIOWARN, value);
}

bool RevoScanConfig::getScanGuide(bool defaultValue)
{
    JsonConfig& config = getConfig();
    QJsonValue oldValue = config.getValue(KEY_SCAN, KEY_SCANGUIDE);
    return oldValue.toBool(defaultValue);
}

void RevoScanConfig::setScanGuide(bool value)
{
    JsonConfig& config = getConfig();
    config.setValue(KEY_SCAN, KEY_SCANGUIDE, value);
}

//退出时的扫描场景
tagScanScene RevoScanConfig::getLastSceneParam()
{
    tagScanScene scene = {"", 0, 0, 0, false};
    JsonConfig& config = getConfig();
    QJsonValue value = config.getValue(KEY_SCAN, KEY_LASTSCENEPARAM);
    if(value.isObject()){
        QJsonObject obj = value.toObject();
        scene.name = obj[KEY_NAME].toString();
        scene.accuracy = obj[KEY_ACCURACY].toInt();
        scene.mode = obj[KEY_MODE].toInt();
        scene.object = obj[KEY_OBJECT].toInt();
        scene.texture = obj[KEY_TEXTURE].toInt();
    }
    return scene;
}

void RevoScanConfig::setLastSceneParam(tagScanScene value)
{
    JsonConfig& config = getConfig();
    QJsonObject obj;
    obj[KEY_NAME] = value.name;
    obj[KEY_ACCURACY] = value.accuracy;
    obj[KEY_MODE] = value.mode;
    obj[KEY_OBJECT] = value.object;
    obj[KEY_TEXTURE] = value.texture;

    config.setValue(KEY_SCAN, KEY_LASTSCENEPARAM, obj);
}

QList<tagScanScene> RevoScanConfig::geSceneParamList()
{
    QList<tagScanScene> list;
    JsonConfig& config = getConfig();
    QJsonValue value = config.getValue(KEY_SCAN, KEY_SCANSCENELIST);
    if(value.isArray()){
        QJsonArray array = value.toArray();
        foreach(QJsonValue v, array){
            QJsonObject obj = v.toObject();
            tagScanScene scene;
            scene.name = obj[KEY_NAME].toString();
            scene.accuracy = obj[KEY_ACCURACY].toInt();
            scene.mode = obj[KEY_MODE].toInt();
            scene.object = obj[KEY_OBJECT].toInt();
            scene.texture = obj[KEY_TEXTURE].toInt();
            list.append(scene);
        }
    }
    return list;
}

void RevoScanConfig::setSceneParamList(QList<tagScanScene> scenes)
{
    QJsonArray array;
    for(int i=0; i<scenes.size(); i++){
        tagScanScene value = scenes[i];
        QJsonObject obj;
        obj[KEY_NAME] = value.name;
        obj[KEY_ACCURACY] = value.accuracy;
        obj[KEY_MODE] = value.mode;
        obj[KEY_OBJECT] = value.object;
        obj[KEY_TEXTURE] = value.texture;
        array.append(obj);
    }
    JsonConfig& config = getConfig();
    config.setValue(KEY_SCAN, KEY_SCANSCENELIST, array);
}


int RevoScanConfig::getEditModelSetting(int defaultValue)
{
    JsonConfig& config = getConfig();
    QJsonValue oldValue = config.getValue(QString("%1/%2").arg(KEY_EDIT, KEY_COMMON), KEY_MODELSETTING);
    return oldValue.toInt(defaultValue);
}

void RevoScanConfig::setEditModelSetting(int value)
{
    JsonConfig& config = getConfig();
    config.setValue(QString("%1/%2").arg(KEY_EDIT, KEY_COMMON), KEY_MODELSETTING, value);
}


int RevoScanConfig::getEditViewSetting(int defaultValue)
{
    JsonConfig& config = getConfig();
    QJsonValue oldValue = config.getValue(QString("%1/%2").arg(KEY_EDIT, KEY_COMMON), KEY_VIEWSETTING);
    return oldValue.toInt(defaultValue);
}

void RevoScanConfig::setEditViewSetting(int value)
{
    JsonConfig& config = getConfig();
    config.setValue(QString("%1/%2").arg(KEY_EDIT, KEY_COMMON), KEY_VIEWSETTING, value);
}


int RevoScanConfig::getEditModelBackColor(int defaultValue)
{
    JsonConfig& config = getConfig();
    QJsonValue oldValue = config.getValue(QString("%1/%2").arg(KEY_EDIT, KEY_COMMON), KEY_MODELBACKCOLOR);
    return oldValue.toInt(defaultValue);
}

void RevoScanConfig::setEditModelBackColor(int value)
{
    JsonConfig& config = getConfig();
    config.setValue(QString("%1/%2").arg(KEY_EDIT, KEY_COMMON), KEY_MODELBACKCOLOR, value);
}


double RevoScanConfig::getEditPointCloudFuseSpacing(const QString& key, double defaultValue)
{
    JsonConfig& config = getConfig();
    QJsonValue oldValue = config.getValue(QString("%1/%2/%3").arg(KEY_EDIT, KEY_POINTCLOUD, KEY_FUSE), KEY_SPACING);
    return oldValue.toObject()[key].toDouble(defaultValue);
}

void RevoScanConfig::setEditPointCloudFuseSpacing(const QString& key,double value)
{
    JsonConfig& config = getConfig();
    QJsonValue oldValue = config.getValue(QString("%1/%2/%3").arg(KEY_EDIT, KEY_POINTCLOUD, KEY_FUSE), KEY_SPACING);
    QJsonObject obj = oldValue.toObject();
    obj[key] = value;
    config.setValue(QString("%1/%2/%3").arg(KEY_EDIT, KEY_POINTCLOUD, KEY_FUSE), KEY_SPACING, obj);
}



double RevoScanConfig::getEditPointCloudOverlapDetectionSpacing(const QString& key, double defaultValue)
{
    JsonConfig& config = getConfig();
    QJsonValue oldValue = config.getValue(QString("%1/%2/%3").arg(KEY_EDIT, KEY_POINTCLOUD, KEY_OVERLAPDETECTION), KEY_SPACING);
    return oldValue.toObject()[key].toDouble(defaultValue);
}

void RevoScanConfig::setEditPointCloudOverlapDetectionSpacing(const QString& key,double value)
{
    JsonConfig& config = getConfig();
    QJsonValue oldValue = config.getValue(QString("%1/%2/%3").arg(KEY_EDIT, KEY_POINTCLOUD, KEY_OVERLAPDETECTION), KEY_SPACING);
    QJsonObject obj = oldValue.toObject();
    obj[key] = value;
    config.setValue(QString("%1/%2/%3").arg(KEY_EDIT, KEY_POINTCLOUD, KEY_OVERLAPDETECTION), KEY_SPACING, obj);
}

double RevoScanConfig::getEditPointCloudWeightOfSimplify(const QString& key, double defaultValue)
{
    JsonConfig& config = getConfig();
    QJsonValue oldValue = config.getValue(QString("%1/%2/%3").arg(KEY_EDIT, KEY_POINTCLOUD, KEY_SIMPLIFY), KEY_WEIGHT);
    return oldValue.toObject()[key].toDouble(defaultValue);
}

void RevoScanConfig::setEditPointCloudWeightOfSimplify(const QString& key, double value)
{
    JsonConfig& config = getConfig();
    QJsonValue oldValue = config.getValue(QString("%1/%2/%3").arg(KEY_EDIT, KEY_POINTCLOUD, KEY_SIMPLIFY), KEY_WEIGHT);
    QJsonObject obj = oldValue.toObject();
    obj[key] = value;
    config.setValue(QString("%1/%2/%3").arg(KEY_EDIT, KEY_POINTCLOUD, KEY_SIMPLIFY), KEY_WEIGHT, obj);
}

int RevoScanConfig::getEditPointCloudSmoothRadius(const QString& key, int defaultValue)
{
    JsonConfig& config = getConfig();
    QJsonValue oldValue = config.getValue(QString("%1/%2/%3").arg(KEY_EDIT, KEY_POINTCLOUD, KEY_SMOOTH), KEY_RADIUS);
    return oldValue.toObject()[key].toInt(defaultValue);
}

void RevoScanConfig::setEditPointCloudSmoothRadius(const QString& key,int value)
{
    JsonConfig& config = getConfig();
    QJsonValue oldValue = config.getValue(QString("%1/%2/%3").arg(KEY_EDIT, KEY_POINTCLOUD, KEY_SMOOTH), KEY_RADIUS);
    QJsonObject obj = oldValue.toObject();
    obj[key] = value;
    config.setValue(QString("%1/%2/%3").arg(KEY_EDIT, KEY_POINTCLOUD, KEY_SMOOTH), KEY_RADIUS, obj);
}

int RevoScanConfig::getEditPointCloudSmoothIterations(const QString& key, int defaultValue)
{
    JsonConfig& config = getConfig();
    QJsonValue oldValue = config.getValue(QString("%1/%2/%3").arg(KEY_EDIT, KEY_POINTCLOUD, KEY_SMOOTH), KEY_ITERATIONS);
    return oldValue.toObject()[key].toInt(defaultValue);
}

void RevoScanConfig::setEditPointCloudSmoothIterations(const QString& key, int value)
{
    JsonConfig& config = getConfig();
    QJsonValue oldValue = config.getValue(QString("%1/%2/%3").arg(KEY_EDIT, KEY_POINTCLOUD, KEY_SMOOTH), KEY_ITERATIONS);
    QJsonObject obj = oldValue.toObject();
    obj[key] = value;
    config.setValue(QString("%1/%2/%3").arg(KEY_EDIT, KEY_POINTCLOUD, KEY_SMOOTH), KEY_ITERATIONS, obj);
}


int RevoScanConfig::getEditPointCloudIsolatedObjectsRadius(const QString& key, int defaultValue)
{
    JsonConfig& config = getConfig();
    QJsonValue oldValue = config.getValue(QString("%1/%2/%3").arg(KEY_EDIT, KEY_POINTCLOUD, KEY_ISOLATEDOBJECTS), KEY_RADIUS);
    return oldValue.toObject()[key].toInt(defaultValue);
}

void RevoScanConfig::setEditPointCloudIsolatedObjectsRadius(const QString& key,int value)
{
    JsonConfig& config = getConfig();
    QJsonValue oldValue = config.getValue(QString("%1/%2/%3").arg(KEY_EDIT, KEY_POINTCLOUD, KEY_ISOLATEDOBJECTS), KEY_RADIUS);
    QJsonObject obj = oldValue.toObject();
    obj[key] = value;
    config.setValue(QString("%1/%2/%3").arg(KEY_EDIT, KEY_POINTCLOUD, KEY_ISOLATEDOBJECTS), KEY_RADIUS, obj);
}


int RevoScanConfig::getEditPointCloudIsolatedObjectsAngle(const QString& key, int defaultValue)
{
    JsonConfig& config = getConfig();
    QJsonValue oldValue = config.getValue(QString("%1/%2/%3").arg(KEY_EDIT, KEY_POINTCLOUD, KEY_ISOLATEDOBJECTS), KEY_ANGLE);
    return oldValue.toObject()[key].toInt(defaultValue);
}

void RevoScanConfig::setEditPointCloudIsolatedObjectsAngle(const QString& key,int value)
{
    JsonConfig& config = getConfig();
    QJsonValue oldValue = config.getValue(QString("%1/%2/%3").arg(KEY_EDIT, KEY_POINTCLOUD, KEY_ISOLATEDOBJECTS), KEY_ANGLE);
    QJsonObject obj = oldValue.toObject();
    obj[key] = value;
    config.setValue(QString("%1/%2/%3").arg(KEY_EDIT, KEY_POINTCLOUD, KEY_ISOLATEDOBJECTS), KEY_ANGLE, obj);
}


double RevoScanConfig::getEditPointCloudProportionOfIsolatedObject(const QString& key, double defaultValue)
{
    JsonConfig& config = getConfig();
    QJsonValue oldValue = config.getValue(QString("%1/%2/%3").arg(KEY_EDIT, KEY_POINTCLOUD, KEY_ISOLATEDOBJECTS), KEY_PROPORTIONOFISOLATEDOBJECT);
    return oldValue.toObject()[key].toDouble(defaultValue);
}

void RevoScanConfig::setEditPointCloudProportionOfIsolatedObject(const QString& key, double value)
{
    JsonConfig& config = getConfig();
    QJsonValue oldValue = config.getValue(QString("%1/%2/%3").arg(KEY_EDIT, KEY_POINTCLOUD, KEY_ISOLATEDOBJECTS), KEY_PROPORTIONOFISOLATEDOBJECT);
    QJsonObject obj = oldValue.toObject();
    obj[key] = value;
    config.setValue(QString("%1/%2/%3").arg(KEY_EDIT, KEY_POINTCLOUD, KEY_ISOLATEDOBJECTS), KEY_PROPORTIONOFISOLATEDOBJECT, obj);
}


double RevoScanConfig::getEditMeshingLevel(const QString& key, double defaultValue)
{
    JsonConfig& config = getConfig();
    QJsonValue oldValue = config.getValue(QString("%1/%2/%3").arg(KEY_EDIT, KEY_MESH, KEY_MESHING), KEY_LEVEL);
    return oldValue.toObject()[key].toDouble(defaultValue);
}

void RevoScanConfig::setEditMeshingLevel(const QString& key,double value)
{
    JsonConfig& config = getConfig();
    QJsonValue oldValue = config.getValue(QString("%1/%2/%3").arg(KEY_EDIT, KEY_MESH, KEY_MESHING), KEY_LEVEL);
    QJsonObject obj = oldValue.toObject();
    obj[key] = value;
    config.setValue(QString("%1/%2/%3").arg(KEY_EDIT, KEY_MESH, KEY_MESHING), KEY_LEVEL, obj);
}

int RevoScanConfig::getEditMeshingProportion(const QString& key, int defaultValue)
{
    JsonConfig& config = getConfig();
    QJsonValue oldValue = config.getValue(QString("%1/%2/%3").arg(KEY_EDIT, KEY_MESH, KEY_MESHING), KEY_PROPORTION);
    return oldValue.toObject()[key].toInt(defaultValue);
}

void RevoScanConfig::setEditMeshingProportion(const QString& key, int value)
{
    JsonConfig& config = getConfig();
    QJsonValue oldValue = config.getValue(QString("%1/%2/%3").arg(KEY_EDIT, KEY_MESH, KEY_MESHING), KEY_PROPORTION);
    QJsonObject obj = oldValue.toObject();
    obj[key] = value;
    config.setValue(QString("%1/%2/%3").arg(KEY_EDIT, KEY_MESH, KEY_MESHING), KEY_PROPORTION, obj);
}

double RevoScanConfig::getEditMeshSmoothnessLevel(const QString& key, double defaultValue)
{
    JsonConfig& config = getConfig();
    QJsonValue oldValue = config.getValue(QString("%1/%2/%3").arg(KEY_EDIT, KEY_MESH, KEY_SMOOTH), KEY_SMOOTHNESSLEVEL);
    return oldValue.toObject()[key].toDouble(defaultValue);
}

void RevoScanConfig::setEditMeshSmoothnessLevel(const QString& key,double value)
{
    JsonConfig& config = getConfig();
    QJsonValue oldValue = config.getValue(QString("%1/%2/%3").arg(KEY_EDIT, KEY_MESH, KEY_SMOOTH), KEY_SMOOTHNESSLEVEL);
    QJsonObject obj = oldValue.toObject();
    obj[key] = value;
    config.setValue(QString("%1/%2/%3").arg(KEY_EDIT, KEY_MESH, KEY_SMOOTH), KEY_SMOOTHNESSLEVEL, obj);
}

int RevoScanConfig::getEditMeshSmoothnessIterations(const QString& key, int defaultValue)
{
    JsonConfig& config = getConfig();
    QJsonValue oldValue = config.getValue(QString("%1/%2/%3").arg(KEY_EDIT, KEY_MESH, KEY_SMOOTH), KEY_SMOOTHNESSITERATIONS);
    return oldValue.toObject()[key].toInt(defaultValue);
}

void RevoScanConfig::setEditMeshSmoothnessIterations(const QString& key, int value)
{
    JsonConfig& config = getConfig();
    QJsonValue oldValue = config.getValue(QString("%1/%2/%3").arg(KEY_EDIT, KEY_MESH, KEY_SMOOTH), KEY_SMOOTHNESSITERATIONS);
    QJsonObject obj = oldValue.toObject();
    obj[key] = value;
    config.setValue(QString("%1/%2/%3").arg(KEY_EDIT, KEY_MESH, KEY_SMOOTH), KEY_SMOOTHNESSITERATIONS, obj);
}


double RevoScanConfig::getEditMeshProportionOfIsolatedObject(const QString& key, double defaultValue)
{
    JsonConfig& config = getConfig();
    QJsonValue oldValue = config.getValue(QString("%1/%2/%3").arg(KEY_EDIT, KEY_MESH, KEY_ISOLATEDOBJECTS), KEY_PROPORTIONOFISOLATEDOBJECT);
    return oldValue.toObject()[key].toDouble(defaultValue);
}

void RevoScanConfig::setEditMeshProportionOfIsolatedObject(const QString& key,double value)
{
    JsonConfig& config = getConfig();
    QJsonValue oldValue = config.getValue(QString("%1/%2/%3").arg(KEY_EDIT, KEY_MESH, KEY_ISOLATEDOBJECTS), KEY_PROPORTIONOFISOLATEDOBJECT);
    QJsonObject obj = oldValue.toObject();
    obj[key] = value;
    config.setValue(QString("%1/%2/%3").arg(KEY_EDIT, KEY_MESH, KEY_ISOLATEDOBJECTS), KEY_PROPORTIONOFISOLATEDOBJECT, obj);
}


double RevoScanConfig::getEditMeshSharpStrenght(const QString& key, double defaultValue)
{
    JsonConfig& config = getConfig();
    QJsonValue oldValue = config.getValue(QString("%1/%2/%3").arg(KEY_EDIT, KEY_MESH, KEY_SHARP), KEY_STRENGTH);
    return oldValue.toObject()[key].toDouble(defaultValue);
}

void RevoScanConfig::setEditMeshSharpStrenght(const QString& key,double value)
{
    JsonConfig& config = getConfig();
    QJsonValue oldValue = config.getValue(QString("%1/%2/%3").arg(KEY_EDIT, KEY_MESH, KEY_SHARP), KEY_STRENGTH);
    QJsonObject obj = oldValue.toObject();
    obj[key] = value;
    config.setValue(QString("%1/%2/%3").arg(KEY_EDIT, KEY_MESH, KEY_SHARP), KEY_STRENGTH, obj);
}


float RevoScanConfig::getScreenRatio(const int wideth, const int height)
{
    float scale = 0;
    // if (SCREEN_HD_WIDTH >= wideth) {
    //     scale = 1.0;
    // }
    // else if (SCREEN_HD_WIDTH < wideth && SCREEN_2K_WIDTH >= wideth){
    //     scale = 1.5;
    // }
    // else if (SCREEN_2K_WIDTH < wideth && SCREEN_4K_WIDTH >= wideth) {
    //     scale = 2.5;
    // }
    // else {
    //     scale = 3.0;  //屏幕分辨率宽度大于4K屏幕宽时，放大3倍数。即（SCREEN_4K_WIDTH < wideth）
    // }

    float scale_w = (float)wideth / MAINWINDOW_WIDTH;
    float scale_h = (float)height / MAINWINDOW_HEIGHT;

    scale = scale_w > scale_h ? scale_h : scale_w;
    
    if (scale < 1.5 )
    {
        scale = 1.0;
    }
    else if (scale < 2.0 && scale >= 1.5)
    {
        scale = 1.5;
    }
    else if (scale < 2.5 && scale >= 2.0)
    {
        scale = 2.0;
    }
    else if (scale < 3.0 && scale >= 2.5)
    {
        scale = 2.5;
    }
    else
    {
        scale = 3.0;     // 最多放大三倍
    }


    return scale;
}

bool RevoScanConfig::getFullDump()
{
    JsonConfig& config = getConfig();
    QJsonValue value = config.getValue(KEY_COMMON, KEY_FULL_DUMP);
    return value.toBool(false);
}

QMap<QString, QVariant> RevoScanConfig::getScanScene()
{
    QMap<QString, QVariant> scanScene;
    JsonConfig& config = getConfig();
    QStringList keys = config.getKeys(KEY_SCAN_SCENE);
    foreach (QString k, keys) {
        QJsonValue value = config.getValue(KEY_SCAN_SCENE, k);
        scanScene.insert(k, value);
    }
    return scanScene;
}

void RevoScanConfig::setScanScene(QMap<QString, QVariant> &scanScene)
{
    JsonConfig& config = getConfig();
    for (auto it=scanScene.begin(); it!=scanScene.end(); ++it) {
        config.setValue(KEY_SCAN_SCENE, it.key(), QJsonValue::fromVariant(it.value()));
    }
}

QString RevoScanConfig::getLastestConnectedFirmwareSn()
{
    JsonConfig& config = getConfig();
    QJsonValue value = config.getValue(QString("%1").arg(KEY_FIRMWARE), KEY_FIRMWARE_LASTEST_CONNECTED);
    return value.toString();
}

void RevoScanConfig::setLastestConnectedFirmwareSn(const QString &snCode)
{
    JsonConfig& config = getConfig();
    config.setValue(QString("%1").arg(KEY_FIRMWARE), KEY_FIRMWARE_LASTEST_CONNECTED, snCode);
}

QString RevoScanConfig::getFirmwarePackageName(const QString &snCode)
{
    JsonConfig& config = getConfig();
    QJsonValue oldValue = config.getValue(QString("%1").arg(KEY_FIRMWARE), KEY_FIRMWARE_PACKAGE_NAME);
    return oldValue.toObject()[snCode].toString();
}

void RevoScanConfig::setFirmwarePackageName(const QString &snCode, const QString &packageName)
{
    JsonConfig& config = getConfig();
    QJsonValue oldValue = config.getValue(QString("%1").arg(KEY_FIRMWARE), KEY_FIRMWARE_PACKAGE_NAME);
    QJsonObject obj = oldValue.toObject();
    obj[snCode] = packageName;
    config.setValue(QString("%1").arg(KEY_FIRMWARE), KEY_FIRMWARE_PACKAGE_NAME, obj);
}

// 固件当前的版本
QString RevoScanConfig::getFirmwareVersion(const QString &snCode)
{
    JsonConfig& config = getConfig();
    QJsonValue oldValue = config.getValue(QString("%1").arg(KEY_FIRMWARE), KEY_FIRMWARE_VERSION);
    return oldValue.toObject()[snCode].toString();
}

void RevoScanConfig::setFirmwareVersion(const QString &snCode, const QString &version)
{
    JsonConfig& config = getConfig();
    QJsonValue oldValue = config.getValue(QString("%1").arg(KEY_FIRMWARE), KEY_FIRMWARE_VERSION);
    QJsonObject obj = oldValue.toObject();
    obj[snCode] = version;
    config.setValue(QString("%1").arg(KEY_FIRMWARE), KEY_FIRMWARE_VERSION, obj);
}

QString RevoScanConfig::getFirmwareSkipVersion(const QString &snCode)
{
    JsonConfig& config = getConfig();
    QJsonValue oldValue = config.getValue(QString("%1").arg(KEY_FIRMWARE), KEY_FIRMWARE_SKIP_VERSION);
    return oldValue.toObject()[snCode].toString();
}

void RevoScanConfig::setFirmwareSkipVersion(const QString &snCode, const QString &skipVersion)
{
    JsonConfig& config = getConfig();
    QJsonValue oldValue = config.getValue(QString("%1").arg(KEY_FIRMWARE), KEY_FIRMWARE_SKIP_VERSION);
    QJsonObject obj = oldValue.toObject();
    obj[snCode] = skipVersion;
    config.setValue(QString("%1").arg(KEY_FIRMWARE), KEY_FIRMWARE_SKIP_VERSION, obj);
}

bool RevoScanConfig::getShowExperienceDialog(bool defaultValue)
{
    JsonConfig& config = getConfig();
    QJsonValue oldValue = config.getValue(KEY_COMMON, KEY_EXPERIENCE_DIALOG);
    return oldValue.toBool(defaultValue);
}

void RevoScanConfig::setShowExperienceDialog(bool isShow)
{
    JsonConfig& config = getConfig();
    config.setValue(KEY_COMMON, KEY_EXPERIENCE_DIALOG, isShow);
}

bool RevoScanConfig::getShowSurveyBtn(bool defaultValue)
{
    JsonConfig& config = getConfig();
    QJsonValue oldValue = config.getValue(KEY_COMMON, KEY_SURVEY_BTN);
    return oldValue.toBool(defaultValue);
}

void RevoScanConfig::setShowSurveyBtn(bool isShow)
{
    JsonConfig& config = getConfig();
    config.setValue(KEY_COMMON, KEY_SURVEY_BTN, isShow);
}

// bool RevoScanConfig::getEnterStartpageFirst(bool enterFirst)
// {
//     JsonConfig& config = getConfig();
//     QJsonValue oldValue = config.getValue(KEY_COMMON, KEY_ENTER_STARTPAGE);
//     return oldValue.toBool(enterFirst);
// }

// void RevoScanConfig::setEnterStartpageFirst(bool enterFirst)
// {
//     JsonConfig& config = getConfig();
//     config.setValue(KEY_COMMON, KEY_ENTER_STARTPAGE, enterFirst);
// }

int RevoScanConfig::getTemperatureUnitType(int type)
{
    JsonConfig& config = getConfig();
    QJsonValue oldValue = config.getValue(KEY_COMMON, KEY_TEMP_UNIT_TYPE);
    return oldValue.toInt(type);
}

void RevoScanConfig::setTemperatureUnitType(int type)
{
    JsonConfig& config = getConfig();
    config.setValue(KEY_COMMON, KEY_TEMP_UNIT_TYPE, type);
}

int RevoScanConfig::getRevoPalDowTipsState(int state)
{
    JsonConfig& config = getConfig();
    QJsonValue oldValue = config.getValue(KEY_COMMON, KEY_SHOW_REVOPAL_TIPS);
    return oldValue.toInt(state);
}

void RevoScanConfig::setRevoPalDowTipsState(int state)
{
    JsonConfig& config = getConfig();
    config.setValue(KEY_COMMON, KEY_SHOW_REVOPAL_TIPS, state);
}
