#pragma once
#include "viewsetting.h"
#include <QSettings>

class Settings
{
public:
    void readSettings(viewSetting& data);
    void writeSettings(const viewSetting& data);
};
