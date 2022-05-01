#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QSqlQuery>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Affichage des tableaux au démarrage de l'app
    afficherTableauProjets();

    ui->pushButtonRetourProjets->setVisible(0);



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::afficherTableauProjets()
{
    qDebug()<<"void MainWindow::afficherTableauProjets()";

    QString maRequeteTableau;
    maRequeteTableau = "SELECT nomProjet,dateDebutProjet FROM Projet";
    qDebug()<<maRequeteTableau;
    QSqlQuery maRequete(maRequeteTableau);

    ui->tableWidgetProjects->clear();
    ui->tableWidgetProjects->setRowCount(0);

    //Redimensionnement des colonnes en fonction de la taille du tableau
    ui->tableWidgetProjects->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //Ajout des entêtes du tableau
    QStringList ColumnProjets;
    ColumnProjets<<"Nom"<<"Date de création";
    ui->tableWidgetProjects->setHorizontalHeaderLabels(ColumnProjets);
    ui->tableWidgetProjects->verticalHeader()->setDefaultSectionSize(45);

    int maLigne=0;

    //mise en place du tableau
    while (maRequete.next()) {
        ui->tableWidgetProjects->insertRow(maLigne);
        ui->tableWidgetProjects->setItem(maLigne, 0, new QTableWidgetItem(maRequete.value("nomProjet").toString()));
        ui->tableWidgetProjects->setItem(maLigne, 1, new QTableWidgetItem(maRequete.value("dateDebutProjet").toString()));

        //Centrage des cellules
        ui->tableWidgetProjects->item(maLigne,0)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidgetProjects->item(maLigne,1)->setTextAlignment(Qt::AlignCenter);
        maLigne++;
    }
    //Cacher le bouton d'ouverture de projet tant qu'aucun projet n'est selectionné
    ui->pushButtonOpenProject->setVisible(0);
    ui->pushButtonSupprProjet->setVisible(0);
}

void MainWindow::afficherTableauTaches()
{
    qDebug()<<"void MainWindow::afficherTableauTaches()";
    //Bouton de retour rendu visible
    ui->pushButtonSupprTache->setVisible(0);
    //Requete pour le tableau Taches
    QString maRequeteTableau;
    maRequeteTableau = "SELECT Tache.numeroTache, Tache.libelleTache,Tache.ordreTache,Projet.numeroProjet,Projet.nomProjet "
                       "FROM Tache INNER JOIN Projet ON Tache.numeroProjet=Projet.numeroProjet "
                       "WHERE Projet.nomProjet ='"+ui->labelNomProjet->text()+"' ORDER BY Tache.ordreTache";
    qDebug()<<maRequeteTableau;
    QSqlQuery maRequete(maRequeteTableau);

    ui->tableWidgetTaches->clear();
    ui->tableWidgetTaches->setRowCount(0);

    //Redimensionnement des colonnes en fonction de la taille du tableau
    ui->tableWidgetTaches->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //Ajout des entêtes du tableau
    QStringList ColumnTaches;
    ColumnTaches<<"Nom"<<"Ordre";
    ui->tableWidgetTaches->setHorizontalHeaderLabels(ColumnTaches);
    ui->tableWidgetTaches->verticalHeader()->setDefaultSectionSize(45);

    int maLigne=0;

    //mise en place du tableau
    while (maRequete.next()) {
        ui->tableWidgetTaches->insertRow(maLigne);
        ui->tableWidgetTaches->setItem(maLigne, 0, new QTableWidgetItem(maRequete.value("libelleTache").toString()));
        ui->tableWidgetTaches->setItem(maLigne, 1, new QTableWidgetItem(maRequete.value("ordreTache").toString()));

        //Centrage des cellules
        ui->tableWidgetTaches->item(maLigne,0)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidgetTaches->item(maLigne,1)->setTextAlignment(Qt::AlignCenter);
        maLigne++;
    }
}



void MainWindow::on_tableWidgetProjects_cellClicked(int row, int column)
{
    //Afficher le bouton d'ouverture et de suppression à la sélection d'un projet
    ui->pushButtonOpenProject->setVisible(1);
    ui->pushButtonSupprProjet->setVisible(1);
}


void MainWindow::on_pushButtonOpenProject_clicked()
{
    qDebug()<<"void MainWindow::on_pushButtonOpenProject_clicked()";

    //Affichage du bouton retour à l'accueil
    ui->pushButtonRetourProjets->setVisible(1);
    ui->labelErreurTableauTaches->setVisible(0);

    //desactivation des boutons de la page des taches pour éviter les bugs
    ui->pushButtonUp->setEnabled(0);

    ui->stackedWidget->setCurrentIndex(2);

    QString nom, dateDebut;

    nom = ui->tableWidgetProjects->item(ui->tableWidgetProjects->currentRow(),0)->text();
    dateDebut = ui->tableWidgetProjects->item(ui->tableWidgetProjects->currentRow(),1)->text();

    QString txtRequeteProjet = "SELECT numeroProjet,nomProjet,descProjet,dateDebutProjet,dateFinProjet "
                               "FROM Projet "
                               "WHERE nomProjet = '"+ui->tableWidgetProjects->item(ui->tableWidgetProjects->currentRow(),0)->text()+"'";
    qDebug()<<txtRequeteProjet;
    QSqlQuery requeteProjet(txtRequeteProjet);
    requeteProjet.next();

    numeroProjet = requeteProjet.value("numeroProjet").toInt();

    ui->labelNomProjet->setText(requeteProjet.value("nomProjet").toString());
    ui->labelDescProjet->setText(requeteProjet.value("descProjet").toString());
    ui->labelDateCreation->setText("Projet crée le "+requeteProjet.value("dateDebutProjet").toString());
    ui->labelDateFin->setText("et finit le "+requeteProjet.value("dateFinProjet").toString());

    afficherTableauTaches();
}


void MainWindow::on_pushButtonNewProject_clicked()
{ 
    //Changement de page vers la page CreerProjet
    ui->stackedWidget->setCurrentIndex(1);

    //Activation du bouton de retours
    ui->pushButtonRetourProjets->setVisible(1);

    //Mise à jour de la date du jour dans les sélecteurs de date
    ui->dateEditDebutProjet->setDate(QDate::currentDate());
    ui->dateEditFinProjet->setDate(QDate::currentDate());
}


void MainWindow::on_pushButtonCreerProjet_clicked()
{
    qDebug()<<"void MainWindow::on_pushButtonCreerProjet_clicked()";

    //Activation du bouton de retour vers l'accueil
    ui->pushButtonRetourProjets->setVisible(1);

    if(!ui->lineEditNomProjet->text().isEmpty() && !ui->plainTextEditDescProjet->toPlainText().isEmpty()) {

        //changement de page vers la page d'index 2 (AjoutTaches)
        ui->stackedWidget->setCurrentIndex(2);


        //Récupération du plus grand id
        QSqlQuery max("SELECT IFNULL((SELECT MAX(numeroProjet)+1 FROM Projet),0)");
        max.next();
        QString idProjet = max.value(0).toString();

        numeroProjet = idProjet.toInt();

        QString txtRequeteProjet;

        txtRequeteProjet = "INSERT INTO Projet VALUES("+idProjet+","
                                                      "'"+ui->lineEditNomProjet->text()+"',"
                                                      "'"+ui->plainTextEditDescProjet->toPlainText()+"',"
                                                      "'"+ui->dateEditDebutProjet->text()+"',"
                                                      "'"+ui->dateEditFinProjet->text()+"',"
                                                      "0)";
        qDebug()<<txtRequeteProjet;

        QSqlQuery requeteProjet(txtRequeteProjet);

        //Affichage du nom / description du projet sur le titre de la page suivante
        ui->labelNomProjet->setText(ui->lineEditNomProjet->text());
        ui->labelDescProjet->setText(ui->plainTextEditDescProjet->toPlainText());

        afficherTableauTaches();

    }else if(ui->lineEditNomProjet->text().isEmpty()){

        ui->lineEditNomProjet->setStyleSheet("background: #e82c2c; color: white;");

    }else if(ui->plainTextEditDescProjet->toPlainText().isEmpty()){

        ui->plainTextEditDescProjet->setStyleSheet("background: #e82c2c; color: white;");

    }
}


void MainWindow::on_pushButtonRetourProjets_clicked()
{
    //changement de page vers la page d'index 0 (Accueil)
    ui->stackedWidget->setCurrentIndex(0);

    //Desactivation du bouton de retour
    ui->pushButtonRetourProjets->setVisible(0);

    //Reaffichage du tableau des projets
    afficherTableauProjets();
}




void MainWindow::on_pushButtonRetour_clicked()
{
    //changement de page vers la page d'index 2 (EnsembleTaches)
    ui->stackedWidget->setCurrentIndex(2);
}


void MainWindow::on_pushButtonNewTache_clicked()
{
    //changement de page vers la page d'index 3 (CreerTache)
    ui->stackedWidget->setCurrentIndex(3);

    //reset des phrases d'erreur en invisible
    ui->labelErreurNomTache->setVisible(0);
    ui->labelErreurOrdreTache->setVisible(0);
}


void MainWindow::on_pushButtonAjouterTache_clicked()
{
    qDebug()<<"void MainWindow::on_pushButtonAjouterTache_clicked()";
    //si le nom n'est pas rempli j'affiche une erreur
    if(ui->lineEditTitreTache->text().isEmpty()){
        //affichage de l'erreur
        ui->labelErreurNomTache->setVisible(1);
        ui->lineEditTitreTache->setStyleSheet("background: #e82c2c;");
    }
    //sinon j'exécute la requête
    else
    {
        //requête qui cherche si une tache à le même ordre que la nouvelle
        QString txtRequeteOrdreCheck;
        txtRequeteOrdreCheck = "SELECT Tache.ordreTache, Projet.numeroProjet "
                               "From Tache "
                               "INNER JOIN Projet ON Tache.numeroProjet=Projet.numeroProjet "
                               "WHERE Tache.ordreTache = "+ui->spinBoxOrdrePriorite->text()+" AND Projet.numeroProjet = "+QString::number(numeroProjet);

        qDebug()<<txtRequeteOrdreCheck;
        QSqlQuery requeteOrdreCheck(txtRequeteOrdreCheck);
        requeteOrdreCheck.first();

        //si il trouve pas de tache avec le même ordre
        if(requeteOrdreCheck.value(0).toString().isEmpty())
        {
            //Récupération du plus grand id
            QSqlQuery max("SELECT IFNULL((SELECT MAX(numeroTache)+1 FROM Tache),0)");
            max.next();
            QString idTache = max.value(0).toString();

            //requete d'insertion d'une tache
            QString txtRequeteTache = "INSERT INTO Tache "
                                      "VALUES("+idTache+",'"+ui->lineEditTitreTache->text()+"',"+ui->spinBoxOrdrePriorite->text()+","+QString::number(numeroProjet)+")";
            qDebug()<<txtRequeteTache;
            QSqlQuery requeteTache(txtRequeteTache);

            //changement de page vers la page d'index 2 (EnsembleTaches)
            ui->stackedWidget->setCurrentIndex(2);

            afficherTableauTaches();

        }
        //sinon il affiche une erreur d'ordre
        else
        {
            ui->labelErreurOrdreTache->setVisible(1);
        }
    }
}


void MainWindow::on_tableWidgetTaches_cellClicked(int row, int column)
{
    //Si une ligne est sélectionnée
    //Affichage du bouton de suppression
    ui->pushButtonSupprTache->setVisible(1);

    //Masquer la phrase d'erreur
    ui->labelErreurTableauTaches->setVisible(0);

    //Activer les boutons d'ordre
    ui->pushButtonUp->setEnabled(1);
}


void MainWindow::on_dateEditDebutProjet_dateChanged(const QDate &date)
{
    //Minimum de la date de fin mise à jour pour ne pas être inferieure à la date de début
    QString saDate;
    saDate = ui->dateEditDebutProjet->date().toString();
    ui->dateEditFinProjet->setMinimumDate(QDate::fromString(saDate));
}


void MainWindow::on_pushButtonSupprTache_clicked()
{
    qDebug()<<"void MainWindow::on_pushButtonSupprTache_clicked()";

    QString txtMaRequeteSupprimerTache;
    txtMaRequeteSupprimerTache = "DELETE FROM Tache "
                                 "WHERE ordreTache = "+ui->tableWidgetTaches->item(ui->tableWidgetTaches->currentRow(),1)->text();
    qDebug()<<txtMaRequeteSupprimerTache;
    QSqlQuery maRequeteSupprimerTache(txtMaRequeteSupprimerTache);

    afficherTableauTaches();
}


void MainWindow::on_pushButtonUp_clicked()
{
    qDebug()<<"void MainWindow::on_pushButtonUp_clicked()";

    if(ui->tableWidgetTaches->currentRow()==0)
    {

        ui->labelErreurTableauTaches->setVisible(1);
        ui->labelErreurTableauTaches->setText("C'est déjà la plus haute tâche !");

    }
    else
    {
        ui->labelErreurTableauTaches->setVisible(0);

        QString txtRequeteBaisserTacheSup;
        txtRequeteBaisserTacheSup = "UPDATE Tache "
                                    "SET ordreTache= "+ui->tableWidgetTaches->item(ui->tableWidgetTaches->currentRow(),1)->text()+" "
                                    "WHERE libelleTache= '"+ui->tableWidgetTaches->item(ui->tableWidgetTaches->currentRow()-1,0)->text()+"'";

        qDebug()<<txtRequeteBaisserTacheSup;
        QSqlQuery requeteBaisseTacheSup(txtRequeteBaisserTacheSup);

        QString txtRequeteMonterTacheInf;
        txtRequeteMonterTacheInf = "UPDATE Tache "
                                   "SET ordreTache= "+ui->tableWidgetTaches->item(ui->tableWidgetTaches->currentRow()-1,1)->text()+" "
                                   "WHERE libelleTache= '"+ui->tableWidgetTaches->item(ui->tableWidgetTaches->currentRow(),0)->text()+"'";

        qDebug()<<txtRequeteMonterTacheInf;
        QSqlQuery requeteMonterTacheInf(txtRequeteMonterTacheInf);

        afficherTableauTaches();
    }

}


void MainWindow::on_pushButtonSupprProjet_clicked()
{
    qDebug()<<"void MainWindow::on_pushButtonSupprProjet_clicked()";

    QString txtMaRequeteSupprimerProjet;
    txtMaRequeteSupprimerProjet = "DELETE FROM Projet "
                                  "WHERE nomProjet = '"+ui->tableWidgetProjects->item(ui->tableWidgetProjects->currentRow(),0)->text()+"'";
    qDebug()<<txtMaRequeteSupprimerProjet;
    QSqlQuery maRequeteSupprimerProjet(txtMaRequeteSupprimerProjet);

    afficherTableauProjets();
}

