#ifndef VIEWSETTING_H
#define VIEWSETTING_H

#include <QDialog>

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

private slots:
    void on_cancel_clicked();

    void on_apply_clicked();

private:
    Ui::viewSetting *ui;
};

#endif // VIEWSETTING_H
