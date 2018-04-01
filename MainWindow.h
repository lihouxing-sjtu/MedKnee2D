#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "ImageDataCenter.h"
#include "ImageItem.h"
#include "PlanWidget.h"
#include "stdafx.h"
#include <QMainWindow>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private:
  Ui::MainWindow *ui;
  ImageDataCenter *m_imageDataCenter;
  QList<ImageItem *> m_imageItemList;
  PlanWidget *m_planWidget;
  void setUpViewWidget();

protected:
  virtual void resizeEvent(QResizeEvent *event);
protected slots:
  void onOpen();
  void onQuit();
  void onImageBorderVisibility(ImageItem *);
  void onDataCenterResized();
  void onImportImageData(ImageItem *);
  void onShowDataCenter();
};

#endif // MAINWINDOW_H
