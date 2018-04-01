#ifndef PLANWIDGET_H
#define PLANWIDGET_H

#include "TYCircleWidget.h"
#include "TYLineWidget.h"
#include "stdafx.h"
#include <QWidget>

namespace Ui {
class PlanWidget;
}

class PlanWidget : public QWidget {
  Q_OBJECT

public:
  explicit PlanWidget(QWidget *parent = 0);
  ~PlanWidget();
  void setImageData(vtkImageData *img);

private:
  Ui::PlanWidget *ui;

  vtkSmartPointer<vtkGenericOpenGLRenderWindow> m_mainviewRenWin;
  vtkSmartPointer<vtkRenderer> m_mainviewRener;
  vtkSmartPointer<vtkRenderWindowInteractor> m_mainviewInteractor;
  vtkSmartPointer<vtkInteractorStyleImage> m_mainviewInteractStyle;

  vtkSmartPointer<vtkGenericOpenGLRenderWindow> m_femurviewRenWin;
  vtkSmartPointer<vtkRenderer> m_femurviewRener;
  vtkSmartPointer<vtkRenderWindowInteractor> m_femurviewInteractor;
  vtkSmartPointer<vtkInteractorStyleImage> m_femurviewInteractStyle;

  vtkSmartPointer<vtkGenericOpenGLRenderWindow> m_kneeviewRenWin;
  vtkSmartPointer<vtkRenderer> m_kneeviewRener;
  vtkSmartPointer<vtkRenderWindowInteractor> m_kneeviewInteractor;
  vtkSmartPointer<vtkInteractorStyleImage> m_kneeviewInteractStyle;

  vtkSmartPointer<vtkGenericOpenGLRenderWindow> m_ankleviewRenWin;
  vtkSmartPointer<vtkRenderer> m_ankleviewRener;
  vtkSmartPointer<vtkRenderWindowInteractor> m_ankleviewInteractor;
  vtkSmartPointer<vtkInteractorStyleImage> m_ankleviewInteractStyle;

  vtkSmartPointer<vtkImageData> m_imageData;

  vtkSmartPointer<vtkImageSlice> m_mainviewImageActor;
  vtkSmartPointer<vtkImageSlice> m_femurviewImageActor;
  vtkSmartPointer<vtkImageSlice> m_kneeviewImageActor;
  vtkSmartPointer<vtkImageSlice> m_ankleviewImageActor;

  vtkSmartPointer<vtkEventQtSlotConnect> m_eventConnector;

  TYCircleWidget *m_femurCircleMainview;
  TYCircleWidget *m_femurCircleSubview;

  TYLineWidget *m_kneeupLineMainview;
  TYLineWidget *m_kneeupLineSubview;

  TYLineWidget *m_kneedownLineMainview;
  TYLineWidget *m_kneedownLineSubview;

  TYLineWidget *m_ankleLineMainview;
  TYLineWidget *m_ankleLineSubview;

  TYLineWidget *m_femurConnectKneeupLine;
  TYLineWidget *m_kneedownConnectAnkleLine;
  TYLineWidget *m_femurConnectAnkleLine;

  double m_kneeupAngleFemur;
  double m_kneedownAngleAnkle;
  double m_ankleAnglekneedown;
  double m_kneeupAngleKneedown;

  TYLineWidget *m_fujisawaLineMainview;
  TYLineWidget *m_fujisawaLineSubview;

  TYLineWidget *m_femurConnectFujisawaLine;
  TYLineWidget *m_extendFujisawaLine;

  TYLineWidget *m_htoCutLineMainview;
  TYLineWidget *m_htoCutLineSubview;

  double m_htoCutAngle;

protected:
  void InitializeView();
  void setUpActor(vtkImageSlice *actor, vtkRenderer *ren, double window,
                  double level);
  void setUpCamera(int orientation, vtkRenderer *render);
  void InitialzeCircleLine();
  void ResetFocusCamera(vtkRenderer *ren, double focalPT[],
                        double distance = 10);
  void upDateBasicConnectLines();
  void upDateBasicAngle();
  void upDateHTOAngle();
  double computeVectorAngle(double p1[], double p2[], double q1[], double q2[]);
  void setBasicMode(bool flag);
  void setHTOMode(bool flag);
  void upDateHTOConnectLines();
  void resetCircleAndLines();

  void buildPlanePolyData(double origion[], double spacing[], double point1[],
                          double point2[], vtkPolyData *out);
  virtual void resizeEvent(QResizeEvent *event);
signals:
  void quitApp();
  void showDataCenter();
protected slots:
  void onMainFemurChanged(int);
  void onSubFemurChanged(int);

  void onMainKneeUpChanged(int);
  void onMainKneeDownChanged(int);

  void onSubKneeUpChanged(int);
  void onSubKneeDownChanged(int);

  void onMainAnkleChanged(int);
  void onsubAnkleChanged(int);

  void onMainFujisawaChanged(int);
  void onSubFujisawaChanged(int);

  void onMainHTOCutLineChanged(int);
  void onSubHTOCutLineChanged(int);

  void onBasicButton();
  void onHTOButton();
  void onTKAButton();

  void onQuit();
  void onMinimize();
  void onDataCenter();
};

#endif // PLANWIDGET_H
