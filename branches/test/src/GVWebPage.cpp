#include "GVWebPage.h"
#include "Singletons.h"
#include "GVI_XMLJsonHandler.h"

#define GV_DATA_BASE "https://www.google.com/voice"
#define SYMBIAN_SIGNED 0

GVWebPage::GVWebPage(QObject *parent/* = NULL*/)
: GVAccess (parent)
, bUseIphoneUA (true)
, webPage (this)
, garbageTimer (this)
, nwCfg (this)
, pageTimeoutTimer (this)
, pCurrentReply (NULL)
, bInDialCancel (false)
{
    webPage.settings()->setAttribute (QWebSettings::JavaEnabled, false);
    webPage.settings()->setAttribute (QWebSettings::AutoLoadImages, false);
    //TODO: Do not download images. Speed up load. Look at other speed ups.
    webPage.setForwardUnsupportedContent (true);

    garbageTimer.setSingleShot (true);
    garbageTimer.setInterval (1000 * 60 * 2);   // 2 minutes

    // For progress bars
    QObject::connect (&webPage, SIGNAL (loadStarted ()),
                       this   , SIGNAL (loadStarted ()));
    QObject::connect (&webPage, SIGNAL (loadFinished (bool)),
                       this   , SIGNAL (loadFinished (bool)));
    QObject::connect (&webPage, SIGNAL (loadProgress (int)),
                       this   , SIGNAL (loadProgress (int)));

    // Garbage timer
    QObject::connect (&garbageTimer, SIGNAL (timeout ()),
                       this        , SLOT   (garbageTimerTimeout ()));
    garbageTimer.start ();

    // Page timeout timer
    QObject::connect (&pageTimeoutTimer, SIGNAL (timeout()),
                       this            , SLOT   (onPageTimeout()));
}//GVWebPage::GVWebPage

GVWebPage::~GVWebPage(void)
{
    if (garbageTimer.isActive ())
    {
        garbageTimer.stop ();
    }
}//GVWebPage::~GVWebPage

void
GVWebPage::setView (QWidget *view)
{
    QWebView *wv = (QWebView *)view;
    wv->setPage (&webPage);
}//GVWebPage::setView

void
GVWebPage::getHostAndQuery (QString &strHost, QString &strQuery)
{
    QUrl urlCurrent = webPage.mainFrame()->url();
    QString strOrig = urlCurrent.toString();
    strQuery        = strOrig.mid (
                      urlCurrent.toString (QUrl::RemoveQuery).count ());
    strHost         = urlCurrent.toString (QUrl::RemovePath|QUrl::RemoveQuery);
}//GVWebPage::getHostAndQuery

void
GVWebPage::loadUrlString (const QString &strUrl)
{
    webPage.mainFrame()->load (QUrl (strUrl));
    onPageProgress (0);
}//GVWebPage::loadUrlString

bool
GVWebPage::isLoadFailed (bool bOk)
{
    bool rv = true;
    do // Begin cleanup block (not a loop)
    {
        if (!bOk) break;

        QMutexLocker locker(&mutex);
        if (workCurrent.bCancel)
        {
            qDebug ("Work canceled. Fail safely");
            break;
        }
        if (GVAW_Nothing == workCurrent.whatwork)
        {
            qDebug ("Invalid work. Fail safely");
            break;
        }

        rv = false;
    } while (0); // End cleanup block (not a loop)

    return (rv);
}//GVWebPage::isLoadFailed

QNetworkReply *
GVWebPage::postRequest (QString            strUrl  ,
                        QStringPairList    arrPairs,
                        QString            strUA   ,
                        QObject           *receiver,
                        const char        *method  )
{
    return GVAccess::postRequest (webPage.networkAccessManager (),
                                  strUrl, arrPairs, strUA,
                                  receiver, method);
}//GVWebPage::postRequest

QWebElement
GVWebPage::doc ()
{
    return (webPage.mainFrame()->documentElement());
}//GVWebPage::doc

bool
GVWebPage::isLoggedIn ()
{
    QWebFrame *f = doc().webFrame();
    QWebElementCollection t = f->findAllElements("form [name=\"_rnr_se\"]");
    return (0 != t.count ());
}//GVWebPage::isLoggedIn

bool
GVWebPage::isOnline ()
{
#if !defined(Q_OS_SYMBIAN) || SYMBIAN_SIGNED
    return nwCfg.isOnline ();
#else
    // In Symbian with no signing, pretend we're always online.
    // This is because we don't want to sign... yet
    return true;
#endif
}//GVWebPage::isOnline

bool
GVWebPage::aboutBlank ()
{
    QObject::connect (&webPage, SIGNAL (loadFinished (bool)),
                       this   , SLOT   (aboutBlankDone (bool)));
    this->loadUrlString ("about:blank");

    return (true);
}//GVWebPage::aboutBlank

void
GVWebPage::aboutBlankDone (bool bOk)
{
    QObject::disconnect (&webPage, SIGNAL (loadFinished (bool)),
                          this   , SLOT   (aboutBlankDone (bool)));

    completeCurrentWork (GVAW_aboutBlank, bOk);
}//GVWebPage::aboutBlankDone

bool
GVWebPage::login ()
{
    if (!this->isOnline ()) {
        qDebug ("Cannot login when offline");
        completeCurrentWork (GVAW_login, false);
        return false;
    }

    webPage.setUA (bUseIphoneUA);

    // GV page load complete will begin the login process.
    QObject::connect (&webPage, SIGNAL (loadFinished (bool)),
                       this   , SLOT   (loginStage1 (bool)));
    this->loadUrlString (GV_HTTPS);

    return (true);
}//GVWebPage::login

void
GVWebPage::loginStage1 (bool bOk)
{
    QObject::disconnect (&webPage, SIGNAL (loadFinished (bool)),
                          this   , SLOT   (loginStage1 (bool)));
    do // Begin cleanup block (not a loop)
    {
        if (isLoadFailed (bOk))
        {
            bOk = false;
            qWarning ("Main login page load failed");
            break;
        }
        bOk = false;

        qDebug ("Login page loaded");

        QWebElement email = doc().findFirst ("#Email");
        QWebElement passwd = doc().findFirst ("#Passwd");
        QWebElement weLogin = doc().findFirst("#gaia_loginform");

        if (email.isNull () || passwd.isNull () || weLogin.isNull ())
        {
            // The browser may have logged in using prior credentials.
            if (isLoggedIn ())
            {
                // We logged in using prior credentials. Go directly to the end!
                bOk = true;
                completeCurrentWork (GVAW_login, true);
            }
            else
            {
                qWarning ("Invalid page!");
            }
            break;
        }

        QObject::connect (&webPage, SIGNAL (loadFinished (bool)),
                           this   , SLOT   (loginStage2 (bool)));

        email.setAttribute ("value", workCurrent.arrParams[0].toString());
        passwd.setAttribute ("value", workCurrent.arrParams[1].toString());
        weLogin.evaluateJavaScript("this.submit();");

        bOk = true;
    } while (0); // End cleanup block (not a loop)

    if (!bOk)
    {
        completeCurrentWork (GVAW_login, false);
    }
}//GVWebPage::loginStage1

void
GVWebPage::loginStage2 (bool bOk)
{
    QObject::disconnect (&webPage, SIGNAL (loadFinished (bool)),
                          this   , SLOT   (loginStage2 (bool)));
    do // Begin cleanup block (not a loop)
    {
        if (isLoadFailed (bOk))
        {
            bOk = false;
            qWarning ("Page load actual login failed");
            break;
        }
        bOk = false;

        QMutexLocker locker(&mutex);
        if (bUseIphoneUA)
        {
            QNetworkCookieJar *jar = webPage.networkAccessManager()->cookieJar();
            QList<QNetworkCookie> cookies =
                jar->cookiesForUrl (webPage.mainFrame()->url ());
            foreach (QNetworkCookie cookie, cookies)
            {
                if (cookie.name() == "gvx")
                {
                    bLoggedIn = true;
                }
            }
            QObject::connect (&webPage, SIGNAL (loadFinished (bool)),
                               this   , SLOT   (loginStage3 (bool)));
            this->loadUrlString (GV_HTTPS_M "/i/all");
        }
        else
        {
            if (!isLoggedIn ())
            {
                qWarning ("Failed to login!");
                break;
            }

            // Whats the GV number?
#define GVSELECTOR "div b[class=\"ms3\"]"
            QWebElement num = doc().findFirst (GVSELECTOR);
#undef GVSELECTOR
            if (num.isNull ())
            {
                qWarning ("Failed to get a google voice number!!");
                break;
            }

            strSelfNumber = num.toPlainText ();
            simplify_number (strSelfNumber, false);
            workCurrent.arrParams += QVariant (strSelfNumber);

#define GVSELECTOR "input[name=\"_rnr_se\"]"
            QWebElement rnr_se = doc().findFirst (GVSELECTOR);
#undef GVSELECTOR
            if (rnr_se.isNull ())
            {
                qWarning ("Could not find rnr_se");
                break;
            }
            strRnr_se = rnr_se.attribute ("value");

            bLoggedIn = true;
            completeCurrentWork (GVAW_login, true);
        }

        bOk = true;
    } while (0); // End cleanup block (not a loop)

    if (!bOk) {
        completeCurrentWork (GVAW_login, false);
    }
}//GVWebPage::loginStage2

void
GVWebPage::loginStage3 (bool bOk)
{
    QObject::disconnect (&webPage, SIGNAL (loadFinished (bool)),
                          this   , SLOT   (loginStage3 (bool)));
    do // Begin cleanup block (not a loop)
    {
        if (isLoadFailed (bOk))
        {
            bOk = false;
            qWarning ("Page load actual login failed");
            break;
        }
        bOk = false;

        // Whats the GV number?
#define GVSELECTOR "div b[class=\"ms3\"]"
        QWebElement num = doc().findFirst (GVSELECTOR);
#undef GVSELECTOR
        if (num.isNull ())
        {
            qWarning ("Failed to get a google voice number!!");
            break;
        }

        strSelfNumber = num.toPlainText ();
        simplify_number (strSelfNumber, false);
        workCurrent.arrParams += QVariant (strSelfNumber);

#define GVSELECTOR "input[name=\"_rnr_se\"]"
        QWebElement rnr_se = doc().findFirst (GVSELECTOR);
#undef GVSELECTOR
        if (rnr_se.isNull ())
        {
            qWarning ("Could not find rnr_se");
            break;
        }
        strRnr_se = rnr_se.attribute ("value");

        bLoggedIn = true;
        bOk = true;
    } while (0); // End cleanup block (not a loop)

    completeCurrentWork (GVAW_login, bOk);
}//GVWebPage::loginStage3

bool
GVWebPage::logout ()
{
    if (!this->isOnline ()) {
        qDebug ("Cannot logout when offline");
        completeCurrentWork (GVAW_logout, false);
        return false;
    }

    QObject::connect (&webPage, SIGNAL (loadFinished (bool)),
                       this   , SLOT   (logoutDone (bool)));
    this->loadUrlString (GV_HTTPS "/account/signout");

    return (true);
}//GVWebPage::logout

void
GVWebPage::logoutDone (bool bOk)
{
    QObject::disconnect (&webPage, SIGNAL (loadFinished (bool)),
                          this   , SLOT   (logoutDone (bool)));

    if (bOk)
    {
        QMutexLocker locker(&mutex);
        bLoggedIn = false;
    }

    completeCurrentWork (GVAW_logout, bOk);
}//GVWebPage::logoutDone

bool
GVWebPage::retrieveContacts ()
{
    if (!this->isOnline ()) {
        qDebug ("Cannot retrieve contacts when offline");
        completeCurrentWork (GVAW_getAllContacts, false);
        return false;
    }

    QMutexLocker locker(&mutex);
    if (!bLoggedIn)
    {
        completeCurrentWork (GVAW_getAllContacts, false);
        return (false);
    }

    nCurrent = 1;
    QString strLink = GV_HTTPS_M "/contacts?p=1";
    QObject::connect (&webPage, SIGNAL (loadFinished (bool)),
                       this   , SLOT   (contactsLoaded (bool)));
    this->loadUrlString (strLink);

    return (true);
}//GVWebPage::retrieveContacts

void
GVWebPage::contactsLoaded (bool bOk)
{
    QString msg;
    QObject::disconnect (&webPage, SIGNAL (loadFinished (bool)),
                          this   , SLOT   (contactsLoaded (bool)));
    do // Begin cleanup block (not a loop)
    {
        if (isLoadFailed (bOk))
        {
            bOk = false;
            qWarning ("Failed to load contacts page");
            break;
        }
        bOk = false;

        QWebFrame *frame = webPage.mainFrame();
        if (NULL == frame)
        {
            qWarning ("No frame!!");
            break;
        }

        QWebElementCollection results;
        results = frame->findAllElements("div[class=\"ms2\"] > a");
        int max = results.count ();
        int nContactCount = 0;
        for (int i = 0; i < max; i++)
        {
            QWebElement e = results.at(i);
            QString href = e.attribute ("href");
            if (( href.isEmpty ()) ||
                (!href.startsWith ("/voice/m/contact/")) ||
                ( href.startsWith ("/voice/m/contact/p=")))
            {
                continue;
            }

            emit gotContact (e.toPlainText (), href);
            nContactCount++;
        }

        if (0 != nContactCount)
        {
            msg = QString ("Found %1 contacts on page %2")
                  .arg(nContactCount)
                  .arg(nCurrent);
            qDebug () << msg;
        }
        else
        {
            bOk = true;
            completeCurrentWork (GVAW_getAllContacts, true);
            break;
        }

        nCurrent++;
        QString strNextPage = QString (GV_HTTPS_M "/contacts?p=%1")
                              .arg(nCurrent);

        QObject::connect (&webPage, SIGNAL (loadFinished (bool)),
                           this   , SLOT   (contactsLoaded (bool)));
        this->loadUrlString (strNextPage);

        bOk = true;
    } while (0); // End cleanup block (not a loop)

    if (!bOk)
    {
        completeCurrentWork (GVAW_getAllContacts, false);
    }
}//GVWebPage::contactsLoaded

bool
GVWebPage::isNextContactsPageAvailable ()
{
    QWebFrame *f = webPage.mainFrame();
    QString strTest = QString("a[href=\"/voice/m/contacts?p=%1\"]")
                         .arg(nCurrent+1);
    QWebElementCollection t = f->findAllElements (strTest);
    return (0 != t.count ());
}//GVWebPage::isNextContactsPageAvailable

bool
GVWebPage::dialCallback (bool bCallback)
{
    if (!this->isOnline ()) {
        qDebug ("Cannot dial back when offline");
        completeCurrentWork (bCallback?GVAW_dialCallback:GVAW_dialOut, false);
        return false;
    }

    QMutexLocker locker(&mutex);
    if (!bLoggedIn)
    {
        completeCurrentWork (bCallback?GVAW_dialCallback:GVAW_dialOut, false);
        return (false);
    }

    QVariantList &arrParams = workCurrent.arrParams;
    QStringPairList arrPairs;
    workCurrent.cancel = (WebPageCancel) &GVWebPage::cancelDataDial2;
    QNetworkReply *reply = NULL;

    if (!bCallback)
    {
        if (!bUseIphoneUA)
        {
            qWarning ("Cannot callout if the UA is not the iPhone UA");
            completeCurrentWork (GVAW_dialOut, false);
            return (false);
        }

        QString strUA = UA_IPHONE;
        QString strUrl = QString("https://www.google.com/voice/m/x"
                                 "?m=call"
                                 "&n=%1"
                                 "&f="
                                 "&v=6")
                                 .arg(arrParams[0].toString());

        QNetworkRequest request(strUrl);
        request.setRawHeader ("User-Agent", strUA.toAscii ());

        QNetworkAccessManager *mgr = webPage.networkAccessManager ();
        QNetworkCookieJar *jar = mgr->cookieJar();
        QList<QNetworkCookie> cookies =
            jar->cookiesForUrl (webPage.mainFrame()->url ());
        QList<QNetworkCookie> sendCookies;
        QString gvxVal;
        foreach (QNetworkCookie cookie, cookies)
        {
            if ((cookie.name() == "gv")   ||
                (cookie.name() == "gvx")  ||
                (cookie.name() == "PREF") ||
                (cookie.name() == "S")    ||
                (cookie.name() == "SID")  ||
                (cookie.name() == "HSID") ||
                (cookie.name() == "SSID"))
            {
                sendCookies += cookie;
            }

            if (cookie.name () == "gvx")
            {
                gvxVal = cookie.value ();
            }
        }

        // Our own number is added separately
        // The expected format is "number|type" but it seems just "number" also
        // works. Of course not sending this cookie also works...
//        QString gvph = QString ("%1|%2")
//                        .arg (strCurrentCallback)
//                        .arg (chCurrentCallbackType);
        QString gvph = QString ("%1")
                        .arg (strCurrentCallback);
        sendCookies += QNetworkCookie ("gv-ph", gvph.toAscii ());

        // Set up the cookies in the request
        request.setHeader (QNetworkRequest::CookieHeader,
                           QVariant::fromValue(sendCookies));

        // This cookie needs to also be added as contect data
        QString strContent = QString("{\"gvx\":\"%1\"}").arg(gvxVal);

        QObject::connect (mgr , SIGNAL (finished (QNetworkReply *)),
                          this, SLOT   (onDataCallDone (QNetworkReply *)));
        this->bIsCallback = false;
        reply = mgr->post (request, strContent.toAscii());
    }
    else
    {
        arrPairs += QStringPair("outgoingNumber"  , arrParams[0].toString());
        arrPairs += QStringPair("forwardingNumber", arrParams[2].toString());
        arrPairs += QStringPair("subscriberNumber", strSelfNumber);
        arrPairs += QStringPair("phoneType"       , arrParams[3].toString());
        arrPairs += QStringPair("remember"        , "1");
        arrPairs += QStringPair("_rnr_se"         , strRnr_se);
        this->bIsCallback = true;
        reply =
        postRequest (GV_DATA_BASE "/call/connect/", arrPairs, UA_IPHONE,
                     this, SLOT (onDataCallDone (QNetworkReply *)));
    }

    startTimerForReply (reply);

    return (true);
}//GVWebPage::dialCallback

void
GVWebPage::onDataCallDone (QNetworkReply * reply)
{
    QNetworkAccessManager *mgr = webPage.networkAccessManager ();
    QObject::disconnect (mgr , SIGNAL (finished (QNetworkReply *)),
                         this, SLOT (onDataCallDone (QNetworkReply *)));
    QByteArray ba = reply->readAll ();
    QString msg = ba;

    bool bOk = false;
    do { // Begin cleanup block (not a loop)
        qDebug () << msg;
        QRegExp rx("\"access_number\":\"([+\\d]*)\"");
        if (msg.contains (rx) && (1 == rx.captureCount ()))
        {
            QMutexLocker locker(&mutex);
            if (GVAW_dialOut != workCurrent.whatwork)
            {
                qWarning ("What the hell??");
                break;
            }

            QString strAccess = rx.cap(1);
            qWarning () << QString ("access number = \"%1\"").arg(strAccess);

            emit dialAccessNumber (strAccess, workCurrent.arrParams[1]);

            // Don't need to check if this is a callback - if we got an access
            // number it most definitely means that this is supposed to be a
            // call out.
            completeCurrentWork (GVAW_dialOut, true);
            bOk = true;
            break;
        }

        // Old style callout
        msg = msg.simplified ();
        msg.remove(QRegExp("[ \t\n]*"));
        if (!msg.contains ("\"ok\":true", Qt::CaseSensitive))
        {
            qWarning() << "Failed to dial out. Response to dial out request ="
                       << msg;
            completeCurrentWork(this->bIsCallback ? GVAW_dialCallback
                                                  : GVAW_dialOut,
                                false);
            break;
        }

        emit dialInProgress (workCurrent.arrParams[0].toString ());
        bOk = true;
    } while (0); // End cleanup block (not a loop)

    reply->deleteLater ();
    if (pCurrentReply == reply) {
        pCurrentReply = NULL;
    }
}//GVWebPage::onDataCallDone

void
GVWebPage::cancelDataDial2 ()
{
    if (!this->isOnline () || bInDialCancel) {
        bInDialCancel = false;
        qDebug ("Cannot cancel dial back when offline");
        QMutexLocker locker(&mutex);
        if ((GVAW_dialCallback == workCurrent.whatwork) ||
            (GVAW_dialOut      == workCurrent.whatwork))
        {
            completeCurrentWork (workCurrent.whatwork, false);
        }
        return;
    }

    bInDialCancel = true;

    QStringPairList arrPairs;
    arrPairs += QStringPair("outgoingNumber"  , "undefined");
    arrPairs += QStringPair("forwardingNumber", strCurrentCallback);
    arrPairs += QStringPair("cancelType"      , "C2C");
    arrPairs += QStringPair("_rnr_se"         , strRnr_se);

    QNetworkReply *reply =
    postRequest (GV_DATA_BASE "/call/cancel/", arrPairs, QString (),
                 this, SLOT (onDataCallCanceled (QNetworkReply *)));
    startTimerForReply (reply);
}//GVWebPage::cancelDataDial2

void
GVWebPage::onDataCallCanceled (QNetworkReply * reply)
{
    bInDialCancel = false;
    QNetworkAccessManager *mgr = webPage.networkAccessManager ();
    QObject::disconnect (mgr , SIGNAL (finished (QNetworkReply *)),
                         this, SLOT (onDataCallCanceled (QNetworkReply *)));
    QByteArray ba = reply->readAll ();
    QString msg = ba;

    QMutexLocker locker(&mutex);
    if ((GVAW_dialCallback == workCurrent.whatwork) ||
        (GVAW_dialOut      == workCurrent.whatwork))
    {
        completeCurrentWork (workCurrent.whatwork, false);
    }

    reply->deleteLater ();
}//GVWebPage::onDataCallCanceled

bool
GVWebPage::getContactInfoFromLink ()
{
    if (!this->isOnline ()) {
        qDebug ("Cannot retrieve contact info when offline");
        completeCurrentWork (GVAW_getContactFromLink, false);
        return false;
    }

    QMutexLocker locker(&mutex);
    if (!bLoggedIn)
    {
        completeCurrentWork (GVAW_getContactFromLink, false);
        return (false);
    }

    QString strQuery, strHost;
    this->getHostAndQuery (strHost, strQuery);
    QString strGoto = strHost
                    + workCurrent.arrParams[0].toString();
    QObject::connect (&webPage, SIGNAL (loadFinished (bool)),
                       this   , SLOT   (contactInfoLoaded (bool)));
    this->loadUrlString (strGoto);

    return (true);
}//GVWebPage::getContactInfoFromLink

void
GVWebPage::contactInfoLoaded (bool bOk)
{
    ContactInfo info;
    QObject::disconnect (&webPage, SIGNAL (loadFinished (bool)),
                          this   , SLOT   (contactInfoLoaded (bool)));
    do // Begin cleanup block (not a loop)
    {
        if (isLoadFailed (bOk))
        {
            bOk = false;
            qWarning ("Failed to load call page 2");
            break;
        }
        bOk = false;

#define GVSELECTOR "div span strong"
        QWebElement user = doc().findFirst (GVSELECTOR);
#undef GVSELECTOR
        if (user.isNull ())
        {
            qWarning ("Couldn't find user name on page");
            break;
        }
        info.strTitle = user.toPlainText ();

#define GVSELECTOR "div form div input[name=\"call\"]"
        QWebElementCollection numbers = doc().findAll (GVSELECTOR);
#undef GVSELECTOR
        if (0 == numbers.count ())
        {
            qWarning ("No numbers found for this contact");
            break;
        }

        QRegExp rx("([A-Z])\\)$", Qt::CaseInsensitive);
        int i = 0;
        foreach (QWebElement btnCall, numbers)
        {
            QWebElement divParent = btnCall.parent ();
            PhoneInfo gvNumber;
            gvNumber.strNumber = divParent.toPlainText().simplified ();
            int pos = rx.indexIn (gvNumber.strNumber);
            if (-1 != pos)
            {
                QString strChr = rx.cap ();
                gvNumber.Type = PhoneInfo::charToType (strChr[0].toAscii ());
                gvNumber.strNumber.chop (3);
                gvNumber.strNumber = gvNumber.strNumber.trimmed ();
            }
            info.arrPhones += gvNumber;
            QString strLHS = workCurrent.arrParams[1].toString();
            QString strRHS = gvNumber.strNumber;
            simplify_number (strLHS);
            simplify_number (strRHS);
            if (strLHS == strRHS)
            {
                info.selected = i;
            }

            i++;
        }

        bOk = true;
    } while (0); // End cleanup block (not a loop)
    if (bOk)
    {
        info.strId = workCurrent.arrParams[0].toString();
        emit contactInfo (info);
    }

    completeCurrentWork (GVAW_getContactFromLink, bOk);
}//GVWebPage::contactInfoLoaded

bool
GVWebPage::getRegisteredPhones ()
{
    if (!this->isOnline ()) {
        qDebug ("Cannot get registered phones when offline");
        completeCurrentWork (GVAW_getRegisteredPhones, false);
        return false;
    }

    QMutexLocker locker(&mutex);
    if (!bLoggedIn)
    {
        completeCurrentWork (GVAW_getRegisteredPhones, false);
        return (false);
    }

    QString strGoto = GV_HTTPS_M "/selectphone";
    QObject::connect (&webPage, SIGNAL (loadFinished (bool)),
                       this   , SLOT   (phonesListLoaded (bool)));
    this->loadUrlString (strGoto);

    return (true);
}//GVWebPage::getRegisteredPhones

void
GVWebPage::phonesListLoaded (bool bOk)
{
    QObject::disconnect (&webPage, SIGNAL (loadFinished (bool)),
                          this   , SLOT   (phonesListLoaded (bool)));
    do // Begin cleanup block (not a loop)
    {
        if (isLoadFailed (bOk))
        {
            bOk = false;
            qWarning ("Failed to load phones settings page");
            break;
        }

#define GVSELECTOR "div form div input[type=\"radio\"]"
        QWebElementCollection numbers = doc().findAll (GVSELECTOR);
#undef GVSELECTOR
        if (0 == numbers.count ())
        {
            qWarning ("No registered phones found for this account");
            break;
        }

        QString strText = numbers[0].parent().toPlainText ();
        QStringList astrPhones = strText.split ("\n", QString::SkipEmptyParts);
        int index = 0;
        foreach (strText, astrPhones)
        {
            GVRegisteredNumber regNumber;
            strText = strText.simplified ();
            QStringList arrSplit = strText.split (":");
            regNumber.strName = arrSplit[0].trimmed ();
            regNumber.strDescription = arrSplit[1].trimmed();
            // Make the actual number follow the form: +1aaabbbcccc
            regNumber.strDescription.remove (QRegExp("[ \t\n()-]"));
            simplify_number (regNumber.strDescription);

            QString strFromInput = numbers[index].attribute("value");
            QRegExp rx1("(.*)\\|(.)");
            if ((strFromInput.contains (rx1)) && (2 == rx1.captureCount ()))
            {
                QString strTemp = rx1.cap (1);
                simplify_number (strTemp);
                if (strTemp == regNumber.strDescription) {
                    regNumber.chType = rx1.cap (2)[0].toAscii ();
                }
            }
            emit registeredPhone (regNumber);
            index++;
        }

        bOk = true;
    } while (0); // End cleanup block (not a loop)

    completeCurrentWork (GVAW_getRegisteredPhones, bOk);
}//GVWebPage::phonesListLoaded

void
GVWebPage::userCancel ()
{
    QMutexLocker locker(&mutex);
    webPage.triggerAction (QWebPage::Stop);
}//GVWebPage::userCancel

bool
GVWebPage::sendInboxRequest ()
{
    if (!this->isOnline ()) {
        qDebug ("Cannot send request for inbox when offline");
        completeCurrentWork (GVAW_getInbox, false);
        return false;
    }

    QMutexLocker locker(&mutex);
    if (!bLoggedIn)
    {
        completeCurrentWork (GVAW_getInbox, false);
        return (false);
    }

    do // Begin cleanup block (not a loop)
    {
        QString strWhich = workCurrent.arrParams[0].toString();

        emit status (QString("Getting inbox page %1...").arg(nCurrent), 0);

        QString strLink = QString (GV_HTTPS "/inbox/recent/%1?page=p%2")
                            .arg(strWhich).arg(nCurrent);
        QNetworkRequest request(strLink);
        request.setRawHeader ("User-Agent", UA_IPHONE);

        QNetworkAccessManager *mgr = webPage.networkAccessManager ();
        QNetworkCookieJar *jar = mgr->cookieJar();
        QList<QNetworkCookie> cookies =
            jar->cookiesForUrl (webPage.mainFrame()->url ());
        QList<QNetworkCookie> sendCookies;
        QString gvxVal;
        foreach (QNetworkCookie cookie, cookies)
        {
            if ((cookie.name() == "gv")   ||
                (cookie.name() == "gvx")  ||
                (cookie.name() == "PREF") ||
                (cookie.name() == "S")    ||
                (cookie.name() == "SID")  ||
                (cookie.name() == "HSID") ||
                (cookie.name() == "SSID"))
            {
                sendCookies += cookie;
            }

            if (cookie.name () == "gvx")
            {
                gvxVal = cookie.value ();
            }
        }

        // Set up the cookies in the request
        request.setHeader (QNetworkRequest::CookieHeader,
                           QVariant::fromValue(sendCookies));

        QObject::connect (mgr , SIGNAL (finished (QNetworkReply *)),
                          this, SLOT   (onGotInboxXML (QNetworkReply *)));
        mgr->get (request);
    } while (0); // End cleanup block (not a loop)

    return (true);
}//GVWebPage::sendInboxRequest

bool
GVWebPage::getInbox ()
{
    if (!this->isOnline ()) {
        qDebug ("Cannot get inbox when offline");
        completeCurrentWork (GVAW_getInbox, false);
        return false;
    }

    QMutexLocker locker(&mutex);
    if (!bLoggedIn)
    {
        completeCurrentWork (GVAW_getInbox, false);
        return (false);
    }

    nFirstPage = nCurrent = workCurrent.arrParams[1].toString().toInt ();

    return sendInboxRequest ();
}//GVWebPage::getInbox

void
GVWebPage::onGotInboxXML (QNetworkReply *reply)
{
    QNetworkAccessManager *mgr = webPage.networkAccessManager ();
    QObject::disconnect (mgr , SIGNAL (finished (QNetworkReply *)),
                         this, SLOT   (onGotInboxXML (QNetworkReply *)));

    QString strReply = reply->readAll ();
    QXmlInputSource inputSource;
    QXmlSimpleReader simpleReader;
    inputSource.setData (strReply);
    GVI_XMLJsonHandler xmlHandler;

    QObject::connect (
        &xmlHandler, SIGNAL (oneElement (const GVInboxEntry &)),
         this      , SIGNAL (oneInboxEntry (const GVInboxEntry &)));

    bool bOk = false;
    do // Begin cleanup block (not a loop)
    {
        simpleReader.setContentHandler (&xmlHandler);
        simpleReader.setErrorHandler (&xmlHandler);

        qDebug ("Begin parsing");
        if (!simpleReader.parse (&inputSource, false))
        {
            qWarning ("Failed to parse XML");
            break;
        }
        qDebug ("End parsing");

        QDateTime dtUpdate = workCurrent.arrParams[3].toDateTime ();
        bool bGotOld = false;
        int nNew;
        if (!xmlHandler.parseJSON (dtUpdate, bGotOld, nNew))
        {
            qWarning ("Failed to parse GV Inbox JSON");
            break;
        }
        if (workCurrent.arrParams.count() < 5) {
            workCurrent.arrParams.append (nNew);
        } else {
            nNew += workCurrent.arrParams[4].toInt();
            workCurrent.arrParams[4] = nNew;
        }

        QMutexLocker locker(&mutex);
        nCurrent++;

        bOk = true;

        int count = workCurrent.arrParams[2].toString().toInt ();
        if (((nCurrent-nFirstPage) >= count) ||
            (bGotOld) ||
            (0 == xmlHandler.getUsableMsgsCount ())) {
            completeCurrentWork (GVAW_getInbox, true);
            break;
        }

        sendInboxRequest ();
    } while (0); // End cleanup block (not a loop)

    if (!bOk)
    {
        completeCurrentWork (GVAW_getInbox, false);
    }

    reply->deleteLater ();
}//GVWebPage::onGotInboxXML

bool
GVWebPage::getContactFromInboxLink ()
{
    if (!this->isOnline ()) {
        qDebug ("Cannot get contact from inbox link when offline");
        completeCurrentWork (GVAW_getContactFromInboxLink, false);
        return false;
    }

    QMutexLocker locker(&mutex);
    if (!bLoggedIn)
    {
        qWarning ("User not logged in when calling Inbox link");
        completeCurrentWork (GVAW_getContactFromInboxLink, false);
        return (false);
    }

    QString strQuery, strHost;
    this->getHostAndQuery (strHost, strQuery);
    QString strGoto = strHost
                    + workCurrent.arrParams[0].toString();
    QObject::connect (
        &webPage, SIGNAL (loadFinished (bool)),
         this   , SLOT   (getContactFromInboxLinkLoaded (bool)));
    this->loadUrlString (strGoto);

    return (true);
}//GVWebPage::getContactFromInboxLink

void
GVWebPage::getContactFromInboxLinkLoaded (bool bOk)
{
    QObject::disconnect (
        &webPage, SIGNAL (loadFinished (bool)),
         this   , SLOT   (getContactFromInboxLinkLoaded (bool)));

    ContactInfo info;
    do // Begin cleanup block (not a loop)
    {
        if (isLoadFailed (bOk))
        {
            bOk = false;
            qWarning ("Failed to load Inbox link page");
            break;
        }
        bOk = false;

#define GVSELECTOR "div form input[name=\"number\"]"
        QWebElement user = doc().findFirst (GVSELECTOR);
#undef GVSELECTOR
        if (user.isNull ())
        {
            qWarning ("Couldn't find user name on page");
            break;
        }
        info.strTitle = user.attribute ("name");
        if (0 == info.strTitle.compare ("number", Qt::CaseInsensitive))
        {
            info.strTitle = user.attribute ("value");
        }

#define GVSELECTOR "div form div input[name=\"call\"]"
        QWebElementCollection numbers = doc().findAll (GVSELECTOR);
#undef GVSELECTOR
        if (0 == numbers.count ())
        {
            qWarning ("No numbers found for this contact");
            break;
        }

        QRegExp rx("([A-Z])\\)$", Qt::CaseInsensitive);
        foreach (QWebElement btnCall, numbers)
        {
            QWebElement divParent = btnCall.parent ();
            PhoneInfo gvNumber;
            gvNumber.strNumber = divParent.toPlainText().simplified ();
            int pos = rx.indexIn (gvNumber.strNumber);
            if (-1 != pos)
            {
                QString strChr = rx.cap ();
                gvNumber.Type = PhoneInfo::charToType (strChr[0].toAscii ());
                gvNumber.strNumber.chop (3);
                gvNumber.strNumber = gvNumber.strNumber.trimmed ();
            }
            info.arrPhones += gvNumber;
        }

        bOk = true;
    } while (0); // End cleanup block (not a loop)
    if (bOk)
    {
        info.strId = workCurrent.arrParams[0].toString();
        emit contactInfo (info);
    }

    completeCurrentWork (GVAW_getContactFromInboxLink, bOk);
}//GVWebPage::getContactFromInboxLinkLoaded

void
GVWebPage::garbageTimerTimeout ()
{
    webPage.settings()->clearIconDatabase ();
    webPage.settings()->clearMemoryCaches ();

    garbageTimer.start ();
}//GVWebPage::garbageTimerTimeout

bool
GVWebPage::sendSMS ()
{
    if (!this->isOnline ()) {
        qDebug ("Cannot send SMS when offline");
        completeCurrentWork (GVAW_sendSMS, false);
        return false;
    }

    QMutexLocker locker(&mutex);
    if (!bLoggedIn)
    {
        qWarning ("User not logged in when attempting to send an SMS");
        completeCurrentWork (GVAW_sendSMS, false);
        return (false);
    }

    QVariantList &arrParams = workCurrent.arrParams;
    QStringPairList arrPairs;

    do // Begin cleanup block (not a loop)
    {
        QString strUA = UA_IPHONE;
        QString strUrl = QString("https://www.google.com/voice/m/x"
                                 "?m=sms"
                                 "&n=%1"
                                 "&v=7")
                                 .arg(arrParams[0].toString());
        QUrl url(strUrl);
        url.addQueryItem ("txt", arrParams[1].toString());

        QNetworkRequest request(url);
        request.setRawHeader ("User-Agent", strUA.toAscii ());

        QNetworkAccessManager *mgr = webPage.networkAccessManager ();
        QNetworkCookieJar *jar = mgr->cookieJar();
        QList<QNetworkCookie> cookies =
            jar->cookiesForUrl (webPage.mainFrame()->url ());
        QList<QNetworkCookie> sendCookies;
        QString gvxVal;
        foreach (QNetworkCookie cookie, cookies)
        {
            if ((cookie.name() == "gv")   ||
                (cookie.name() == "gvx")  ||
                (cookie.name() == "PREF") ||
                (cookie.name() == "S")    ||
                (cookie.name() == "SID")  ||
                (cookie.name() == "HSID") ||
                (cookie.name() == "SSID"))
            {
                sendCookies += cookie;
            }

            if (cookie.name () == "gvx")
            {
                gvxVal = cookie.value ();
            }
        }

        // Our own number is added separately
        // The expected format is "number|type" but it seems just "number" also
        // works. Of course not sending this cookie also works...
//        QString gvph = QString ("%1|%2")
//                        .arg (strCurrentCallback)
//                        .arg (chCurrentCallbackType);
        QString gvph = QString ("%1")
                        .arg (strCurrentCallback);
        sendCookies += QNetworkCookie ("gv-ph", gvph.toAscii ());

        // Set up the cookies in the request
        request.setHeader (QNetworkRequest::CookieHeader,
                           QVariant::fromValue(sendCookies));

        // This cookie needs to also be added as contect data
        QString strContent = QString("{\"gvx\":\"%1\"}").arg(gvxVal);

        QObject::connect (mgr , SIGNAL (finished        (QNetworkReply *)),
                          this, SLOT   (sendSMSResponse (QNetworkReply *)));
        QNetworkReply *reply = mgr->post (request, strContent.toAscii());

        startTimerForReply (reply);
    } while (0); // End cleanup block (not a loop)
    return (true);
}//GVWebPage::sendSMS

void
GVWebPage::sendSMSResponse (QNetworkReply *reply)
{
    QNetworkAccessManager *mgr = webPage.networkAccessManager ();
    QObject::disconnect (mgr , SIGNAL (finished        (QNetworkReply *)),
                         this, SLOT   (sendSMSResponse (QNetworkReply *)));
    QByteArray ba = reply->readAll ();
    QString msg = ba;

    bool rv = false;
    if (ba.contains ("\"send_sms_response\":{\"status\":{\"status\":0}"))
    {
        rv = true;
    }
    completeCurrentWork (GVAW_sendSMS, rv);
}//GVWebPage::sendSMSResponse

bool
GVWebPage::playVmail ()
{
    if (!this->isOnline ()) {
        qDebug ("Cannot download vmail when offline");
        completeCurrentWork (GVAW_playVmail, false);
        return false;
    }

    QMutexLocker locker(&mutex);
    if (!bLoggedIn)
    {
        completeCurrentWork (GVAW_playVmail, false);
        return (false);
    }

    do // Begin cleanup block (not a loop)
    {
        QString strWhich = workCurrent.arrParams[0].toString();

        QString strLink = QString (GV_HTTPS "/media/send_voicemail/%1")
                            .arg(workCurrent.arrParams[0].toString());
        QNetworkRequest request(strLink);
        request.setRawHeader ("User-Agent", UA_IPHONE);

        QNetworkAccessManager *mgr = webPage.networkAccessManager ();
        QNetworkCookieJar *jar = mgr->cookieJar();
        QList<QNetworkCookie> cookies =
            jar->cookiesForUrl (webPage.mainFrame()->url ());
        QList<QNetworkCookie> sendCookies;
        QString gvxVal;
        foreach (QNetworkCookie cookie, cookies)
        {
            if ((cookie.name() == "gv")   ||
                (cookie.name() == "gvx")  ||
                (cookie.name() == "PREF") ||
                (cookie.name() == "S")    ||
                (cookie.name() == "SID")  ||
                (cookie.name() == "HSID") ||
                (cookie.name() == "SSID"))
            {
                sendCookies += cookie;
            }

            if (cookie.name () == "gvx")
            {
                gvxVal = cookie.value ();
            }
        }

        // Set up the cookies in the request
        request.setHeader (QNetworkRequest::CookieHeader,
                           QVariant::fromValue(sendCookies));

        emit status ("Starting vmail download");
        QObject::connect (mgr , SIGNAL (finished          (QNetworkReply *)),
                          this, SLOT   (onVmailDownloaded (QNetworkReply *)));
        QNetworkReply *reply = mgr->get (request);

        startTimerForReply (reply);
    } while (0); // End cleanup block (not a loop)

    return (true);
}//GVWebPage::playVmail

void
GVWebPage::onVmailDownloaded (QNetworkReply *reply)
{
    QNetworkAccessManager *mgr = webPage.networkAccessManager ();
    QObject::disconnect (mgr , SIGNAL (finished          (QNetworkReply *)),
                         this, SLOT   (onVmailDownloaded (QNetworkReply *)));

    bool rv = true;
    do // Begin cleanup block (not a loop)
    {
        QFile file(workCurrent.arrParams[1].toString());
        if (!file.open(QFile::ReadWrite))
        {
            qWarning ("Failed to open the vmail file. Abort!");
            break;
        }

        qDebug () << QString ("Saving vmail in %1").arg(file.fileName ());
        file.write(reply->readAll());
        emit status ("vmail saved");

        rv = true;
    } while (0); // End cleanup block (not a loop)

    completeCurrentWork (GVAW_playVmail, rv);
    reply->deleteLater ();
}//GVWebPage::onVmailDownloaded

void
GVWebPage::onPageTimeout ()
{
    if (NULL != pCurrentReply) {
        qWarning ("Request has timed out. Aborting!!!");
        pCurrentReply->abort ();

        QObject::disconnect (
            pCurrentReply, SIGNAL(downloadProgress(qint64,qint64)),
            this         , SLOT(onSocketXfer(qint64,qint64)));
        QObject::disconnect (
            pCurrentReply, SIGNAL(uploadProgress(qint64,qint64)),
            this         , SLOT(onSocketXfer(qint64,qint64)));
        pCurrentReply = NULL;

        //@@UV: Test if this is required
        cancelWork ();
    } else {
        qWarning ("Web page load has timed out. Aborting!!!");
        userCancel ();
    }
}//GVWebPage::onPageTimeout

void
GVWebPage::onPageProgress(int progress)
{
    pageTimeoutTimer.stop ();
    if (0 == progress) {
        qDebug ("Page timeout timer started");
    } else {
        qDebug("Page progressed. Not timing out!");
    }
    pageTimeoutTimer.setInterval (timeout * 1000);
    pageTimeoutTimer.setSingleShot (true);
    pageTimeoutTimer.start ();
}//GVWebPage::onPageProgress

void
GVWebPage::onSocketXfer (qint64 bytesXfer, qint64 bytesTotal)
{
    pageTimeoutTimer.stop ();
    if ((0 == bytesXfer) && (0 == bytesTotal)) {
        qDebug("Started the timeout timer");
    } else {
        qDebug() << QString("Socket transferred %1 byte%2 of data. "
                            "Not timing out!")
                    .arg (bytesXfer)
                    .arg (1 == bytesXfer ? "" : "s");
    }
    pageTimeoutTimer.setInterval (timeout * 1000);
    pageTimeoutTimer.setSingleShot (true);
    pageTimeoutTimer.start ();
}//GVWebPage::onSocketXfer

void
GVWebPage::completeCurrentWork (GVAccess_Work whatwork, bool bOk)
{
    pageTimeoutTimer.stop ();
    if (NULL != pCurrentReply) {
        QObject::disconnect (
            pCurrentReply, SIGNAL(downloadProgress(qint64,qint64)),
            this         , SLOT(onSocketXfer(qint64,qint64)));
        QObject::disconnect (
            pCurrentReply, SIGNAL(uploadProgress(qint64,qint64)),
            this         , SLOT(onSocketXfer(qint64,qint64)));
        pCurrentReply = NULL;
    }

    GVAccess::completeCurrentWork (whatwork, bOk);
}//GVWebPage::completeCurrentWork

void
GVWebPage::startTimerForReply (QNetworkReply *reply)
{
    pCurrentReply = reply;
    QObject::connect (reply, SIGNAL(downloadProgress(qint64,qint64)),
                      this , SLOT(onSocketXfer(qint64,qint64)));
    QObject::connect (reply, SIGNAL(uploadProgress(qint64,qint64)),
                      this , SLOT(onSocketXfer(qint64,qint64)));
    onSocketXfer (0,0);
}//GVWebPage::startTimerForReply