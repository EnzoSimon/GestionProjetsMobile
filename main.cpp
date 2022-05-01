#include "mainwindow.h"
#include <QApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Instanciation de la variable membre db_GestionProjets
    QSqlDatabase base = QSqlDatabase::addDatabase("QSQLITE");

    // Définition des paramètres de connexion à la base de données
    base.setDatabaseName("db_GestionProjets.sqlite"); // Nom de la base

    if(base.open())
    {
        qDebug()<<"CONNECTED TO db_GestionProjets DATABASE";
        QStringList listeDesTables = base.tables();
        //Si il n'y a pas de tables
        if(listeDesTables.isEmpty()){
            //On les créer une par une
            QString txtReqTables;
            txtReqTables = "CREATE TABLE Projet(numeroProjet INTEGER,nomProjet VARCHAR(40),descProjet VARCHAR(150),dateDebutProjet DATE,dateFinProjet DATE,etatProjet BOOL,primary key(numeroProjet))";
            QSqlQuery maRequete(txtReqTables);
            txtReqTables = "CREATE TABLE Tache(numeroTache INTEGER,libelleTache VARCHAR(25),ordreTache INTEGER,numeroProjet INTEGER NOT NULL, foreign key (numeroProjet) references Projet(numeroProjet),primary key(numeroTache))";
            maRequete.exec(txtReqTables);

            txtReqTables = "CREATE TABLE Session(numeroSession INTEGER,dateDebutSession DATE,dateFinSession DATE,heureDebutSession TIME,heureFinSession TIME,numeroTache INTEGER NOT NULL, foreign key (numeroTache) references Tache(numeroTache),primary key(numeroSession))";
            maRequete.exec(txtReqTables);

            txtReqTables = "INSERT INTO Projet(numeroProjet,nomProjet,dateDebutProjet) VALUES(1,'Test','2021-10-05')";
            maRequete.exec(txtReqTables);

            txtReqTables = "INSERT INTO Tache VALUES(1,'Faire l\'ui','2022-04-04')";
            maRequete.exec(txtReqTables);

        }

    }
    else
    {
        qDebug()<<"ERROR : NOT CONNECTED TO DATABASE";
    }


    MainWindow w;

    w.show();

    return a.exec();
}
