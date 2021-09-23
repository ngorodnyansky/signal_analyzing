#pragma once

#include <QColor>
#include <QSettings>

class Settings
{
public:
    Settings();
    void readSettings();
    void writeSettings();
    int getRed() const;
    int getGreen() const;
    int getBlue() const;
    int getSizeLine() const;
    int getBackgroundColor() const;
    int getRedPoints() const;
    int getGreenPoints() const;
    int getBluePoints() const;
    int getSizePoints() const;
    bool getAntialiasing() const;
    bool getViewPoints() const;
    void setLineColor(int r,int g, int b);
    void setPointColor(int r,int g,int b);
    void setSizeLine(int sizeLine);
    void setBackgroundColor(int backColor);
    void setSizePoints(int sizePopints);
    void setAntialiasing(bool antialiasing);
    void setViewPoints(bool viewPoints);
    QColor getLineColor() const;
    QColor getPointColor() const;
private:
    QColor m_lineColor,m_pointColor;
    int  m_size_line, m_background_color, m_size_points;
    bool m_antialiasing, m_viewPoints;
};
