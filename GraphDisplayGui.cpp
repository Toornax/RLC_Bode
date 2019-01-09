#include "GraphDisplayGui.hpp"
#include <QtCharts/QValueAxis>
#include <QtCharts/QLogValueAxis>

GraphDisplayGui::GraphDisplayGui(QWidget* parent) :
    QMainWindow(parent)
{
    this->series_db = new QLineSeries(this);
    this->series_phase = new QLineSeries(this);
    this->chart = new QChart();
    this->chart_view = new QChartView(this);

    this->chart_view->setRenderHint(QPainter::Antialiasing);

    this->setCentralWidget(this->chart_view);
}


GraphDisplayGui::~GraphDisplayGui() {
    delete this->chart;
}


void GraphDisplayGui::SetData(const QList<QPair<double, std::complex<double>>>& datas) {
    this->datas = datas;
}


void GraphDisplayGui::Update(GraphType type) {
    for(const auto& data : this->datas) {
        const double conv {180./3.141592653};
        double value_amp {20 * log(abs(data.second))};
        double value_phase {conv * arg(data.second)};
        this->series_db->append(data.first, value_amp);
        this->series_phase->append(data.first, value_phase);
    }

    this->chart->legend()->hide();
    this->chart->addSeries(this->series_db);
    this->chart->addSeries(this->series_phase);
    this->chart->setTitle(tr("Diagramme de Bode"));

    if(type == GraphType::Linear) {
        QValueAxis* axis_x {new QValueAxis(this)};
        axis_x->setTitleText(tr("Fréquence (Hz)"));
        axis_x->setLabelFormat("%d");
        axis_x->setTickCount(10);
        this->chart->addAxis(axis_x, Qt::AlignBottom);
        this->series_db->attachAxis(axis_x);
        this->series_phase->attachAxis(axis_x);
    } else if(type == GraphType::Log) {
        QLogValueAxis* axis_x {new QLogValueAxis(this)};
        axis_x->setTitleText(tr("Fréquence (Hz)"));
        axis_x->setLabelFormat("%d");
        axis_x->setMinorTickCount(10);
        this->chart->addAxis(axis_x, Qt::AlignBottom);
        this->series_db->attachAxis(axis_x);
        this->series_phase->attachAxis(axis_x);
    }

    QValueAxis* axis_y_left {new QValueAxis(this)};
    axis_y_left->setTitleText(tr("Amplitude (dB)"));
    axis_y_left->setLabelFormat("%g");
    axis_y_left->setTickCount(10);
    this->chart->addAxis(axis_y_left, Qt::AlignLeft);
    this->series_db->attachAxis(axis_y_left);

    QValueAxis* axis_y_right {new QValueAxis(this)};
    axis_y_right->setTitleText(tr("Phase (°)"));
    axis_y_right->setLabelFormat("%d");
    axis_y_right->setTickCount(10);
    this->chart->addAxis(axis_y_right, Qt::AlignRight);
    this->series_phase->attachAxis(axis_y_right);

    this->chart_view->setChart(this->chart);
}
