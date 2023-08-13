#ifndef MYFORM_H
#define MYFORM_H

#include <QtGui/QWidget>

class QLabel;
class QLineEdit;
class QPushButton;


class MyForm : public QWidget
{
    Q_OBJECT
    QLabel *_renderLbl, *_editLbl;
    QLineEdit* _editEdt;
    QPushButton *_clearBtn, *_closeBtn;

public:
    MyForm(QWidget *parent = 0);
    ~MyForm();
signals:
    void someSignal();
public slots:
    void clearEdit();
    void renderText();
};

#endif // MYFORM_H
