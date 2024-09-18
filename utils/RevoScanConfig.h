#ifndef REVOSCANCONFIG_H
#define REVOSCANCONFIG_H



#include <QObject>
#include <QList>
#include <QMap>
#include <QFile>
#include <QStandardPaths>
#include <qstandardpaths.h>
#include <QDir>

struct tagScanScene
{
    QString name;
    int accuracy;
    int mode;
    int object;
    bool texture;
};


//key可以是max,min,default,current
#define KEY_MAX "max"
#define KEY_MIN "min"
#define KEY_DEFAULT "default"
#define KEY_CURRENT "current"

class RevoScanConfig : public QObject
{
    Q_OBJECT
public:

    explicit RevoScanConfig(QObject *parent = nullptr);
    //工程列表
    static QStringList getProjectList();
    static void addToProjectList(QString value);
    static void removeFromProjectList(QString value);
    static void clearProjectList();
    //语言
    static QString getLanguage(const QString& defaultValue = "Auto");
    static void setLangeuage(const QString& lang);
    //快捷键
    static QMap<QString, QString> getShortcutList();
    static QString getShortcut(const QString& name);
    static void setShortcut(const QString& name, const QString& shortcut);
    //教程网址
    static QString getCourseUrl(const QString& defaultValue = "");
    static void setCourseUrl(const QString& url);

    //email
    static QString getEmail(const QString& defaultValue = "");
    static void setEmail(const QString& value);

    //savePath
    static QString getSavePath(const QString& defaultValue = QDir::toNativeSeparators(QStandardPaths::standardLocations(QStandardPaths::AppDataLocation).first()+"/Projects"));
    static void setSavePath(const QString& value);

    //操作步数
    static int getOperatorRecordNum(int defaultValue=0);
    static void setOperatorRecordNum(int value);

    //是否开启新手引导
    static bool getGuide(bool defaultValue=false);
    static void setGuide(bool value);

    //新手引导列表    
    static void addToGuideList(const QString &value);
    //获取对应的新手引导项  true-成功，false-失败
    static bool getGuideItem(const QString &value);
    static void clearGuideList();

    //扫描仪wifi模式
    static int getWifiMode();
    static void setWifiMode(int value);

    //扫描仪wifi通道
    static int getWifiChannel(int defaultValue=0);
    static void setWifiChannel(int value);

    //日志文件名称
    static QString getLogFileName(const QString& defaultValue="");
    static void setLogFileName(const QString& value);

    //日志输出级别
    static int getLogLevel(int defaultValue=0);
    static void setLogLevel(int value);

    //双轴转台z轴转一圈耗时
    static int getZAxisSpeed(int defaultValue=0);
    static void setZAxisSpeed(int value);

    //双轴转台每圈的角度
    static int getAngle(int index, int defaultValue=0);
    static void setAngle(int index, int value);

    //扫描
    //是否启用扫描音频警告
    static bool getAudioWarn(bool defaultValue=false);
    static void setAudioWarn(bool value);

    //是否启用扫描新手向导
    static bool getScanGuide(bool defaultValue=false);
    static void setScanGuide(bool value);

    //退出时的扫描场景
    static tagScanScene getLastSceneParam();
    static void setLastSceneParam(tagScanScene value);
    //扫描场景
    static QList<tagScanScene> geSceneParamList();
    static void setSceneParamList(QList<tagScanScene> value);

    //编辑

    //模型显示设置
    static int getEditModelSetting(int defaultValue=0);
    static void setEditModelSetting(int value);
    //三维窗口投影方式
    static int getEditViewSetting(int defaultValue=0);
    static void setEditViewSetting(int value);
    //模型背面颜色
    static int getEditModelBackColor(int defaultValue=0);
    static void setEditModelBackColor(int value);

    //点云整合参数，key可以是max,min,default,current
    static double getEditPointCloudFuseSpacing(const QString& key = KEY_DEFAULT, double defaultValue=1);
    static void setEditPointCloudFuseSpacing(const QString& key,double value);

    //重叠检测参数，key可以是max,min,default,current
    static double getEditPointCloudOverlapDetectionSpacing(const QString& key = KEY_DEFAULT, double defaultValue=0);
    static void setEditPointCloudOverlapDetectionSpacing(const QString& key,double value);

    //平滑参数,半径，key可以是max,min,default,current
    static int getEditPointCloudSmoothRadius(const QString& key = KEY_DEFAULT, int defaultValue=10);
    static void setEditPointCloudSmoothRadius(const QString& key,int value);

    //平滑参数,迭代次数，key可以是max,min,default,current
    static int getEditPointCloudSmoothIterations(const QString& key = KEY_DEFAULT, int defaultValue = 10);
    static void setEditPointCloudSmoothIterations(const QString& key, int value);

    //孤 立项参数，key可以是max,min,default,current
    static int getEditPointCloudIsolatedObjectsRadius(const QString& key = KEY_DEFAULT, int defaultValue=10);
    static void setEditPointCloudIsolatedObjectsRadius(const QString& key,int value);

    //角度参数，key可以是max,min,default,current
    static int getEditPointCloudIsolatedObjectsAngle(const QString& key = KEY_DEFAULT, int defaultValue= 60);
    static void setEditPointCloudIsolatedObjectsAngle(const QString& key,int value);

    //孤立率参数，key可以是max,min,default,current
    static double getEditPointCloudProportionOfIsolatedObject(const QString& key = KEY_DEFAULT, double defaultValue=15);
    static void setEditPointCloudProportionOfIsolatedObject(const QString& key, double value);
    //简化，key可以是max,min,default,current
    static double getEditPointCloudWeightOfSimplify(const QString& key = KEY_DEFAULT, double defaultValue = 0);
    static void setEditPointCloudWeightOfSimplify(const QString& key, double value);

    //网格
    //构网级别
    static double getEditMeshingLevel(const QString& key = KEY_DEFAULT, double defaultValue=5);
    static void setEditMeshingLevel(const QString& key,double value);
    //构网占比率
    static int getEditMeshingProportion(const QString& key = KEY_DEFAULT, int defaultValue = 30);
    static void setEditMeshingProportion(const QString& key, int value);

    //平滑权重
    static double getEditMeshSmoothnessLevel(const QString& key = KEY_DEFAULT, double defaultValue=0.3);
    static void setEditMeshSmoothnessLevel(const QString& key,double value);
    //平滑迭代次数
    static int getEditMeshSmoothnessIterations(const QString& key = KEY_DEFAULT, int defaultValue = 3);
    static void setEditMeshSmoothnessIterations(const QString& key, int value);

    //孤立率
    static double getEditMeshProportionOfIsolatedObject(const QString& key = KEY_DEFAULT, double defaultValue=15);
    static void setEditMeshProportionOfIsolatedObject(const QString& key,double value);

    //锐化
    static double getEditMeshSharpStrenght(const QString& key = KEY_DEFAULT, double defaultValue=2);
    static void setEditMeshSharpStrenght(const QString& key,double value);


    //获取屏幕缩放比率
    static float getScreenRatio(const int wideth, const int height);

    static bool getFullDump();

    // 扫描场景
    static QMap<QString, QVariant> getScanScene();
    static void setScanScene(QMap<QString, QVariant>& scanScene);

    // 最近连接的固件版本号
    static QString getLastestConnectedFirmwareSn();
    static void setLastestConnectedFirmwareSn(const QString &snCode);

    // 固件升级资源包的包名
    static QString getFirmwarePackageName(const QString &snCode);
    static void setFirmwarePackageName(const QString &snCode, const QString &packageName);

    // 固件当前的版本
    static QString getFirmwareVersion(const QString &snCode);
    static void setFirmwareVersion(const QString &snCode, const QString &version);

    // 固件升级本次跳过的版本
    static QString getFirmwareSkipVersion(const QString &snCode);
    static void setFirmwareSkipVersion(const QString &snCode, const QString &skipVersion);

    // 满意度调研入口弹窗标志位
    static bool getShowExperienceDialog(bool defaultValue = true);
    static void setShowExperienceDialog(bool isShow);

    // 问卷调查入口弹窗标志位
    static bool getShowSurveyBtn(bool defaultValue = true);
    static void setShowSurveyBtn(bool isShow);

    // 是否首次进入启动页 
    // static bool getEnterStartpageFirst(bool enterFirst = true);
    // static void setEnterStartpageFirst(bool enterFirst);

    // 设备温度单位的类型，0表示用户未设置
    static int getTemperatureUnitType(int type = 0);
    static void setTemperatureUnitType(int type);

    // RevoPal下载提示显隐， 0：隐藏  1：显示
    static int getRevoPalDowTipsState(int state = 1);
    static void setRevoPalDowTipsState(int state);

signals:

};

#endif // REVOSCANCONFIG_H
