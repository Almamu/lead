/*

MIT License

Copyright (c) 2017 Noah Andreas

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/


#include "sensor.h"
#include <QEvent>
#include <QDebug>
#include <QProcess>


namespace Lead {


Sensor::Sensor(QRect rect, QString action)
{
    qDebug() << "lead::Sensor() " << rect.x() << "," << rect.y() << "," << rect.width() << "," << rect.height() << " : " << action;

    this->action = action;
    this->rect = rect;
}

Sensor::Sensor(int x, int y, int w, int h, QString action)
{
    this->rect = {x, y, w, h};

    qDebug() << "lead::Sensor() " << this->rect.x() << "," << this->rect.y() << "," << this->rect.width() << "," << this->rect.height() << " : " << action;

    this->action = action;
}

Sensor::~Sensor()
{}

bool
Sensor::check(QPoint& start, QPoint& end)
{
    bool collide = false;

    // top line
    collide = collide || this->checkLines(
        start.x(), start.y(),
        end.x(), end.y(),
        this->rect.x(), this->rect.y(),
        this->rect.x() + this->rect.width(), this->rect.y() + this->rect.height()
    );

    // right line
    collide = collide || this->checkLines(
        start.x(), start.y(),
        end.x(), end.y(),
        this->rect.x() + this->rect.width(), this->rect.y(),
        this->rect.x() + this->rect.width(), this->rect.y() + this->rect.height()
    );

    // bottom line
    collide = collide || this->checkLines(
        start.x(), start.y(),
        end.x(), end.y(),
        this->rect.x(), this->rect.y() + this->rect.height(),
        this->rect.x() + this->rect.width(), this->rect.y() + this->rect.height()
    );

    // left line
    collide = collide || this->checkLines(
        start.x(), start.y(),
        end.x(), end.y(),
        this->rect.x(), this->rect.y(),
        this->rect.x(), this->rect.y() + this->rect.height()
    );

    if (collide)
    {
        if (this->lastCheckStatus == false)
        {
            this->fire();
            this->lastCheckStatus = true;

            return true;
        }
    }
    else
    {
        this->lastCheckStatus = false;
    }

    return false;
}

bool
Sensor::checkLines(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
{
    int xcmp = x3 - x1, ycmp = y3 - y1;
    int xr = x2 - x1, yr = y2 - y1;
    int xs = x4 - x3, ys = y4 - y3;

    int cmpxr = xcmp * yr - ycmp * xr;
    int cmpxs = xcmp * ys - ycmp * xs;
    int rxs = xr * ys - yr * xs;

    if(cmpxr == 0)
    {
        return (
            ((x3 - x1 < 0) != (x3 - x2 < 0)) ||
            ((y3 - y1 < 0) != (y3 - y2 < 0))
        );
    }

    if(rxs == 0)
    {
        return false;
    }

    int rxsr = ((rxs > 0) ? rxs : -rxs);
    int t = cmpxs * rxsr;
    int u = cmpxr * rxsr;

    return (t >= 0) && (t <= 1) && (u >= 0) && (u <= 1);
}

void
Sensor::fire()
{
    qDebug() << "lead::Sensor::fire() " << this->rect.x() << ":" << this->rect.y() << " action: " << this->action;

    QProcess::startDetached(action);
}


} // namespace
