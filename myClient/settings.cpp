#include "settings.h"


Settings::Settings()
{
    m_lineColor.setRgb(0,0,0),m_pointColor.setRgb(0,0,0);
    m_size_line=1, m_background_color=1;
    m_size_points=4;
    m_antialiasing=1, m_viewPoints=1;
}
void Settings::writeSettings()
{
    QSettings settings("Setings.ini",QSettings::IniFormat);
    settings.beginGroup("/Settings");

        settings.setValue("/redColorLine",m_lineColor.red());
        settings.setValue("/greenColorLine",m_lineColor.green());
        settings.setValue("/blueColorLine",m_lineColor.blue());
        settings.setValue("/sizeLine",m_size_line);
        settings.setValue("/antialiasing",m_antialiasing);
        settings.setValue("/redPointsColorLine",m_pointColor.red());
        settings.setValue("/greenPointsColorLine",m_pointColor.green());
        settings.setValue("/bluePointsColorLine",m_pointColor.blue());
        settings.setValue("/sizePoints",m_size_points);
        settings.setValue("/pointsView",m_viewPoints);
        settings.setValue("/antialiasing",m_antialiasing);
        settings.setValue("/backgroundColor",m_background_color);

        settings.endGroup();
}

int Settings::getRed() const
{
    return m_lineColor.red();
}

int Settings::getGreen() const
{
    return m_lineColor.green();
}

int Settings::getBlue() const
{
    return m_lineColor.blue();
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
    return m_pointColor.red();
}

int Settings::getGreenPoints() const
{
    return m_pointColor.green();
}

int Settings::getBluePoints() const
{
    return m_pointColor.blue();
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

void Settings::setLineColor(int r, int g, int b)
{
    m_lineColor.setRgb(r,g,b);
}

void Settings::setPointColor(int r, int g, int b)
{
    m_pointColor.setRgb(r,g,b);
}

void Settings::setSizeLine(int sizeLine)
{
    m_size_line=sizeLine;
}

void Settings::setBackgroundColor(int backColor)
{
    m_background_color=backColor;
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

QColor Settings::getLineColor() const
{
    return m_lineColor;
}

QColor Settings::getPointColor() const
{
    return m_pointColor;
}


void Settings::readSettings()
{
    QSettings settings("Setings.ini",QSettings::IniFormat);
    settings.beginGroup("/Settings");

        m_lineColor.setRgb(settings.value("/redColorLine",0).toInt(), settings.value("/greenColorLine",0).toInt(),settings.value("/blueColorLine",0).toInt());
        m_pointColor.setRgb(settings.value("/redPointsColorLine",0).toInt(), settings.value("/greenPointsColorLine",0).toInt(),settings.value("/bluePointsColorLine",0).toInt());
        m_size_line = settings.value("/sizeLine",1).toInt();
        m_antialiasing = settings.value("/antialiasing",1).toBool();
        m_size_points = settings.value("/sizePoints",4).toInt();
        m_viewPoints = settings.value("/pointsView",1).toBool();
        m_background_color = settings.value("/backgroundColor",1).toInt();

    settings.endGroup();
}

