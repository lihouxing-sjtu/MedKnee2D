#include "TYLineWidget.h"
#include "ui_TYLineWidget.h"

TYLineWidget::TYLineWidget(QWidget *parent, vtkRenderer *render,
                           vtkRenderWindowInteractor *interactor,
                           double *origion, double proportion, double length)
    : QWidget(parent), ui(new Ui::TYLineWidget) {
  ui->setupUi(this);
  m_render = vtkSmartPointer<vtkRenderer>::New();
  m_render = render;
  m_interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  m_interactor = interactor;
  double directionZ[3];
  m_render->GetActiveCamera()->GetDirectionOfProjection(directionZ);
  for (int i = 0; i < 3; i++) {
    m_origionPoint[i] -= 2 * directionZ[i];
  }
  m_proportion = proportion;
  m_length = length;

  for (int i = 0; i < 3; i++) {
    m_origionPoint[i] = origion[i];
    m_point1[i] = m_origionPoint[i];
    m_point2[i] = m_origionPoint[i];
  }
  m_point1[0] -= m_proportion * m_length;
  m_point2[0] += (1 - m_proportion) * m_length;

  m_planeZ = vtkSmartPointer<vtkPlane>::New();
  m_planeZ->SetOrigin(m_origionPoint);
  m_planeZ->SetNormal(0, 0, 1);

  m_sphereRadius = 10;
  m_connector = vtkSmartPointer<vtkEventQtSlotConnect>::New();
  m_lineActor = vtkSmartPointer<vtkActor>::New();
  m_lineActor->GetProperty()->SetColor(1, 0.5, 0.2);
  m_lineActor->GetProperty()->SetOpacity(0.7);
  m_lineActor->GetProperty()->SetLineWidth(4);
  m_render->AddActor(m_lineActor);
  isInteractive = true;
  isDottedLine = false;
  this->InitializeSphereWidget();
  OnUpDateLine();
}
TYLineWidget::~TYLineWidget() { delete ui; }

void TYLineWidget::SetDottedLine(bool flag) { isDottedLine = flag; }

void TYLineWidget::SetOrigion(double *pt) {
  m_origionWidget->SetCenter(pt);
  // m_origionWidget->Off();
  isInteractive = false;
  this->OnUpDateLine();
}

void TYLineWidget::GetPoints(vtkPoints *points) {
  auto pts = vtkSmartPointer<vtkPoints>::New();

  pts->Initialize();
  pts->InsertNextPoint(m_point1);
  pts->InsertNextPoint(m_origionPoint);
  pts->InsertNextPoint(m_point2);
  points->DeepCopy(pts);
}

void TYLineWidget::GetOrigion(double *pt) {
  for (int i = 0; i < 3; i++) {
    pt[i] = m_origionPoint[i];
  }
}

void TYLineWidget::OrigionAddStep(double *step) {
  double origionChanged[3];

  for (int i = 0; i < 3; i++) {
    origionChanged[i] = m_origionPoint[i];
    origionChanged[i] += step[i];
  }
  isInteractive = false;
  m_origionWidget->SetCenter(origionChanged);
  this->OnUpDateLine();
}

void TYLineWidget::SetPoint1and2(double *p1, double *p2) {
  isInteractive = false;
  m_point1Widget->SetCenter(p1);
  this->OnUpDateLine();
  isInteractive = false;
  m_point2Widget->SetCenter(p2);
  this->OnUpDateLine();
}

void TYLineWidget::SetWidgetOff(bool isOff) {
  if (isOff) {
    m_point1Widget->Off();
    m_point2Widget->Off();
    m_origionWidget->Off();
  } else {
    m_point1Widget->On();
    m_point2Widget->On();
    m_origionWidget->On();
  }
}

void TYLineWidget::SetColor(double *color) {
  for (int i = 0; i < 3; i++)
    m_color[i] = color[i];
  m_lineActor->GetProperty()->SetColor(m_color);
  m_render->GetRenderWindow()->Render();
}

void TYLineWidget::GetPoint1(double *pt) {
  for (int i = 0; i < 3; i++)
    pt[i] = m_point1[i];
}

void TYLineWidget::GetPoint2(double *pt) {
  for (int i = 0; i < 3; i++)
    pt[i] = m_point2[i];
}

double TYLineWidget::GetLength() {
  double distance = sqrt(vtkMath::Distance2BetweenPoints(m_point1, m_point2));
  return distance;
}

void TYLineWidget::GetDirection(double direction[]) {
  for (int i = 0; i < 3; i++) {
    direction[i] = m_point1[i] - m_point2[i];
  }
  vtkMath::Normalize(direction);
}

void TYLineWidget::SetOpacity(double op) {
  m_point1Widget->GetSphereProperty()->SetOpacity(op);
  m_point2Widget->GetSphereProperty()->SetOpacity(op);
  m_origionWidget->GetSphereProperty()->SetOpacity(op);

  m_lineActor->GetProperty()->SetOpacity(op);
  m_render->GetRenderWindow()->Render();
}

void TYLineWidget::setProportion(double proportion) {
  double direction[3];
  for (int i = 0; i < 3; i++) {
    direction[i] = m_point2[i] - m_point1[i];
  }
  vtkMath::Normalize(direction);
  double distance = sqrt(vtkMath::Distance2BetweenPoints(m_point1, m_point2));

  for (int i = 0; i < 3; i++) {
    m_origionPoint[i] = m_point1[i] + distance * direction[i] * proportion;
  }
  m_origionWidget->SetCenter(m_origionPoint);
  emit lineChanged(2);
}

void TYLineWidget::setWidgetOff(int index, bool isOff) {
  if (isOff) {
    switch (index) {
    case 1:
      m_point1Widget->Off();
      break;
    case 2:
      m_point2Widget->Off();
      break;
    case 0:
      m_origionWidget->Off();
      break;
    default:
      break;
    }
  } else {
    switch (index) {
    case 1:
      m_point1Widget->On();
      break;
    case 2:
      m_point2Widget->On();
      break;
    case 0:
      m_origionWidget->On();
      break;
    default:
      break;
    }
  }
}

void TYLineWidget::InitializeSphereWidget() {
  m_point1Widget = vtkSmartPointer<vtkSphereWidget>::New();
  this->SetUpSphereWidget(m_point1Widget, m_point1);

  m_point2Widget = vtkSmartPointer<vtkSphereWidget>::New();
  this->SetUpSphereWidget(m_point2Widget, m_point2);

  m_origionWidget = vtkSmartPointer<vtkSphereWidget>::New();
  this->SetUpSphereWidget(m_origionWidget, m_origionPoint);
}

void TYLineWidget::SetUpSphereWidget(vtkSmartPointer<vtkSphereWidget> widget,
                                     double *pt) {
  widget->SetCenter(pt);
  widget->SetRadius(m_sphereRadius);
  widget->SetInteractor(m_interactor);
  widget->ScaleOff();
  widget->GetSphereProperty()->SetOpacity(0.4);
  widget->GetSelectedSphereProperty()->SetColor(0, 1, 0);
  widget->GetSelectedSphereProperty()->SetOpacity(0.4);
  widget->SetRepresentationToSurface();
  widget->GetSphereProperty()->SetColor(0, 1, 1);
  widget->On();
  m_connector->Connect(widget, vtkCommand::InteractionEvent, this,
                       SLOT(OnUpDateLine()));
}
/// return true if a !=b, size of a and b should be  3
bool TYLineWidget::IsChanged(double *a, double *b) {
  for (int i = 0; i < 3; i++) {
    bool isretun = a[i] != b[i] ? 1 : 0;
    if (isretun)
      return true;
  }
  return false;
}

void TYLineWidget::BuildLine(double *p1, double *p2, double segment,
                             vtkPolyData *out) {
  auto points = vtkSmartPointer<vtkPoints>::New();
  points->Initialize();
  auto cells = vtkSmartPointer<vtkCellArray>::New();
  cells->Initialize();
  /// solid line
  if (!segment) {
    points->InsertNextPoint(p1);
    points->InsertNextPoint(p2);

    auto idlist = vtkSmartPointer<vtkIdList>::New();
    idlist->Initialize();
    idlist->InsertNextId(0);
    idlist->InsertNextId(1);
    cells->InsertNextCell(idlist);
  } else {
    double distance = sqrt(vtkMath::Distance2BetweenPoints(m_point1, m_point2));
    double numOfPoints = distance / segment;
    double direction[3];
    for (int i = 0; i < 3; i++) {
      direction[i] = p2[i] - p1[i];
    }
    vtkMath::Normalize(direction);

    for (int i = 0; i < numOfPoints; i++) {
      double pt[3];
      for (int j = 0; j < 3; j++) {
        pt[j] = p1[j] + direction[j] * i * segment;
      }
      points->InsertNextPoint(pt);
    }

    for (int i = 0; i < numOfPoints; i = i + 2) {
      auto idlist = vtkSmartPointer<vtkIdList>::New();
      idlist->Initialize();
      if (i + 2 >= numOfPoints)
        continue;
      idlist->InsertNextId(i);
      idlist->InsertNextId(i + 1);
      cells->InsertNextCell(idlist);
    }
  }
  auto polydata = vtkSmartPointer<vtkPolyData>::New();
  polydata->SetPoints(points);
  polydata->SetLines(cells);

  out->DeepCopy(polydata);
}
/// update when spherewidget changed position
void TYLineWidget::OnUpDateLine() {

  double p1[3], p2[3], origion[3];
  m_point1Widget->GetCenter(p1);
  m_point2Widget->GetCenter(p2);
  m_origionWidget->GetCenter(origion);
  int changeMark = 0;
  // if point1 changed
  if (IsChanged(p1, m_point1)) {
    m_planeZ->ProjectPoint(p1, p1);
    for (int i = 0; i < 3; i++) {
      double distance;
      distance = p2[i] - p1[i];
      m_origionPoint[i] = p1[i] + distance * m_proportion;
      m_point1[i] = p1[i];
    }
    m_point1Widget->SetCenter(m_point1);
    m_origionWidget->SetCenter(m_origionPoint);
    changeMark = 1;
  } /// if origion changed
  else if (IsChanged(origion, m_origionPoint)) {
    m_planeZ->ProjectPoint(origion, origion);
    for (int i = 0; i < 3; i++) {
      double distance = origion[i] - m_origionPoint[i];
      m_point1[i] += distance;
      m_point2[i] += distance;
      m_origionPoint[i] = origion[i];
    }
    m_origionWidget->SetCenter(m_origionPoint);
    m_point1Widget->SetCenter(m_point1);
    m_point2Widget->SetCenter(m_point2);
    changeMark = 2;
  } else if (IsChanged(p2, m_point2)) {
    m_planeZ->ProjectPoint(p2, p2);
    for (int i = 0; i < 3; i++) {
      double distance;
      distance = p2[i] - p1[i];
      m_origionPoint[i] = p1[i] + distance * m_proportion;
      m_point2[i] = p2[i];
    }
    m_point2Widget->SetCenter(m_point2);
    m_origionWidget->SetCenter(m_origionPoint);
    changeMark = 1;
  }

  auto polydata = vtkSmartPointer<vtkPolyData>::New();
  this->BuildLine(m_point1, m_point2, isDottedLine * 20, polydata);

  auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputData(polydata);

  m_lineActor->SetMapper(mapper);
  m_render->GetRenderWindow()->Render();
  if (isInteractive)
    emit lineChanged(changeMark);
  else {
    isInteractive = true;
  }
}
