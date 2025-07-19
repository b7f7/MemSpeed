#ifndef MEMSPEED_H
#define MEMSPEED_H

#include <QMainWindow>
#include <QtCharts/QChartView>
#include "speedmeasureworker.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MemSpeed;
}
QT_END_NAMESPACE

class MemSpeed : public QMainWindow
{
    Q_OBJECT

public:
    MemSpeed(QWidget *parent = nullptr);
    ~MemSpeed();

private slots:
    void on_ChartReady();
    void on_StartButton_clicked();

    void on_EndSizeValue_editingFinished();

    void on_StartSizeValue_editingFinished();

    void on_actionSave_As_Image_triggered();

private:
    Ui::MemSpeed *ui;
    QChartView* m_ChartView {nullptr};
    SpeedMeasureWorker* m_Worker;

};
#endif // MEMSPEED_H
