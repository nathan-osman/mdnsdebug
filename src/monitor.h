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

#ifndef MONITOR_H
#define MONITOR_H

#include <string>

#include <QDateTime>
#include <QObject>

#include <qmdnsengine/cache.h>
#include <qmdnsengine/server.h>

namespace QMdnsEngine
{
class Query;
class Record;
}

class Monitor : public QObject
{
    Q_OBJECT

public:

    Monitor();

private Q_SLOTS:

    void onMessageReceived(const QMdnsEngine::Message &message);

private:

    void printColor(const std::string &text) const;
    void printQuery(const QMdnsEngine::Query &query) const;
    void printRecord(const QMdnsEngine::Record &record) const;

    bool mColor;

    QDateTime mStart;

    QMdnsEngine::Server mServer;
    QMdnsEngine::Cache mCache;
};

#endif // MONITOR_H
