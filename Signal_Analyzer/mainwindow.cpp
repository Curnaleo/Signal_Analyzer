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
    ui->normalGraf->xAxis->setRange(0, 5);
    ui->normalGraf->yAxis->setRange(-150, 150);
    ui->normalGraf->axisRect()->setupFullAxesBox();

    ui->normalGraf->xAxis->setLabel("Time(s)");
    ui->normalGraf->yAxis->setLabel("Electric Strength");

    ui->filterGraf->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                    QCP::iSelectLegend | QCP::iSelectPlottables);
    ui->filterGraf->xAxis->setRange(0, 5);
    ui->filterGraf->yAxis->setRange(-150, 150);
    ui->filterGraf->axisRect()->setupFullAxesBox();

    ui->filterGraf->xAxis->setLabel("Time(s)");
    ui->filterGraf->yAxis->setLabel("Electric Strength");


    ui->furierGraf->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                    QCP::iSelectLegend | QCP::iSelectPlottables);
    ui->furierGraf->xAxis->setRange(0, 100);
    ui->furierGraf->yAxis->setRange(-150, 150);
    ui->furierGraf->axisRect()->setupFullAxesBox();

    ui->furierGraf->xAxis->setLabel("Frequency(Hz)");
    ui->furierGraf->yAxis->setLabel("Frequency Strength(dB)");

    ui->listView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->listView->setDragEnabled(true);
    ui->listView->setAcceptDrops(true);
    ui->listView->setDropIndicatorShown(true);

    //SET LISTVIEW
    connect(ui->listView, &CustomListView::changed, this, &MainWindow::updateFormatsTable);
    connect(ui->listView, &CustomListView::selectedItem, this, &MainWindow::PrintSelectedFile);
    connect(ui->btn_transform, &QAbstractButton::clicked, this, &MainWindow::PrintTransformfile);

    //SET MODEL
    itemModel = new QStandardItemModel();
    ui->listView->setModel(itemModel);

    //SET FILELIST
    fileList = new QStringList();
    

    // connect slot that shows a message in the status bar when a graph is clicked:
    //connect(ui->normalGraf, SIGNAL(plottableClick(QCPAbstractPlottable*,int,QMouseEvent*)), this, SLOT(graphClicked(QCPAbstractPlottable*,int)));
}

MainWindow::~MainWindow()
{
    delete itemModel;
    delete fileList;
    delete ui;
}

void MainWindow::updateFormatsTable(const QMimeData *mimeData)
{
    if (!mimeData)
        return;

    QString text;

    if(mimeData->hasText())
    {
        text = mimeData->text();
        
        #ifdef _WIN32
        text.replace(0,8,"");
        #elif __linux__
        text.replace(0,5,"");
        #endif
        itemModel->appendRow(new QStandardItem(text.split(u'/').last()));
    
        fileList->append(text);
    }
}

void MainWindow::PrintSelectedFile(const QModelIndex *index)
{

    QFile file(fileList->at(index->row()));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QVector<double> x1, v, *t1, *t2;

    double x, f, y, deltaHz, i, aux;
    f = 1 / 173.61;

    ui->normalGraf->clearGraphs();
    ui->normalGraf->addGraph();

    while(!file.atEnd())
    {
        x = f * i;
        y = file.readLine().toDouble();
        v.append(y);
        ui->normalGraf->graph(0)->addData(x, y);
        i++;
    }

    aux =i;

    deltaHz = 173.61/i;

    for(i=0;i<aux;i++)
    {

        x1.append(deltaHz*i);
    }

    file.close();

    ui->normalGraf->graph()->setLineStyle((QCPGraph::LineStyle)(5));
    ui->normalGraf->graph()->setScatterStyle(QCPScatterStyle((QCPScatterStyle::ScatterShape)(5)));
    QPen graphPen;
    graphPen.setColor(QColor(std::rand()%245+10, std::rand()%245+10, std::rand()%245+10));

    t1 = DFT(&v);

    ui->normalGraf->graph()->setPen(graphPen);
    ui->normalGraf->replot();

    ui->furierGraf->clearGraphs();
    ui->furierGraf->addGraph();

    ui->furierGraf->graph(0)->addData(x1,*t1,true);

    ui->furierGraf->graph(0)->setLineStyle((QCPGraph::LineStyle)(5));
    ui->furierGraf->graph(0)->setScatterStyle(QCPScatterStyle((QCPScatterStyle::ScatterShape)(5)));

    graphPen.setColor(QColor(std::rand()%245+10, std::rand()%245+10, std::rand()%245+10));
    ui->furierGraf->graph(0)->setPen(graphPen);

    ui->furierGraf->replot();


}

void MainWindow::PrintTransformfile()
{

}


