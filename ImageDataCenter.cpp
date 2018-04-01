#include "ImageDataCenter.h"
#include "ImageItem.h"
#include "ui_ImageDataCenter.h"
ImageDataCenter::ImageDataCenter(QWidget *parent)
    : QWidget(parent), ui(new Ui::ImageDataCenter) {
  ui->setupUi(this);
  m_gridLayOut = new QGridLayout(0);
  ui->Image2DViewWidget->setLayout(m_gridLayOut);
  m_plusWidget = new PlusWidget(0);
  connect(m_plusWidget, SIGNAL(addImageItem()), this, SLOT(onAddImageItem()));
  int maxwidth = this->width() / 5 > 400 ? this->width() / 5 : 400;
  ui->InformationEdit->setMaximumWidth(maxwidth);
}

ImageDataCenter::~ImageDataCenter() { delete ui; }

void ImageDataCenter::UpDateViewWidget(QList<ImageItem *> widgetList) {
  while (m_gridLayOut->count() > 0) {
    QWidget *widget = m_gridLayOut->itemAt(0)->widget();
    m_gridLayOut->removeWidget(widget);
  }
  int numOfItem = widgetList.size();
  for (int i = 0; i < numOfItem; i++) {
    m_gridLayOut->addWidget(widgetList.at(i)->GetViewWidget(), i / 4, i % 4);
  }

  m_gridLayOut->addWidget(m_plusWidget, numOfItem / 4, numOfItem % 4);
  QApplication::processEvents();
  emit resizeWidget();
}

void ImageDataCenter::setTagInformation(QString tagInfor) {
  ui->InformationEdit->setText(tagInfor);
}

void ImageDataCenter::resizeEvent(QResizeEvent *event) {
  QWidget::resizeEvent(event);
  int maxwidth = this->width() / 5 > 400 ? this->width() / 5 : 400;
  ui->InformationEdit->setMaximumWidth(maxwidth);
  emit resizeWidget();
}

void ImageDataCenter::onAddImageItem() { emit addImageItem(); }
