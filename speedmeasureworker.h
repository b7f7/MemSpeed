#ifndef SPEEDMEASUREWORKER_H
#define SPEEDMEASUREWORKER_H

#include <QObject>
#include <QtCharts/QLineSeries>
#include <QSharedPointer>
#include <QtCharts/QtCharts>



#include <atomic>
#include <thread>
class SpeedMeasureWorker : public QObject
{
    Q_OBJECT
public:
    typedef QList<QPointF> point_list;

    explicit SpeedMeasureWorker(QObject *parent = nullptr);
    ~SpeedMeasureWorker();
    void Start( uint32_t StartVal, uint32_t EndVal, uint32_t Increment);
    void Cancel();

    QChart* ToChart() const;
signals:
    void Ready();
    void SetProgress(int);
private:
    uint32_t m_StartVal;
    uint32_t m_EndVal;
    uint32_t m_Increment;
    point_list m_MinSeries;
    point_list m_MaxSeries;
    point_list m_MedianSeries;
    void run( );

    std::thread m_ProcessingThread;
    std::atomic_bool m_IsRunning;
    std::atomic_bool m_CancelPreocessing {false};

};

#endif // SPEEDMEASUREWORKER_H
