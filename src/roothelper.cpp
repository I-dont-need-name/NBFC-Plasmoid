#include "roothelper.h"


ActionReply RootHelper::setgovernor(const QVariantMap& args){

    QString program = "cpupower";
    QStringList arguments;
    arguments << "frequency-set" << "--governor" << args["governor"].toString();

    QProcess process;
    process.start(program, arguments);
    if (!process.waitForStarted() || !process.waitForFinished()) {
        qDebug() << "Error executing command:" << process.errorString();
        return 1;
    }

    qDebug() << "Command executed successfully";
    return 0;
}

ActionReply RootHelper::setfanspeed(const QVariantMap& args)
{
    QProcess process;

    process.setProgram("nbfc");

    QStringList arguments;

    if (args["speed"].toInt() < 35){
        arguments << "set" << "-a" << "-f" << args["fan"].toString() ;
    }
    else {
        arguments << "set" << "-s" << args["speed"].toString() << "-f" << args["fan"].toString() ;
    }

    process.setArguments(arguments);
    process.startDetached();

    process.waitForFinished(-1);

    if (!process.waitForStarted() || !process.waitForFinished()) {
        qDebug() << "Error executing command:" << process.errorString();
        return 1;
    }

    qDebug() << "Command executed successfully";
    return 0;


}


KAUTH_HELPER_MAIN("org.kde.omenctl", RootHelper)
