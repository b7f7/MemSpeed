#include "speedmeasureworker.h"
#include "WindowsTimer.h"
#include "data_buffer.h"

SpeedMeasureWorker::SpeedMeasureWorker(QObject *parent)
    : QObject{parent}
{}
SpeedMeasureWorker::~SpeedMeasureWorker()
{
    Cancel();
}
void SpeedMeasureWorker::run()
{
    point_list minSeries;
    point_list maxSeries;
    point_list medianSeries;

    WindowsTimer t;
    const size_t height = 1000;
    for (size_t width = m_StartVal; width <= m_EndVal && ! m_CancelPreocessing; width += m_Increment)
    {
        emit(SetProgress(width));

        try
        {

            buffer b(width, height);
            buffer c(width, height);

            std::vector<uint8_t*> b_rows = b.rows();
            std::vector<uint8_t*> c_rows = c.rows();
            std::vector<double> times;
            for( size_t sample = 0; sample < 5  && ! m_CancelPreocessing; ++sample)
            {
                double time_s = 0.0;
                const size_t loopCount = 100;
                for (size_t pos = 0; pos < loopCount  && ! m_CancelPreocessing; ++pos)
                {
                    size_t line = rand() % height;
                    t.Start();
                    memmove(c_rows[line], b_rows[line], width);
                    time_s += t.GetTime();

                }
                time_s /= loopCount;
                times.push_back(time_s);
            }
            if( times.size() == 5)
            {
                std::sort(times.begin(), times.end());
                minSeries.append(QPointF(width, (width / 1024.0 / 1024.0 / 1024.0) /times[0]));
                medianSeries.append(QPointF(width, (width / 1024.0 / 1024.0 / 1024.0) /times[2]));
                maxSeries.append(QPointF(width, (width / 1024.0 / 1024.0 / 1024.0) /times[4]));
            }
        }
        catch (...)
        {
            break;
        }
    }
    m_MinSeries.swap(minSeries);
    m_MaxSeries.swap(maxSeries);
    m_MedianSeries.swap(medianSeries);
    emit(Ready());
}

void SpeedMeasureWorker::Start( uint32_t StartVal, uint32_t EndVal, uint32_t Increment)
{
    Cancel();
    bool expected {false};
    if (m_IsRunning.compare_exchange_strong(expected, true) )
    {

        m_StartVal = StartVal;
        m_EndVal = EndVal;
        m_Increment = Increment;

        if( m_ProcessingThread.joinable())
        {
            m_CancelPreocessing.store(true);
            m_ProcessingThread.join();
        }
        m_CancelPreocessing.store(false);
        m_ProcessingThread = std::thread(&SpeedMeasureWorker::run, this);

    }
}
void SpeedMeasureWorker::Cancel()
{
    m_CancelPreocessing.store(true);
    if( m_ProcessingThread.joinable())
    {
        m_ProcessingThread.join();
    }
    bool expected {true};
    m_IsRunning.compare_exchange_strong(expected, false);
}

QChart* SpeedMeasureWorker::ToChart() const
{
    QChart* chart = new QChart();
    QLineSeries* minSeries = new QLineSeries();
    minSeries->append(m_MinSeries);

    QLineSeries* maxSeries = new QLineSeries();
    maxSeries->append(m_MaxSeries);

    QLineSeries* medianSeries = new QLineSeries();
    medianSeries->append(m_MedianSeries);

    chart->addSeries( minSeries);
    chart->addSeries( medianSeries);
    chart->addSeries( maxSeries);




    auto axisY3 = new QCategoryAxis;

    axisY3->append("DDR3-1066", 8533/2048.0);
    axisY3->append("DDR3-1333", 10667/2048.0);
    axisY3->append("DDR3-1600", 12800/2048.0);

    axisY3->append("DDR4-2400", 19200/2048.0);
    axisY3->append("DDR4-2666", 21300/2048.0);
    axisY3->append("DDR4-2933", 23400/2048.0);
    axisY3->append("DDR4-3000", 24000/2048.0);
    axisY3->append("DDR4-3200", 25600/2048.0);
    axisY3->append("DDR4-3600", 28800/2048.0);
    axisY3->append("DDR4-4000", 32000/2048.0);
    axisY3->append("DDR4-4400", 35200/2048.0);

    axisY3->append("DDR5-5600", 69.21/2.0);
    axisY3->append("DDR5-5200", 66.12/2.0);
    axisY3->append("DDR5-4800", 62.74/2.0);
    axisY3->append("DDR5-4400", 58.81/2.0);
    axisY3->append("DDR5-4000", 54.65/2.0);
    axisY3->append("DDR5-3600", 50.26/2.0);
    axisY3->append("DDR5-3200", 45.62/2.0);


    axisY3->setLinePenColor(QColor("black"));
    axisY3->setGridLinePen(QColor("black"));

    chart->addAxis(axisY3, Qt::AlignRight);

    auto axisX = new QValueAxis;
    axisX->setTickCount(1024);
    chart->addAxis(axisX, Qt::AlignBottom);
    auto axisY = new QValueAxis;
    axisX->setTickCount(5);
    chart->addAxis(axisY, Qt::AlignLeft);

    minSeries->attachAxis(axisX);
    minSeries->attachAxis(axisY);
    minSeries->attachAxis(axisY3);

    maxSeries->attachAxis(axisX);
    maxSeries->attachAxis(axisY);
    maxSeries->attachAxis(axisY3);

    medianSeries->attachAxis(axisX);
    medianSeries->attachAxis(axisY);
    medianSeries->attachAxis(axisY3);

    return chart;
}
