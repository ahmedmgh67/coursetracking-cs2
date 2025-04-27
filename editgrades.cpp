#include "editgrades.h"
#include "ui_editgrades.h"
#include <QLineEdit>
#include <QFile>
#include <QTextStream>
#include <QStringList>

EditGrades::EditGrades(QWidget *parent, QString said, QString caid)
    :QDialog(parent), ui(new Ui::EditGrades), cid(caid),  sid(said)
{
    ui->setupUi(this);

    int found;




    QList<QLineEdit*> lineEdits;
    lineEdits<<ui->lineEdit <<  ui->lineEdit_2 << ui->lineEdit_3;

    QFile file("/Users/ahmedgamal/UNI/CS2/coursetracking-cs2/grades.csv");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning("Could not open CSV file aaaa");
    }

    QTextStream in(&file);

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(',');
        if (fields.contains(sid) && fields.contains(cid)) {
            for (int i = 0; i < fields.size() && i < lineEdits.size(); ++i) {
                lineEdits[i]->setText(fields[i+2]);


            }
            file.close();
        }
    }

    file.close();




}

EditGrades::~EditGrades()
{
    delete ui;
}

void EditGrades::on_pushButton_clicked()
{
    QFile file("/Users/ahmedgamal/UNI/CS2/coursetracking-cs2/grades.csv");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning("Could not open CSV for reading");
    }

    QTextStream in(&file);
    QStringList lines;
    bool updated = false;

    QList<QLineEdit*> lineEdits;
    lineEdits<<ui->lineEdit <<  ui->lineEdit_2 << ui->lineEdit_3;
    if((lineEdits[0]->text().toDouble()>30) || (lineEdits[1]->text().toDouble()>30) || (lineEdits[2]->text().toDouble()>40)){
        ui->resultlabel->setText("Maximum for Midterms is 30 and for final is 40.");
        return;
    }
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(',');

        if (!updated && (fields.contains(cid)&&fields.contains(sid))) {
            QStringList newFields;
            newFields<<sid<<cid;

            for (QLineEdit* edit : lineEdits) {
                newFields << edit->text();
            }
            lines.append(newFields.join(','));
            updated = true;
        } else {
            lines.append(line);
        }
    }

    file.close();

    if (!updated) {
        qWarning("No matching row found to update");
    }

    // Now overwrite the file
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        qWarning("Could not open CSV for writing");
    }

    QTextStream out(&file);
    for (const QString& updatedLine : lines) {
        out << updatedLine << "\n";
    }

    file.close();
    QWidget* parent1 = this->parentWidget();
    if (parent1) {
        QWidget* parent2 = parent1->parentWidget();
        if (parent2) {
            parent2->close();
        }
        parent1->close();
    }
    this->close();

}

