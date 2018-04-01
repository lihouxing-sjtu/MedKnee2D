#include "PlusWidget.h"
#include "ui_PlusWidget.h"
#include <QPainter>

PlusWidget::PlusWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::PlusWidget) {
  ui->setupUi(this);
}

PlusWidget::~PlusWidget() { delete ui; }

void PlusWidget::paintEvent(QPaintEvent *event) {
  int width = this->width();
  int height = this->height();

  QPainter painter(this);
  painter.setPen(QPen(Qt::black, width / 40));
  painter.drawRoundRect(0, 0, width, height, 10, 10);
  painter.setPen(QPen(Qt::white, width / 20));

  painter.drawLine(width / 2, width / 15, width / 2, height - width / 15);
  painter.drawLine(height / 15, height / 2, width - height / 15, height / 2);
  QWidget::paintEvent(event);
}

void PlusWidget::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton)
    emit addImageItem();
  QWidget::mousePressEvent(event);
}
