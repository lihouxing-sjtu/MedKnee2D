#include "ImageItem.h"
#include "ui_ImageItem.h"

ImageItem::ImageItem(QWidget *parent, std::vector<std::string> path)
    : QWidget(parent), ui(new Ui::ImageItem) {
  ui->setupUi(this);

  m_render = vtkSmartPointer<vtkRenderer>::New();
  m_rendWin = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
  m_rendInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  m_rendInteractorStyle = vtkSmartPointer<vtkInteractorStyleImage>::New();

  ui->ViewWidget->SetRenderWindow(m_rendWin);
  m_rendWin->AddRenderer(m_render);
  m_rendWin->SetInteractor(m_rendInteractor);
  m_rendInteractor->SetInteractorStyle(m_rendInteractorStyle);
  m_rendInteractor->Initialize();
  m_imagePath = path;
  m_borderActor = vtkSmartPointer<vtkActor2D>::New();

  m_eventConnector = vtkSmartPointer<vtkEventQtSlotConnect>::New();
  m_eventConnector->Connect(m_rendInteractor, vtkCommand::LeftButtonPressEvent,
                            this, SLOT(onUpdateFrame()));
  bool isread = this->readDicom();
  if (isread) {
    double range[2];
    m_imageData->GetScalarRange(range);
    auto sliceMapper = vtkSmartPointer<vtkImageSliceMapper>::New();
    sliceMapper->SetInputData(m_imageData);
    sliceMapper->SetOrientationToZ();
    sliceMapper->SetSliceNumber(m_imageData->GetDimensions()[2] / 2);
    m_imageActor = vtkSmartPointer<vtkImageSlice>::New();
    m_imageActor->SetMapper(sliceMapper);
    m_imageActor->GetProperty()->SetColorLevel((range[0] + range[1]) / 2.0 +
                                               range[0]);
    m_imageActor->GetProperty()->SetColorWindow(range[1] - range[0]);

    m_render->AddViewProp(m_imageActor);
    setUpCamera(2);
    m_render->ResetCamera();
    m_rendWin->Render();
  }
  int viewWidth = ui->ViewWidget->width();
  int viewHight = ui->ViewWidget->height();

  ui->ImportButton->setGeometry(viewWidth * 0.625,
                                viewHight - viewWidth / 5 - 20, viewWidth / 5,
                                viewWidth / 5);

  connect(ui->ImportButton, SIGNAL(clicked(bool)), this, SLOT(onImportItem()));
}

ImageItem::~ImageItem() { delete ui; }

QVTKOpenGLWidget *ImageItem::GetViewWidget() { return ui->ViewWidget; }

void ImageItem::SetBorderOff() {
  m_borderActor->VisibilityOff();
  ui->ImportButton->setVisible(false);
  m_rendWin->Render();
}

bool ImageItem::readDicom() {
  typedef itk::GDCMImageIO ImageIOType;
  ImageIOType::Pointer gdcmIO = ImageIOType::New();
  typedef itk::ImageSeriesReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetImageIO(gdcmIO);
  reader->SetFileNames(m_imagePath);
  try {
    reader->Update();
  } catch (itk::ExceptionObject &e) {
    m_readFailedMsg = QString(e.GetDescription());
    emit readFailed(m_readFailedMsg);
    qDebug() << m_readFailedMsg;
    return false;
  }
  auto tovtk = itk::ImageToVTKImageFilter<ImageType>::New();
  tovtk->SetInput(reader->GetOutput());
  tovtk->Update();
  m_imageData = vtkSmartPointer<vtkImageData>::New();
  m_imageData->DeepCopy(tovtk->GetOutput());
  this->InsertTagVolume();
  this->GetImageTagValue(gdcmIO);

  return true;
}

void ImageItem::setUpCamera(int orientation) {
  if (orientation > 3)
    return;
  double viewRight[4][3] = {{0, -1, 0}, {1, 0, 0}, {1, 0, 0}, {1, 0, 0}};
  double viewUp[4][3] = {{0, 0, 1}, {0, 0, 1}, {0, -1, 0}, {0, 0, 1}};
  vtkCamera *camera;

  camera = m_render->GetActiveCamera();
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

void ImageItem::upDateFrame() {
  if (m_borderActor->GetVisibility())
    this->onUpdateFrame();
}

vtkImageData *ImageItem::GetImageData() { return m_imageData; }

QString ImageItem::getTagInformation() { return m_tagvaluelist; }

void ImageItem::GetImageTagValue(itk::GDCMImageIO::Pointer gdcmio) {
  QList<QMap<QString, QString>> tagMaps = {
      TagMapVolume, TagMapImaging, TagMapPatient, TagMapStudy, TagMapSeries};

  QString title[5] = {tr("- Volume  -"), tr("- Imaging -"), tr("- Patient -"),
                      tr("- Study   -"), tr("- Series  -")};

  for (int i = 0; i < 5; ++i) {
    m_tagvaluelist += title[i] + "\n";
    foreach (auto key, tagMaps[i].keys()) {
      std::string value;
      gdcmio->GetValueFromTag(tagMaps[i].value(key).toStdString(), value);
      m_tagvaluelist +=
          key + ": " + QString::fromLocal8Bit(value.c_str()) + "\n";
    }
    if (tagMaps[i] != tagMaps.last())
      m_tagvaluelist += "\n";
  }

  std::string value;
  m_tabletags[0] = QString::number(m_imagePath.size());
  gdcmio->GetValueFromTag(std::string("0018|0050"), value);
  m_tabletags[1] = QString::fromStdString(value);
  gdcmio->GetValueFromTag(std::string("0018|0088"), value);
  m_tabletags[2] = QString::fromStdString(value);
  gdcmio->GetValueFromTag(std::string("0008|0060"), value);
  m_tabletags[3] = QString::fromStdString(value);
  gdcmio->GetValueFromTag(std::string("0010|0020"), value);
  m_tabletags[4] = QString::fromStdString(value);
  gdcmio->GetValueFromTag(std::string("0008|1030"), value);
  m_tabletags[5] = QString::fromStdString(value);
  gdcmio->GetValueFromTag(std::string("0008|103e"), value);
  m_tabletags[6] = QString::fromStdString(value);
}

void ImageItem::InsertTagVolume() {
  TagMapVolume.clear();
  TagMapImaging.clear();
  TagMapPatient.clear();
  TagMapStudy.clear();
  TagMapSeries.clear();

  TagMapVolume.insert(tr("01. Slice Thickness"), "0018|0050");
  TagMapVolume.insert(tr("02. Pixel Spacing"), "0028|0030");
  TagMapVolume.insert(tr("03. Rows"), "0028|0010");
  TagMapVolume.insert(tr("04. Columns"), "0028|0011");

  TagMapImaging.insert(tr("01. Image Type"), "0008|0008");
  TagMapImaging.insert(tr("02. Modality"), "0008|0060");
  TagMapImaging.insert(tr("03. Contrast/Bolus Agent"), "0018|0010");
  TagMapImaging.insert(tr("04. Repetition Time"), "0018|0080");
  TagMapImaging.insert(tr("05. Echo Time"), "0018|0081");
  TagMapImaging.insert(tr("06. Echo Number(s)"), "0018|0086");
  TagMapImaging.insert(tr("07. Scanning Sequence"), "0018|0020");
  TagMapImaging.insert(tr("08. Manufacturer"), "0008|0070");
  TagMapImaging.insert(tr("09. Institution Name"), "0008|0080");
  TagMapImaging.insert(tr("10. Institution Address"), "0008|0081");

  TagMapPatient.insert(tr("01. Patient's Name"), "0010|0010");
  TagMapPatient.insert(tr("02. Patient's Sex"), "0010|0040");
  TagMapPatient.insert(tr("03. Patient's Age"), "0010|1010");
  TagMapPatient.insert(tr("04. Patient's Weight"), "0010|1030");
  TagMapPatient.insert(tr("05. Patient's Birth Date"), "0010|0030");
  TagMapPatient.insert(tr("06. Patient's ID"), "0010|0020");
  TagMapPatient.insert(tr("07. Patient's Address"), "0010|1040");

  TagMapStudy.insert(tr("01. Study Description"), "0008|1030");
  TagMapStudy.insert(tr("02. Study Date"), "0008|0020");
  TagMapStudy.insert(tr("03. Study Time"), "0008|0030");
  TagMapStudy.insert(tr("04. Study ID"), "0020|0010");

  TagMapSeries.insert(tr("01. Series Description"), "0008|103e");
  TagMapSeries.insert(tr("02. Series Date"), "0008|0021");
  TagMapSeries.insert(tr("03. Series Time"), "0008|0031");
  TagMapSeries.insert(tr("04. Series Number"), "0020|0011");
}

void ImageItem::onUpdateFrame() {
  double width = ui->ViewWidget->width();
  double height = ui->ViewWidget->height();
  double halfWidth = 5;
  double p[4][3] = {{halfWidth, halfWidth, 0},
                    {width - halfWidth, halfWidth, 0},
                    {width - halfWidth, height - halfWidth, 0},
                    {halfWidth, height - halfWidth, 0}};
  auto points = vtkSmartPointer<vtkPoints>::New();
  points->Initialize();
  for (int i = 0; i < 4; i++) {
    points->InsertNextPoint(p[i]);
  }
  auto cells = vtkSmartPointer<vtkCellArray>::New();
  cells->Initialize();
  auto idlist1 = vtkSmartPointer<vtkIdList>::New();
  idlist1->Initialize();
  for (int i = 0; i < 4; i++)
    idlist1->InsertNextId(i);
  idlist1->InsertNextId(0);

  cells->InsertNextCell(idlist1);
  auto pd = vtkSmartPointer<vtkPolyData>::New();
  pd->SetPoints(points);
  pd->SetLines(cells);

  auto mapper = vtkSmartPointer<vtkPolyDataMapper2D>::New();
  mapper->SetInputData(pd);
  m_borderActor->SetMapper(mapper);
  m_borderActor->VisibilityOn();
  m_borderActor->GetProperty()->SetOpacity(0.8);
  m_borderActor->GetProperty()->SetColor(1, 0.5, 0.2);
  m_borderActor->GetProperty()->SetLineWidth(10);
  if (!m_render->GetActors()->IsItemPresent(m_borderActor))
    m_render->AddActor(m_borderActor);
  m_render->GetRenderWindow()->Render();
  ui->ImportButton->setGeometry(width * 0.625, height - width / 10 - 20,
                                width / 5, width / 10);
  ui->ImportButton->setVisible(true);
  emit interacted(this);
}

void ImageItem::onImportItem() { emit importData(this); }
