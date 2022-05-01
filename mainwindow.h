#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void afficherTableauProjets();
    void afficherTableauTaches();

    void on_tableWidgetProjects_cellClicked(int row, int column);

    void on_pushButtonOpenProject_clicked();

    void on_pushButtonNewProject_clicked();

    void on_pushButtonCreerProjet_clicked();

    void on_pushButtonRetourProjets_clicked();


    void on_pushButtonRetour_clicked();

    void on_pushButtonNewTache_clicked();

    void on_pushButtonAjouterTache_clicked();

    void on_tableWidgetTaches_cellClicked(int row, int column);

    void on_dateEditDebutProjet_dateChanged(const QDate &date);

    void on_pushButtonSupprTache_clicked();

    void on_pushButtonUp_clicked();

    void on_pushButtonSupprProjet_clicked();

private:
    Ui::MainWindow *ui;

    int numeroProjet;
};
#endif // MAINWINDOW_H
