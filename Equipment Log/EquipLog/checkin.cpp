#include "checkin.h"
#include "database.h"
#include "ui_checkin.h"
#include <iostream>
#include "dash.cpp"

using namespace std;

CheckIn::CheckIn(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CheckIn)
{
    ui->setupUi(this);
}

CheckIn::~CheckIn()
{
    delete ui;

}

void CheckIn::on_checkin_item_b_clicked()
{
    QString item_name;
    int item_version,loan_time;
    loan_time = 0;
    item_name = ui->comboBox_names->currentText();
    item_version = ui->comboBox_vers->currentText().toInt();

    QSqlQuery qry;
    qry.prepare("select * from currently_out where equip_name= (:item_name) and equip_vers=(:item_version)");
    qry.bindValue(":item_name",item_name);
    qry.bindValue(":item_version",item_version);

    if(qry.exec()){

        if(qry.next()) {

          // get the record from currently_out table
          QString first = qry.value(0).toString();
          QString last = qry.value(1).toString();
          QString id = qry.value(2).toString();
          QString out = qry.value(5).toString();

          // get that item's loan time from the desk_equipment table
          QSqlQuery get_lt;
          get_lt.prepare("select loan_time from desk_equipment where equip_name = (:item_name) and equip_vers = (:item_version)");
          get_lt.bindValue(":item_name",item_name);
          get_lt.bindValue(":item_version",item_version);
          if (get_lt.exec()) {
            while (get_lt.next()) {
                loan_time = get_lt.value(0).toInt();
            }
          }
          else {
            qDebug() << "Loan time not assigned";
          }

          // delete from currently_out
          QSqlQuery qry2;
          qry2.prepare("delete from currently_out where equip_name = (:item_name) and equip_vers =(:item_version) and id = (:id)");
          qry2.bindValue(":item_name",item_name);
          qry2.bindValue(":item_version",item_version);
          qry2.bindValue(":id",id);

          if (qry2.exec()) {
              ui->stat_label->setText("Successfully Checked In: " + item_name + " from " + last + ", " + first + " ID:" + id);

              // check if the item is overdue
              if(dash->is_Overdue(out,loan_time)) {
                   ui->lab_od->setText("<font color:red> Item is Late! </font>");
              }
              else {
                  qDebug() << "Item is not overdue";
              }

              //update the desk_equipment table to set the is_out value to 0
              QSqlQuery update;
              update.prepare("update desk_equipment set is_out=0 where equip_name = (:name) and equip_vers = (:vers)");
              update.bindValue(":name",item_name);
              update.bindValue(":vers",item_version);
              update.exec();
              ui->comboBox_vers->clear();
              ui->comboBox_names->clear();
              ui->comboBox_types->clear();
              ui->comboBox_types->addItems({"Enter Item Type","movie","tech","game","rec","tool"});
          }

          else {
              qDebug()<<"CheckIn was unsuccessful";
              qDebug()<< qry2.lastError();
              ui->stat_label->setText("Check In Unsuccessful");
          }
        }
    }
    else {
        ui->stat_label->setText("Record not Found, try again.");
    }

}

void CheckIn::on_comboBox_types_currentTextChanged(const QString &arg1)
{
    QSqlQuery namefill;
    namefill.prepare("select equip_name from currently_out where type ='"+arg1+"'");
    if (namefill.exec()){
        ui->comboBox_names->clear();
        while(namefill.next()) {
            if (ui->comboBox_names->findText(namefill.value(0).toString()) == -1){
                QString n = namefill.value(0).toString();
                ui->comboBox_names->addItem(n);
            }
        }
    }
}

void CheckIn::on_comboBox_names_currentTextChanged(const QString &arg1)
{
    QSqlQuery verfill;
    verfill.prepare("select equip_vers from currently_out where equip_name = '"+arg1+"'");
    if (verfill.exec()) {
        ui->comboBox_vers->clear();
        while (verfill.next()) {
            QString v = verfill.value(0).toString();
            ui->comboBox_vers->addItem(v);
        }
    }
}

void CheckIn::on_pushButton_clicked()
{
    this->close();
};

