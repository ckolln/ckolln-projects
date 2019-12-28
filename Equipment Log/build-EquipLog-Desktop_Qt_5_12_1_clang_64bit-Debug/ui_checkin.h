/********************************************************************************
** Form generated from reading UI file 'checkin.ui'
**
** Created by: Qt User Interface Compiler version 5.12.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHECKIN_H
#define UI_CHECKIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CheckIn
{
public:
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QHBoxLayout *horizontalLayout;
    QLabel *label_name;
    QLabel *label_vers;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout_2;
    QComboBox *comboBox_types;
    QComboBox *comboBox_names;
    QComboBox *comboBox_vers;
    QPushButton *checkin_item_b;
    QWidget *layoutWidget2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *stat_label;
    QPushButton *pushButton;
    QLabel *lab_od;

    void setupUi(QDialog *CheckIn)
    {
        if (CheckIn->objectName().isEmpty())
            CheckIn->setObjectName(QString::fromUtf8("CheckIn"));
        CheckIn->resize(605, 415);
        layoutWidget = new QWidget(CheckIn);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(120, 70, 147, 101));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setPointSize(17);
        label->setFont(font);

        verticalLayout->addWidget(label);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_name = new QLabel(layoutWidget);
        label_name->setObjectName(QString::fromUtf8("label_name"));
        label_name->setFont(font);

        horizontalLayout->addWidget(label_name);


        verticalLayout->addLayout(horizontalLayout);

        label_vers = new QLabel(layoutWidget);
        label_vers->setObjectName(QString::fromUtf8("label_vers"));
        label_vers->setFont(font);

        verticalLayout->addWidget(label_vers);

        layoutWidget1 = new QWidget(CheckIn);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(280, 70, 211, 146));
        verticalLayout_2 = new QVBoxLayout(layoutWidget1);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        comboBox_types = new QComboBox(layoutWidget1);
        comboBox_types->addItem(QString());
        comboBox_types->addItem(QString());
        comboBox_types->addItem(QString());
        comboBox_types->addItem(QString());
        comboBox_types->addItem(QString());
        comboBox_types->addItem(QString());
        comboBox_types->setObjectName(QString::fromUtf8("comboBox_types"));
        QFont font1;
        font1.setPointSize(13);
        comboBox_types->setFont(font1);

        verticalLayout_2->addWidget(comboBox_types);

        comboBox_names = new QComboBox(layoutWidget1);
        comboBox_names->addItem(QString());
        comboBox_names->setObjectName(QString::fromUtf8("comboBox_names"));

        verticalLayout_2->addWidget(comboBox_names);

        comboBox_vers = new QComboBox(layoutWidget1);
        comboBox_vers->addItem(QString());
        comboBox_vers->setObjectName(QString::fromUtf8("comboBox_vers"));

        verticalLayout_2->addWidget(comboBox_vers);

        checkin_item_b = new QPushButton(layoutWidget1);
        checkin_item_b->setObjectName(QString::fromUtf8("checkin_item_b"));
        checkin_item_b->setFont(font);

        verticalLayout_2->addWidget(checkin_item_b);

        layoutWidget2 = new QWidget(CheckIn);
        layoutWidget2->setObjectName(QString::fromUtf8("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(10, 230, 571, 111));
        horizontalLayout_3 = new QHBoxLayout(layoutWidget2);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        stat_label = new QLabel(layoutWidget2);
        stat_label->setObjectName(QString::fromUtf8("stat_label"));
        QFont font2;
        font2.setPointSize(15);
        stat_label->setFont(font2);

        horizontalLayout_3->addWidget(stat_label);

        pushButton = new QPushButton(CheckIn);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(10, 10, 111, 32));
        lab_od = new QLabel(CheckIn);
        lab_od->setObjectName(QString::fromUtf8("lab_od"));
        lab_od->setGeometry(QRect(20, 360, 561, 31));

        retranslateUi(CheckIn);

        QMetaObject::connectSlotsByName(CheckIn);
    } // setupUi

    void retranslateUi(QDialog *CheckIn)
    {
        CheckIn->setWindowTitle(QApplication::translate("CheckIn", "Dialog", nullptr));
        label->setText(QApplication::translate("CheckIn", "Item Type:", nullptr));
        label_name->setText(QApplication::translate("CheckIn", "Item Name:", nullptr));
        label_vers->setText(QApplication::translate("CheckIn", "Item Version:", nullptr));
        comboBox_types->setItemText(0, QApplication::translate("CheckIn", "Enter Item Type", nullptr));
        comboBox_types->setItemText(1, QApplication::translate("CheckIn", "movie", nullptr));
        comboBox_types->setItemText(2, QApplication::translate("CheckIn", "tech", nullptr));
        comboBox_types->setItemText(3, QApplication::translate("CheckIn", "game", nullptr));
        comboBox_types->setItemText(4, QApplication::translate("CheckIn", "rec", nullptr));
        comboBox_types->setItemText(5, QApplication::translate("CheckIn", "tool", nullptr));

        comboBox_names->setItemText(0, QApplication::translate("CheckIn", "Item Name", nullptr));

        comboBox_vers->setItemText(0, QApplication::translate("CheckIn", "Item Version", nullptr));

        checkin_item_b->setText(QApplication::translate("CheckIn", "Check In", nullptr));
        stat_label->setText(QApplication::translate("CheckIn", "Status: ", nullptr));
        pushButton->setText(QApplication::translate("CheckIn", "Back to Main", nullptr));
        lab_od->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class CheckIn: public Ui_CheckIn {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHECKIN_H
