#ifndef PLUSWIDGET_H
#define PLUSWIDGET_H
#include "stdafx.h"
#include <QWidget>

namespace Ui {
class PlusWidget;
}

class PlusWidget : public QWidget {
  Q_OBJECT

public:
  explicit PlusWidget(QWidget *parent = 0);
  ~PlusWidget();

protected:
  virtual void paintEvent(QPaintEvent *event);
  virtual void mousePressEvent(QMouseEvent *event);

private:
  Ui::PlusWidget *ui;
signals:
  void addImageItem();
};

#endif // PLUSWIDGET_H
