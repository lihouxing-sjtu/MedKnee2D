#ifndef IMAGEITEM_H
#define IMAGEITEM_H

#include "stdafx.h"
#include <QWidget>
VTK_MODULE_INIT(vtkInteractionStyle)
VTK_MODULE_INIT(vtkRenderingFreeType)
VTK_MODULE_INIT(vtkRenderingOpenGL2)
namespace Ui {
class ImageItem;
}

class ImageItem : public QWidget {
  Q_OBJECT

public:
  explicit ImageItem(QWidget *parent = 0, std::vector<std::string> path = {""});
  ~ImageItem();
  QVTKOpenGLWidget *GetViewWidget();
  void SetBorderOff();
  void upDateFrame();
  vtkImageData *GetImageData();
  QString getTagInformation();

private:
  Ui::ImageItem *ui;

  typedef short PixelType;
  typedef itk::Image<PixelType, 3> ImageType;
  typedef itk::MetaDataDictionary DictionaryType;

  DictionaryType m_imageDictionary;

  vtkSmartPointer<vtkGenericOpenGLRenderWindow> m_rendWin;
  vtkSmartPointer<vtkRenderer> m_render;
  vtkSmartPointer<vtkRenderWindowInteractor> m_rendInteractor;
  vtkSmartPointer<vtkInteractorStyleImage> m_rendInteractorStyle;

  std::vector<std::string> m_imagePath;
  QString m_readFailedMsg;
  vtkSmartPointer<vtkImageData> m_imageData;
  vtkSmartPointer<vtkImageSlice> m_imageActor;

  vtkSmartPointer<vtkEventQtSlotConnect> m_eventConnector;
  vtkSmartPointer<vtkActor2D> m_borderActor;

  QString m_tagvaluelist;
  QString m_tabletags[7];
  QMap<QString, QString> TagMapVolume;
  QMap<QString, QString> TagMapImaging;
  QMap<QString, QString> TagMapPatient;
  QMap<QString, QString> TagMapStudy;
  QMap<QString, QString> TagMapSeries;

  void GetImageTagValue(itk::GDCMImageIO::Pointer gdcmio);
  void InsertTagVolume();

  bool readDicom();
  void setUpCamera(int orientation);

protected slots:
  void onUpdateFrame();
  void onImportItem();
signals:
  void readFailed(QString);
  void interacted(ImageItem *);
  void importData(ImageItem *);
};

#endif // IMAGEITEM_H
