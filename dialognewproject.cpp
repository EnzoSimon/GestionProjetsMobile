#include "dialognewproject.h"
#include "ui_dialognewproject.h"

DialogNewProject::DialogNewProject(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewProject)
{
    ui->setupUi(this);

    showMaximized();

    ui->dateEditDebut->setMinimumDate(QDate::currentDate());
    ui->dateEditDebut->setDate(QDate::currentDate());

    ui->dateEditFin->setMinimumDate(QDate::currentDate());
    ui->dateEditFin->setDate(QDate::currentDate());
}

DialogNewProject::~DialogNewProject()
{
    delete ui;
}



void DialogNewProject::on_pushButtonCreer_clicked()
{
    qDebug()<<"void DialogNewProject::on_pushButtonCreer_clicked()";

    QString nom,description,dateDebut,dateFin;
    nom = ui->lineEditNom->text();
    description = ui->plainTextEditDesc->toPlainText();
    dateDebut = ui->dateEditDebut->text();
    dateFin = ui->dateEditFin->text();

    QString txtRequeteCreation;
    txtRequeteCreation = "INSERT INTO Projet VALUES()";

    close();
}

