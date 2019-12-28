#include "database.h"

Database::Database()
{
}

bool Database::connOpen(){
    this->db = QSqlDatabase::addDatabase("QSQLITE");
    this->db.setDatabaseName("/Users/clairekolln/Documents/UO/Past Terms/Winter 2019/330/cslifeguide/cslifeguide-330/cslg.db");

    if (!this->db.open())
    {
        qDebug() << "Database not open";
        return false;
    }
    else
    {
       qDebug() << "Connected. . .";
       return true;
    }

}

void Database::connClosed(){
    this->db.close();
    this->db.removeDatabase(QSqlDatabase::defaultConnection);
    qDebug() << "Database Closed";
}

Database::~Database()
{
}
