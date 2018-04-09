#pragma once
#pragma comment(lib, "rpcrt4.lib")
//#pragma comment(lib, "WS2_32.lib")

#include <windowsx.h>

#include <itkAntiAliasBinaryImageFilter.h>
#include <itkAutomaticTopologyMeshSource.h>
#include <itkBilateralImageFilter.h>
#include <itkBinaryDilateImageFilter.h>
#include <itkBinaryErodeImageFilter.h>
#include <itkBinaryMedianImageFilter.h>
#include <itkBinaryMinMaxCurvatureFlowImageFilter.h>
#include <itkCannyEdgeDetectionImageFilter.h>
#include <itkCannySegmentationLevelSetImageFilter.h>
#include <itkCastImageFilter.h>
#include <itkCenteredEuler3DTransform.h>
#include <itkCheckerBoardImageFilter.h>
#include <itkCommand.h>
#include <itkConfidenceConnectedImageFilter.h>
#include <itkConnectedThresholdImageFilter.h>
#include <itkCurvatureAnisotropicDiffusionImageFilter.h>
#include <itkCurvatureFlowImageFilter.h>
#include <itkExtractImageFilter.h>
#include <itkFastMarchingImageFilter.h>
#include <itkFileOutputWindow.h>
#include <itkGDCMImageIO.h>
#include <itkGDCMSeriesFileNames.h>
#include <itkGeodesicActiveContourLevelSetImageFilter.h>
#include <itkGradientAnisotropicDiffusionImageFilter.h>
#include <itkGradientMagnitudeImageFilter.h>
#include <itkGradientMagnitudeRecursiveGaussianImageFilter.h>
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkImageRegistrationMethodv4.h>
#include <itkImageSeriesReader.h>
#include <itkImageSeriesWriter.h>
#include <itkImageToVTKImageFilter.h>
#include <itkIsolatedConnectedImageFilter.h>
#include <itkJPEGImageIO.h>
#include <itkLabelStatisticsImageFilter.h>
#include <itkLabelToRGBImageFilter.h>
#include <itkLaplacianSegmentationLevelSetImageFilter.h>
#include <itkMattesMutualInformationImageToImageMetricv4.h>
#include <itkMeanImageFilter.h>
#include <itkMedianImageFilter.h>
#include <itkMinMaxCurvatureFlowImageFilter.h>
#include <itkNeighborhoodConnectedImageFilter.h>
#include <itkNumericSeriesFileNames.h>
#include <itkOrientImageFilter.h>
#include <itkOtsuMultipleThresholdsCalculator.h>
#include <itkOtsuThresholdImageFilter.h>
#include <itkOutputWindow.h>
#include <itkPNGImageIO.h>
#include <itkQuadEdgeMesh.h>
#include <itkRayCastInterpolateImageFunction.h>
#include <itkRegionOfInterestImageFilter.h>
#include <itkRegularStepGradientDescentOptimizerv4.h>
#include <itkRelabelComponentImageFilter.h>
#include <itkResampleImageFilter.h>
#include <itkRescaleIntensityImageFilter.h>
#include <itkScalarConnectedComponentImageFilter.h>
#include <itkScalarImageToHistogramGenerator.h>
#include <itkShapeDetectionLevelSetImageFilter.h>
#include <itkSigmoidImageFilter.h>
#include <itkTIFFImageIO.h>
#include <itkThresholdImageFilter.h>
#include <itkThresholdSegmentationLevelSetImageFilter.h>
#include <itkTranslationTransform.h>
#include <itkVTKImageToImageFilter.h>
#include <itkVTKPolyDataReader.h>
#include <itkVTKPolyDataWriter.h>
#include <itkWatershedImageFilter.h>

#include <vtkAVIWriter.h>
#include <vtkAnnotatedCubeActor.h>
#include <vtkAppendFilter.h>
#include <vtkAppendPolyData.h>
#include <vtkArcSource.h>
#include <vtkArrowSource.h>
#include <vtkAssembly.h>
#include <vtkAutoInit.h>
#include <vtkAxesActor.h>
#include <vtkAxisActor.h>
#include <vtkAxisFollower.h>
#include <vtkBMPReader.h>
#include <vtkBMPWriter.h>
#include <vtkBalloonRepresentation.h>
#include <vtkBalloonWidget.h>
#include <vtkBezierContourLineInterpolator.h>
#include <vtkBooleanOperationPolyDataFilter.h>
#include <vtkBoundedPlanePointPlacer.h>
#include <vtkBoundedPointSource.h>
#include <vtkBoundingBox.h>
#include <vtkBox.h>
#include <vtkBoxWidget.h>
#include <vtkButtonRepresentation.h>
#include <vtkButtonWidget.h>
#include <vtkCallbackCommand.h>
#include <vtkCamera.h>
#include <vtkCameraActor.h>
#include <vtkCaptionActor2D.h>
#include <vtkCellArray.h>
#include <vtkCellData.h>
#include <vtkCellLocator.h>
#include <vtkCellPicker.h>
#include <vtkCenterOfMass.h>
#include <vtkCleanPolyData.h>
#include <vtkClipClosedSurface.h>
#include <vtkClipPolyData.h>
#include <vtkColorTransferFunction.h>
#include <vtkCone.h>
#include <vtkConeSource.h>
#include <vtkContourFilter.h>
#include <vtkContourWidget.h>
#include <vtkCornerAnnotation.h>
#include <vtkCubeSource.h>
#include <vtkCutter.h>
#include <vtkCylinder.h>
#include <vtkCylinderSource.h>
#include <vtkDecimatePro.h>
#include <vtkDeformPointSet.h>
#include <vtkDelaunay2D.h>
#include <vtkDelaunay3D.h>
#include <vtkDepthSortPolyData.h>
#include <vtkDiskSource.h>
#include <vtkDistancePolyDataFilter.h>
#include <vtkDistanceRepresentation3D.h>
#include <vtkDistanceWidget.h>
#include <vtkDoubleArray.h>
#include <vtkEventQtSlotConnect.h>
#include <vtkExtractEdges.h>
#include <vtkExtractSurface.h>
#include <vtkExtractVOI.h>
#include <vtkFileOutputWindow.h>
#include <vtkFillHolesFilter.h>
#include <vtkFitImplicitFunction.h>
#include <vtkFlyingEdges2D.h>
#include <vtkFlyingEdges3D.h>
#include <vtkFlyingEdgesPlaneCutter.h>
#include <vtkFocalPlanePointPlacer.h>
#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkGeometryFilter.h>
#include <vtkGlyph3D.h>
#include <vtkImageActor.h>
#include <vtkImageAnisotropicDiffusion3D.h>
#include <vtkImageCanvasSource2D.h>
#include <vtkImageCast.h>
#include <vtkImageChangeInformation.h>
#include <vtkImageConstantPad.h>
#include <vtkImageContinuousDilate3D.h>
#include <vtkImageContinuousErode3D.h>
#include <vtkImageCorrelation.h>
#include <vtkImageData.h>
#include <vtkImageDataGeometryFilter.h>
#include <vtkImageEuclideanDistance.h>
#include <vtkImageFlip.h>
#include <vtkImageGaussianSmooth.h>
#include <vtkImageGradient.h>
#include <vtkImageHistogramStatistics.h>
#include <vtkImageInterpolator.h>
#include <vtkImageLaplacian.h>
#include <vtkImageMagnitude.h>
#include <vtkImageMapToColors.h>
#include <vtkImageMapToWindowLevelColors.h>
#include <vtkImageMarchingCubes.h>
#include <vtkImageMask.h>
#include <vtkImageMathematics.h>
#include <vtkImageMedian3D.h>
#include <vtkImageOpenClose3D.h>
#include <vtkImagePlaneWidget.h>
#include <vtkImageProgressIterator.h>
#include <vtkImageProperty.h>
#include <vtkImageRange3D.h>
#include <vtkImageResample.h>
#include <vtkImageReslice.h>
#include <vtkImageResliceMapper.h>
#include <vtkImageSkeleton2D.h>
#include <vtkImageSlice.h>
#include <vtkImageSliceMapper.h>
#include <vtkImageSobel3D.h>
#include <vtkImageStencil.h>
#include <vtkImageStencilIterator.h>
#include <vtkImageThreshold.h>
#include <vtkImageThresholdConnectivity.h>
#include <vtkImageVariance3D.h>
#include <vtkImageViewer.h>
#include <vtkImplicitBoolean.h>
#include <vtkImplicitModeller.h>
#include <vtkImplicitPolyDataDistance.h>
#include <vtkImplicitSelectionLoop.h>
#include <vtkInformation.h>
#include <vtkInteractorStyleImage.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkIterativeClosestPointTransform.h>
#include <vtkJPEGReader.h>
#include <vtkKdTreePointLocator.h>
#include <vtkKochanekSpline.h>
#include <vtkLandmarkTransform.h>
#include <vtkLassoStencilSource.h>
#include <vtkLight.h>
#include <vtkLine.h>
#include <vtkLineSource.h>
#include <vtkLineWidget.h>
#include <vtkLinearContourLineInterpolator.h>
#include <vtkLinearTransform.h>
#include <vtkLookupTable.h>
#include <vtkPlaneSource.h>
#include <vtkRuledSurfaceFilter.h>
#include <vtkTexturedButtonRepresentation2D.h>
#include <vtkWindowLevelLookupTable.h>

//#include <vtkMPIController.h>
#include <vtkLandmarkTransform.h>
#include <vtkMassProperties.h>
#include <vtkMath.h>
#include <vtkMatrix4x4.h>
#include <vtkMedicalImageProperties.h>
#include <vtkMultiBlockDataSet.h>
#include <vtkNamedColors.h>
#include <vtkOBBDicer.h>
#include <vtkOBBTree.h>
#include <vtkOctreePointLocator.h>
#include <vtkOpenGLRenderWindow.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkOrientedGlyphContourRepresentation.h>
#include <vtkOutlineCornerSource.h>
#include <vtkOutlineFilter.h>
#include <vtkOutlineSource.h>
#include <vtkPCANormalEstimation.h>
#include <vtkPCAStatistics.h>
#include <vtkPLYReader.h>
#include <vtkPLYWriter.h>
#include <vtkPNGReader.h>
#include <vtkPNGWriter.h>
#include <vtkPProbeFilter.h>
#include <vtkParametricFunctionSource.h>
#include <vtkParametricSpline.h>
#include <vtkPiecewiseFunction.h>
#include <vtkPlane.h>
#include <vtkPlaneCollection.h>
#include <vtkPlaneSource.h>
#include <vtkPlaneWidget.h>
#include <vtkPlanes.h>
#include <vtkPointData.h>
#include <vtkPointHandleRepresentation3D.h>
#include <vtkPointInterpolator.h>
#include <vtkPointLocator.h>
#include <vtkPointPicker.h>
#include <vtkPolyData.h>
#include <vtkPolyDataCollection.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyDataMapper2D.h>
#include <vtkPolyDataNormals.h>
#include <vtkPolyDataToImageStencil.h>
#include <vtkPolyDataWriter.h>
#include <vtkPolyLine.h>
#include <vtkPolygon.h>
#include <vtkPolygonalSurfaceContourLineInterpolator.h>
#include <vtkPolygonalSurfacePointPlacer.h>
#include <vtkPropAssembly.h>
#include <vtkPropPicker.h>
#include <vtkProperty.h>
#include <vtkProperty2D.h>
#include <vtkQuad.h>
#include <vtkQuadricDecimation.h>
#include <vtkQuadricLODActor.h>
#include <vtkQuaternion.h>
#include <vtkRegularPolygonSource.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderedAreaPicker.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkResliceCursor.h>
#include <vtkResliceCursorActor.h>
#include <vtkResliceCursorLineRepresentation.h>
#include <vtkResliceCursorPicker.h>
#include <vtkResliceCursorPolyDataAlgorithm.h>
#include <vtkResliceCursorThickLineRepresentation.h>
#include <vtkResliceCursorWidget.h>
#include <vtkReverseSense.h>
#include <vtkRuledSurfaceFilter.h>
#include <vtkSCurveSpline.h>
#include <vtkSPHCubicKernel.h>
#include <vtkSTLReader.h>
#include <vtkSTLWriter.h>
#include <vtkSampleFunction.h>
#include <vtkShortArray.h>
#include <vtkShrinkPolyData.h>
#include <vtkSignedDistance.h>
#include <vtkSmartPointer.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkSmoothPolyDataFilter.h>
#include <vtkSpatialRepresentationFilter.h>
#include <vtkSphere.h>
#include <vtkSphereSource.h>
#include <vtkSphereWidget.h>
#include <vtkSplineFilter.h>
#include <vtkSplineWidget.h>
#include <vtkSplineWidget2.h>
#include <vtkStaticPointLocator.h>
#include <vtkStreamingDemandDrivenPipeline.h>
#include <vtkStringArray.h>
#include <vtkStripper.h>
#include <vtkStructuredGrid.h>
#include <vtkSurfaceReconstructionFilter.h>
#include <vtkTIFFReader.h>
#include <vtkTIFFWriter.h>
#include <vtkTable.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkTriangle.h>
#include <vtkTriangleFilter.h>
#include <vtkTubeFilter.h>
#include <vtkUnstructuredGrid.h>
#include <vtkUnstructuredGridVolumeRayCastMapper.h>
#include <vtkVector.h>
#include <vtkVersion.h>
#include <vtkVertex.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkVolume.h>
#include <vtkVolumeProperty.h>
#include <vtkVoxelModeller.h>
#include <vtkWidgetEvent.h>
#include <vtkWidgetEventTranslator.h>
#include <vtkWindowToImageFilter.h>
#include <vtkWindowedSincPolyDataFilter.h>
#include <vtkWorldPointPicker.h>
#include <vtkXMLImageDataReader.h>
#include <vtkXMLImageDataWriter.h>

#include <QActionGroup>

#include <QFile>
#include <QFuture>
#include <QFutureWatcher>
#include <QGridLayout>
#include <QLayout>
#include <QLineEdit>
#include <QMenu>
#include <QMutex>
#include <QPair>
#include <QStack>
#include <QTemporaryFile>
#include <QTextStream>
#include <QVTKOpenGLWidget.h>
#include <QVTKWidget.h>
#include <QtConcurrent/QtConcurrent>
#include <QtCore/qglobal.h>
#include <QtCore/qmath.h>
#include <QtWidgets/QtWidgets>
#define vsp vtkSmartPointer
#define VTKNew(name, type)                                                     \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()

#define IyDistance(a, b)                                                       \
  qSqrt((a[0] - b[0]) * (a[0] - b[0]) + (a[1] - b[1]) * (a[1] - b[1]) +        \
        (a[2] - b[2]) * (a[2] - b[2]))

#define IyAngle(a, b) qRadiansToDegrees(vtkMath::AngleBetweenVectors(a, b))

#define argf0(x) arg(double(x), 0, 'f', 0)
#define argf1(x) arg(double(x), 0, 'f', 1)
#define argf2(x) arg(double(x), 0, 'f', 2)
#define argf3(x) arg(double(x), 0, 'f', 3)
#define argf6(x) arg(double(x), 0, 'f', 6)

#define YaHei "\345\276\256\350\275\257\351\233\205\351\273\221"
#define YaHeiLight "\345\276\256\350\275\257\351\233\205\351\273\221 Light"
#define FangSong "\345\275\261\344\270\272\345\214\273\347\226\227"

#define vtkPtMidNew(ptMid, pt1, pt2)                                           \
  double ptMid[3];                                                             \
  vtkMath::Add(pt1, pt2, ptMid);                                               \
  vtkMath::MultiplyScalar(ptMid, 0.5);

#define vtkPtMid(ptMid, pt1, pt2)                                              \
  vtkMath::Add(pt1, pt2, ptMid);                                               \
  vtkMath::MultiplyScalar(ptMid, 0.5);

#define vtkDirNew(dir, pt1, pt2)                                               \
  double dir[3];                                                               \
  vtkMath::Subtract(pt2, pt1, dir);                                            \
  vtkMath::Normalize(dir);

#define vtkDir(dir, pt1, pt2)                                                  \
  vtkMath::Subtract(pt2, pt1, dir);                                            \
  vtkMath::Normalize(dir);
