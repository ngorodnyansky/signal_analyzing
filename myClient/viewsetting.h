#pragma once

#include <QDialog>
#include <QSettings>
#include "settings.h"

namespace Ui {
class viewSetting;
}

class viewSetting : public QDialog
{
    Q_OBJECT

public:
    explicit viewSetting(Settings& settings,QWidget *parent = nullptr);
    ~viewSetting();
    void updateValue(Settings& settings);
public slots:
    void on_cancel_clicked();
    void on_apply_clicked();

private:
    Ui::viewSetting *ui;
    Settings& settings;

};
