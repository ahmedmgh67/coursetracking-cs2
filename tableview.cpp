#include "tableview.h"
#include "ui_tableview.h"

TableView::TableView(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TableView)
{
    ui->setupUi(this);
}

TableView::~TableView()
{
    delete ui;
}
