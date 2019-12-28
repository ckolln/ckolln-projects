/********************************************************************************
** Form generated from reading UI file 'checkout.ui'
**
** Created by: Qt User Interface Compiler version 5.12.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHECKOUT_H
#define UI_CHECKOUT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CheckOut
{
public:
    QPushButton *checkout_b;
    QSplitter *splitter;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QLabel *label_5;
    QLabel *label;
    QLabel *label_6;
    QLabel *label_4;
    QLabel *label_3;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout;
    QLineEdit *lineEdit_fn;
    QLineEdit *lineEdit_ln;
    QLineEdit *lineEdit_id;
    QComboBox *comboBox_type;
    QComboBox *comboBox_name;
    QComboBox *comboBox_vers;
    QPushButton *pushButton;
    QWidget *widget;
    QVBoxLayout *verticalLayout_3;
    QLabel *co_status_2;
    QLabel *co_status_3;
    QLabel *co_status;

    void setupUi(QDialog *CheckOut)
    {
        if (CheckOut->objectName().isEmpty())
            CheckOut->setObjectName(QString::fromUtf8("CheckOut"));
        CheckOut->resize(371, 405);
        checkout_b = new QPushButton(CheckOut);
        checkout_b->setObjectName(QString::fromUtf8("checkout_b"));
        checkout_b->setGeometry(QRect(120, 200, 111, 32));
        splitter = new QSplitter(CheckOut);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setGeometry(QRect(60, 10, 251, 181));
        splitter->setOrientation(Qt::Horizontal);
        layoutWidget = new QWidget(splitter);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        verticalLayout_2 = new QVBoxLayout(layoutWidget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout_2->addWidget(label_2);

        label_5 = new QLabel(layoutWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        verticalLayout_2->addWidget(label_5);

        label = new QLabel(layoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout_2->addWidget(label);

        label_6 = new QLabel(layoutWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        verticalLayout_2->addWidget(label_6);

        label_4 = new QLabel(layoutWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        verticalLayout_2->addWidget(label_4);

        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        verticalLayout_2->addWidget(label_3);

        splitter->addWidget(layoutWidget);
        layoutWidget1 = new QWidget(splitter);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        verticalLayout = new QVBoxLayout(layoutWidget1);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        lineEdit_fn = new QLineEdit(layoutWidget1);
        lineEdit_fn->setObjectName(QString::fromUtf8("lineEdit_fn"));

        verticalLayout->addWidget(lineEdit_fn);

        lineEdit_ln = new QLineEdit(layoutWidget1);
        lineEdit_ln->setObjectName(QString::fromUtf8("lineEdit_ln"));

        verticalLayout->addWidget(lineEdit_ln);

        lineEdit_id = new QLineEdit(layoutWidget1);
        lineEdit_id->setObjectName(QString::fromUtf8("lineEdit_id"));

        verticalLayout->addWidget(lineEdit_id);

        comboBox_type = new QComboBox(layoutWidget1);
        comboBox_type->addItem(QString());
        comboBox_type->addItem(QString());
        comboBox_type->addItem(QString());
        comboBox_type->addItem(QString());
        comboBox_type->addItem(QString());
        comboBox_type->addItem(QString());
        comboBox_type->setObjectName(QString::fromUtf8("comboBox_type"));
        comboBox_type->setEditable(false);
        comboBox_type->setFrame(false);

        verticalLayout->addWidget(comboBox_type);

        comboBox_name = new QComboBox(layoutWidget1);
        comboBox_name->addItem(QString());
        comboBox_name->setObjectName(QString::fromUtf8("comboBox_name"));

        verticalLayout->addWidget(comboBox_name);

        comboBox_vers = new QComboBox(layoutWidget1);
        comboBox_vers->addItem(QString());
        comboBox_vers->setObjectName(QString::fromUtf8("comboBox_vers"));

        verticalLayout->addWidget(comboBox_vers);

        splitter->addWidget(layoutWidget1);
        pushButton = new QPushButton(CheckOut);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(120, 230, 111, 32));
        widget = new QWidget(CheckOut);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(10, 260, 351, 141));
        verticalLayout_3 = new QVBoxLayout(widget);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        co_status_2 = new QLabel(widget);
        co_status_2->setObjectName(QString::fromUtf8("co_status_2"));

        verticalLayout_3->addWidget(co_status_2);

        co_status_3 = new QLabel(widget);
        co_status_3->setObjectName(QString::fromUtf8("co_status_3"));

        verticalLayout_3->addWidget(co_status_3);

        co_status = new QLabel(widget);
        co_status->setObjectName(QString::fromUtf8("co_status"));

        verticalLayout_3->addWidget(co_status);


        retranslateUi(CheckOut);

        QMetaObject::connectSlotsByName(CheckOut);
    } // setupUi

    void retranslateUi(QDialog *CheckOut)
    {
        CheckOut->setWindowTitle(QApplication::translate("CheckOut", "Dialog", nullptr));
        checkout_b->setText(QApplication::translate("CheckOut", "Check Out", nullptr));
        label_2->setText(QApplication::translate("CheckOut", "First Name", nullptr));
        label_5->setText(QApplication::translate("CheckOut", "Last Name", nullptr));
        label->setText(QApplication::translate("CheckOut", "95#", nullptr));
        label_6->setText(QApplication::translate("CheckOut", "Item Type", nullptr));
        label_4->setText(QApplication::translate("CheckOut", "Item Name", nullptr));
        label_3->setText(QApplication::translate("CheckOut", "Item Version", nullptr));
        comboBox_type->setItemText(0, QApplication::translate("CheckOut", "Enter Item Type", nullptr));
        comboBox_type->setItemText(1, QApplication::translate("CheckOut", "movie", nullptr));
        comboBox_type->setItemText(2, QApplication::translate("CheckOut", "tool", nullptr));
        comboBox_type->setItemText(3, QApplication::translate("CheckOut", "rec", nullptr));
        comboBox_type->setItemText(4, QApplication::translate("CheckOut", "tech", nullptr));
        comboBox_type->setItemText(5, QApplication::translate("CheckOut", "game", nullptr));

        comboBox_name->setItemText(0, QApplication::translate("CheckOut", "Item Name", nullptr));

        comboBox_vers->setItemText(0, QApplication::translate("CheckOut", "Item Version", nullptr));

        pushButton->setText(QApplication::translate("CheckOut", "Back to Main", nullptr));
        co_status_2->setText(QString());
        co_status_3->setText(QString());
        co_status->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class CheckOut: public Ui_CheckOut {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHECKOUT_H
