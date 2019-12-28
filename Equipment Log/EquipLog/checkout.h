#ifndef CHECKOUT_H
#define CHECKOUT_H
#include <QDialog>

namespace Ui {
class CheckOut;
}

class CheckOut : public QDialog
{
    Q_OBJECT

public:
    explicit CheckOut(QWidget *parent = nullptr);
    ~CheckOut();


private slots:
    void on_checkout_b_clicked();

    void on_comboBox_type_currentTextChanged(const QString &arg1);

    void on_comboBox_name_currentTextChanged(const QString &arg1);

    void on_pushButton_clicked();

private:
    Ui::CheckOut *ui;
};

#endif // CHECKOUT_H
