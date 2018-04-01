#ifndef TYCIRCLEWIDGET_H
#define TYCIRCLEWIDGET_H
#include "stdafx.h"
#include <QWidget>

namespace Ui {
class TYCircleWidget;
}

class TYCircleWidget : public QWidget {
  Q_OBJECT

public:
  explicit TYCircleWidget(QWidget *parent = 0, vtkRenderer *ren = 0,
                          vtkRenderWindowInteractor *interactor = 0,
                          double *origion = 0, double radius = 80);
  ~TYCircleWidget();

  void SetOrigion(double *origion);
  void SetOrigion(double *origion, double *step);
  void GetPoints(vtkPoints *points);
  void GetStep(double *step);
  void GetOrigion(double *pt);
  void SetWidgetOff(bool flag);
  void SetVisibility(bool flag);
  void GetPoints(double p1[], double p2[], double p3[]);
  void SetPoints(double p1[], double p2[], double p3[]);

private:
  Ui::TYCircleWidget *ui;

  double m_origion[3], m_point1[3], m_point2[3], m_point3[3];
  double m_radius;

  vtkSmartPointer<vtkRenderer> m_render;
  vtkSmartPointer<vtkRenderWindowInteractor> m_interactor;

  vtkSmartPointer<vtkSphereWidget> m_point1Widget;
  vtkSmartPointer<vtkSphereWidget> m_point2Widget;
  vtkSmartPointer<vtkSphereWidget> m_point3Widget;
  vtkSmartPointer<vtkSphereWidget> m_origionWidget;

  vtkSmartPointer<vtkEventQtSlotConnect> m_connector;
  vtkSmartPointer<vtkActor> m_circleActor;

  vtkSmartPointer<vtkPlane> m_planeZ;
  double m_step[3];

protected:
  void InitializeWidget();
  void SetUpWidget(vtkSphereWidget *widget, double *pt);
  bool isChanged(double p1[], double p2[]);
  void BuildCircle(double *center, double radius, vtkPolyData *out);
  void CalculateCircle(double p1[], double p2[], double p3[],
                       double outcenter[], double &outradius);

signals:
  void circleChanged(int);
protected slots:
  void OnUpDateCircle();
};

#endif // TYCIRCLEWIDGET_H
