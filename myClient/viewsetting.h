#pragma once

#include <QDialog>
#include <QSettings>

namespace Ui {
class viewSetting;
}

class viewSetting : public QDialog
{
    Q_OBJECT

public:
    explicit viewSetting(QWidget *parent = nullptr);
    ~viewSetting();
    int red=0, green=0, blue=0, size_line=1, background_color=1;
    int redPoints=0, greenPoints=0, bluePoints=0, size_points=4;
    bool antialiasing=1, viewPoints=1;
    void updateValue();
private slots:
    void on_cancel_clicked();
    void on_apply_clicked();

private:
    Ui::viewSetting *ui;
};
