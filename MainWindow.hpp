#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QtWidgets/QMainWindow>
#include <QtCore/QList>
#include <QtCore/QPair>
#include <complex>
#include "CircuitDrawer.hpp"

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QList<QPair<double, std::complex<double>>> calculate() const;
    std::complex<double> getImpedance(double, double, Component) const;

private slots:
    void on_sb_nb_freq_valueChanged(int arg1);
    void on_pb_show_bode_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_HPP
