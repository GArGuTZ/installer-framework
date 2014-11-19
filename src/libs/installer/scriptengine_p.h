/**************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt Installer Framework.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights. These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
**
** $QT_END_LICENSE$
**
**************************************************************************/

#ifndef SCRIPTENGINE_P_H
#define SCRIPTENGINE_P_H

#include "component.h"
#include "packagemanagercore.h"

#include <QDebug>
#include <QDesktopServices>
#include <QFileDialog>
#include <QJSEngine>
#include <QStandardPaths>

namespace QInstaller {

class ConsoleProxy : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(ConsoleProxy)

public:
    ConsoleProxy() {}

public slots :
        void log(const QString &log) { qDebug() << log; }
};

class InstallerProxy : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(InstallerProxy)

public:
    InstallerProxy(QJSEngine *engine, PackageManagerCore *core)
        : m_engine(engine), m_core(core) {}

public slots :
    QJSValue componentByName(const QString &componentName) {
        if (m_core)
            return m_engine->newQObject(m_core->componentByName(componentName));
        return QJSValue();
    }

private:
    QJSEngine *m_engine;
    PackageManagerCore *m_core;
};

class QFileDialogProxy : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(QFileDialogProxy)

public:
    QFileDialogProxy() {}

public slots :
    QString getExistingDirectory(const QString &caption, const QString &dir) const {
        return QFileDialog::getExistingDirectory(0, caption, dir);
    }
    QString getOpenFileName(const QString &caption, const QString &dir, const QString &filter) const {
        return QFileDialog::getOpenFileName(0, caption, dir, filter);
    }
};

class QDesktopServicesProxy : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(QDesktopServicesProxy)

public:
    QDesktopServicesProxy() {}

public slots :
    bool openUrl(const QString &url) const {
        QString urlToOpen = url;
        urlToOpen.replace(QLatin1String("\\\\"), QLatin1String("/"));
        urlToOpen.replace(QLatin1String("\\"), QLatin1String("/"));
        return QDesktopServices::openUrl(QUrl::fromUserInput(urlToOpen));
    }
    QString displayName(qint32 location) const {
        return QStandardPaths::displayName(QStandardPaths::StandardLocation(location));
    }
    QString storageLocation(qint32 location) const {
        return QStandardPaths::writableLocation(QStandardPaths::StandardLocation(location));
    }
};

#if QT_VERSION < 0x050400
class QCoreApplicationProxy : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(QCoreApplicationProxy)

public:
    QCoreApplicationProxy() {}

public slots:
    QString qsTr(const QString &text = QString(), const QString &disambiguation = QString(), int n = -1) const
    {
        return QCoreApplication::translate(QCoreApplication::applicationName().toUtf8().constData(),
            text.toUtf8().constData(), disambiguation.toUtf8().constData(), n);
    }
};
#endif

} // namespace QInstaller

Q_DECLARE_METATYPE(QInstaller::ConsoleProxy*)
Q_DECLARE_METATYPE(QInstaller::InstallerProxy*)
Q_DECLARE_METATYPE(QInstaller::QFileDialogProxy*)
Q_DECLARE_METATYPE(QInstaller::QDesktopServicesProxy*)
#if QT_VERSION < 0x050400
Q_DECLARE_METATYPE(QInstaller::QCoreApplicationProxy*)
#endif

#endif // SCRIPTENGINE_H