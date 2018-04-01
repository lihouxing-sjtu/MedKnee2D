#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  m_imageDataCenter = new ImageDataCenter(0);
  connect(m_imageDataCenter, SIGNAL(addImageItem()), this, SLOT(onOpen()));
  m_planWidget = new PlanWidget(0);

  this->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
  connect(ui->NewButton, SIGNAL(clicked(bool)), this, SLOT(onOpen()));
  connect(m_imageDataCenter, SIGNAL(resizeWidget()), this,
          SLOT(onDataCenterResized()));
  connect(ui->QuitButton, SIGNAL(clicked(bool)), this, SLOT(onQuit()));
  connect(m_planWidget, SIGNAL(quitApp()), this, SLOT(onQuit()));
  connect(m_planWidget, SIGNAL(showDataCenter()), this,
          SLOT(onShowDataCenter()));
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::resizeEvent(QResizeEvent *event) {
  int height = this->height() / 6;

  ui->NewButton->setMaximumHeight(height);
  ui->NewButton->setMaximumWidth(height * 2);

  ui->OpenButton->setMaximumHeight(height);
  ui->OpenButton->setMaximumWidth(height * 2);

  ui->QuitButton->setMaximumHeight(height);
  ui->QuitButton->setMaximumWidth(height * 2);

  ui->AboutButton->setMaximumHeight(height);
  ui->AboutButton->setMaximumWidth(height * 2);
  QMainWindow::resizeEvent(event);
}

void MainWindow::onOpen() {
  QString dirPath =
      QFileDialog::getExistingDirectory(this, tr("Select dir"), 0);
  qDebug() << dirPath;
  if (dirPath.isEmpty())
    return;

  auto nameGenerator = itk::GDCMSeriesFileNames::New();
  nameGenerator->SetUseSeriesDetails(true);
  nameGenerator->SetRecursive(true);
  nameGenerator->SetDirectory(qPrintable(dirPath));
  nameGenerator->Update();

  std::vector<std::string> seriesUID = nameGenerator->GetSeriesUIDs();
  if (seriesUID.empty())
    return;

  for (auto seriesItr = seriesUID.begin(); seriesItr != seriesUID.end();
       ++seriesItr) {
    std::vector<std::string> filenames =
        nameGenerator->GetFileNames(seriesItr->c_str());
    ImageItem *item = new ImageItem(0, filenames);
    item->upDateFrame();
    m_imageItemList.append(item);
    int listSize = m_imageItemList.size();
    if (listSize - 2 >= 0)
      m_imageItemList.at(listSize - 2)->SetBorderOff();
    connect(item, SIGNAL(interacted(ImageItem *)), this,
            SLOT(onImageBorderVisibility(ImageItem *)));
    connect(item, SIGNAL(importData(ImageItem *)), this,
            SLOT(onImportImageData(ImageItem *)));
  }
  m_imageDataCenter->UpDateViewWidget(m_imageItemList);

  m_imageItemList.last()->upDateFrame();
  m_imageDataCenter->showMaximized();
  QApplication::processEvents();
  this->hide();
}

void MainWindow::onQuit() {
  this->close();
  m_imageDataCenter->close();
}

void MainWindow::onImageBorderVisibility(ImageItem *item) {
  int numOfImageItem = m_imageItemList.size();
  for (int i = 0; i < numOfImageItem; i++) {
    if (m_imageItemList.at(i) != item)
      m_imageItemList.at(i)->SetBorderOff();
  }
  m_imageDataCenter->setTagInformation(item->getTagInformation());
}

void MainWindow::onDataCenterResized() {
  for (int i = 0; i < m_imageItemList.size(); i++)
    m_imageItemList.at(i)->upDateFrame();
}

void MainWindow::onImportImageData(ImageItem *item) {
  auto img = vtkSmartPointer<vtkImageData>::New();
  img = item->GetImageData();
  m_imageDataCenter->hide();
  m_planWidget->showMaximized();
  m_planWidget->setImageData(img);
}

void MainWindow::onShowDataCenter() { m_imageDataCenter->show(); }
