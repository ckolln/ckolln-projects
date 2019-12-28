#pragma once
#ifndef DASH_H
#define DASH_H
#include <QtSql>
#include <QMainWindow>
#include <QtDebug>
#include "checkin.h"
#include "checkout.h"
#include "database.h"

namespace Ui {
class Dash;
}

class Dash : public QMainWindow
{
    Q_OBJECT

public:
    explicit Dash(QWidget *parent = nullptr);
    ~Dash();
    Database db;
    bool is_Overdue(QString t_out, int loan_time);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_tbl_button_clicked();

private:
    Ui::Dash *ui;
};

#endif // DASH_H
