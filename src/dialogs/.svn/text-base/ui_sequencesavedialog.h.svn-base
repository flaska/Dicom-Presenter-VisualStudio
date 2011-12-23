/********************************************************************************
** Form generated from reading ui file 'sequencesavedialog.ui'
**
** Created: Sat 10. Oct 17:50:59 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SEQUENCESAVEDIALOG_H
#define UI_SEQUENCESAVEDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DialogSaveSequence
{
public:
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *lblWidth;
    QSpinBox *spinWidth;
    QLabel *lblHeight;
    QSpinBox *spinHeight;
    QHBoxLayout *horizontalLayout_3;
    QCheckBox *checkBoxPreserve;
    QCheckBox *checkBoxKeepFrames;
    QHBoxLayout *horizontalLayout_2;
    QLabel *lblFPS;
    QDoubleSpinBox *doubleSpinFPS;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *DialogSaveSequence)
    {
        if (DialogSaveSequence->objectName().isEmpty())
            DialogSaveSequence->setObjectName(QString::fromUtf8("DialogSaveSequence"));
        DialogSaveSequence->resize(317, 110);
        layoutWidget = new QWidget(DialogSaveSequence);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 10, 299, 83));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        lblWidth = new QLabel(layoutWidget);
        lblWidth->setObjectName(QString::fromUtf8("lblWidth"));

        horizontalLayout->addWidget(lblWidth);

        spinWidth = new QSpinBox(layoutWidget);
        spinWidth->setObjectName(QString::fromUtf8("spinWidth"));

        horizontalLayout->addWidget(spinWidth);

        lblHeight = new QLabel(layoutWidget);
        lblHeight->setObjectName(QString::fromUtf8("lblHeight"));

        horizontalLayout->addWidget(lblHeight);

        spinHeight = new QSpinBox(layoutWidget);
        spinHeight->setObjectName(QString::fromUtf8("spinHeight"));

        horizontalLayout->addWidget(spinHeight);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        checkBoxPreserve = new QCheckBox(layoutWidget);
        checkBoxPreserve->setObjectName(QString::fromUtf8("checkBoxPreserve"));

        horizontalLayout_3->addWidget(checkBoxPreserve);

        checkBoxKeepFrames = new QCheckBox(layoutWidget);
        checkBoxKeepFrames->setObjectName(QString::fromUtf8("checkBoxKeepFrames"));

        horizontalLayout_3->addWidget(checkBoxKeepFrames);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        lblFPS = new QLabel(layoutWidget);
        lblFPS->setObjectName(QString::fromUtf8("lblFPS"));

        horizontalLayout_2->addWidget(lblFPS);

        doubleSpinFPS = new QDoubleSpinBox(layoutWidget);
        doubleSpinFPS->setObjectName(QString::fromUtf8("doubleSpinFPS"));

        horizontalLayout_2->addWidget(doubleSpinFPS);

        buttonBox = new QDialogButtonBox(layoutWidget);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        horizontalLayout_2->addWidget(buttonBox);


        verticalLayout->addLayout(horizontalLayout_2);


        retranslateUi(DialogSaveSequence);
        QObject::connect(buttonBox, SIGNAL(accepted()), DialogSaveSequence, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), DialogSaveSequence, SLOT(reject()));

        QMetaObject::connectSlotsByName(DialogSaveSequence);
    } // setupUi

    void retranslateUi(QDialog *DialogSaveSequence)
    {
        DialogSaveSequence->setWindowTitle(QApplication::translate("DialogSaveSequence", "Save sequence", 0, QApplication::UnicodeUTF8));
        lblWidth->setText(QApplication::translate("DialogSaveSequence", "Width", 0, QApplication::UnicodeUTF8));
        lblHeight->setText(QApplication::translate("DialogSaveSequence", "Height", 0, QApplication::UnicodeUTF8));
        checkBoxPreserve->setText(QApplication::translate("DialogSaveSequence", "Preserve aspect ratio", 0, QApplication::UnicodeUTF8));
        checkBoxKeepFrames->setText(QApplication::translate("DialogSaveSequence", "Keep the temporary bitmaps", 0, QApplication::UnicodeUTF8));
        lblFPS->setText(QApplication::translate("DialogSaveSequence", "FPS", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(DialogSaveSequence);
    } // retranslateUi

};

namespace Ui {
    class DialogSaveSequence: public Ui_DialogSaveSequence {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEQUENCESAVEDIALOG_H
