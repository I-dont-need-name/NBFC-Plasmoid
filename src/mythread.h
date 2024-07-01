#ifndef MYTHREAD_H
#define MYTHREAD_H
#include <QThread>
#include <QProcess>

class MyThread : public QThread
{
public:

    short _value = 40;
    bool _fan = 0;

    MyThread(short value, bool fan){
        _value = value;
        _fan = fan;
    }
    MyThread(){
        _value = 40;
        _fan = 0;
    }

private:
    void run() override
    {
        QProcess proc;
        proc.setProgram("nbfc");
        proc.setArguments({"set", "-s", QString::number(_value), "-f", QString::number(_fan)});
        if (_value <= 30)
        {
            proc.setArguments({"set", "-a", "-f" + QString::number(_fan)});
        }
        proc.start();
        proc.waitForFinished(-1);
    }

};

#endif // MYTHREAD_H
