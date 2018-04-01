#include "TYCircleWidget.h"
#include "ui_TYCircleWidget.h"

TYCircleWidget::TYCircleWidget(QWidget *parent, vtkRenderer *ren,
                               vtkRenderWindowInteractor *interactor,
                               double *origion, double radius)
    : QWidget(parent), ui(new Ui::TYCircleWidget) {
  ui->setupUi(this);

  m_render = vtkSmartPointer<vtkRenderer>::New();
  m_render = ren;

  m_interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  m_interactor = interactor;

  m_radius = radius;
  double directionZ[3] = {0, 0, 1};
  // m_render->GetActiveCamera()->GetDirectionOfProjection(directionZ);
  for (int i = 0; i < 3; i++) {
    m_origion[i] = origion[i];
    m_origion[i] -= 2 * directionZ[i];
  }

  double direction1[3], direction2[3], direction3[3];

  double degreeInRadian = vtkMath::Pi() / 180 * 120;
  vtkMath::Perpendiculars(directionZ, direction1, nullptr, 0);
  vtkMath::Perpendiculars(directionZ, direction2, nullptr, degreeInRadian);
  vtkMath::Perpendiculars(directionZ, direction3, nullptr, degreeInRadian * 2);

  for (int i = 0; i < 3; i++) {
    m_point1[i] = m_origion[i] + direction1[i] * m_radius;
    m_point2[i] = m_origion[i] + direction2[i] * m_radius;
    m_point3[i] = m_origion[i] + direction3[i] * m_radius;
  }
  m_planeZ = vtkSmartPointer<vtkPlane>::New();
  m_planeZ->SetOrigin(m_origion);
  m_planeZ->SetNormal(0, 0, 1);

  m_connector = vtkSmartPointer<vtkEventQtSlotConnect>::New();
  m_circleActor = vtkSmartPointer<vtkActor>::New();
  m_circleActor->GetProperty()->SetColor(1, 0.5, 0.2);
  m_circleActor->GetProperty()->SetOpacity(0.7);
  m_circleActor->GetProperty()->SetLineWidth(4);
  m_render->AddActor(m_circleActor);

  this->InitializeWidget();
  this->OnUpDateCircle();
}

TYCircleWidget::~TYCircleWidget() { delete ui; }

void TYCircleWidget::SetOrigion(double *origion) {
  m_origionWidget->SetCenter(origion);

  this->OnUpDateCircle();
}

void TYCircleWidget::SetOrigion(double *origion, double *step) {

  double projectedOrigion[3];
  m_planeZ->ProjectPoint(origion, projectedOrigion);
  for (int i = 0; i < 3; i++) {
    step[i] = projectedOrigion[i] - m_origion[i];
  }
  m_origionWidget->SetCenter(origion);

  this->OnUpDateCircle();
}

void TYCircleWidget::GetPoints(vtkPoints *points) {
  auto pts = vtkSmartPointer<vtkPoints>::New();
  pts->Initialize();
  pts->InsertNextPoint(m_origion);
  pts->InsertNextPoint(m_point1);
  pts->InsertNextPoint(m_point2);
  pts->InsertNextPoint(m_point3);

  points->DeepCopy(pts);
}

void TYCircleWidget::GetStep(double *step) {
  for (int i = 0; i < 3; i++) {
    step[i] = m_step[i];
  }
}

void TYCircleWidget::GetOrigion(double *pt) {
  for (int i = 0; i < 3; i++) {
    pt[i] = m_origion[i];
  }
}

void TYCircleWidget::SetWidgetOff(bool flag) {
  if (flag) {
    m_origionWidget->Off();
    m_point1Widget->Off();
    m_point2Widget->Off();
    m_point3Widget->Off();
  } else {
    m_origionWidget->On();
    m_point1Widget->On();
    m_point2Widget->On();
    m_point3Widget->On();
  }
}

void TYCircleWidget::SetVisibility(bool flag) {
  if (flag) {
    m_circleActor->VisibilityOn();
    SetWidgetOff(false);
  } else {
    m_circleActor->VisibilityOff();
    SetWidgetOff(true);
  }
  m_render->GetRenderWindow()->Render();
}

void TYCircleWidget::GetPoints(double p1[], double p2[], double p3[]) {
  for (int i = 0; i < 3; i++) {
    p1[i] = m_point1[i];
    p2[i] = m_point2[i];
    p3[i] = m_point3[i];
  }
}

void TYCircleWidget::SetPoints(double p1[], double p2[], double p3[]) {
  m_point1Widget->SetCenter(p1);
  m_point2Widget->SetCenter(p2);
  m_point3Widget->SetCenter(p3);
  this->OnUpDateCircle();
}

void TYCircleWidget::InitializeWidget() {
  m_point1Widget = vtkSmartPointer<vtkSphereWidget>::New();
  this->SetUpWidget(m_point1Widget, m_point1);

  m_point2Widget = vtkSmartPointer<vtkSphereWidget>::New();
  this->SetUpWidget(m_point2Widget, m_point2);

  m_point3Widget = vtkSmartPointer<vtkSphereWidget>::New();
  this->SetUpWidget(m_point3Widget, m_point3);

  m_origionWidget = vtkSmartPointer<vtkSphereWidget>::New();
  this->SetUpWidget(m_origionWidget, m_origion);
}

void TYCircleWidget::SetUpWidget(vtkSphereWidget *widget, double *pt) {

  widget->SetCenter(pt);
  widget->SetRadius(15);
  widget->SetInteractor(m_interactor);
  widget->ScaleOff();
  widget->GetSphereProperty()->SetOpacity(0.4);
  widget->GetSelectedSphereProperty()->SetColor(0, 1, 0);
  widget->SetRepresentationToSurface();
  widget->GetSphereProperty()->SetColor(0, 1, 1);
  widget->On();
  m_connector->Connect(widget, vtkCommand::InteractionEvent, this,
                       SLOT(OnUpDateCircle()));
}

bool TYCircleWidget::isChanged(double a[], double b[]) {
  for (int i = 0; i < 3; i++) {
    bool isretun = a[i] != b[i] ? 1 : 0;
    if (isretun)
      return true;
  }
  return false;
}

void TYCircleWidget::BuildCircle(double *center, double radius,
                                 vtkPolyData *out) {
  auto points = vtkSmartPointer<vtkPoints>::New();
  points->Initialize();
  auto cells = vtkSmartPointer<vtkCellArray>::New();
  cells->Initialize();

  double directionZ[3] = {0, 0, 1};
  for (int i = 0; i < 360; i++) {
    double pt[3];
    double crossDirection[3];
    vtkMath::Perpendiculars(directionZ, crossDirection, nullptr,
                            i * vtkMath::Pi() / 180.0);
    for (int m = 0; m < 3; m++) {
      pt[m] = center[m] + crossDirection[m] * radius;
    }
    points->InsertNextPoint(pt);
    auto idlist = vtkSmartPointer<vtkIdList>::New();
    idlist->Initialize();
    if (i == 359) {
      idlist->InsertNextId(i);
      idlist->InsertNextId(0);
    } else {
      idlist->InsertNextId(i);
      idlist->InsertNextId(i + 1);
    }
    cells->InsertNextCell(idlist);
  }

  auto pd = vtkSmartPointer<vtkPolyData>::New();
  pd->SetPoints(points);
  pd->SetLines(cells);

  out->DeepCopy(pd);
}

void TYCircleWidget::CalculateCircle(double p1[], double p2[], double p3[],
                                     double outcenter[], double &outradius) {
  double a1, b1, c1, d1;
  double a2, b2, c2, d2;
  double a3, b3, c3, d3;

  double x1 = p1[0], y1 = p1[1], z1 = p1[2];
  double x2 = p2[0], y2 = p2[1], z2 = p2[2];
  double x3 = p3[0], y3 = p3[1], z3 = p3[2];

  a1 = (y1 * z2 - y2 * z1 - y1 * z3 + y3 * z1 + y2 * z3 - y3 * z2);
  b1 = -(x1 * z2 - x2 * z1 - x1 * z3 + x3 * z1 + x2 * z3 - x3 * z2);
  c1 = (x1 * y2 - x2 * y1 - x1 * y3 + x3 * y1 + x2 * y3 - x3 * y2);
  d1 = -(x1 * y2 * z3 - x1 * y3 * z2 - x2 * y1 * z3 + x2 * y3 * z1 +
         x3 * y1 * z2 - x3 * y2 * z1);

  a2 = 2 * (x2 - x1);
  b2 = 2 * (y2 - y1);
  c2 = 2 * (z2 - z1);
  d2 = x1 * x1 + y1 * y1 + z1 * z1 - x2 * x2 - y2 * y2 - z2 * z2;

  a3 = 2 * (x3 - x1);
  b3 = 2 * (y3 - y1);
  c3 = 2 * (z3 - z1);
  d3 = x1 * x1 + y1 * y1 + z1 * z1 - x3 * x3 - y3 * y3 - z3 * z3;

  outcenter[0] = -(b1 * c2 * d3 - b1 * c3 * d2 - b2 * c1 * d3 + b2 * c3 * d1 +
                   b3 * c1 * d2 - b3 * c2 * d1) /
                 (a1 * b2 * c3 - a1 * b3 * c2 - a2 * b1 * c3 + a2 * b3 * c1 +
                  a3 * b1 * c2 - a3 * b2 * c1);
  outcenter[1] = (a1 * c2 * d3 - a1 * c3 * d2 - a2 * c1 * d3 + a2 * c3 * d1 +
                  a3 * c1 * d2 - a3 * c2 * d1) /
                 (a1 * b2 * c3 - a1 * b3 * c2 - a2 * b1 * c3 + a2 * b3 * c1 +
                  a3 * b1 * c2 - a3 * b2 * c1);
  outcenter[2] = -(a1 * b2 * d3 - a1 * b3 * d2 - a2 * b1 * d3 + a2 * b3 * d1 +
                   a3 * b1 * d2 - a3 * b2 * d1) /
                 (a1 * b2 * c3 - a1 * b3 * c2 - a2 * b1 * c3 + a2 * b3 * c1 +
                  a3 * b1 * c2 - a3 * b2 * c1);
  double distance[3];
  for (int i = 0; i < 3; i++) {
    distance[i] = outcenter[i] - p1[i];
  }
  outradius =
      sqrt(pow(distance[0], 2) + pow(distance[1], 2) + pow(distance[2], 2));
}

void TYCircleWidget::OnUpDateCircle() {
  double p1[3], p2[3], p3[3], origion[3];
  m_point1Widget->GetCenter(p1);
  m_point2Widget->GetCenter(p2);
  m_point3Widget->GetCenter(p3);
  m_origionWidget->GetCenter(origion);
  int changedMark = 0;
  // if point1 changed
  if (isChanged(p1, m_point1) | isChanged(p2, m_point2) |
      isChanged(p3, m_point3)) {
    m_planeZ->ProjectPoint(p1, p1);
    m_planeZ->ProjectPoint(p2, p2);
    m_planeZ->ProjectPoint(p3, p3);
    double changedCenter[3], changedRadius;
    CalculateCircle(p1, p2, p3, changedCenter, changedRadius);
    for (int i = 0; i < 3; i++) {
      m_point1[i] = p1[i];
      m_point2[i] = p2[i];
      m_point3[i] = p3[i];
      m_step[i] = changedCenter[i] - m_origion[i];
      m_origion[i] = changedCenter[i];
    }
    m_radius = changedRadius;
    m_origionWidget->SetCenter(m_origion);
    m_point1Widget->SetCenter(m_point1);
    m_point2Widget->SetCenter(m_point2);
    m_point3Widget->SetCenter(m_point3);
    changedMark = 1;

  } else if (isChanged(origion, m_origion)) {
    m_planeZ->ProjectPoint(origion, origion);
    for (int i = 0; i < 3; i++) {
      double distance = origion[i] - m_origion[i];
      m_point1[i] += distance;
      m_point2[i] += distance;
      m_point3[i] += distance;
      m_step[i] = origion[i] - m_origion[i];
      m_origion[i] = origion[i];
    }
    m_origionWidget->SetCenter(m_origion);
    m_point1Widget->SetCenter(m_point1);
    m_point2Widget->SetCenter(m_point2);
    m_point3Widget->SetCenter(m_point3);
    changedMark = 2;
  }

  auto pd = vtkSmartPointer<vtkPolyData>::New();
  BuildCircle(m_origion, m_radius, pd);
  auto pdmapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  pdmapper->SetInputData(pd);

  m_circleActor->SetMapper(pdmapper);
  m_render->GetRenderWindow()->Render();

  emit circleChanged(changedMark);
}
