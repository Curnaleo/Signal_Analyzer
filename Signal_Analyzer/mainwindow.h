#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"
#include "customlistview.h"
#include <QString>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QStringList>
#include <QFile>
#include "dft.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void updateFormatsTable(const QMimeData *mimeData);
    void PrintSelectedFile(const QModelIndex *index);
    void PrintTransformfile(void);

private:
    Ui::MainWindow *ui;
    QStandardItemModel *itemModel;
    QStandardItem *item;
    QStringList *fileList;
};
#endif // PRUEBA2_H
