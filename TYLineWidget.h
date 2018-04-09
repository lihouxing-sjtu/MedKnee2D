#ifndef TYLINEWIDGET_H
#define TYLINEWIDGET_H

#include "stdafx.h"
#include <QWidget>
namespace Ui {
class TYLineWidget;
}

class TYLineWidget : public QWidget {
  Q_OBJECT

public:
  /// define by origion and  parameter
  explicit TYLineWidget(QWidget *parent, vtkRenderer *render = 0,
                        vtkRenderWindowInteractor *interactor = 0,
                        double *origion = 0, double proportion = 0.5,
                        double length = 100);

  ~TYLineWidget();

  void SetDottedLine(bool flag);
  void SetOrigion(double *pt);
  void GetPoints(vtkPoints *points);
  void GetOrigion(double *pt);
  void OrigionAddStep(double *step);
  void SetPoint1and2(double *p1, double *p2);
  void SetWidgetOff(bool isOff);
  void SetColor(double *color);
  void GetPoint1(double *pt);
  void GetPoint2(double *pt);
  double GetLength();
  void GetDirection(double direction[]);
  void SetOpacity(double op);
  void setProportion(double proportion);

private:
  Ui::TYLineWidget *ui;
  vtkSmartPointer<vtkPlane> m_planeZ;
  vtkSmartPointer<vtkRenderer> m_render;
  vtkSmartPointer<vtkRenderWindowInteractor> m_interactor;
  vtkSmartPointer<vtkSphereWidget> m_point1Widget;
  vtkSmartPointer<vtkSphereWidget> m_point2Widget;
  vtkSmartPointer<vtkSphereWidget> m_origionWidget;

  vtkSmartPointer<vtkEventQtSlotConnect> m_connector;
  double m_sphereRadius;
  double m_origionPoint[3];
  double m_point1[3];
  double m_point2[3];

  double m_proportion;
  double m_length;
  vtkSmartPointer<vtkActor> m_lineActor;
  bool isDottedLine;
  bool isInteractive;
  double m_color[3];

protected:
  void InitializeSphereWidget();
  void SetUpSphereWidget(vtkSmartPointer<vtkSphereWidget> widget, double *pt);
  bool IsChanged(double *a, double *b);
  void BuildLine(double *p1, double *p2, double segment, vtkPolyData *out);
signals:
  void lineChanged(int);
protected slots:
  void OnUpDateLine();
};

#endif // TYLINEWIDGET_H
