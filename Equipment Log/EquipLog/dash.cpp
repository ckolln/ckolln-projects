#include "dash.h"
#include "database.cpp"
#include "ui_dash.h"

Dash::Dash(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Dash)
{
    ui->setupUi(this);
    if(db.connOpen()) {
        ui->label->setText("Equipment Database Connected");
    };
}

Dash::~Dash()
{
    qDebug()<<"Closing Database, deleting UI";
    db.connClose();
    delete ui;
}

void Dash::on_pushButton_clicked()
{
    CheckIn checkin;
    checkin.setModal(true);
    checkin.exec();

}

void Dash::on_pushButton_2_clicked()
{
    CheckOut checkout;
    checkout.setModal(true);
    checkout.exec();
}

void Dash::on_tbl_button_clicked()
{
    // Display the table
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery qry;
    qry.prepare("select * from currently_out");
    qry.exec();
    model->setQuery(qry);
    ui->tableView->setModel(model);

    // Display the late items, with "Late Items" as the title in red
    ui->late_list->clear();
    QString lateMessage = "Late Items:";
    ui->late_list->addItem(lateMessage);
    ui->late_list->item(0)->setForeground(Qt::red);

    // Populate the list widget on main dash with overdue items
    QString name,out,vers;
    QSqlQuery late;
    late.prepare("select equip_name,equip_vers,dt_out from currently_out");
    if (late.exec()) {
        while(late.next()) {
            out = late.value(2).toString();
            name = late.value(0).toString();
            vers = late.value(1).toString();

            // query to grab loan time for item
            QSqlQuery lt;
            lt.prepare("select loan_time from desk_equipment where equip_name = (:name) and equip_vers = 1");
            lt.bindValue(":name",name);
            if (lt.exec()) {
                while(lt.next()) {
                    int loantime = lt.value(0).toInt();
                    if(is_Overdue(out,loantime)) {
                        ui->late_list->addItem(name + " - " +vers);
                    }
                }
            }
            else {
                qDebug() << "Loan time not grabbed";
                qDebug() << lt.lastError();
            }
        }
    }
    else {
        qDebug() << "Unable to populate list table";
        qDebug() << late.lastError();
    }


}

bool Dash::is_Overdue(QString dtout, int loan_time) {
    QDateTime in = QDateTime::currentDateTime();
    QDateTime out = QDateTime::fromString(dtout,"MM-dd-yyyy hh:mm AP");
    if (in > out.addSecs(3600*loan_time)) {
        return true;
    }
    return false;
}
