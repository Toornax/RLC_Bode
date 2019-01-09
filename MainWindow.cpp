#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include <QtCore/QDebug>
#include <GraphDisplayGui.hpp>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->ui->lbl_freq_min->setVisible(false);
    this->ui->lbl_freq_max->setVisible(false);
    this->ui->sb_freq_min->setVisible(false);
    this->ui->sb_freq_max->setVisible(false);
    this->ui->lbl_freq->setVisible(true);
    this->ui->sb_freq->setVisible(true);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_sb_nb_freq_valueChanged(int val) {
    if(val == 1) {
        this->ui->lbl_freq_min->setVisible(false);
        this->ui->lbl_freq_max->setVisible(false);
        this->ui->sb_freq_min->setVisible(false);
        this->ui->sb_freq_max->setVisible(false);
        this->ui->lbl_freq->setVisible(true);
        this->ui->sb_freq->setVisible(true);
    } else {
        this->ui->lbl_freq_min->setVisible(true);
        this->ui->lbl_freq_max->setVisible(true);
        this->ui->sb_freq_min->setVisible(true);
        this->ui->sb_freq_max->setVisible(true);
        this->ui->lbl_freq->setVisible(false);
        this->ui->sb_freq->setVisible(false);
    }
}


QList<QPair<double, std::complex<double>>> MainWindow::calculate() const {
    double val1 {this->ui->circuit_drawer->getValueComponent1()};
    Component type1 {this->ui->circuit_drawer->getTypeComponent1()};
    double val2 {this->ui->circuit_drawer->getValueComponent2()};
    Component type2 {this->ui->circuit_drawer->getTypeComponent2()};
    double val3 {this->ui->circuit_drawer->getValueComponent3()};
    Component type3 {this->ui->circuit_drawer->getTypeComponent3()};

    QList<QPair<double, std::complex<double>>> list;
    int nb_freq{this->ui->sb_nb_freq->value()};
    double step {nb_freq == 1 ? this->ui->sb_freq->value() : (this->ui->sb_freq_max->value() - this->ui->sb_freq_min->value()) / static_cast<double>(nb_freq)};

    for(int i{0}; i < nb_freq; i++) {
        auto freq {static_cast<double>(i+1) * step};
        auto z1 {getImpedance(freq, val1, type1)};
        auto z2 {getImpedance(freq, val2, type2)};
        auto z3 {getImpedance(freq, val3, type3)};
        auto z23 {type2 == Component::Open ? z3 :
                  type3 == Component::Open ? z2 :
                  (z2 * z3) / (z2 + z3)};
        auto zt {z23 / (z1 + z23)};

        list << QPair<double, std::complex<double>>(freq, zt);
    }

    return list;
}

std::complex<double> MainWindow::getImpedance(double freq, double val, Component type) const {
    const double PI2 {2 * 3.141592653};
    double w {PI2 * freq};
    switch(type) {
        case Component::Resistor:
            return std::complex<double>{val, 0};
        case Component::Capacitor:
            return std::complex<double>{0, -1. / (w * val)};
        case Component::Inductance:
            return std::complex<double>{0, w * val};
        default:
            return std::complex<double>{0, 0};
    }
}

void MainWindow::on_pb_show_bode_clicked() {
    GraphType type;

    auto datas {calculate()};
    if(this->ui->rb_lin->isChecked())
        type = GraphType::Linear;
    else if(this->ui->rb_log->isChecked())
        type = GraphType::Log;


    auto gdg {new GraphDisplayGui(this)};
    gdg->SetData(datas);
    gdg->Update(type);
    gdg->show();
}
