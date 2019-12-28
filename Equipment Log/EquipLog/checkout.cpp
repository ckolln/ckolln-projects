#include "checkout.h"
#include "ui_checkout.h"
#include "database.h"
#include <iostream>
#include <QDateTime>

using namespace std;

CheckOut::CheckOut(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CheckOut)
{
    ui->setupUi(this);
}

CheckOut::~CheckOut()
{
    delete ui;
}

void CheckOut::on_checkout_b_clicked()
{
    //ui->co_status->clear();
    ui->co_status_2->clear();
    ui->co_status_3->clear();


    // get inputs from ui
    QString fname, lname, equip_name,equip_type,id, evers;
    int equip_vers;
    bool can_checkout = true;
    fname = ui->lineEdit_fn->text();
    lname = ui->lineEdit_ln->text();
    id = ui->lineEdit_id->text();
    equip_type = ui->comboBox_type->currentText();
    equip_name = ui->comboBox_name->currentText();
    evers = ui->comboBox_vers->currentText();
    equip_vers = ui->comboBox_vers->currentText().toInt();

    // check to see if the input are of valid length / form
    if (equip_type == "Enter Item Type" or equip_name == "" or equip_vers == 0) {
        ui->co_status_3->setText("<font color='red'> Unsuccessful Checkout, Empty item fields </font>");
        can_checkout = false;
    }
    if (evers == "None Available") {
        ui->co_status_3->setText("<font color='red'> No items of name: " + equip_name + " currently available </red>");
    }
    if (id.length() != 9 or fname == "" or lname == "") {
        ui->co_status_2->setText("<font color='red'> Unsuccessful Checkout, please enter valid Resident Info</font>");
        can_checkout = false;
    }


    // get the date and time for the check out, then put into string format for table view
    QDateTime dt = QDateTime::currentDateTime();
    QString dts = dt.toString("MM-dd-yyyy hh:mm AP");


    if (can_checkout){
        // prepare checkout query, add to currently_out table with user info
        QSqlQuery co;
        co.prepare("INSERT INTO currently_out(fname,lname,id,equip_name,equip_vers,dt_out,type) values(:fname,:lname,:id,:equip_name,:equip_vers,:time,:equip_type)");
        co.bindValue(":fname",fname);
        co.bindValue(":lname",lname);
        co.bindValue(":id",id);
        co.bindValue(":equip_name",equip_name);
        co.bindValue(":equip_vers",equip_vers);
        co.bindValue(":equip_type",equip_type);
        co.bindValue(":time",dts);
        co.exec();

        //change is_out val in desk_equipment to 1
        QSqlQuery update;
        update.prepare("update desk_equipment set is_out=1 where equip_name = (:name) and equip_vers = (:vers)");
        update.bindValue(":name",equip_name);
        update.bindValue(":vers",equip_vers);
        update.exec();

        // clear status Labels from possible previous error messages
        ui->co_status_2->clear();
        ui->co_status_3->clear();
        ui->co_status->setText("<font color='green'>Successfully checked out " + equip_name + " to " + lname + ", " + fname + "</font>");

        // clear comboBoxes so another item can be checked out, EX: multiple ping pong paddles to one user
        // repopulates comboboxes only with items that are available
        ui->comboBox_type->clear();
        ui->comboBox_name->clear();
        ui->comboBox_vers->clear();
        ui->comboBox_type->addItems({"Enter Item Type","movie","tool","rec","tech","game"});

    }
    else {
       ui->co_status->setText("<font color='red'> Checkout Unsuccessful... please try again</font>");
    }

}

void CheckOut::on_comboBox_type_currentTextChanged(const QString &arg1)
{
    QSqlQuery namefill;
    namefill.prepare("select equip_name from desk_equipment where equip_type ='"+arg1+"' and equip_vers = 1");
    if (namefill.exec()){
        ui->comboBox_name->clear();
        while(namefill.next()) {
            QString n = namefill.value(0).toString();
            ui->comboBox_name->addItem(n);
            }
    }
}

void CheckOut::on_comboBox_name_currentTextChanged(const QString &arg1)
{
    QSqlQuery verfill;
    verfill.prepare("select equip_vers from desk_equipment where equip_name = '"+arg1+"' and is_out= 0");
    if (verfill.exec()) {
        ui->comboBox_vers->clear();
        while (verfill.next()) {
            QString v = verfill.value(0).toString();
            ui->comboBox_vers->addItem(v);
        }
        if (ui->comboBox_vers->currentText() == "") {
            ui->comboBox_vers->addItem("None Available");
        }
    }
    else {
        qDebug() << "Unable to query versions";
        qDebug() << verfill.lastError();
    }
}

void CheckOut::on_pushButton_clicked()
{
    this->hide();
}
