#include "memspeed.h"
#include "./ui_memspeed.h"
#include <QtCharts/QtCharts>

MemSpeed::MemSpeed(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MemSpeed)
{
    ui->setupUi(this);

    m_Worker = new SpeedMeasureWorker(this);
    QObject::connect(m_Worker, &SpeedMeasureWorker::Ready, this, &MemSpeed::on_ChartReady);
    QObject::connect(m_Worker, &SpeedMeasureWorker::SetProgress, ui->progressBar, &QProgressBar::setValue);

    m_ChartView = new QChartView(this);

    ui->mainLayout->addWidget(m_ChartView);
}

MemSpeed::~MemSpeed()
{
    delete ui;
}

void MemSpeed::on_StartButton_clicked()
{
    if( ui->StartButton->text() == "Start")
    {
        ui->StartButton->setText("Cancel");
        int StartValue = ui->StartSizeValue->value();
        int EndValue = ui->EndSizeValue->value();
        ui->progressBar->reset();
        ui->progressBar->setMinimum(StartValue);
        ui->progressBar->setMaximum(EndValue);

        m_Worker->Start(StartValue, EndValue, ui->IncrementValue->value());

    }
    else if( ui->StartButton->text() == "Cancel")
    {
        m_Worker->Cancel();
        ui->StartButton->setText("Start");
    }
}

void MemSpeed::on_ChartReady()
{

    m_ChartView->setChart(m_Worker->ToChart());
    m_Worker->Cancel();
    ui->StartButton->setText("Start");

}

void MemSpeed::on_EndSizeValue_editingFinished()
{
    auto v = ui->EndSizeValue->value() - ui->StartSizeValue->singleStep();
    ui->StartSizeValue->setMaximum(v);
}


void MemSpeed::on_StartSizeValue_editingFinished()
{
    auto v = ui->StartSizeValue->value() + ui->StartSizeValue->singleStep();
    ui->EndSizeValue->setMinimum(v);

}

