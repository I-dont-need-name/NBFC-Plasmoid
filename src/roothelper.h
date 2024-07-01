#include <KAuthActionReply>
#include <KAuthExecuteJob>
#include <KAuthHelperSupport>

#include <QProcess>
#include <QDebug>
#include <QVariantMap>

using namespace KAuth;

class RootHelper : public QObject
{
    Q_OBJECT
public Q_SLOTS:
    ActionReply setgovernor(const QVariantMap& args);
    ActionReply setfanspeed(const QVariantMap& args);
};
