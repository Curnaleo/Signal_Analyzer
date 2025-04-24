#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qcustomplot.h"
#include "customlistview.h"
#include "dft.h"

int SelectedFile;

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
    ui->furierGraf->xAxis->setRange(-1, 100);
    ui->furierGraf->yAxis->setRange(-0.5, 10);
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
    connect(ui->vs_lowpassfilter, &QAbstractSlider::sliderReleased, this, &MainWindow::PrintFilterfile);
    connect(ui->vs_highpassfilter, &QAbstractSlider::sliderReleased, this, &MainWindow::PrintFilterfile);

    //SET MODEL
    itemModel = new QStandardItemModel();
    ui->listView->setModel(itemModel);

    //SET VECTORLIST
    vectorlist = new QList<QVector<double>>;
    

    // connect slot that shows a message in the status bar when a graph is clicked:
    //connect(ui->normalGraf, SIGNAL(plottableClick(QCPAbstractPlottable*,int,QMouseEvent*)), this, SLOT(graphClicked(QCPAbstractPlottable*,int)));
}

MainWindow::~MainWindow()
{
    delete itemModel;
    delete vectorlist;
    delete ui;
}

void MainWindow::updateFormatsTable(const QMimeData *mimeData)
{
    if (!mimeData)
        return;

    QString text;
    QVector<double> v;
    double y;

    if(mimeData->hasText())
    {
        text = mimeData->text();
        
        #ifdef _WIN32
        text.replace(0,8,"");
        #elif __linux__
        text.replace(0,5,"");
        #endif

        if(!itemModel->findItems(text.split(u'/').last(),Qt::MatchEndsWith,0).size())
        {
            QFile file(text);
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
                return;

            while(!file.atEnd())
            {
                y = file.readLine().toDouble();
                v.append(y);
            }

            file.close();

            itemModel->appendRow(new QStandardItem(text.split(u'/').last()));

            vectorlist->append(v);
        }
    }
}

void MainWindow::PrintSelectedFile(const QModelIndex *index)
{
    QVector<double> x1, *t1;

    int i, N;

    double x, f, y, deltaHz, aux;

    SelectedFile = index->row();

    f = 1 / ui->dsb_SampleRate->value();

    ui->normalGraf->clearGraphs();
    ui->normalGraf->addGraph();

    N = vectorlist->at(SelectedFile).size();

    ui->vs_lowpassfilter->setMaximum(1+((int)ui->dsb_SampleRate->value()/2));
    ui->vs_lowpassfilter->setValue(1+((int)ui->dsb_SampleRate->value()/2));
    ui->vs_highpassfilter->setMaximum(1+((int)ui->dsb_SampleRate->value()/2));
    ui->vs_highpassfilter->setMinimum(0);

    for(i=0;i<N;i++)
        x1.append(f*i);

    ui->normalGraf->graph(0)->addData(x1,vectorlist->at(SelectedFile),true);
    ui->normalGraf->graph(0)->setLineStyle((QCPGraph::LineStyle)(5));
    ui->normalGraf->graph(0)->setScatterStyle(QCPScatterStyle((QCPScatterStyle::ScatterShape)(5)));

    QPen graphPen;
    graphPen.setColor(QColor(std::rand()%245+10, std::rand()%245+10, std::rand()%245+10));
    ui->normalGraf->graph(0)->setPen(graphPen);

    ui->normalGraf->replot();

    PrintFilterfile();
}

void MainWindow::PrintTransformfile()
{
    if(SelectedFile < 0)
        return;

    QVector<double> x1, y1, *t1;

    int i, N;

    double x, y, deltaHz;

    y1 = vectorlist->at(SelectedFile);

    N = vectorlist->at(SelectedFile).size();

    deltaHz = (ui->dsb_SampleRate->value())/N;

    for(i=0;i<(N/2);i++)
    {

        x1.append(deltaHz*i);
    }

    t1 = DFT(&y1);

    ui->furierGraf->clearGraphs();
    ui->furierGraf->addGraph();

    ui->furierGraf->graph(0)->addData(x1,*t1,true);

    ui->furierGraf->graph(0)->setLineStyle((QCPGraph::LineStyle)(5));
    ui->furierGraf->graph(0)->setScatterStyle(QCPScatterStyle((QCPScatterStyle::ScatterShape)(5)));

    QPen graphPen;
    graphPen.setColor(QColor(std::rand()%245+10, std::rand()%245+10, std::rand()%245+10));
    ui->furierGraf->graph(0)->setPen(graphPen);

    ui->furierGraf->replot();

    delete t1;
}

void MainWindow::PrintFilterfile()
{
    QVector<double> x1, y1, *f1;

    int i, N;

    double deltaT;

    deltaT = 1 / ui->dsb_SampleRate->value();

    y1 = vectorlist->at(SelectedFile);

    N = vectorlist->at(SelectedFile).size();

    f1 = Filter(&y1, ui->vs_lowpassfilter->value() * deltaT * N, ui->vs_highpassfilter->value() * deltaT * N);

    for(i=0;i<N;i++)
        x1.append(deltaT*i);

    ui->filterGraf->clearGraphs();
    ui->filterGraf->addGraph();

    ui->filterGraf->graph(0)->addData(x1,*f1,true);

    ui->filterGraf->graph(0)->setLineStyle((QCPGraph::LineStyle)(5));
    ui->filterGraf->graph(0)->setScatterStyle(QCPScatterStyle((QCPScatterStyle::ScatterShape)(5)));

    QPen graphPen;
    graphPen.setColor(QColor(std::rand()%245+10, std::rand()%245+10, std::rand()%245+10));
    ui->filterGraf->graph(0)->setPen(graphPen);

    ui->filterGraf->replot();

    delete f1;
}


