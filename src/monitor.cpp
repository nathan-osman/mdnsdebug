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
#include <string>

#include <qmdnsengine/dns.h>
#include <qmdnsengine/message.h>
#include <qmdnsengine/query.h>
#include <qmdnsengine/record.h>

#include "monitor.h"

Monitor::Monitor()
    : mStart(QDateTime::currentDateTime())
{
    connect(&mServer, &QMdnsEngine::Server::messageReceived, this, &Monitor::onMessageReceived);

    std::cout << "Initialization complete" << std::endl;
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
        }
    }

    std::cout << std::endl;
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

    std::cout << "\"" << query.name().constData() << "\"" << std::endl;
}

void Monitor::printRecord(const QMdnsEngine::Record &record) const
{
    std::string name = record.name().toStdString();
    std::string target = record.target().toStdString();

    switch (record.type()) {
    case QMdnsEngine::A:
    case QMdnsEngine::AAAA:
        std::cout << "    - address for \"" << name << "\" is "
                  << record.address().toString().toStdString() << std::endl;
        break;
    case QMdnsEngine::PTR:
        std::cout << "    - \"" << target << "\" provides \"" << name
                  << "\"" << std::endl;
        break;
    case QMdnsEngine::SRV:
        std::cout << "    - \"" << name << "\" is at \"" << target
                  << "\" port " << record.port() << std::endl;
        break;
    case QMdnsEngine::TXT:
        std::cout << "    - \"" << name << "\" has the following data:" << std::endl;
        for (auto i = record.attributes().constBegin(); i != record.attributes().constEnd(); ++i) {
            std::cout << "        - " << i.key().toStdString() << ": "
                      << i.value().toStdString() << std::endl;
        }
        break;
    default:
        std::cout << "    - [unknown]" << std::endl;
        break;
    }
}
