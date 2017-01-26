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


namespace lead {


Sensor::Sensor(int x, int y, int w, int h, QString action):
    QWidget()
{
    qDebug() << "lead::Sensor() " << x << "," << y << "," << w << "," << h << " : " << action;

    this->action = action;    

    //setStyleSheet("background-color:red;");
    setGeometry(x, y, w, h);    
    setAttribute(Qt::WA_TranslucentBackground, true);    
    setWindowFlags(windowFlags() | Qt::X11BypassWindowManagerHint | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint );

    show();
}


Sensor::~Sensor()
{}


void
Sensor::enterEvent(QEvent * event)
{
    qDebug() << "lead::Sensor::enterEvent() " << this->x() << ":" << this->y() << " action: " << this->action;

    QProcess::startDetached(action);
}


} // namespace
