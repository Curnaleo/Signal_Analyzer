#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    //SET GRAFs
    ui->normalGraf->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                    QCP::iSelectLegend | QCP::iSelectPlottables);
    ui->normalGraf->xAxis->setRange(0, 25);
    ui->normalGraf->yAxis->setRange(-50, 50);
    ui->normalGraf->axisRect()->setupFullAxesBox();

    ui->normalGraf->xAxis->setLabel("Tiempo(s)");
    ui->normalGraf->yAxis->setLabel("Intencidad Electrica");

    ui->filterGraf->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                    QCP::iSelectLegend | QCP::iSelectPlottables);
    ui->filterGraf->xAxis->setRange(0, 25);
    ui->filterGraf->yAxis->setRange(-50, 50);
    ui->filterGraf->axisRect()->setupFullAxesBox();

    ui->filterGraf->xAxis->setLabel("Tiempo(s)");
    ui->filterGraf->yAxis->setLabel("Intencidad Electrica");


    ui->furierGraf->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                    QCP::iSelectLegend | QCP::iSelectPlottables);
    ui->furierGraf->xAxis->setRange(0, 25);
    ui->furierGraf->yAxis->setRange(-50, 50);
    ui->furierGraf->axisRect()->setupFullAxesBox();

    ui->furierGraf->xAxis->setLabel("Frecuencia(Hz)");
    ui->furierGraf->yAxis->setLabel("Intencidad Frecuencial(dB)");

    ui->listView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->listView->setDragEnabled(true);
    ui->listView->setAcceptDrops(true);
    ui->listView->setDropIndicatorShown(true);

    //SET LISTVIEW
    connect(ui->listView, &CustomListView::changed, this, &MainWindow::updateFormatsTable);
    connect(ui->listView, &CustomListView::selectedItem, this, &MainWindow::PrintSelectedFile);

    //SET MODEL
    itemModel = new QStandardItemModel();
    ui->listView->setModel(itemModel);

    // connect slot that shows a message in the status bar when a graph is clicked:
    //connect(ui->normalGraf, SIGNAL(plottableClick(QCPAbstractPlottable*,int,QMouseEvent*)), this, SLOT(graphClicked(QCPAbstractPlottable*,int)));
}

MainWindow::~MainWindow()
{
    delete itemModel;
    delete ui;
}

void MainWindow::updateFormatsTable(const QMimeData *mimeData)
{
    if (!mimeData)
        return;
    const QStringList formats = mimeData->formats();
    for (const QString &format : formats)
    {
        QString text;
        itemModel->appendRow(new QStandardItem(text));
        if (format == u"text/plain")
        {
            text = mimeData->text();
            text.replace(0,5,"");

            fileList.append(text);

            itemModel->appendRow(new QStandardItem(text.split(u'/').last()));
        }
    }
}

void MainWindow::PrintSelectedFile(const QModelIndex *index)
{

    QFile file(fileList.at(index->row()));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    double i=0;

    ui->normalGraf->clearGraphs();
    ui->normalGraf->addGraph();

    while(!file.atEnd())
    {
        ui->normalGraf->graph(0)->addData(i, file.readLine().toDouble());
        i++;
    }

    file.close();

    ui->normalGraf->graph()->setLineStyle((QCPGraph::LineStyle)(5));
    ui->normalGraf->graph()->setScatterStyle(QCPScatterStyle((QCPScatterStyle::ScatterShape)(5)));
    QPen graphPen;
    graphPen.setColor(QColor(std::rand()%245+10, std::rand()%245+10, std::rand()%245+10));

    ui->normalGraf->graph()->setPen(graphPen);
    ui->normalGraf->replot();
}


