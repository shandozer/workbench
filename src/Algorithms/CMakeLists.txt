#
# Name of project
#
PROJECT (Algorithms)

#
# Need XML from Qt
#
SET(QT_DONT_USE_QTGUI)

#
# Add QT for includes
#
INCLUDE (${QT_USE_FILE})

#
# Create the helper library
#
ADD_LIBRARY(Algorithms
AbstractAlgorithm.h
AlgorithmBorderResample.h
AlgorithmBorderToVertices.h
AlgorithmCiftiAllLabelsToROIs.h
AlgorithmCiftiAverage.h
AlgorithmCiftiAverageDenseROI.h
AlgorithmCiftiAverageROICorrelation.h
AlgorithmCiftiCorrelation.h
AlgorithmCiftiCorrelationGradient.h
AlgorithmCiftiCreateDenseScalar.h
AlgorithmCiftiCreateDenseTimeseries.h
AlgorithmCiftiCreateLabel.h
AlgorithmCiftiCrossCorrelation.h
AlgorithmCiftiDilate.h
AlgorithmCiftiErode.h
AlgorithmCiftiExtrema.h
AlgorithmCiftiFalseCorrelation.h
AlgorithmCiftiFindClusters.h
AlgorithmCiftiGradient.h
AlgorithmCiftiLabelAdjacency.h
AlgorithmCiftiLabelToBorder.h
AlgorithmCiftiLabelToROI.h
AlgorithmCiftiMergeDense.h
AlgorithmCiftiMergeParcels.h
AlgorithmCiftiPairwiseCorrelation.h
AlgorithmCiftiParcellate.h
AlgorithmCiftiParcelMappingToLabel.h
AlgorithmCiftiReduce.h
AlgorithmCiftiReorder.h
AlgorithmCiftiReplaceStructure.h
AlgorithmCiftiResample.h
AlgorithmCiftiRestrictDenseMap.h
AlgorithmCiftiROIsFromExtrema.h
AlgorithmCiftiSeparate.h
AlgorithmCiftiSmoothing.h
AlgorithmCiftiTranspose.h
AlgorithmCiftiVectorOperation.h
AlgorithmCreateSignedDistanceVolume.h
AlgorithmException.h
AlgorithmFiberDotProducts.h
AlgorithmFociResample.h
AlgorithmGiftiAllLabelsToROIs.h
AlgorithmGiftiLabelAddPrefix.h
AlgorithmGiftiLabelToROI.h
AlgorithmLabelDilate.h
AlgorithmLabelErode.h
AlgorithmLabelModifyKeys.h
AlgorithmLabelResample.h
AlgorithmLabelToBorder.h
AlgorithmLabelToVolumeMapping.h
AlgorithmMetricDilate.h
AlgorithmMetricErode.h
AlgorithmMetricEstimateFWHM.h
AlgorithmMetricExtrema.h
AlgorithmMetricFalseCorrelation.h
AlgorithmMetricFillHoles.h
AlgorithmMetricFindClusters.h
AlgorithmMetricGradient.h
AlgorithmMetricReduce.h
AlgorithmMetricRegression.h
AlgorithmMetricRemoveIslands.h
AlgorithmMetricResample.h
AlgorithmMetricROIsFromExtrema.h
AlgorithmMetricROIsToBorder.h
AlgorithmMetricSmoothing.h
AlgorithmMetricTFCE.h
AlgorithmMetricToVolumeMapping.h
AlgorithmMetricVectorOperation.h
AlgorithmMetricVectorTowardROI.h
AlgorithmNodesInsideBorder.h
AlgorithmSignedDistanceToSurface.h
AlgorithmSurfaceAffineRegression.h
AlgorithmSurfaceApplyAffine.h
AlgorithmSurfaceApplyWarpfield.h
AlgorithmSurfaceAverage.h
AlgorithmSurfaceCortexLayer.h
AlgorithmSurfaceCreateSphere.h
AlgorithmSurfaceCurvature.h
AlgorithmSurfaceDistortion.h
AlgorithmSurfaceFlipLR.h
AlgorithmSurfaceGenerateInflated.h
AlgorithmSurfaceInflation.h
AlgorithmSurfaceMatch.h
AlgorithmSurfaceModifySphere.h
AlgorithmSurfaceResample.h
AlgorithmSurfaceSmoothing.h
AlgorithmSurfaceSphereProjectUnproject.h
AlgorithmSurfaceToSurface3dDistance.h
AlgorithmSurfaceWedgeVolume.h
AlgorithmVolumeAffineResample.h
AlgorithmVolumeAllLabelsToROIs.h
AlgorithmVolumeDilate.h
AlgorithmVolumeErode.h
AlgorithmVolumeEstimateFWHM.h
AlgorithmVolumeExtrema.h
AlgorithmVolumeFillHoles.h
AlgorithmVolumeFindClusters.h
AlgorithmVolumeGradient.h
AlgorithmVolumeLabelProbability.h
AlgorithmVolumeLabelToROI.h
AlgorithmVolumeLabelToSurfaceMapping.h
AlgorithmVolumeParcelResampling.h
AlgorithmVolumeParcelResamplingGeneric.h
AlgorithmVolumeParcelSmoothing.h
AlgorithmVolumeReduce.h
AlgorithmVolumeRemoveIslands.h
AlgorithmVolumeROIsFromExtrema.h
AlgorithmVolumeSmoothing.h
AlgorithmVolumeTFCE.h
AlgorithmVolumeToSurfaceMapping.h
AlgorithmVolumeVectorOperation.h
AlgorithmVolumeWarpfieldResample.h
OverlapLogicEnum.h

AbstractAlgorithm.cxx
AlgorithmBorderResample.cxx
AlgorithmBorderToVertices.cxx
AlgorithmCiftiAllLabelsToROIs.cxx
AlgorithmCiftiAverage.cxx
AlgorithmCiftiAverageDenseROI.cxx
AlgorithmCiftiAverageROICorrelation.cxx
AlgorithmCiftiCorrelation.cxx
AlgorithmCiftiCorrelationGradient.cxx
AlgorithmCiftiCreateDenseScalar.cxx
AlgorithmCiftiCreateDenseTimeseries.cxx
AlgorithmCiftiCreateLabel.cxx
AlgorithmCiftiCrossCorrelation.cxx
AlgorithmCiftiDilate.cxx
AlgorithmCiftiErode.cxx
AlgorithmCiftiExtrema.cxx
AlgorithmCiftiFalseCorrelation.cxx
AlgorithmCiftiFindClusters.cxx
AlgorithmCiftiGradient.cxx
AlgorithmCiftiLabelAdjacency.cxx
AlgorithmCiftiLabelToBorder.cxx
AlgorithmCiftiLabelToROI.cxx
AlgorithmCiftiMergeDense.cxx
AlgorithmCiftiMergeParcels.cxx
AlgorithmCiftiPairwiseCorrelation.cxx
AlgorithmCiftiParcellate.cxx
AlgorithmCiftiParcelMappingToLabel.cxx
AlgorithmCiftiReduce.cxx
AlgorithmCiftiReorder.cxx
AlgorithmCiftiReplaceStructure.cxx
AlgorithmCiftiResample.cxx
AlgorithmCiftiRestrictDenseMap.cxx
AlgorithmCiftiROIsFromExtrema.cxx
AlgorithmCiftiSeparate.cxx
AlgorithmCiftiSmoothing.cxx
AlgorithmCiftiTranspose.cxx
AlgorithmCiftiVectorOperation.cxx
AlgorithmCreateSignedDistanceVolume.cxx
AlgorithmException.cxx
AlgorithmFiberDotProducts.cxx
AlgorithmFociResample.cxx
AlgorithmGiftiAllLabelsToROIs.cxx
AlgorithmGiftiLabelAddPrefix.cxx
AlgorithmGiftiLabelToROI.cxx
AlgorithmLabelDilate.cxx
AlgorithmLabelErode.cxx
AlgorithmLabelModifyKeys.cxx
AlgorithmLabelResample.cxx
AlgorithmLabelToBorder.cxx
AlgorithmLabelToVolumeMapping.cxx
AlgorithmMetricDilate.cxx
AlgorithmMetricErode.cxx
AlgorithmMetricEstimateFWHM.cxx
AlgorithmMetricExtrema.cxx
AlgorithmMetricFalseCorrelation.cxx
AlgorithmMetricFillHoles.cxx
AlgorithmMetricFindClusters.cxx
AlgorithmMetricGradient.cxx
AlgorithmMetricReduce.cxx
AlgorithmMetricRegression.cxx
AlgorithmMetricRemoveIslands.cxx
AlgorithmMetricResample.cxx
AlgorithmMetricROIsFromExtrema.cxx
AlgorithmMetricROIsToBorder.cxx
AlgorithmMetricSmoothing.cxx
AlgorithmMetricTFCE.cxx
AlgorithmMetricToVolumeMapping.cxx
AlgorithmMetricVectorOperation.cxx
AlgorithmMetricVectorTowardROI.cxx
AlgorithmNodesInsideBorder.cxx
AlgorithmSignedDistanceToSurface.cxx
AlgorithmSurfaceAffineRegression.cxx
AlgorithmSurfaceApplyAffine.cxx
AlgorithmSurfaceApplyWarpfield.cxx
AlgorithmSurfaceAverage.cxx
AlgorithmSurfaceCortexLayer.cxx
AlgorithmSurfaceCreateSphere.cxx
AlgorithmSurfaceCurvature.cxx
AlgorithmSurfaceDistortion.cxx
AlgorithmSurfaceFlipLR.cxx
AlgorithmSurfaceGenerateInflated.cxx
AlgorithmSurfaceInflation.cxx
AlgorithmSurfaceMatch.cxx
AlgorithmSurfaceModifySphere.cxx
AlgorithmSurfaceResample.cxx
AlgorithmSurfaceSmoothing.cxx
AlgorithmSurfaceSphereProjectUnproject.cxx
AlgorithmSurfaceToSurface3dDistance.cxx
AlgorithmSurfaceWedgeVolume.cxx
AlgorithmVolumeAffineResample.cxx
AlgorithmVolumeAllLabelsToROIs.cxx
AlgorithmVolumeDilate.cxx
AlgorithmVolumeErode.cxx
AlgorithmVolumeEstimateFWHM.cxx
AlgorithmVolumeExtrema.cxx
AlgorithmVolumeFillHoles.cxx
AlgorithmVolumeFindClusters.cxx
AlgorithmVolumeGradient.cxx
AlgorithmVolumeLabelProbability.cxx
AlgorithmVolumeLabelToROI.cxx
AlgorithmVolumeLabelToSurfaceMapping.cxx
AlgorithmVolumeParcelResampling.cxx
AlgorithmVolumeParcelResamplingGeneric.cxx
AlgorithmVolumeParcelSmoothing.cxx
AlgorithmVolumeReduce.cxx
AlgorithmVolumeRemoveIslands.cxx
AlgorithmVolumeROIsFromExtrema.cxx
AlgorithmVolumeSmoothing.cxx
AlgorithmVolumeTFCE.cxx
AlgorithmVolumeToSurfaceMapping.cxx
AlgorithmVolumeVectorOperation.cxx
AlgorithmVolumeWarpfieldResample.cxx
OverlapLogicEnum.cxx
)

#
# Find Headers
#
INCLUDE_DIRECTORIES(
${CMAKE_SOURCE_DIR}/Algorithms
${CMAKE_SOURCE_DIR}/Annotations
${CMAKE_SOURCE_DIR}/OperationsBase
${CMAKE_SOURCE_DIR}/FilesBase
${CMAKE_SOURCE_DIR}/Files
${CMAKE_SOURCE_DIR}/Gifti
${CMAKE_SOURCE_DIR}/Cifti
${CMAKE_SOURCE_DIR}/Palette
${CMAKE_SOURCE_DIR}/Nifti
${CMAKE_SOURCE_DIR}/Scenes
${CMAKE_SOURCE_DIR}/Xml
${CMAKE_SOURCE_DIR}/Charting
${CMAKE_SOURCE_DIR}/Common
)
