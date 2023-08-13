#include "myform.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>


MyForm::MyForm(QWidget *parent)
    : QWidget(parent)
{
    _renderLbl = new QLabel(this);
    _editLbl = new QLabel(tr("Enter some text"), this);
    _editEdt = new QLineEdit(this);
    _clearBtn = new QPushButton(tr("Clear"), this);
    _closeBtn = new QPushButton(tr("Close"), this);
    QHBoxLayout* tLt = new QHBoxLayout;
    tLt->addWidget(_editLbl);
    tLt->addWidget(_editEdt);
    QHBoxLayout* mLt = new QHBoxLayout;
    mLt->addStretch();
    mLt->addWidget(_renderLbl);
    QHBoxLayout* bLt = new QHBoxLayout;
    bLt->addStretch();
    bLt->addWidget(_clearBtn);
    bLt->addWidget(_closeBtn);
    QVBoxLayout* mainLt = new QVBoxLayout;
    mainLt->addLayout(tLt);
    mainLt->addLayout(mLt);
    mainLt->addLayout(bLt);
    setLayout(mainLt);

    connect(_editEdt, SIGNAL(returnPressed()), this, SLOT(renderText()));
    connect(_clearBtn, SIGNAL(clicked()), this, SLOT(clearEdit()));
    connect(_closeBtn, SIGNAL(clicked()), this, SLOT(close()));
}

void MyForm::renderText() {_renderLbl->setText(_editEdt->text());}
void MyForm::clearEdit() {_editEdt->clear(); _renderLbl->clear();}

MyForm::~MyForm()
{

}
