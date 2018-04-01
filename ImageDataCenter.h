#ifndef IMAGEDATACENTER_H
#define IMAGEDATACENTER_H
#include "ImageItem.h"
#include "PlusWidget.h"
#include "stdafx.h"
#include <QWidget>

namespace Ui {
class ImageDataCenter;
}

class ImageDataCenter : public QWidget {
  Q_OBJECT

public:
  explicit ImageDataCenter(QWidget *parent = 0);
  ~ImageDataCenter();
  void UpDateViewWidget(QList<ImageItem *> widgetList);
  void setTagInformation(QString tagInfor);

private:
  Ui::ImageDataCenter *ui;
  QGridLayout *m_gridLayOut;
  PlusWidget *m_plusWidget;

protected:
  virtual void resizeEvent(QResizeEvent *event);

signals:
  void resizeWidget();
  void addImageItem();

protected slots:
  void onAddImageItem();
};

#endif // IMAGEDATACENTER_H
