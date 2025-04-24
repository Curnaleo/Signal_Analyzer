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
    ui->graf_normal->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                    QCP::iSelectLegend | QCP::iSelectPlottables);
    ui->graf_normal->xAxis->setRange(-0.1, 5);
    ui->graf_normal->yAxis->setRange(-150, 150);
    ui->graf_normal->axisRect()->setupFullAxesBox();

    ui->graf_normal->xAxis->setLabel("Time(s)");
    ui->graf_normal->yAxis->setLabel("Electric Magnitude");

    ui->graf_filter->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                    QCP::iSelectLegend | QCP::iSelectPlottables);
    ui->graf_filter->xAxis->setRange(-0.1, 5);
    ui->graf_filter->yAxis->setRange(-150, 150);
    ui->graf_filter->axisRect()->setupFullAxesBox();

    ui->graf_filter->xAxis->setLabel("Time(s)");
    ui->graf_filter->yAxis->setLabel("Electric Magnitude");

    ui->graf_furier->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                    QCP::iSelectLegend | QCP::iSelectPlottables);
    ui->graf_furier->xAxis->setRange(-1, 90);
    ui->graf_furier->yAxis->setRange(-30000, 30000);
    ui->graf_furier->axisRect()->setupFullAxesBox();

    ui->graf_furier->xAxis->setLabel("Frequency(Hz)");
    ui->graf_furier->yAxis->setLabel("Frequency Magnitude");

    ui->graf_spectre->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                    QCP::iSelectLegend | QCP::iSelectPlottables);
    ui->graf_spectre->xAxis->setRange(-1, 90);
    ui->graf_spectre->yAxis->setRange(-100, 30000);
    ui->graf_spectre->axisRect()->setupFullAxesBox();

    ui->graf_spectre->xAxis->setLabel("Frequency(Hz)");
    ui->graf_spectre->yAxis->setLabel("Frequency Strength(dB)");

    ui->graf_autocor->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                    QCP::iSelectLegend | QCP::iSelectPlottables);
    ui->graf_autocor->xAxis->setRange(-10, 10);
    ui->graf_autocor->yAxis->setRange(-0.1, 1.05);
    ui->graf_autocor->axisRect()->setupFullAxesBox();

    ui->graf_autocor->xAxis->setLabel("Time(s)");
    ui->graf_autocor->yAxis->setLabel("Correlation");

    //SET LISTVIEW
    ui->listView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->listView->setDragEnabled(true);
    ui->listView->setAcceptDrops(true);
    ui->listView->setDropIndicatorShown(true);

    //SET SIGNALs
    connect(ui->listView, &CustomListView::changed, this, &MainWindow::updateFormatsTable);
    connect(ui->listView, &CustomListView::selectedItem, this, &MainWindow::PrintSelectedFile);

    connect(ui->btn_transform, &QAbstractButton::clicked, this, &MainWindow::PrintTransformfile);
    connect(ui->btn_transform, &QAbstractButton::clicked, this, &MainWindow::PrintSpectrefile);
    connect(ui->btn_transform, &QAbstractButton::clicked, this, &MainWindow::PrintAutocorfile);

    connect(ui->vs_lowpassfilter, &QAbstractSlider::sliderReleased, this, &MainWindow::PrintFilterfile);
    connect(ui->vs_highpassfilter, &QAbstractSlider::sliderReleased, this, &MainWindow::PrintFilterfile);

    //SET MODEL
    itemModel = new QStandardItemModel();
    ui->listView->setModel(itemModel);

    //SET VECTORLIST
    vectorlist = new QList<QVector<double>>;
    SelectedFile = -1;
    

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
    if(ui->dsb_SampleRate->value() == 0)
        return;

    QVector<double> x1, *t1;

    int i, N;

    double x, f, y, deltaHz, aux;

    SelectedFile = index->row();

    f = 1 / ui->dsb_SampleRate->value();

    ui->graf_normal->clearGraphs();
    ui->graf_normal->addGraph();

    N = vectorlist->at(SelectedFile).size();

    for(i=0;i<N;i++)
        x1.append(f*i);

    ui->graf_normal->graph(0)->addData(x1,vectorlist->at(SelectedFile),true);
    ui->graf_normal->graph(0)->setLineStyle((QCPGraph::LineStyle)(5));
    ui->graf_normal->graph(0)->setScatterStyle(QCPScatterStyle((QCPScatterStyle::ScatterShape)(5)));

    QPen graphPen;
    graphPen.setColor(QColor(std::rand()%245+10, std::rand()%245+10, std::rand()%245+10));
    ui->graf_normal->graph(0)->setPen(graphPen);

    ui->graf_normal->replot();

    //SETUP GRAF_FILTER
    ui->vs_lowpassfilter->setMaximum(1+((int)ui->dsb_SampleRate->value()/2));
    ui->vs_lowpassfilter->setValue(1+((int)ui->dsb_SampleRate->value()/2));
    ui->vs_highpassfilter->setMaximum(1+((int)ui->dsb_SampleRate->value()/2));
    ui->vs_highpassfilter->setMinimum(0);

    PrintFilterfile();
}

void MainWindow::PrintFilterfile()
{
    if(SelectedFile < 0 && ui->dsb_SampleRate->value() == 0)
        return;

    QVector<double> x1, y1, *f1;

    int i, N;

    double deltaT;

    deltaT = 1 / ui->dsb_SampleRate->value();

    y1 = vectorlist->at(SelectedFile);

    N = vectorlist->at(SelectedFile).size();

    f1 = Filter(&y1, ui->vs_lowpassfilter->value() * deltaT * N, ui->vs_highpassfilter->value() * deltaT * N);

    for(i=0;i<N;i++)
        x1.append(deltaT*i);

    ui->graf_filter->clearGraphs();
    ui->graf_filter->addGraph();

    ui->graf_filter->graph(0)->addData(x1,*f1,true);

    ui->graf_filter->graph(0)->setLineStyle((QCPGraph::LineStyle)(5));
    ui->graf_filter->graph(0)->setScatterStyle(QCPScatterStyle((QCPScatterStyle::ScatterShape)(5)));

    QPen graphPen;
    graphPen.setColor(QColor(std::rand()%245+10, std::rand()%245+10, std::rand()%245+10));
    ui->graf_filter->graph(0)->setPen(graphPen);

    ui->graf_filter->replot();

    delete f1;
}


void MainWindow::PrintTransformfile()
{
    if(SelectedFile < 0 && ui->dsb_SampleRate->value() == 0)
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

    t1 = DFTAmplitude(&y1);

    ui->graf_furier->clearGraphs();
    ui->graf_furier->addGraph();

    ui->graf_furier->graph(0)->addData(x1,*t1,true);

    ui->graf_furier->graph(0)->setLineStyle((QCPGraph::LineStyle)(5));
    ui->graf_furier->graph(0)->setScatterStyle(QCPScatterStyle((QCPScatterStyle::ScatterShape)(5)));

    QPen graphPen;
    graphPen.setColor(QColor(std::rand()%245+10, std::rand()%245+10, std::rand()%245+10));
    ui->graf_furier->graph(0)->setPen(graphPen);

    ui->graf_furier->replot();

    delete t1;
}

void MainWindow::PrintSpectrefile()
{
    if(SelectedFile < 0 && ui->dsb_SampleRate->value() == 0)
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

    t1 = DFTPowerSpectre(&y1);

    ui->graf_spectre->clearGraphs();
    ui->graf_spectre->addGraph();

    ui->graf_spectre->graph(0)->addData(x1,*t1,true);

    ui->graf_spectre->graph(0)->setLineStyle((QCPGraph::LineStyle)(5));
    ui->graf_spectre->graph(0)->setScatterStyle(QCPScatterStyle((QCPScatterStyle::ScatterShape)(5)));

    QPen graphPen;
    graphPen.setColor(QColor(std::rand()%245+10, std::rand()%245+10, std::rand()%245+10));
    ui->graf_spectre->graph(0)->setPen(graphPen);

    ui->graf_spectre->replot();

    delete t1;
}

void MainWindow::PrintAutocorfile()
{
    if(SelectedFile < 0 && ui->dsb_SampleRate->value() == 0)
        return;

    QVector<double> x1, y1, *f1;

    int i, N;

    double deltaT;

    deltaT = 1 / ui->dsb_SampleRate->value();

    y1 = vectorlist->at(SelectedFile);

    N = vectorlist->at(SelectedFile).size();

    f1 = Autocorrelation(&y1);

    for(i=((N-1)*(-1));i<N;i++)
        x1.append(deltaT*i);

    ui->graf_autocor->clearGraphs();
    ui->graf_autocor->addGraph();

    ui->graf_autocor->graph(0)->addData(x1,*f1,true);

    ui->graf_autocor->graph(0)->setLineStyle((QCPGraph::LineStyle)(5));
    ui->graf_autocor->graph(0)->setScatterStyle(QCPScatterStyle((QCPScatterStyle::ScatterShape)(5)));

    QPen graphPen;
    graphPen.setColor(QColor(std::rand()%245+10, std::rand()%245+10, std::rand()%245+10));
    ui->graf_autocor->graph(0)->setPen(graphPen);

    ui->graf_autocor->replot();

    delete f1;
}
