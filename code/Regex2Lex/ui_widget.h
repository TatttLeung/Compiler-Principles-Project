/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QWidget *widget;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QLabel *label;
    QSpacerItem *horizontalSpacer_2;
    QPlainTextEdit *plainTextEdit;
    QWidget *widget_3;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *pushButton;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *pushButton_4;
    QSpacerItem *horizontalSpacer_9;
    QPushButton *pushButton_3;
    QSpacerItem *horizontalSpacer_8;
    QPushButton *pushButton_5;
    QSpacerItem *horizontalSpacer_10;
    QPushButton *pushButton_2;
    QSpacerItem *horizontalSpacer_11;
    QPushButton *pushButton_8;
    QSpacerItem *horizontalSpacer_5;
    QWidget *widget_4;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_6;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer_7;
    QLabel *label_3;
    QLabel *label_4;
    QTableWidget *tableWidget;
    QPushButton *pushButton_6;
    QPushButton *pushButton_7;
    QPlainTextEdit *plainTextEdit_2;
    QPushButton *pushButton_9;
    QCheckBox *checkBox;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(936, 699);
        widget = new QWidget(Widget);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(10, 10, 910, 661));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);
        widget_2 = new QWidget(widget);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        widget_2->setGeometry(QRect(0, 0, 911, 71));
        horizontalLayout = new QHBoxLayout(widget_2);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(226, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        label = new QLabel(widget_2);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font.setPointSize(23);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);

        horizontalLayout->addWidget(label);

        horizontalSpacer_2 = new QSpacerItem(225, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        plainTextEdit = new QPlainTextEdit(widget);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(0, 300, 911, 361));
        plainTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        plainTextEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        plainTextEdit->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        plainTextEdit->setReadOnly(true);
        widget_3 = new QWidget(widget);
        widget_3->setObjectName(QString::fromUtf8("widget_3"));
        widget_3->setGeometry(QRect(0, 240, 911, 61));
        horizontalLayout_2 = new QHBoxLayout(widget_3);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_3 = new QSpacerItem(148, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        pushButton = new QPushButton(widget_3);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font1.setPointSize(15);
        pushButton->setFont(font1);

        horizontalLayout_2->addWidget(pushButton);

        horizontalSpacer_4 = new QSpacerItem(148, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);

        pushButton_4 = new QPushButton(widget_3);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setFont(font1);

        horizontalLayout_2->addWidget(pushButton_4);

        horizontalSpacer_9 = new QSpacerItem(148, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_9);

        pushButton_3 = new QPushButton(widget_3);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setFont(font1);

        horizontalLayout_2->addWidget(pushButton_3);

        horizontalSpacer_8 = new QSpacerItem(148, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_8);

        pushButton_5 = new QPushButton(widget_3);
        pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));
        pushButton_5->setFont(font1);

        horizontalLayout_2->addWidget(pushButton_5);

        horizontalSpacer_10 = new QSpacerItem(148, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_10);

        pushButton_2 = new QPushButton(widget_3);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setFont(font1);

        horizontalLayout_2->addWidget(pushButton_2);

        horizontalSpacer_11 = new QSpacerItem(148, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_11);

        pushButton_8 = new QPushButton(widget_3);
        pushButton_8->setObjectName(QString::fromUtf8("pushButton_8"));
        pushButton_8->setFont(font1);

        horizontalLayout_2->addWidget(pushButton_8);

        horizontalSpacer_5 = new QSpacerItem(148, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_5);

        widget_4 = new QWidget(widget);
        widget_4->setObjectName(QString::fromUtf8("widget_4"));
        widget_4->setGeometry(QRect(-1, 69, 911, 41));
        horizontalLayout_3 = new QHBoxLayout(widget_4);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer_6 = new QSpacerItem(182, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_6);

        label_2 = new QLabel(widget_4);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        QFont font2;
        font2.setPointSize(12);
        label_2->setFont(font2);
        label_2->setLayoutDirection(Qt::LeftToRight);
        label_2->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(label_2);

        horizontalSpacer_7 = new QSpacerItem(181, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_7);

        label_3 = new QLabel(widget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(0, 230, 911, 16));
        QFont font3;
        font3.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        label_3->setFont(font3);
        label_4 = new QLabel(widget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(0, 110, 421, 16));
        label_4->setFont(font3);
        tableWidget = new QTableWidget(widget);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setGeometry(QRect(0, 300, 911, 361));
        tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        tableWidget->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        pushButton_6 = new QPushButton(widget);
        pushButton_6->setObjectName(QString::fromUtf8("pushButton_6"));
        pushButton_6->setGeometry(QRect(660, 190, 101, 28));
        QFont font4;
        font4.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font4.setPointSize(10);
        pushButton_6->setFont(font4);
        pushButton_7 = new QPushButton(widget);
        pushButton_7->setObjectName(QString::fromUtf8("pushButton_7"));
        pushButton_7->setGeometry(QRect(780, 190, 111, 28));
        pushButton_7->setFont(font4);
        plainTextEdit_2 = new QPlainTextEdit(widget);
        plainTextEdit_2->setObjectName(QString::fromUtf8("plainTextEdit_2"));
        plainTextEdit_2->setGeometry(QRect(0, 130, 651, 91));
        plainTextEdit_2->setFont(font4);
        pushButton_9 = new QPushButton(widget);
        pushButton_9->setObjectName(QString::fromUtf8("pushButton_9"));
        pushButton_9->setGeometry(QRect(660, 160, 231, 28));
        QFont font5;
        font5.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font5.setPointSize(12);
        pushButton_9->setFont(font5);
        checkBox = new QCheckBox(widget);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        checkBox->setGeometry(QRect(660, 140, 221, 16));
        checkBox->setFont(font4);
        checkBox->setTristate(false);

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "\351\241\271\347\233\2561  author\357\274\232\346\235\216\350\276\276\350\211\257", nullptr));
        label->setText(QApplication::translate("Widget", "\351\241\271\347\233\256\344\270\200\357\274\232\346\255\243\345\210\231\350\241\250\350\276\276\345\274\217\350\275\254lex", nullptr));
        plainTextEdit->setDocumentTitle(QString());
        plainTextEdit->setPlaceholderText(QApplication::translate("Widget", "\346\255\244\345\244\204\345\260\206\344\274\232\346\230\276\347\244\272\347\273\223\346\236\234", nullptr));
        pushButton->setText(QApplication::translate("Widget", "\345\274\200\345\247\213\345\210\206\346\236\220", nullptr));
        pushButton_4->setText(QApplication::translate("Widget", "NFA", nullptr));
        pushButton_3->setText(QApplication::translate("Widget", "DFA", nullptr));
        pushButton_5->setText(QApplication::translate("Widget", "DFA\346\234\200\345\260\217\345\214\226", nullptr));
        pushButton_2->setText(QApplication::translate("Widget", "\350\257\215\346\263\225\345\210\206\346\236\220\347\250\213\345\272\217", nullptr));
        pushButton_8->setText(QApplication::translate("Widget", "\346\237\245\347\234\213lex\346\226\207\344\273\266", nullptr));
        label_2->setText(QApplication::translate("Widget", "\345\247\223\345\220\215\357\274\232\346\235\216\350\276\276\350\211\257 \347\217\255\347\272\247\357\274\232\350\256\241\347\247\2211\347\217\255 \345\255\246\345\217\267\357\274\23220203231004", nullptr));
        label_3->setText(QApplication::translate("Widget", "\345\212\237\350\203\275\351\200\211\346\213\251\357\274\232\350\276\223\345\205\245\346\255\243\345\210\231\350\241\250\350\276\276\345\274\217\345\220\216\357\274\214\350\257\267\345\205\210\347\202\271\345\207\273\345\274\200\345\247\213\345\210\206\346\236\220\357\274\214\345\206\215\347\202\271\345\207\273\345\205\266\344\273\226\346\214\211\351\222\256\346\237\245\347\234\213\347\273\223\346\236\234\357\274\214\346\263\250\346\204\217\347\224\237\346\210\220\347\232\204NFA\345\222\214DFA\345\233\276\344\270\215\345\220\253\345\205\263\351\224\256\350\257\215\343\200\202", nullptr));
        label_4->setText(QApplication::translate("Widget", "\350\257\267\350\276\223\345\205\245\346\255\243\345\210\231\350\241\250\350\276\276\345\274\217\357\274\214\345\205\267\344\275\223\350\276\223\345\205\245\350\247\204\345\210\231\350\257\267\347\202\271\345\207\273\345\217\263\350\276\271\342\200\234\346\237\245\347\234\213\350\247\204\345\210\231\342\200\235\346\214\211\351\222\256", nullptr));
        pushButton_6->setText(QApplication::translate("Widget", "\344\270\212\344\274\240\346\255\243\345\210\231\350\241\250\350\276\276\345\274\217", nullptr));
        pushButton_7->setText(QApplication::translate("Widget", "\344\270\213\350\275\275\346\255\243\345\210\231\350\241\250\350\276\276\345\274\217", nullptr));
#ifndef QT_NO_TOOLTIP
        plainTextEdit_2->setToolTip(QApplication::translate("Widget", "<html><head/><body><p><br/></p></body></html>", nullptr));
#endif // QT_NO_TOOLTIP
        plainTextEdit_2->setPlainText(QString());
        plainTextEdit_2->setPlaceholderText(QString());
        pushButton_9->setText(QApplication::translate("Widget", "\346\237\245\347\234\213\350\247\204\345\210\231", nullptr));
        checkBox->setText(QApplication::translate("Widget", "\350\213\245\350\257\215\346\263\225\345\210\206\346\236\220\345\277\275\347\225\245\345\244\247\345\260\217\345\206\231\357\274\214\350\257\267\345\213\276\351\200\211", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
