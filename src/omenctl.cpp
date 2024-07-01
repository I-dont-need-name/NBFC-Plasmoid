/*
    SPDX-FileCopyrightText: 2023 illusion <vladmak04@gmail.com>
    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "omenctl.h"

OmenCtl::OmenCtl(QObject *parent, const QVariantList &args) : Plasma::Applet(parent, args)
{
    /*gpuWriteThread._fan = 1;
    initialize();
    */
}

OmenCtl::~OmenCtl()
{
}

QString OmenCtl::nativeText() const
{
    return "String";
}

int OmenCtl::getCpuCool() const
{
    return cpuCool;
}

int OmenCtl::getGpuCool() const
{
    return gpuCool;
}

/*void OmenCtl::initialize()
{
    QProcess nbfc;
    nbfc.setProgram("nbfc");
    nbfc.setArguments({"status", "-f0"});

    nbfc.start();
    nbfc.waitForFinished();
    QString nbfcOutput(nbfc.readAllStandardOutput());

    QStringList l = nbfcOutput.split('\n');
    nbfcOutput = l[4];
    nbfcOutput = nbfcOutput.chopped(-4);
    cpuCool = nbfcOutput.toDouble();

    getAvailableGovernors();
}
*/

QStringList OmenCtl::getAvailableGovernors() const{

    QStringList list;
    cpufreq_available_governors* governors = cpufreq_get_available_governors(0);
    cpufreq_available_governors* governorsPTR = governors;

    while (governors != NULL){
        list.append(governors->governor);
        qDebug() << governors->governor << "\n";
        governors = governors->next;
    }
    cpufreq_put_available_governors(governorsPTR);
    delete governors;

    return list;

}

QString OmenCtl::getGovernor()
{
    std::ifstream governorFile("/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor");
    std::string governor;

    if (governorFile) {
        std::getline(governorFile, governor);
        governorFile.close();
    } else {
        // Handle error if the file cannot be opened or read
        // For example, you can log an error message or throw an exception
    }
    qDebug() << QString::fromStdString(governor);
    return QString::fromStdString(governor);
};

void OmenCtl::setGovernor(QString governor){

    KAuth::Action set_governor_action("org.kde.omenctl.setgovernor");
    set_governor_action.setHelperId("org.kde.omenctl");

    QVariantMap map;
    map["governor"] = governor;
    set_governor_action.setArguments(map);
    ExecuteJob* job = set_governor_action.execute();
    if (!job->exec()) {
        qDebug() << "KAuth returned an error code:" << job->error();
    } else {
        qDebug() << "KAuth succeeded." ;
    }
};


void OmenCtl::setFanSpeed(unsigned short fan, unsigned short speed)
{
    KAuth::Action set_fan_action("org.kde.omenctl.setfanspeed");
    set_fan_action.setHelperId("org.kde.omenctl");

    QVariantMap map;
    map["fan"] = fan;
    map["speed"] = speed;
    set_fan_action.setArguments(map);
    ExecuteJob* job = set_fan_action.execute();
    if (!job->exec()) {
        qDebug() << "KAuth returned an error code:" << job->error() << '\n';
        qDebug() << job->errorString();
    } else {
        qDebug() << "KAuth succeeded." ;
    }
}

void OmenCtl::setCpuCool(short n)
{
    setFanSpeed(0, n);
}

void OmenCtl::setGpuCool(short n)
{
    setFanSpeed(1, n);
}


QString OmenCtl::getNvidiaPower()
{
    std::ifstream powerStateFile("/sys/bus/pci/devices/0000:01:00.0/power_state");
    std::string powerState;

    if (powerStateFile) {
        std::getline(powerStateFile, powerState);
        powerStateFile.close();
    } else {
        // Handle error if the file cannot be opened or read
        // For example, you can log an error message or throw an exception
    }
    qDebug() << QString::fromStdString(powerState);
    return QString::fromStdString(powerState);
}


K_PLUGIN_CLASS_WITH_JSON(OmenCtl, "package/metadata.json")

#include "omenctl.moc"
