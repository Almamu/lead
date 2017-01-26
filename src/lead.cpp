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


#include "lead.h"
#include "sensor.h"
#include <QDebug>
#include <QScreen>
#include <QFileSystemWatcher>


Lead::Lead(int &argc, char** argv) :
    QApplication(argc, argv),
    settings("lead", "lead"),
    watcher()
{   
    loadScreens();
    watchSettings();
}


Lead::~Lead()
{
    qDeleteAll(sensors);
}


void
Lead::watchSettings()
{    
    watcher.addPath(settings.fileName());

    connect(&watcher, SIGNAL(fileChanged(QString)), this, SLOT(fileChanged(QString)));
}


void
Lead::fileChanged(QString fileName)
{
    qDebug() << "Lead::fileChanged() fileName: " << fileName;

    // this reloads the settings from the file
    settings.sync();

    // some editors dont update but delete/create the config file, in this 
    // case our watcher loses the file so we have to add it again
    watcher.addPath( fileName );

    // simply reload everything
    reloadScreens();
}


void
Lead::screenAdded(QScreen* screen)
{
    loadScreen(screen);
}


void
Lead::screenRemoved(QScreen* screen)
{
    // thats the easiest way
    reloadScreens();
}


void
Lead::reloadScreens()
{
    qDeleteAll(sensors);
    sensors.clear();

    loadScreens();
}


void
Lead::loadScreens()
{
    foreach (QScreen* screen, screens()) 
    {
        loadScreen(screen);
    }
}


void
Lead::loadScreen(QScreen* screen)
{
    qDebug() << "Lead::loadScreen() " << screen->name();

    QRect rec = screen->geometry();
    
    loadSensor(screen, "top", rec.width() / 3, 0, rec.width() / 3, 10);
    loadSensor(screen, "right", rec.width() - 1, rec.height() / 3, 1, rec.height() / 3);
    loadSensor(screen, "bottom", rec.width() / 3, rec.height() - 1, rec.width() / 3, 1);
    loadSensor(screen, "left", 0, rec.height() / 3, 1, rec.height() / 3);
    loadSensor(screen, "topLeft", 0, 0, 1, 1);
    loadSensor(screen, "topRight", rec.width() - 1, 0, 1, 1);
    loadSensor(screen, "bottomRight", rec.width() - 1, rec.height() - 1, 1, 1);
    loadSensor(screen, "bottomLeft", 0, rec.height() - 1, 1, 1);
}


void
Lead::loadSensor(QScreen* screen, QString name, int x, int y, int w, int h)
{

    QString key = screen->name() + "/" + name;


    if (!settings.contains(key) )
    {
        qDebug() << "Lead::loadSensor() key " << name << " not found";
        
        // restore missing key
        settings.setValue(key, QString());
        return;
    }

    if (settings.value(key).toString().isEmpty())
    {
        qDebug() << "Lead::loadSensor() key " << name << " is empty";
        return;
    }
    

    // create sensor and save in list so we can delete all sensors on delete

    sensors.append( new lead::Sensor(x, y, w, h, settings.value(key).toString()) );
}