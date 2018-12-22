#include "MainWindow.hpp"
#include "ui_MainWindow.h"

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


QList<std::complex<double>> MainWindow::calculate(double val1, Component type1,
                                                  double val2, Component type2,
                                                  double val3, Component type3) const {

    QList<std::complex<double>> list;
    int nb_freq{this->ui->sb_nb_freq->value()};
    double step = (this->ui->sb_freq_max->value() - this->ui->sb_freq_min->value()) / static_cast<double>(nb_freq);

    for(int i{0}; i < nb_freq; i++) {
        auto z1 = getImpedance(static_cast<double>(i+1) * step, val1, type1);
        auto z2 = getImpedance(static_cast<double>(i+1) * step, val2, type2);
        auto z3 = getImpedance(static_cast<double>(i+1) * step, val3, type3);
        auto z23 = (z2 * z3) / (z2 + z3);
        auto zt = z23 / (z1 + z23);

        list << zt;
    }

    return list;
}

std::complex<double> MainWindow::getImpedance(double freq, double val, Component type) const {
    const double PI = 3.141592653;
    switch(type) {
        case Component::Resistor:
            return std::complex<double>{val, 0};
        case Component::Capacitor:
            return std::complex<double>{0, -1. / (2 * PI * freq * val)};
        case Component::Inductance:
            return std::complex<double>{0, 2 * PI * freq * val};
        default:
            return std::complex<double>{0, 0};
    }
}

void MainWindow::on_pb_show_bode_clicked() {
    calculate(this->ui->circuit_drawer->getValueComponent1(), this->ui->circuit_drawer->getTypeComponent1(),
              this->ui->circuit_drawer->getValueComponent2(), this->ui->circuit_drawer->getTypeComponent2(),
              this->ui->circuit_drawer->getValueComponent3(), this->ui->circuit_drawer->getTypeComponent3());

}
