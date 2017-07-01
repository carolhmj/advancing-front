#if INTERFACE

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_triangulate_clicked()
{
    ui->widget->triangulateModel();
}

#endif

void MainWindow::on_actionOpen_triggered()
{
    ui->widget->openArchive();
}
