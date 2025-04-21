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

    float i=0, x, f;
    f = 1 / 173.61;

    ui->normalGraf->clearGraphs();
    ui->normalGraf->addGraph();

    while(!file.atEnd())
    {
        x = f * i;
        ui->normalGraf->graph(0)->addData(x, file.readLine().toDouble());
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


