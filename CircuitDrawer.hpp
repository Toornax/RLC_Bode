#ifndef CIRCUITDRAWER_HPP
#define CIRCUITDRAWER_HPP

#include <QtWidgets/QWidget>

class QPixmap;
class QDoubleSpinBox;

enum Component {
    Resistor = 1,
    Capacitor = 2,
    Inductance = 4,
    Wire = 8,
    Open = 16
};


class CircuitDrawer : public QWidget
{
    Q_OBJECT
public:
    explicit CircuitDrawer(QWidget* parent = nullptr);

    double getValueComponent1() const;
    Component getTypeComponent1() const;
    double getValueComponent2() const;
    Component getTypeComponent2() const;
    double getValueComponent3() const;
    Component getTypeComponent3() const;

protected:
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent*);

private:
    void drawComponent(QPainter&, int, Component);

private:
    QPixmap* res;
    QPixmap* cap;
    QPixmap* coil;
    QDoubleSpinBox* sb_comp1;
    QDoubleSpinBox* sb_comp2;
    QDoubleSpinBox* sb_comp3;
    Component comp1, comp2, comp3;
};

#endif // CIRCUITDRAWER_HPP
