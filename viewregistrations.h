#ifndef VIEWREGISTRATIONS_H
#define VIEWREGISTRATIONS_H

#include <QDialog>

namespace Ui {
class ViewRegistrations;
}

class ViewRegistrations : public QDialog
{
    Q_OBJECT

public:
    explicit ViewRegistrations(QWidget *parent = nullptr, std::string val = "");
    ~ViewRegistrations();

private:
    Ui::ViewRegistrations *ui;
};

#endif // VIEWREGISTRATIONS_H
