/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2017 Nathan Osman
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <iomanip>
#include <iostream>

#ifdef Q_OS_WIN
#  include <windows.h>
#endif

#include <QProcess>

#include <qmdnsengine/dns.h>
#include <qmdnsengine/message.h>
#include <qmdnsengine/query.h>
#include <qmdnsengine/record.h>

#include "monitor.h"

Monitor::Monitor()
    : mColor(false),
      mStart(QDateTime::currentDateTime())
{
#ifdef Q_OS_UNIX
    QProcess proc;
    proc.start("tput", QStringList{"colors"});
    proc.waitForFinished(-1);
    if (proc.readAll().trimmed().toInt() > 1) {
        mColor = true;
    }
#elif Q_OS_WIN
    mColor = true;
#endif

    connect(&mServer, &QMdnsEngine::Server::messageReceived, this, &Monitor::onMessageReceived);

    std::cout << "Initialization complete" << std::endl;
    std::cout << "  " << mStart.toString().toStdString() << std::endl;
    std::cout << "  color support? " << (mColor ? "yes" : "no") << std::endl;
    std::cout << std::endl;
}

void Monitor::onMessageReceived(const QMdnsEngine::Message &message)
{
    double msSince = mStart.msecsTo(QDateTime::currentDateTime());

    std::cout << "[" << std::setfill('0') << std::setw(10) << msSince / 1000
              << "] message (" << (message.isResponse() ? "response" : "query")
              << ") from " << message.address().toString().toStdString() << std::endl;

    if (message.queries().count()) {
        std::cout << "  " << message.queries().count() << " query(s):" << std::endl;
        foreach (const QMdnsEngine::Query &q, message.queries()) {
            printQuery(q);
        }
    }

    if (message.records().count()) {
        std::cout << "  " << message.records().count() << " record(s):" << std::endl;
        foreach (const QMdnsEngine::Record &r, message.records()) {
            printRecord(r);
            mCache.addRecord(r);
        }
    }

    std::cout << std::endl;
}

void Monitor::printColor(const std::string &text) const
{
#if Q_OS_WIN
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#endif

#ifdef Q_OS_UNIX
    std::cout << (mColor ? "\033[0;33m" : "\"");
#elif Q_OS_WIN
    SetConsoleTextAttribute(hConsole, 14);
#endif

    std::cout << text;

#ifdef Q_OS_UNIX
    std::cout << (mColor ? "\033[0m" : "\"");
#elif Q_OS_WIN
    SetConsoleTextAttribute(hConsole, 0);
#endif
}

void Monitor::printQuery(const QMdnsEngine::Query &query) const
{
    switch (query.type()) {
    case QMdnsEngine::A:
    case QMdnsEngine::AAAA:
        std::cout << "    - " << (query.type() == QMdnsEngine::A ? "IPv4" : "IPv6")
                  << " address for ";
        break;
    case QMdnsEngine::ANY:
        std::cout << "    - probing for a record named ";
        break;
    case QMdnsEngine::PTR:
        std::cout << "    - services providing ";
        break;
    case QMdnsEngine::SRV:
        std::cout << "    - service information for ";
        break;
    case QMdnsEngine::TXT:
        std::cout << "    - TXT record for ";
        break;
    default:
        std::cout << "    - [unknown]" << std::endl;
        return;
    }

    printColor(query.name().toStdString());
    std::cout << std::endl;
}

void Monitor::printRecord(const QMdnsEngine::Record &record) const
{
    std::string name = record.name().toStdString();
    std::string target = record.target().toStdString();

    switch (record.type()) {
    case QMdnsEngine::A:
    case QMdnsEngine::AAAA:
        std::cout << "    - address for ";
        printColor(name);
        std::cout << " is " << record.address().toString().toStdString() << std::endl;
        break;
    case QMdnsEngine::PTR:
        std::cout << "    - ";
        printColor(target);
        std::cout << " provides ";
        printColor(name);
        std::cout << std::endl;
        break;
    case QMdnsEngine::SRV:
        std::cout << "    - ";
        printColor(name);
        std::cout << " is at ";
        printColor(target);
        std::cout << " port ";
        printColor(std::to_string(record.port()));
        std::cout << std::endl;
        break;
    case QMdnsEngine::TXT:
        std::cout << "    - ";
        printColor(name);
        std::cout << " has the following data:" << std::endl;
        for (auto i = record.attributes().constBegin(); i != record.attributes().constEnd(); ++i) {
            std::cout << "        - ";
            printColor(i.key().toStdString());
            std::cout << ": ";
            printColor(i.value().toStdString());
            std::cout << std::endl;
        }
        break;
    default:
        std::cout << "    - [unknown]" << std::endl;
        break;
    }
}
