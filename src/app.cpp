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


#include "app.h"
#include "sensor.h"
#include <QDebug>
#include <QScreen>
#include <QFileSystemWatcher>

#define SENSOR_WIDTH 5
#define SENSOR_HEIGHT 5
#define POLL_RATE 10

namespace Lead {


App::App(int &argc, char** argv) :
    QApplication(argc, argv),
    settings("lead", "lead"),
    watcher(),
    pollTimer(this)
{   
    loadScreens();
    watchSettings();

    // bind timer
    connect(&pollTimer, SIGNAL(timeout()), this, SLOT(pollMouse()));
}


App::~App()
{
    qDeleteAll(sensors);
}


void
App::watchSettings()
{    
    watcher.addPath(settings.fileName());

    connect(&watcher, SIGNAL(fileChanged(QString)), this, SLOT(fileChanged(QString)));
}


void
App::fileChanged(QString fileName)
{
    qDebug() << "App::fileChanged() fileName: " << fileName;

    // this reloads the settings from the file
    settings.sync();

    // some editors dont update but delete/create the config file, in this 
    // case our watcher loses the file so we have to add it again
    watcher.addPath( fileName );

    // simply reload everything
    reloadScreens();
}

void
App::pollMouse()
{
    lastMousePosition = currentMousePosition;
    currentMousePosition = QCursor::pos();

    if(
        currentMousePosition.x() > (1080 + 1800) && currentMousePosition.x() < (1080 + 1920 + 150) &&
        currentMousePosition.y() > 300 && currentMousePosition.y() < 380
        )
    {
        qDebug() << lastMousePosition.x() << "," << lastMousePosition.y() << ":" << currentMousePosition.x() << "," << currentMousePosition.y();
    }

    if(lastMousePosition.x() != currentMousePosition.x() || lastMousePosition.y() != currentMousePosition.y())
    {
        // check if the mouse movement intersects with the hot corners we're supporting
        foreach(Sensor* sensor, sensors)
        {
            if(sensor->check(lastMousePosition, currentMousePosition) == true)
            {
                qDebug() << "Sensor fired!";
            }
        }
    }
}


void
App::screenAdded(QScreen* screen)
{
    loadScreen(screen);
}


void
App::screenRemoved(QScreen* screen)
{
    // thats the easiest way
    reloadScreens();
}


void
App::reloadScreens()
{
    qDeleteAll(sensors);
    sensors.clear();

    loadScreens();
}


void
App::loadScreens()
{
    foreach (QScreen* screen, screens()) 
    {
        loadScreen(screen);
    }

    loadTimer();
}


void
App::loadScreen(QScreen* screen)
{
    qDebug() << "App::loadScreen() " << screen->name();

    QRect rec = screen->geometry();
    
    loadSensor(screen, "top", rec.x() + (rec.width() / 3), rec.y(), rec.width() / 3, SENSOR_WIDTH);
    loadSensor(screen, "right", rec.x() + rec.width() - SENSOR_WIDTH, rec.y() + (rec.height() / 3), SENSOR_WIDTH, rec.height() / 3);
    loadSensor(screen, "bottom", rec.x() + (rec.width() / 3), rec.y() + rec.height() - SENSOR_HEIGHT, rec.width() / 3, SENSOR_HEIGHT);
    loadSensor(screen, "left", 0, rec.x() + (rec.height() / 3), SENSOR_WIDTH, rec.height() / 3);
    loadSensor(screen, "topLeft", rec.x(), rec.y(), SENSOR_WIDTH, SENSOR_HEIGHT);
    loadSensor(screen, "topRight", rec.x() + rec.width() - SENSOR_WIDTH, rec.y(), SENSOR_WIDTH, SENSOR_HEIGHT);
    loadSensor(screen, "bottomRight", rec.x() + rec.width() - SENSOR_WIDTH, rec.y() + rec.height() - SENSOR_HEIGHT, SENSOR_WIDTH, SENSOR_HEIGHT);
    loadSensor(screen, "bottomLeft", rec.x(), rec.y() + rec.height() - SENSOR_HEIGHT, SENSOR_WIDTH, SENSOR_HEIGHT);
}


void
App::loadSensor(QScreen* screen, QString name, int x, int y, int w, int h)
{

    QString key = screen->name() + "/" + name;


    if (!settings.contains(key) )
    {
        qDebug() << "App::loadSensor() key " << name << " not found";
        
        // restore missing key
        settings.setValue(key, QString());
        return;
    }

    if (settings.value(key).toString().isEmpty())
    {
        qDebug() << "App::loadSensor() key " << name << " is empty";
        return;
    }
    

    // create sensor and save in list so we can delete all sensors on delete

    sensors.append( new Sensor(x, y, w, h, settings.value(key).toString()) );
}

void
App::loadTimer()
{
    int interval = POLL_RATE;

    if (!settings.contains("general/timer") )
    {
        qDebug() << "App::loadTimer() setting default timer msec to " << POLL_RATE << " msecs";

        settings.setValue("general/timer", QString::number(POLL_RATE));
    }
    else if (settings.value("general/timer").toInt() <= 0)
    {
        qDebug() << "App::loadTimer() mouse poll rate too fast, adjusting to 10 ms again";
    }
    else
    {
        interval = settings.value("general/timer").toInt();
        qDebug() << "App::loadTimer() mouse poll rate set to " << interval;
    }

    // set timer interval
    this->pollTimer.setInterval(interval);
    this->pollTimer.setSingleShot(false);

    // run the timer if it isn't running already
    if(this->pollTimer.isActive() == false)
    {
        this->pollTimer.start();
    }
}


} // namespace