#include "CircuitDrawer.hpp"
#include <QtGui/QPainter>
#include <QtGui/QPen>
#include <QtGui/QMouseEvent>
#include <QtWidgets/QLabel>
#include <QtWidgets/QDoubleSpinBox>
#include <QtCore/QDebug>


CircuitDrawer::CircuitDrawer(QWidget* parent) :
    QWidget(parent)
{
    this->res = new QPixmap(":/img/res");
    this->cap = new QPixmap(":/img/cap");
    this->coil = new QPixmap(":/img/coil");
    this->sb_comp1 = new QDoubleSpinBox(this);
    this->sb_comp2 = new QDoubleSpinBox(this);
    this->sb_comp3 = new QDoubleSpinBox(this);
    this->comp1 = Component::Resistor;
    this->comp2 = Component::Resistor;
    this->comp3 = Component::Resistor;

    this->sb_comp1->resize(100, 20);
    this->sb_comp1->move(100, 115);
    this->sb_comp1->setSuffix(" Ω");
    this->sb_comp1->setDecimals(3);
    this->sb_comp1->setRange(1e-3, 1e8);
    this->sb_comp1->setValue(1);

    this->sb_comp2->resize(100, 20);
    this->sb_comp2->move(250, 240);
    this->sb_comp2->setSuffix(" Ω");
    this->sb_comp2->setDecimals(3);
    this->sb_comp2->setRange(1e-3, 1e8);
    this->sb_comp2->setValue(1);

    this->sb_comp3->resize(100, 20);
    this->sb_comp3->move(400, 240);
    this->sb_comp3->setSuffix(" Ω");
    this->sb_comp3->setDecimals(3);
    this->sb_comp3->setRange(1e-3, 1e8);
    this->sb_comp3->setValue(1);
}

double CircuitDrawer::getValueComponent1() const {
    return this->sb_comp1->value();
}

double CircuitDrawer::getValueComponent2() const {
    return this->sb_comp2->value();
}

double CircuitDrawer::getValueComponent3() const {
    return this->sb_comp3->value();
}

Component CircuitDrawer::getTypeComponent1() const {
    return this->comp1;
}

Component CircuitDrawer::getTypeComponent2() const {
    return this->comp2;
}

Component CircuitDrawer::getTypeComponent3() const {
    return this->comp3;
}


void CircuitDrawer::paintEvent(QPaintEvent*) {
    QPainter painter{this};
    QPen pen_line{Qt::black, 3, Qt::SolidLine};
    painter.setPen(pen_line);

    // Upper left line
    painter.drawLine(10, 50, 50, 50);

    // Draw first element
    drawComponent(painter, 1, this->comp1);

    // Upper mid line
    painter.drawLine(250, 50, 300, 50);

    // Draw second element
    drawComponent(painter, 2, this->comp2);

    // Upper right line
    painter.drawLine(300, 50, 550, 50);

    // Draw third element
    drawComponent(painter, 3, this->comp3);

    // Bottom left vertical line
    painter.drawLine(300, 250, 300, 300);

    // Bottom right vertical line
    painter.drawLine(450, 250, 450, 300);

    // Bottom ground line
    painter.drawLine(10, 300, 550, 300);


    QPen pen_border{Qt::blue, 1, Qt::SolidLine};
    painter.setPen(pen_border);

    // Draw border of the first element
    painter.drawRect(75, 0, 150, 100);

    // Draw border of the second element
    painter.drawRect(250, 75, 100, 150);

    // Draw border of the third element
    painter.drawRect(400, 75, 100, 150);
}

void CircuitDrawer::drawComponent(QPainter &painter, int position, Component comp) {
    int x1, y1, x2, y2;

    if(comp & (Component::Resistor | Component::Capacitor | Component::Inductance)) {
        x1 = position == 1 ? 50 : position == 2 ? 225 : 375;
        y1 = position == 1 ? -25 : position == 2 ? 50 : 50;
        x2 = position == 1 ? 200 : position == 2 ? 150 : 150;
        y2 = position == 1 ? 150 : position == 2 ? 200 : 200;
    } else if(comp == Component::Wire) {
        x1 = position == 1 ? 50 : position == 2 ? 300 : 450;
        y1 = position == 1 ? 50 : position == 2 ? 50 : 50;
        x2 = position == 1 ? 250 : position == 2 ? 300 : 450;
        y2 = position == 1 ? 50 : position == 2 ? 250 : 250;
    }

    if(comp == Component::Resistor) {
        if(position == 1) {
            painter.drawPixmap(x1, y1, x2, y2, *this->res);
        } else {
            QPixmap rotated = this->res->transformed(QMatrix().rotate(90));
            painter.drawPixmap(x1, y1, x2, y2, rotated);
        }
    } else if(comp == Component::Capacitor) {
        if(position == 1) {
            painter.drawPixmap(x1, y1, x2, y2, *this->cap);
        } else {
            QPixmap rotated = this->cap->transformed(QMatrix().rotate(90));
            painter.drawPixmap(x1, y1, x2, y2, rotated);
        }
    } else if(comp == Component::Inductance) {
        if(position == 1) {
            painter.drawPixmap(x1, y1, x2, y2, *this->coil);
        } else {
            QPixmap rotated = this->coil->transformed(QMatrix().rotate(90));
            painter.drawPixmap(x1, y1, x2, y2, rotated);
        }
    } else if(comp == Component::Wire) {
        painter.drawLine(x1, y1, x2, y2);
    }
}

void CircuitDrawer::mousePressEvent(QMouseEvent* event) {
    int x = event->pos().x();
    int y = event->pos().y();

    if(x >= 75 && x <= 250 && y >= 0 && y <= 100) {
        switch(this->comp1) {
            case Component::Resistor:
                this->comp1 = Component::Capacitor;
                this->sb_comp1->setSuffix(" µF");
                this->sb_comp1->setDecimals(6);
                this->sb_comp1->setRange(1e-6, 1e8);
                break;
            case Component::Capacitor:
                this->comp1 = Component::Inductance;
                this->sb_comp1->setSuffix(" mH");
                this->sb_comp1->setDecimals(3);
                this->sb_comp1->setRange(1e-3, 1e6);
                break;
            case Component::Inductance:
                this->comp1 = Component::Wire;
                this->sb_comp1->setVisible(false);
                break;
            case Component::Wire:
                this->comp1 = Component::Open;
                break;
            case Component::Open:
                this->comp1 = Component::Resistor;
                this->sb_comp1->setSuffix(" Ω");
                this->sb_comp1->setDecimals(3);
                this->sb_comp1->setRange(1e-3, 1e8);
                this->sb_comp1->setVisible(true);
                break;
        }
    }

    if(x >= 250 && x <= 350 && y >= 75 && y <= 225) {
        switch(this->comp2) {
            case Component::Resistor:
                this->comp2 = Component::Capacitor;
                this->sb_comp2->setSuffix(" µF");
                this->sb_comp2->setDecimals(6);
                this->sb_comp2->setRange(1e-6, 1e8);
                break;
            case Component::Capacitor:
                this->comp2 = Component::Inductance;
                this->sb_comp2->setSuffix(" mH");
                this->sb_comp2->setDecimals(3);
                this->sb_comp2->setRange(1e-3, 1e6);
                break;
            case Component::Inductance:
                this->comp2 = Component::Wire;
                this->sb_comp2->setVisible(false);
                break;
            case Component::Wire:
                this->comp2 = Component::Open;
                break;
            case Component::Open:
                this->comp2 = Component::Resistor;
                this->sb_comp2->setSuffix(" Ω");
                this->sb_comp2->setDecimals(3);
                this->sb_comp2->setRange(1e-3, 1e8);
                this->sb_comp2->setVisible(true);
                break;
        }
    }

    if(x >= 400 && x <= 500 && y >= 75 && y <= 225) {
        switch(this->comp3) {
            case Component::Resistor:
                this->comp3 = Component::Capacitor;
                this->sb_comp3->setSuffix(" µF");
                this->sb_comp3->setDecimals(6);
                this->sb_comp3->setRange(1e-6, 1e8);
                break;
            case Component::Capacitor:
                this->comp3 = Component::Inductance;
                this->sb_comp3->setSuffix(" mH");
                this->sb_comp3->setDecimals(3);
                this->sb_comp3->setRange(1e-3, 1e6);
                break;
            case Component::Inductance:
                this->comp3 = Component::Wire;
                this->sb_comp3->setVisible(false);
                break;
            case Component::Wire:
                this->comp3 = Component::Open;
                break;
            case Component::Open:
                this->comp3 = Component::Resistor;
                this->sb_comp3->setSuffix(" Ω");
                this->sb_comp3->setDecimals(3);
                this->sb_comp3->setRange(1e-3, 1e8);
                this->sb_comp3->setVisible(true);
                break;
        }
    }

    update();
}
