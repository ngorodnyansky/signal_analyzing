#include "settings.h"

void Settings::writeSettings()
{
    QSettings settings("Setings.ini",QSettings::IniFormat);
    settings.beginGroup("/Settings");

        settings.setValue("/redColorLine",m_red);
        settings.setValue("/greenColorLine",m_green);
        settings.setValue("/blueColorLine",m_blue);
        settings.setValue("/sizeLine",m_size_line);
        settings.setValue("/antialiasing",m_antialiasing);
        settings.setValue("/redPointsColorLine",m_redPoints);
        settings.setValue("/greenPointsColorLine",m_greenPoints);
        settings.setValue("/bluePointsColorLine",m_bluePoints);
        settings.setValue("/sizePoints",m_size_points);
        settings.setValue("/pointsView",m_viewPoints);
        settings.setValue("/antialiasing",m_antialiasing);
        settings.setValue("/backgroundColor",m_background_color);

        settings.endGroup();
}

int Settings::getRed() const
{
    return m_red;
}

int Settings::getGreen() const
{
    return m_green;
}

int Settings::getBlue() const
{
    return m_blue;
}

int Settings::getSizeLine() const
{
    return m_size_line;
}

int Settings::getBackgroundColor() const
{
    return m_background_color;
}

int Settings::getRedPoints() const
{
    return m_redPoints;
}

int Settings::getGreenPoints() const
{
    return m_greenPoints;
}

int Settings::getBluePoints() const
{
    return m_bluePoints;
}

int Settings::getSizePoints() const
{
    return m_size_points;
}

bool Settings::getAntialiasing() const
{
    return m_antialiasing;
}

bool Settings::getViewPoints() const
{
    return m_viewPoints;
}

void Settings::setRed(int r)
{
    m_red=r;
}

void Settings::setGreen(int g)
{
    m_green=g;
}

void Settings::setBlue(int b)
{
    m_blue=b;
}

void Settings::setSizeLine(int sizeLine)
{
    m_size_line=sizeLine;
}

void Settings::setBackgroundColor(int backColor)
{
    m_background_color=backColor;
}

void Settings::setRedPoints(int rP)
{
    m_redPoints=rP;
}

void Settings::setGreenPoints(int gP)
{
    m_greenPoints=gP;
}

void Settings::setBluePoints(int bP)
{
    m_bluePoints=bP;
}

void Settings::setSizePoints(int sizePoints)
{
    m_size_points=sizePoints;
}

void Settings::setAntialiasing(bool antialiasing)
{
    m_antialiasing=antialiasing;
}

void Settings::setViewPoints(bool viewPoints)
{
    m_viewPoints=viewPoints;
}

void Settings::readSettings()
{
    QSettings settings("Setings.ini",QSettings::IniFormat);
    settings.beginGroup("/Settings");

        m_red = settings.value("/redColorLine",0).toInt();
        m_green = settings.value("/greenColorLine",0).toInt();
        m_blue = settings.value("/blueColorLine",0).toInt();
        m_size_line = settings.value("/sizeLine",1).toInt();
        m_antialiasing = settings.value("/antialiasing",1).toBool();
        m_redPoints = settings.value("/redPointsColorLine",0).toInt();
        m_greenPoints = settings.value("/greenPointsColorLine",0).toInt();
        m_bluePoints = settings.value("/bluePointsColorLine",0).toInt();
        m_size_points = settings.value("/sizePoints",4).toInt();
        m_viewPoints = settings.value("/pointsView",1).toBool();
        m_background_color = settings.value("/backgroundColor",1).toInt();

    settings.endGroup();
}

