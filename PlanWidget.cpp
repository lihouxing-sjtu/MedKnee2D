#include "PlanWidget.h"
#include "ui_PlanWidget.h"

PlanWidget::PlanWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::PlanWidget) {
  ui->setupUi(this);

  m_imageData = vtkSmartPointer<vtkImageData>::New();
  m_mainviewImageActor = vtkSmartPointer<vtkImageSlice>::New();
  m_femurviewImageActor = vtkSmartPointer<vtkImageSlice>::New();
  m_kneeviewImageActor = vtkSmartPointer<vtkImageSlice>::New();
  m_ankleviewImageActor = vtkSmartPointer<vtkImageSlice>::New();

  m_eventConnector = vtkSmartPointer<vtkEventQtSlotConnect>::New();

  m_kneeupAngleFemur = 0;
  m_kneedownAngleAnkle = 0;
  m_ankleAnglekneedown = 0;
  m_kneeupAngleKneedown = 0;
  m_htoCutAngle = 0;

  this->InitializeView();
  this->InitialzeCircleLine();

  ui->BasicButton->setChecked(true);
  ui->HTOButton->setDisabled(true);
  ui->TKAButton->setDisabled(true);
  ui->HTOGroupBox->setDisabled(true);

  int width = this->width() / 6 > 200 ? this->width() / 6 : 200;
  ui->InformationWidget->setMaximumWidth(width);
  connect(ui->BasicButton, SIGNAL(clicked(bool)), this, SLOT(onBasicButton()));
  connect(ui->HTOButton, SIGNAL(clicked(bool)), this, SLOT(onHTOButton()));
  connect(ui->TKAButton, SIGNAL(clicked(bool)), this, SLOT(onTKAButton()));

  connect(ui->DataCenterButton, SIGNAL(clicked(bool)), this,
          SLOT(onDataCenter()));
  connect(ui->MinimizeButton, SIGNAL(clicked(bool)), this, SLOT(onMinimize()));
  connect(ui->QuitButton, SIGNAL(clicked(bool)), this, SLOT(onQuit()));
}

PlanWidget::~PlanWidget() { delete ui; }

void PlanWidget::setImageData(vtkImageData *img) {
  m_imageData = img;
  double range[2];
  m_imageData->GetScalarRange(range);
  double window = range[1] - range[0];
  double level = (range[1] + range[0]) / 2;

  this->setUpActor(m_mainviewImageActor, m_mainviewRener, window, level);
  this->setUpActor(m_femurviewImageActor, m_femurviewRener, window, level);
  this->setUpActor(m_kneeviewImageActor, m_kneeviewRener, window, level);
  this->setUpActor(m_ankleviewImageActor, m_ankleviewRener, window, level);
  resetCircleAndLines();
}

void PlanWidget::InitializeView() {
  m_mainviewRenWin = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
  m_mainviewRener = vtkSmartPointer<vtkRenderer>::New();
  m_mainviewInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  m_mainviewInteractStyle = vtkSmartPointer<vtkInteractorStyleImage>::New();

  m_mainviewRenWin->AddRenderer(m_mainviewRener);
  m_mainviewRenWin->SetInteractor(m_mainviewInteractor);
  m_mainviewInteractor->SetInteractorStyle(m_mainviewInteractStyle);

  ui->MainView->SetRenderWindow(m_mainviewRenWin);
  m_mainviewInteractor->Initialize();

  m_femurviewRenWin = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
  m_femurviewRener = vtkSmartPointer<vtkRenderer>::New();
  m_femurviewInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  m_femurviewInteractStyle = vtkSmartPointer<vtkInteractorStyleImage>::New();

  m_femurviewRenWin->AddRenderer(m_femurviewRener);
  m_femurviewRenWin->SetInteractor(m_femurviewInteractor);
  m_femurviewInteractor->SetInteractorStyle(m_femurviewInteractStyle);

  ui->FemurView->SetRenderWindow(m_femurviewRenWin);
  m_femurviewInteractor->Initialize();

  m_kneeviewRenWin = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
  m_kneeviewRener = vtkSmartPointer<vtkRenderer>::New();
  m_kneeviewInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  m_kneeviewInteractStyle = vtkSmartPointer<vtkInteractorStyleImage>::New();

  m_kneeviewRenWin->AddRenderer(m_kneeviewRener);
  m_kneeviewRenWin->SetInteractor(m_kneeviewInteractor);
  m_kneeviewInteractor->SetInteractorStyle(m_kneeviewInteractStyle);

  ui->KneeView->SetRenderWindow(m_kneeviewRenWin);
  m_kneeviewInteractor->Initialize();

  m_ankleviewRenWin = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
  m_ankleviewRener = vtkSmartPointer<vtkRenderer>::New();
  m_ankleviewInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  m_ankleviewInteractStyle = vtkSmartPointer<vtkInteractorStyleImage>::New();

  m_ankleviewRenWin->AddRenderer(m_ankleviewRener);
  m_ankleviewRenWin->SetInteractor(m_ankleviewInteractor);
  m_ankleviewInteractor->SetInteractorStyle(m_ankleviewInteractStyle);

  ui->AnkleView->SetRenderWindow(m_ankleviewRenWin);
  m_ankleviewInteractor->Initialize();

  this->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
}

void PlanWidget::setUpActor(vtkImageSlice *actor, vtkRenderer *ren,
                            double window, double level) {
  auto sliceMapper = vtkSmartPointer<vtkImageSliceMapper>::New();
  sliceMapper->SetInputData(m_imageData);
  sliceMapper->SetOrientationToZ();
  sliceMapper->SetSliceNumber(m_imageData->GetDimensions()[2] / 2);
  actor->SetMapper(sliceMapper);
  actor->GetProperty()->SetColorWindow(window);
  actor->GetProperty()->SetColorLevel(level);
  if (!ren->GetViewProps()->IsItemPresent(actor))
    ren->AddViewProp(actor);
  this->setUpCamera(2, ren);
  ren->ResetCamera();
  ren->GetRenderWindow()->Render();
}

void PlanWidget::setUpCamera(int orientation, vtkRenderer *render) {
  if (orientation > 3)
    return;
  double viewRight[4][3] = {{0, -1, 0}, {1, 0, 0}, {1, 0, 0}, {1, 0, 0}};
  double viewUp[4][3] = {{0, 0, 1}, {0, 0, 1}, {0, -1, 0}, {0, 0, 1}};
  vtkCamera *camera;

  camera = render->GetActiveCamera();
  camera->ParallelProjectionOn();

  double focus[3];
  camera->GetFocalPoint(focus);
  double d = camera->GetDistance();

  double cross[3];
  vtkMath::Cross(viewRight[orientation], viewUp[orientation], cross);

  camera->SetPosition(focus[0] + d * cross[0], focus[1] + d * cross[1],
                      focus[2] + d * cross[2]);
  camera->SetFocalPoint(focus);
  camera->SetViewUp(viewUp[orientation]);
}

void PlanWidget::InitialzeCircleLine() {
  double origion[3] = {0, 0, 0};
  double directionY[3] = {0, 1, 0};
  double directionX[3] = {1, 0, 0};
  int imageDimension[3];
  double imageSpacing[3], imagex, imagey;

  m_imageData->GetSpacing(imageSpacing);
  m_imageData->GetDimensions(imageDimension);
  m_imageData->GetOrigin(origion);

  imagex = imageSpacing[0] * imageDimension[0];
  imagey = imageSpacing[1] * imageDimension[1];

  double kneeUp[3], kneeDown[3], ankle[3];
  for (int i = 0; i < 3; i++) {
    origion[i] += imagex / 10 * directionX[i] + imagex / 20 * directionY[i];
    kneeUp[i] = origion[i] + directionY[i] * imagey / 2;
    kneeDown[i] = kneeUp[i] + directionY[i] * imagey / 20;
    ankle[i] = kneeDown[i] + directionY[i] * imagey / 3;
  }

  // interact widget
  m_femurCircleMainview =
      new TYCircleWidget(0, m_mainviewRener, m_mainviewInteractor, origion, 70);
  connect(m_femurCircleMainview, SIGNAL(circleChanged(int)), this,
          SLOT(onMainFemurChanged(int)));
  m_femurCircleSubview = new TYCircleWidget(0, m_femurviewRener,
                                            m_femurviewInteractor, origion, 70);
  connect(m_femurCircleSubview, SIGNAL(circleChanged(int)), this,
          SLOT(onSubFemurChanged(int)));

  m_kneeupLineMainview = new TYLineWidget(
      0, m_mainviewRener, m_mainviewInteractor, kneeUp, 0.5, 200);
  connect(m_kneeupLineMainview, SIGNAL(lineChanged(int)), this,
          SLOT(onMainKneeUpChanged(int)));
  m_kneeupLineSubview = new TYLineWidget(
      0, m_kneeviewRener, m_kneeviewInteractor, kneeUp, 0.5, 200);
  connect(m_kneeupLineSubview, SIGNAL(lineChanged(int)), this,
          SLOT(onSubKneeUpChanged(int)));

  m_kneedownLineMainview = new TYLineWidget(
      0, m_mainviewRener, m_mainviewInteractor, kneeDown, 0.5, 200);
  connect(m_kneedownLineMainview, SIGNAL(lineChanged(int)), this,
          SLOT(onMainKneeDownChanged(int)));

  m_kneedownLineSubview = new TYLineWidget(
      0, m_kneeviewRener, m_kneeviewInteractor, kneeDown, 0.5, 200);
  connect(m_kneedownLineSubview, SIGNAL(lineChanged(int)), this,
          SLOT(onSubKneeDownChanged(int)));

  m_ankleLineMainview = new TYLineWidget(0, m_mainviewRener,
                                         m_mainviewInteractor, ankle, 0.5, 200);
  connect(m_ankleLineMainview, SIGNAL(lineChanged(int)), this,
          SLOT(onMainAnkleChanged(int)));

  m_ankleLineSubview = new TYLineWidget(0, m_ankleviewRener,
                                        m_ankleviewInteractor, ankle, 0.5, 200);
  connect(m_ankleLineSubview, SIGNAL(lineChanged(int)), this,
          SLOT(onsubAnkleChanged(int)));

  // connect lines

  m_femurConnectKneeupLine =
      new TYLineWidget(0, m_mainviewRener, m_mainviewInteractor, origion);
  m_femurConnectKneeupLine->SetWidgetOff(true);
  m_femurConnectKneeupLine->SetPoint1and2(origion, kneeUp);

  m_kneedownConnectAnkleLine =
      new TYLineWidget(0, m_mainviewRener, m_mainviewInteractor, origion);
  m_kneedownConnectAnkleLine->SetWidgetOff(true);
  m_kneedownConnectAnkleLine->SetPoint1and2(kneeDown, ankle);

  m_femurConnectAnkleLine =
      new TYLineWidget(0, m_mainviewRener, m_mainviewInteractor, origion);
  m_femurConnectAnkleLine->SetWidgetOff(true);
  m_femurConnectAnkleLine->SetPoint1and2(origion, ankle);

  // hto lines
  double fujisawa[3];
  for (int i = 0; i < 3; i++)
    fujisawa[i] = (kneeUp[i] + kneeDown[i]) / 2;

  m_fujisawaLineMainview = new TYLineWidget(
      0, m_mainviewRener, m_mainviewInteractor, fujisawa, 1 - 0.625, 200);
  m_fujisawaLineMainview->SetOpacity(0);
  double colorRed[3] = {1, 0, 0};
  m_fujisawaLineMainview->SetColor(colorRed);
  connect(m_fujisawaLineMainview, SIGNAL(lineChanged(int)), this,
          SLOT(onMainFujisawaChanged(int)));

  m_fujisawaLineSubview = new TYLineWidget(
      0, m_kneeviewRener, m_kneeviewInteractor, fujisawa, 1 - 0.625, 200);
  m_fujisawaLineSubview->SetOpacity(0);
  m_fujisawaLineSubview->SetColor(colorRed);
  connect(m_fujisawaLineSubview, SIGNAL(lineChanged(int)), this,
          SLOT(onSubFujisawaChanged(int)));

  // hto connect lines
  m_femurConnectFujisawaLine = new TYLineWidget(
      0, m_mainviewRener, m_mainviewInteractor, origion, 0.5, 100);
  m_femurConnectFujisawaLine->SetPoint1and2(origion, fujisawa);
  m_femurConnectFujisawaLine->SetColor(colorRed);
  m_femurConnectFujisawaLine->SetOpacity(0);
  m_femurConnectFujisawaLine->SetWidgetOff(true);

  double extendPoint[3];
  for (int i = 0; i < 3; i++)
    extendPoint[i] = fujisawa[i] + fujisawa[i] - origion[i];
  m_extendFujisawaLine = new TYLineWidget(
      0, m_mainviewRener, m_mainviewInteractor, origion, 0.5, 100);
  m_extendFujisawaLine->SetPoint1and2(extendPoint, fujisawa);
  m_extendFujisawaLine->SetDottedLine(true);
  m_extendFujisawaLine->SetColor(colorRed);
  m_extendFujisawaLine->SetOpacity(0);
  m_extendFujisawaLine->SetWidgetOff(true);

  // hto cut lines
  double cutLinePoint[3];
  for (int i = 0; i < 3; i++)
    cutLinePoint[i] = directionY[i] * imagey / 40 + kneeDown[i];
  double colorYellow[3] = {1, 1, 0};
  m_htoCutLineMainview = new TYLineWidget(
      0, m_mainviewRener, m_mainviewInteractor, cutLinePoint, 0.5, 200);
  m_htoCutLineMainview->SetColor(colorYellow);
  m_htoCutLineMainview->SetOpacity(0);
  connect(m_htoCutLineMainview, SIGNAL(lineChanged(int)), this,
          SLOT(onMainHTOCutLineChanged(int)));

  m_htoCutLineSubview = new TYLineWidget(
      0, m_kneeviewRener, m_kneeviewInteractor, cutLinePoint, 0.5, 200);
  m_htoCutLineSubview->SetColor(colorYellow);
  m_htoCutLineSubview->SetOpacity(0);
  connect(m_htoCutLineSubview, SIGNAL(lineChanged(int)), this,
          SLOT(onSubHTOCutLineChanged(int)));
}

void PlanWidget::ResetFocusCamera(vtkRenderer *ren, double focalPT[],
                                  double scale) {

  vtkCamera *camera = ren->GetActiveCamera();
  double viewRight[3] = {1, 0, 0};
  double viewUp[3] = {0, -1, 0};
  double cross[3];
  vtkMath::Cross(viewRight, viewUp, cross);
  double d = camera->GetDistance();
  double pos[3];
  for (int i = 0; i < 3; i++) {
    pos[i] = focalPT[i] + cross[i] * d;
  }
  camera->SetPosition(pos);
  camera->SetFocalPoint(focalPT);
  camera->SetViewUp(viewUp);
  camera->SetParallelScale(scale);
}

void PlanWidget::upDateBasicConnectLines() {
  double femur[3], kneeup[3], kneedown[3], ankle[3];
  m_femurCircleMainview->GetOrigion(femur);

  m_kneeupLineMainview->GetOrigion(kneeup);

  m_kneedownLineMainview->GetOrigion(kneedown);

  m_ankleLineMainview->GetOrigion(ankle);

  m_femurConnectAnkleLine->SetPoint1and2(femur, ankle);
  m_femurConnectKneeupLine->SetPoint1and2(femur, kneeup);
  m_kneedownConnectAnkleLine->SetPoint1and2(kneedown, ankle);

  this->upDateBasicAngle();
}

void PlanWidget::upDateBasicAngle() {
  double femur[3];
  m_femurCircleMainview->GetOrigion(femur);

  double kneeupP1[3], kneeupP2[3], kneeup[3];
  m_kneeupLineMainview->GetPoint1(kneeupP1);
  m_kneeupLineMainview->GetPoint2(kneeupP2);
  m_kneeupLineMainview->GetOrigion(kneeup);

  double kneedownP1[3], kneedownP2[3], kneedown[3];
  m_kneedownLineMainview->GetPoint1(kneedownP1);
  m_kneedownLineMainview->GetPoint2(kneedownP2);
  m_kneedownLineMainview->GetOrigion(kneedown);

  double ankleP1[3], ankleP2[3], ankle[3];
  m_ankleLineMainview->GetPoint1(ankleP1);
  m_ankleLineMainview->GetPoint2(ankleP2);
  m_ankleLineMainview->GetOrigion(ankle);

  m_kneeupAngleFemur = computeVectorAngle(kneeupP1, kneeupP2, femur, kneeup);
  ui->KneeupAngleFemurSpinBox->setValue(m_kneeupAngleFemur);

  m_kneedownAngleAnkle =
      computeVectorAngle(kneedownP2, kneedownP1, ankle, kneedown);
  ui->KneedownAngleAnkleSpinBox->setValue(m_kneedownAngleAnkle);

  m_ankleAnglekneedown = computeVectorAngle(ankleP1, ankleP2, kneedown, ankle);
  ui->AnkleAngleKneedownSpinBox->setValue(m_ankleAnglekneedown);

  m_kneeupAngleKneedown =
      computeVectorAngle(kneeupP1, kneeupP2, kneedownP1, kneedownP2);
  ui->KneeupAngleKneedownSpinBox->setValue(m_kneeupAngleKneedown);

  this->upDateHTOAngle();
}

void PlanWidget::upDateHTOAngle() {
  double fujisawa[3], femur[3], ankle[3];
  m_fujisawaLineMainview->GetOrigion(fujisawa);
  m_femurCircleMainview->GetOrigion(femur);
  m_ankleLineMainview->GetOrigion(ankle);

  m_htoCutAngle = computeVectorAngle(fujisawa, femur, ankle, femur);
  ui->HTOCutAngleSpinBox->setValue(m_htoCutAngle);
}

double PlanWidget::computeVectorAngle(double p1[], double p2[], double q1[],
                                      double q2[]) {
  double v1[3], v2[3];
  for (int i = 0; i < 3; i++) {
    v1[i] = p1[i] - p2[i];
    v2[i] = q1[i] - q2[i];
  }
  return vtkMath::DegreesFromRadians(vtkMath::AngleBetweenVectors(v1, v2));
}

void PlanWidget::setBasicMode(bool flag) {
  if (flag) {
    m_femurCircleMainview->SetWidgetOff(false);
    m_femurCircleSubview->SetWidgetOff(false);
    m_kneeupLineMainview->SetWidgetOff(false);
    m_kneeupLineSubview->SetWidgetOff(false);
    m_kneedownLineMainview->SetWidgetOff(false);
    m_kneedownLineSubview->SetWidgetOff(false);
    m_ankleLineMainview->SetWidgetOff(false);
    m_ankleLineSubview->SetWidgetOff(false);
  } else {
    m_femurCircleMainview->SetWidgetOff(true);
    m_femurCircleSubview->SetWidgetOff(true);
    m_kneeupLineMainview->SetWidgetOff(true);
    m_kneeupLineSubview->SetWidgetOff(true);
    m_kneedownLineMainview->SetWidgetOff(true);
    m_kneedownLineSubview->SetWidgetOff(true);
    m_ankleLineMainview->SetWidgetOff(true);
    m_ankleLineSubview->SetWidgetOff(true);
  }
}

void PlanWidget::setHTOMode(bool flag) {
  double invisible = 0.1, visible = 0.7;
  if (flag) {
    m_fujisawaLineMainview->SetOpacity(0.7);
    m_fujisawaLineSubview->SetOpacity(0.7);
    m_femurConnectFujisawaLine->SetOpacity(0.7);
    m_extendFujisawaLine->SetOpacity(0.7);
    m_htoCutLineMainview->SetOpacity(0.7);
    m_htoCutLineSubview->SetOpacity(0.7);

    m_femurConnectKneeupLine->SetOpacity(invisible);
    m_kneedownConnectAnkleLine->SetOpacity(invisible);
    m_kneeupLineMainview->SetOpacity(invisible);
    m_kneedownLineMainview->SetOpacity(invisible);
    m_kneeupLineSubview->SetOpacity(invisible);
    m_kneedownLineSubview->SetOpacity(invisible);

  } else {
    m_fujisawaLineMainview->SetOpacity(0);
    m_fujisawaLineSubview->SetOpacity(0);
    m_femurConnectFujisawaLine->SetOpacity(0);
    m_extendFujisawaLine->SetOpacity(0);
    m_htoCutLineMainview->SetOpacity(0);
    m_htoCutLineSubview->SetOpacity(0);

    m_femurConnectKneeupLine->SetOpacity(visible);
    m_kneedownConnectAnkleLine->SetOpacity(visible);
    m_kneeupLineMainview->SetOpacity(visible);
    m_kneedownLineMainview->SetOpacity(visible);
    m_kneeupLineSubview->SetOpacity(visible);
    m_kneedownLineSubview->SetOpacity(visible);
  }
}

void PlanWidget::upDateHTOConnectLines() {
  double femur[3], fujisawa[3], extendPoint[3];
  m_femurCircleMainview->GetOrigion(femur);
  m_fujisawaLineMainview->GetOrigion(fujisawa);

  for (int i = 0; i < 3; i++)
    extendPoint[i] = fujisawa[i] + fujisawa[i] - femur[i];
  m_femurConnectFujisawaLine->SetPoint1and2(femur, fujisawa);
  m_extendFujisawaLine->SetPoint1and2(extendPoint, fujisawa);
  this->upDateHTOAngle();
}

void PlanWidget::resetCircleAndLines() {
  double origion[3] = {0, 0, 0};
  double directionY[3] = {0, 1, 0};
  double directionX[3] = {1, 0, 0};
  int imageDimension[3];
  double imageSpacing[3], imagex, imagey;

  m_imageData->GetSpacing(imageSpacing);
  m_imageData->GetDimensions(imageDimension);
  m_imageData->GetOrigin(origion);

  imagex = imageSpacing[0] * imageDimension[0];
  imagey = imageSpacing[1] * imageDimension[1];

  double kneeUp[3], kneeDown[3], ankle[3];
  for (int i = 0; i < 3; i++) {
    origion[i] += imagex / 10 * directionX[i] + imagex / 20 * directionY[i];
    kneeUp[i] = origion[i] + directionY[i] * imagey / 2;
    kneeDown[i] = kneeUp[i] + directionY[i] * imagey / 20;
    ankle[i] = kneeDown[i] + directionY[i] * imagey / 3;
  }

  // interact widget

  m_femurCircleMainview->SetOrigion(origion);
  m_femurCircleSubview->SetOrigion(origion);

  m_kneeupLineMainview->SetOrigion(kneeUp);
  m_kneeupLineSubview->SetOrigion(kneeUp);

  m_kneedownLineMainview->SetOrigion(kneeDown);

  m_kneedownLineSubview->SetOrigion(kneeDown);

  m_ankleLineMainview->SetOrigion(ankle);

  m_ankleLineSubview->SetOrigion(ankle);

  // connect lines

  m_femurConnectKneeupLine->SetPoint1and2(origion, kneeUp);
  m_kneedownConnectAnkleLine->SetPoint1and2(kneeDown, ankle);
  m_femurConnectAnkleLine->SetPoint1and2(origion, ankle);

  // hto lines
  double fujisawa[3];
  for (int i = 0; i < 3; i++)
    fujisawa[i] = (kneeUp[i] + kneeDown[i]) / 2;

  m_fujisawaLineMainview->SetOrigion(fujisawa);

  m_fujisawaLineSubview->SetOrigion(fujisawa);

  // hto  connect lines
  m_femurConnectFujisawaLine->SetPoint1and2(origion, fujisawa);

  double extendPoint[3];
  for (int i = 0; i < 3; i++)
    extendPoint[i] = fujisawa[i] + fujisawa[i] - origion[i];
  m_extendFujisawaLine->SetPoint1and2(extendPoint, fujisawa);

  // hto cut lines
  double cutLinePoint[3];
  for (int i = 0; i < 3; i++)
    cutLinePoint[i] = directionY[i] * imagey / 40 + kneeDown[i];
  m_htoCutLineMainview->SetOrigion(cutLinePoint);

  m_htoCutLineSubview->SetOrigion(cutLinePoint);
}

void PlanWidget::buildPlanePolyData(double origion[], double spacing[],
                                    double point1[], double point2[],
                                    vtkPolyData *out) {
  int xResolution =
      sqrt(vtkMath::Distance2BetweenPoints(origion, point1)) / spacing[0];
  int yResolution =
      sqrt(vtkMath::Distance2BetweenPoints(origion, point2)) / spacing[1];

  auto planeSource = vtkSmartPointer<vtkPlaneSource>::New();
  planeSource->SetOrigin(origion);
  planeSource->SetPoint1(point1);
  planeSource->SetPoint2(point2);
  planeSource->SetXResolution(xResolution);
  planeSource->SetYResolution(yResolution);
  planeSource->Update();

  out->DeepCopy(planeSource->GetOutput());
}

void PlanWidget::resizeEvent(QResizeEvent *event) {
  QWidget::resizeEvent(event);
  int width = this->width() / 6 > 200 ? this->width() / 6 : 200;
  ui->InformationWidget->setMaximumWidth(width);
}

void PlanWidget::onMainFemurChanged(int mark) {
  switch (mark) {
  case 0:
    return;
    break;
  case 1: {
    double p1[3], p2[3], p3[3];
    m_femurCircleMainview->GetPoints(p1, p2, p3);
    m_femurCircleSubview->SetPoints(p1, p2, p3);
    double origion[3];
    m_femurCircleMainview->GetOrigion(origion);
    this->ResetFocusCamera(m_femurviewRener, origion, 250);
    break;
  }
  case 2: {
    double origion[3];
    m_femurCircleMainview->GetOrigion(origion);
    m_femurCircleSubview->SetOrigion(origion);
    this->ResetFocusCamera(m_femurviewRener, origion, 250);
  }
  default:
    break;
  }
  this->upDateBasicConnectLines();
  this->upDateHTOConnectLines();
}

void PlanWidget::onSubFemurChanged(int mark) {
  switch (mark) {
  case 0:
    return;
    break;
  case 1: {
    double p1[3], p2[3], p3[3];
    m_femurCircleSubview->GetPoints(p1, p2, p3);
    m_femurCircleMainview->SetPoints(p1, p2, p3);
    break;
  }
  case 2: {
    double origion[3];
    m_femurCircleSubview->GetOrigion(origion);
    m_femurCircleMainview->SetOrigion(origion);
  }
  default:
    break;
  }
  this->upDateBasicConnectLines();
  this->upDateHTOConnectLines();
}

void PlanWidget::onMainKneeUpChanged(int mark) {
  double focalPoint[3];
  double origion1[3], origion2[3];
  m_kneeupLineMainview->GetOrigion(origion1);
  m_kneedownLineMainview->GetOrigion(origion2);
  for (int i = 0; i < 3; i++)
    focalPoint[i] = (origion1[i] + origion2[i]) / 2;
  switch (mark) {
  case 0:
    return;
    break;
  case 1: {
    double p1[3], p2[3];
    m_kneeupLineMainview->GetPoint1(p1);
    m_kneeupLineMainview->GetPoint2(p2);
    m_kneeupLineSubview->SetPoint1and2(p1, p2);
    this->ResetFocusCamera(m_kneeviewRener, focalPoint, 250);
    break;
  }
  case 2: {
    double origion[3];
    m_kneeupLineMainview->GetOrigion(origion);
    m_kneeupLineSubview->SetOrigion(origion);
    this->ResetFocusCamera(m_kneeviewRener, focalPoint, 250);
    break;
  }
  default:
    break;
  }
  this->upDateBasicConnectLines();
}

void PlanWidget::onMainKneeDownChanged(int mark) {
  double focalPoint[3];
  double origion1[3], origion2[3];
  m_kneeupLineMainview->GetOrigion(origion1);
  m_kneedownLineMainview->GetOrigion(origion2);
  for (int i = 0; i < 3; i++)
    focalPoint[i] = (origion1[i] + origion2[i]) / 2;
  switch (mark) {
  case 0:
    return;
    break;
  case 1: {
    double p1[3], p2[3];
    m_kneedownLineMainview->GetPoint1(p1);
    m_kneedownLineMainview->GetPoint2(p2);
    m_kneedownLineSubview->SetPoint1and2(p1, p2);
    this->ResetFocusCamera(m_kneeviewRener, focalPoint, 250);
    break;
  }
  case 2: {
    double origion[3];
    m_kneedownLineMainview->GetOrigion(origion);
    m_kneedownLineSubview->SetOrigion(origion);
    this->ResetFocusCamera(m_kneeviewRener, focalPoint, 250);
    break;
  }
  default:
    break;
  }
  this->upDateBasicConnectLines();
}

void PlanWidget::onSubKneeUpChanged(int mark) {
  switch (mark) {
  case 0:
    return;
    break;
  case 1: {
    double p1[3], p2[3];
    m_kneeupLineSubview->GetPoint1(p1);
    m_kneeupLineSubview->GetPoint2(p2);
    m_kneeupLineMainview->SetPoint1and2(p1, p2);
    break;
  }
  case 2: {
    double origion[3];
    m_kneeupLineSubview->GetOrigion(origion);
    m_kneeupLineMainview->SetOrigion(origion);
    break;
  }
  default:
    break;
  }
  this->upDateBasicConnectLines();
}

void PlanWidget::onSubKneeDownChanged(int mark) {
  switch (mark) {
  case 0:
    return;
    break;
  case 1: {
    double p1[3], p2[3];
    m_kneedownLineSubview->GetPoint1(p1);
    m_kneedownLineSubview->GetPoint2(p2);
    m_kneedownLineMainview->SetPoint1and2(p1, p2);
    break;
  }
  case 2: {
    double origion[3];
    m_kneedownLineSubview->GetOrigion(origion);
    m_kneedownLineMainview->SetOrigion(origion);
    break;
  }
  default:
    break;
  }
  this->upDateBasicConnectLines();
}

void PlanWidget::onMainAnkleChanged(int mark) {
  switch (mark) {
  case 0:
    return;
    break;
  case 1: {
    double p1[3], p2[3];
    m_ankleLineMainview->GetPoint1(p1);
    m_ankleLineMainview->GetPoint2(p2);
    m_ankleLineSubview->SetPoint1and2(p1, p2);
    double origion[3];
    m_ankleLineMainview->GetOrigion(origion);
    this->ResetFocusCamera(m_ankleviewRener, origion, 250);
    break;
  }
  case 2: {
    double origion[3];
    m_ankleLineMainview->GetOrigion(origion);
    m_ankleLineSubview->SetOrigion(origion);
    this->ResetFocusCamera(m_ankleviewRener, origion, 250);
    break;
  }
  default:
    break;
  }
  this->upDateBasicConnectLines();
}

void PlanWidget::onsubAnkleChanged(int mark) {
  switch (mark) {
  case 0:
    return;
    break;
  case 1: {
    double p1[3], p2[3];
    m_ankleLineSubview->GetPoint1(p1);
    m_ankleLineSubview->GetPoint2(p2);
    m_ankleLineMainview->SetPoint1and2(p1, p2);
    break;
  }
  case 2: {
    double origion[3];
    m_ankleLineSubview->GetOrigion(origion);
    m_ankleLineMainview->SetOrigion(origion);
    break;
  }
  default:
    break;
  }
  this->upDateBasicConnectLines();
}

void PlanWidget::onMainFujisawaChanged(int mark) {
  switch (mark) {
  case 0:
    return;
    break;
  case 1: {
    double p1[3], p2[3];
    m_fujisawaLineMainview->GetPoint1(p1);
    m_fujisawaLineMainview->GetPoint2(p2);
    m_fujisawaLineSubview->SetPoint1and2(p1, p2);
    break;
  }
  case 2: {
    double origion[3];
    m_fujisawaLineMainview->GetOrigion(origion);
    m_fujisawaLineSubview->SetOrigion(origion);
    break;
  }
  default:
    break;
  }
  this->upDateHTOConnectLines();
}

void PlanWidget::onSubFujisawaChanged(int mark) {
  switch (mark) {
  case 0:
    return;
    break;
  case 1: {
    double p1[3], p2[3];
    m_fujisawaLineSubview->GetPoint1(p1);
    m_fujisawaLineSubview->GetPoint2(p2);
    m_fujisawaLineMainview->SetPoint1and2(p1, p2);
    break;
  }
  case 2: {
    double origion[3];
    m_fujisawaLineSubview->GetOrigion(origion);
    m_fujisawaLineMainview->SetOrigion(origion);
    break;
  }
  default:
    break;
  }
  this->upDateHTOConnectLines();
}

void PlanWidget::onMainHTOCutLineChanged(int mark) {
  switch (mark) {
  case 0:
    return;
    break;
  case 1: {
    double p1[3], p2[3];
    m_htoCutLineMainview->GetPoint1(p1);
    m_htoCutLineMainview->GetPoint2(p2);
    m_htoCutLineSubview->SetPoint1and2(p1, p2);
    break;
  }
  case 2: {
    double origion[3];
    m_htoCutLineMainview->GetOrigion(origion);
    m_htoCutLineSubview->SetOrigion(origion);
    break;
  }
  default:
    break;
  }
}

void PlanWidget::onSubHTOCutLineChanged(int mark) {
  switch (mark) {
  case 0:
    return;
    break;
  case 1: {
    double p1[3], p2[3];
    m_htoCutLineSubview->GetPoint1(p1);
    m_htoCutLineSubview->GetPoint2(p2);
    m_htoCutLineMainview->SetPoint1and2(p1, p2);
    break;
  }
  case 2: {
    double origion[3];
    m_htoCutLineSubview->GetOrigion(origion);
    m_htoCutLineMainview->SetOrigion(origion);
    break;
  }
  default:
    break;
  }
}

void PlanWidget::onBasicButton() {
  if (ui->BasicButton->isChecked()) {
    ui->HTOButton->setDisabled(true);
    ui->TKAButton->setDisabled(true);
    this->setBasicMode(true);
  } else {
    ui->HTOButton->setDisabled(false);
    ui->TKAButton->setDisabled(false);
    this->setBasicMode(false);
  }
}

void PlanWidget::onHTOButton() {
  if (ui->HTOButton->isChecked()) {
    ui->BasicButton->setDisabled(true);
    ui->TKAButton->setDisabled(true);

    ui->HTOGroupBox->setDisabled(false);
    this->setHTOMode(true);
  } else {
    ui->BasicButton->setDisabled(false);
    ui->TKAButton->setDisabled(false);

    ui->HTOGroupBox->setDisabled(true);
    this->setHTOMode(false);
  }
}

void PlanWidget::onTKAButton() {
  if (ui->TKAButton->isChecked()) {
    ui->BasicButton->setDisabled(true);
    ui->HTOButton->setDisabled(true);
  } else {
    ui->BasicButton->setDisabled(false);
    ui->HTOButton->setDisabled(false);
  }
}

void PlanWidget::onQuit() {
  this->close();
  emit quitApp();
}

void PlanWidget::onMinimize() { this->showMinimized(); }

void PlanWidget::onDataCenter() { emit showDataCenter(); }
