#ifndef CHECKIN_H
#define CHECKIN_H
#include <QDebug>
#include <QDialog>
#include <QDateTime>

class Dash;

namespace Ui {
class CheckIn;
}

class CheckIn : public QDialog
{
    Q_OBJECT

public:
    explicit CheckIn(QWidget *parent = nullptr);
    ~CheckIn();


private slots:
    void on_checkin_item_b_clicked();
    void on_comboBox_types_currentTextChanged(const QString &arg1);

    void on_comboBox_names_currentTextChanged(const QString &arg1);

    void on_pushButton_clicked();

private:
    Dash *dash;
    Ui::CheckIn *ui;
};

#endif // CHECKIN_H
