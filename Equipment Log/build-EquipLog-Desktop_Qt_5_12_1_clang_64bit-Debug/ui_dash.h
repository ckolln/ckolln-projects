/********************************************************************************
** Form generated from reading UI file 'dash.ui'
**
** Created by: Qt User Interface Compiler version 5.12.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DASH_H
#define UI_DASH_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Dash
{
public:
    QWidget *centralWidget;
    QLabel *label;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QTableView *tableView;
    QPushButton *tbl_button;
    QListWidget *late_list;
    QMenuBar *menuBar;
    QMenu *menuEquipment_Log;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Dash)
    {
        if (Dash->objectName().isEmpty())
            Dash->setObjectName(QString::fromUtf8("Dash"));
        Dash->resize(730, 552);
        centralWidget = new QWidget(Dash);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(40, 440, 491, 41));
        label->setStyleSheet(QString::fromUtf8("QLabel {color: green}"));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(30, 360, 141, 71));
        QFont font;
        font.setFamily(QString::fromUtf8("Sana"));
        font.setPointSize(19);
        font.setBold(true);
        font.setWeight(75);
        pushButton->setFont(font);
        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(180, 360, 141, 71));
        pushButton_2->setFont(font);
        tableView = new QTableView(centralWidget);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        tableView->setGeometry(QRect(20, 20, 691, 321));
        tbl_button = new QPushButton(centralWidget);
        tbl_button->setObjectName(QString::fromUtf8("tbl_button"));
        tbl_button->setGeometry(QRect(340, 360, 161, 31));
        late_list = new QListWidget(centralWidget);
        late_list->setObjectName(QString::fromUtf8("late_list"));
        late_list->setGeometry(QRect(510, 350, 201, 131));
        Dash->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(Dash);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 730, 22));
        menuEquipment_Log = new QMenu(menuBar);
        menuEquipment_Log->setObjectName(QString::fromUtf8("menuEquipment_Log"));
        Dash->setMenuBar(menuBar);
        mainToolBar = new QToolBar(Dash);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        Dash->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(Dash);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        Dash->setStatusBar(statusBar);

        menuBar->addAction(menuEquipment_Log->menuAction());

        retranslateUi(Dash);

        QMetaObject::connectSlotsByName(Dash);
    } // setupUi

    void retranslateUi(QMainWindow *Dash)
    {
        Dash->setWindowTitle(QApplication::translate("Dash", "Dash", nullptr));
        label->setText(QApplication::translate("Dash", "Status:", nullptr));
        pushButton->setText(QApplication::translate("Dash", "Check In", nullptr));
        pushButton_2->setText(QApplication::translate("Dash", "Check Out", nullptr));
        tbl_button->setText(QApplication::translate("Dash", "Load/ReLoad ", nullptr));
        menuEquipment_Log->setTitle(QApplication::translate("Dash", "Equipment Log", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dash: public Ui_Dash {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DASH_H
