#include "global.h"
#include "CallInitiatorFactory.h"

#if LINUX_DESKTOP || defined (Q_WS_WIN32)
#include "DesktopSkypeCallInitiator.h"
#endif

#if TELEPATHY_CAPABLE
#include "TpCalloutInitiator.h"
#endif

CallInitiatorFactory::CallInitiatorFactory (QObject *parent)
: QObject(parent)
, mutex (QMutex::Recursive)
#if TELEPATHY_CAPABLE
, actMgr (Tp::AccountManager::create ())
, bAccountsReady (false)
#endif
{
    init ();
}//CallInitiatorFactory::CallInitiatorFactory

const CalloutInitiatorList &
CallInitiatorFactory::getInitiators ()
{
    return (listInitiators);
}//CallInitiatorFactory::getInitiators

void
CallInitiatorFactory::init ()
{
#if LINUX_DESKTOP || defined (Q_WS_WIN32)
    CalloutInitiator *initiator = new DesktopSkypeCallInitiator (this);
    listInitiators += initiator;

    QObject::connect (
        initiator, SIGNAL (status(const QString &, int)),
        this     , SIGNAL (status(const QString &, int)));
#endif

#if TELEPATHY_CAPABLE
    QObject::connect (
         actMgr->becomeReady (), SIGNAL (finished(Tp::PendingOperation*)),
         this, SLOT (onAccountManagerReady (Tp::PendingOperation *)));
#endif
}//CallInitiatorFactory::init

#if TELEPATHY_CAPABLE

void
CallInitiatorFactory::onAccountManagerReady (Tp::PendingOperation *op)
{
    if (op->isError ()) {
        qWarning ("Account manager could not become ready");
        op->deleteLater ();
        return;
    }

    allAccounts = actMgr->allAccounts ();
    QMutexLocker locker (&mutex);
    nCounter = 1;
    foreach (Tp::AccountPtr acc, allAccounts) {
        nCounter++;
        QObject::connect (
            acc->becomeReady (), SIGNAL (finished(Tp::PendingOperation*)),
            this, SLOT (onAccountReady(Tp::PendingOperation *)));
    }
    nCounter--;
    if (0 == nCounter) {
        onAllAccountsReady ();
    }

    op->deleteLater ();
}//CallInitiatorFactory::onAccountManagerReady

void
CallInitiatorFactory::onAccountReady (Tp::PendingOperation *op)
{
    if (op->isError ()) {
        qWarning ("Account could not become ready");
        op->deleteLater ();
        return;
    }

    QMutexLocker locker (&mutex);
    nCounter--;
    if (0 == nCounter) {
        onAllAccountsReady ();
    }

    op->deleteLater ();
}//CallInitiatorFactory::onAccountReady

void
CallInitiatorFactory::onAllAccountsReady ()
{
    bAccountsReady = true;
    qDebug () << QString("%1 accounts ready").arg (allAccounts.size ());

    QString msg;
    foreach (Tp::AccountPtr act, allAccounts) {
        msg = QString ("Account cmName = %1\n").arg (act->cmName ());
        if ((act->cmName () != "sofiasip") &&
            (act->cmName () != "spirit") &&
            (act->cmName () != "ring"))
        {
            // Who cares about this one?
            msg += "\tIGNORED!!";
            qDebug () << msg;
            continue;
        }

        CalloutInitiator *initiator = new TpCalloutInitiator (act, this);
        listInitiators += initiator;

        QObject::connect (
            initiator, SIGNAL (status(const QString &, int)),
            this     , SIGNAL (status(const QString &, int)));

        msg += "\tADDED!";
        qDebug () << msg;
    }
}//CallInitiatorFactory::onAllAccountsReady

#endif// TELEPATHY_CAPABLE
