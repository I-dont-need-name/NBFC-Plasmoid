/*
    SPDX-FileCopyrightText: 2022 illusion <vladmak04@gmail.com>
    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "mythread.h"
#include "roothelper.h"

#include "cpufreq.h"
#include <Plasma/Applet>

#include <iostream>
#include <fstream>

#include <KProcess>
#include <KLocalizedString>
#include <KAuth>


class OmenCtl : public Plasma::Applet
{
    Q_OBJECT
    Q_PROPERTY(QString nativeText READ nativeText CONSTANT)
    Q_PROPERTY(int cpuCool READ getCpuCool WRITE setCpuCool)
    Q_PROPERTY(int gpuCool READ getGpuCool WRITE setGpuCool)
    Q_PROPERTY(QStringList governors READ getAvailableGovernors CONSTANT)
    Q_PROPERTY(QString governor READ getGovernor WRITE setGovernor NOTIFY governorChanged)
    Q_PROPERTY(QString nvidiaPower READ getNvidiaPower CONSTANT)


public:
    OmenCtl( QObject *parent, const QVariantList &args );
    ~OmenCtl();

    QString nativeText() const;

    void setCpuCool(short n);
    int getCpuCool() const;

    void setGpuCool(short n);
    int getGpuCool() const;

    Q_INVOKABLE void setFanSpeed(unsigned short fan, unsigned short speed);

    QString getGovernor();
    QString getNvidiaPower();

    QStringList getAvailableGovernors() const;
    Q_INVOKABLE void setGovernor(QString governor);

    //Q_INVOKABLE void initialize();

    signals:
    void governorChanged(QString newValue);

private:
    QString m_nativeText;
    int cpuCool = 30;
    int gpuCool;
    QStringList* profiles;
    MyThread cpuWriteThread;
    MyThread gpuWriteThread;
};
