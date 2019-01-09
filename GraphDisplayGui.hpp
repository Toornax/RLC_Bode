#ifndef GRAPHDISPLAYGUI_HPP
#define GRAPHDISPLAYGUI_HPP

#include <QtWidgets/QMainWindow>
#include <QtCore/QList>
#include <QtCore/QPair>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <complex>

using namespace QtCharts;

enum GraphType {
    Linear = 1,
    Log = 2
};

class GraphDisplayGui : public QMainWindow
{
    Q_OBJECT
public:
    explicit GraphDisplayGui(QWidget* parent = nullptr);
    ~GraphDisplayGui();

    void SetData(const QList<QPair<double, std::complex<double>>>&);

    void Update(GraphType type = GraphType::Linear);

private:
    QList<QPair<double, std::complex<double>>> datas;
    QLineSeries* series_db;
    QLineSeries* series_phase;
    QChart* chart;
    QChartView* chart_view;
};

#endif // GRAPHDISPLAYGUI_HPP
