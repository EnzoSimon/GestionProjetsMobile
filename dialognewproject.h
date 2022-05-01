#ifndef DIALOGNEWPROJECT_H
#define DIALOGNEWPROJECT_H

#include <QDialog>

namespace Ui {
class DialogNewProject;
}

class DialogNewProject : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNewProject(QWidget *parent = nullptr);
    ~DialogNewProject();

private slots:

    void on_pushButtonCreer_clicked();

private:
    Ui::DialogNewProject *ui;


};

#endif // DIALOGNEWPROJECT_H
