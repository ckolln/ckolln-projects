#include "database.h"

Database::Database()
{
}

void Database::connClose(){
    this->db.close();
    this->db.removeDatabase(QSqlDatabase::defaultConnection);
    qDebug() << "Database Connection closed";
}
bool Database::connOpen(){
    // set the database path, add database, open database
    this->db = QSqlDatabase::addDatabase("QSQLITE");
//    QString path = QDir::currentPath();
//    qDebug() << path;

//    path = path + QString("/eq_log.db");
//    this->db.setDatabaseName(path);
    QString path = "./eq_log.db";
    this->db.setDatabaseName("./eq_log.db");
    if (!this->db.open()) {
        qDebug() << "Database Not Open";
        return false;
    }
    else {
        qDebug() << "Connected...";
        return true;
    }
}

Database::~Database()
{
   this->connClose();
}

