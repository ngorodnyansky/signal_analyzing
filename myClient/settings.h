#pragma once

#include <QSettings>

class Settings
{
public:
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
    void setRed(int r);
    void setGreen(int g);
    void setBlue(int b);
    void setSizeLine(int sizeLine);
    void setBackgroundColor(int backColor);
    void setRedPoints(int rP);
    void setGreenPoints(int gP);
    void setBluePoints(int bP);
    void setSizePoints(int sizePopints);
    void setAntialiasing(bool antialiasing);
    void setViewPoints(bool viewPoints);
private:
    int m_red=0, m_green=0, m_blue=0, m_size_line=1, m_background_color=1;
    int m_redPoints=0, m_greenPoints=0, m_bluePoints=0, m_size_points=4;
    bool m_antialiasing=1, m_viewPoints=1;
};
