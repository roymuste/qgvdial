#ifndef GVI_XMLJSONHANDLER_H
#define GVI_XMLJSONHANDLER_H

#include "global.h"
#include "GVI_SMS_Handler.h"

// For some reason the symbian MOC doesn't like it if I don't include QObject
// even though it is present in QtCore which is included in global.h
#include <QObject>

class GVI_XMLJsonHandler : public QObject, public QXmlDefaultHandler
{
    Q_OBJECT

public:
    explicit GVI_XMLJsonHandler(QObject *parent = 0);
    bool parseJSON (const QDateTime &dtUpdate, bool &bGotOld, int &nNew);
    qint32 getUsableMsgsCount ();
    void setEmitLog (bool enable);

signals:
    void oneElement (const GVInboxEntry &element);

protected:
    bool startElement (const QString        &namespaceURI,
                       const QString        &localName   ,
                       const QString        &qName       ,
                       const QXmlAttributes &atts        );

    bool endElement (const QString &namespaceURI,
                     const QString &localName   ,
                     const QString &qName       );

    bool characters (const QString &ch);

private:
    QString strChars;
    QString strJson;
    QString strHtml;
    qint32 nUsableMsgs;

    GVI_SMS_Handler smsHandler;

    bool bEmitLog;
};

#endif // GVI_XMLJSONHANDLER_H