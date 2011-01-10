#ifndef GVI_SMS_HANDLER_H
#define GVI_SMS_HANDLER_H

#include "global.h"

class GVI_SMS_Handler : public QObject, public QXmlDefaultHandler
{
    Q_OBJECT
public:
    explicit GVI_SMS_Handler(QObject *parent = 0);

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
    QString id;

    quint32 uDepth;
    bool bTextStarted;

public:
    QMap<QString, QString> mapTexts;
};

#endif // GVI_SMS_HANDLER_H
