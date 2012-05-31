
/*LICENSE_START*/
/* 
 *  Copyright 1995-2011 Washington University School of Medicine 
 * 
 *  http://brainmap.wustl.edu 
 * 
 *  This file is part of CARET. 
 * 
 *  CARET is free software; you can redistribute it and/or modify 
 *  it under the terms of the GNU General Public License as published by 
 *  the Free Software Foundation; either version 2 of the License, or 
 *  (at your option) any later version. 
 * 
 *  CARET is distributed in the hope that it will be useful, 
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 *  GNU General Public License for more details. 
 * 
 *  You should have received a copy of the GNU General Public License 
 *  along with CARET; if not, write to the Free Software 
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 * 
 */ 

#include <deque>

#define __OVERLAY_SET_DECLARE__
#include "OverlaySet.h"
#undef __OVERLAY_SET_DECLARE__

#include "Brain.h"
#include "BrainStructure.h"
#include "CaretAssert.h"
#include "CaretLogger.h"
#include "CaretMappableDataFile.h"
#include "ConnectivityLoaderFile.h"
#include "LabelFile.h"
#include "MetricFile.h"
#include "ModelSurface.h"
#include "ModelSurfaceMontage.h"
#include "ModelVolume.h"
#include "ModelWholeBrain.h"
#include "ModelYokingGroup.h"
#include "Overlay.h"
#include "Surface.h"
#include "VolumeFile.h"

using namespace caret;

/**
 * \class OverlaySet
 * \brief Contains a set of overlay assignments
 *
 * The maximum number of overlays is fixed.  The number
 * of overlays presented to the user varies and is
 * controlled using the ToolBox in a Browser Window.
 * 
 * The primary overlay is always the overlay at index zero.
 * The underlay is the overlay at (numberOfDisplayedOverlays - 1).
 * When models are colored, the overlays are assigned 
 * starting with the underlay and concluding with the primary
 * overlay.
 */

/**
 * \class OverlaySet
 * \brief Contains a set of overlay assignments
 *
 * The maximum number of overlays is fixed.  The number
 * of overlays presented to the user varies and is
 * controlled using the ToolBox in a Browser Window.
 * 
 * The primary overlay is always the overlay at index zero.
 * The underlay is the overlay at (numberOfDisplayedOverlays - 1).
 * When models are colored, the overlays are assigned 
 * starting with the underlay and concluding with the primary
 * overlay.
 */

/**
 * Constructor for surface controller.
 * @param modelDisplayController
 *     Surface controller that uses this overlay set.
 */
OverlaySet::OverlaySet(BrainStructure* brainStructure)
: CaretObject()
{
    this->initializeOverlaySet(NULL,
                               brainStructure);
    
    for (int i = 0; i < BrainConstants::MAXIMUM_NUMBER_OF_OVERLAYS; i++) {
        this->overlays[i] = new Overlay(brainStructure);
    }
}

/**
 * Constructor for volume controller.
 * @param modelDisplayControllerVolume
 *     Volume controller that uses this overlay set.
 */
OverlaySet::OverlaySet(ModelVolume* modelDisplayControllerVolume)
: CaretObject()
{
    this->initializeOverlaySet(modelDisplayControllerVolume,
                               NULL);
    
    for (int i = 0; i < BrainConstants::MAXIMUM_NUMBER_OF_OVERLAYS; i++) {
        this->overlays[i] = new Overlay(modelDisplayControllerVolume);
    }
}

/**
 * Constructor for surface montage controller.
 * @param modelDisplayControllerSurfaceMontage
 *     surface montage controller that uses this overlay set.
 */
OverlaySet::OverlaySet(ModelSurfaceMontage* modelDisplayControllerSurfaceMontage)
: CaretObject()
{
    this->initializeOverlaySet(modelDisplayControllerSurfaceMontage,
                               NULL);
    
    for (int i = 0; i < BrainConstants::MAXIMUM_NUMBER_OF_OVERLAYS; i++) {
        this->overlays[i] = new Overlay(modelDisplayControllerSurfaceMontage);
    }
}

/**
 * Constructor for whole brain controller.
 * @param modelDisplayControllerWholeBrain
 *     Whole brain controller that uses this overlay set.
 */
OverlaySet::OverlaySet(ModelWholeBrain* modelDisplayControllerWholeBrain)
: CaretObject()
{
    this->initializeOverlaySet(modelDisplayControllerWholeBrain,
                               NULL);
    
    for (int i = 0; i < BrainConstants::MAXIMUM_NUMBER_OF_OVERLAYS; i++) {
        this->overlays[i] = new Overlay(modelDisplayControllerWholeBrain);
    }
}

/**
 * Constructor for yoking controller.
 * @param modelDisplayControllerYoking
 *     Yoking controller that uses this overlay set.
 */
OverlaySet::OverlaySet(ModelYokingGroup* modelDisplayControllerYoking)
: CaretObject()
{
    this->initializeOverlaySet(modelDisplayControllerYoking,
                               NULL);
    
    for (int i = 0; i < BrainConstants::MAXIMUM_NUMBER_OF_OVERLAYS; i++) {
        this->overlays[i] = NULL;
    }
}

/**
 * Destructor.
 */
OverlaySet::~OverlaySet()
{
    for (int i = 0; i < BrainConstants::MAXIMUM_NUMBER_OF_OVERLAYS; i++) {
        delete this->overlays[i];
    }
}

/**
 * Copy the given overlay set to this overlay set.
 * @param overlaySet
 *    Overlay set that is copied.
 */
void 
OverlaySet::copyOverlaySet(const OverlaySet* overlaySet)
{
    this->initializeOverlaySet(overlaySet->modelDisplayController, 
                               overlaySet->brainStructure);
    
    for (int i = 0; i < BrainConstants::MAXIMUM_NUMBER_OF_OVERLAYS; i++) {
        this->overlays[i]->copyData(overlaySet->getOverlay(i));
    }
    this->numberOfDisplayedOverlays = overlaySet->numberOfDisplayedOverlays;
}

/**
 * Initialize the overlay.
 * @param modelDisplayController
 *     Controller that uses this overlay set.
 */
void 
OverlaySet::initializeOverlaySet(Model* modelDisplayController,
                                 BrainStructure* brainStructure)
{
    this->modelDisplayController = modelDisplayController;
    this->brainStructure = brainStructure;
    
    if (this->modelDisplayController == NULL) {
        CaretAssert(this->brainStructure != NULL);
    }
    else if (this->brainStructure == NULL) {
        CaretAssert(this->modelDisplayController != NULL);
    }
    else {
        CaretAssertMessage(0, "Both mode and brain structure are NULL");
    }
    
    this->numberOfDisplayedOverlays = BrainConstants::MINIMUM_NUMBER_OF_OVERLAYS;
}

/**
 * @return Returns the top-most overlay regardless of its enabled status.
 */
Overlay* 
OverlaySet::getPrimaryOverlay()
{
    return this->overlays[0];
}

/**
 * @return Returns the underlay which is the lowest
 * displayed overlay.
 */
Overlay* 
OverlaySet::getUnderlay()
{
    return this->overlays[this->getNumberOfDisplayedOverlays() - 1];
}

/*
 * Get the bottom-most overlay that is a volume file for the given
 * browser tab.
 * @param browserTabContent
 *    Content of browser tab.
 * @return Returns the bottom-most overlay that is set a a volume file.
 * Will return NULL if no, enabled overlays are set to a volume file.
 */
VolumeFile* 
OverlaySet::getUnderlayVolume()
{
    VolumeFile* vf = NULL;
    
    for (int32_t i = (this->getNumberOfDisplayedOverlays() - 1); i >= 0; i--) {
        if (this->overlays[i]->isEnabled()) {
            CaretMappableDataFile* mapFile;
            int32_t mapIndex;
            this->overlays[i]->getSelectionData(mapFile,
                                                mapIndex);
            
            if (mapFile != NULL) {
                vf = dynamic_cast<VolumeFile*>(mapFile);
                if (vf != NULL) {
                    break;
                }
            }
        }
    }
    return vf;
}

/**
 * If NO overlay (any overlay) is set to a volume, set the underlay to the first
 * volume that it finds.
 * @return Returns the volume file that was selected or NULL if no
 *    volume file was found.
 */
VolumeFile* 
OverlaySet::setUnderlayToVolume()
{
    VolumeFile * vf = this->getUnderlayVolume();
    
    if (vf == NULL) {
        const int32_t overlayIndex = this->getNumberOfDisplayedOverlays() - 1;
        if (overlayIndex >= 0) {
            std::vector<CaretMappableDataFile*> mapFiles;
            CaretMappableDataFile* mapFile;
            AString mapUniqueID;
            int32_t mapIndex;
            this->overlays[overlayIndex]->getSelectionData(mapFiles, 
                                                          mapFile, 
                                                          mapUniqueID, 
                                                          mapIndex);
            
            const int32_t numMapFiles = static_cast<int32_t>(mapFiles.size());
            for (int32_t i = 0; i < numMapFiles; i++) {
                vf = dynamic_cast<VolumeFile*>(mapFiles[i]);
                if (vf != NULL) {
                    this->overlays[overlayIndex]->setSelectionData(vf, 0);
                    break;
                }
            }
        }
    }
    
    return vf;
}

/**
 * Get the overlay at the specified index.
 * @param overlayNumber
 *   Index of the overlay.
 * @return Overlay at the given index.
 */
const Overlay* 
OverlaySet::getOverlay(const int32_t overlayNumber) const
{
    CaretAssertArrayIndex(this->overlays, 
                          BrainConstants::MAXIMUM_NUMBER_OF_OVERLAYS, 
                          overlayNumber);
    return this->overlays[overlayNumber];    
}

/**
 * Get the overlay at the specified index.
 * @param overlayNumber
 *   Index of the overlay.
 * @return Overlay at the given index.
 */
Overlay* 
OverlaySet::getOverlay(const int32_t overlayNumber)
{
    CaretAssertArrayIndex(this->overlays, 
                          BrainConstants::MAXIMUM_NUMBER_OF_OVERLAYS, 
                          overlayNumber);
    return this->overlays[overlayNumber];    
}

/**
 * Get a description of this object's content.
 * @return String describing this object's content.
 */
AString 
OverlaySet::toString() const
{
    return "OverlaySet";
}

/**
 * Add a displayed overlay.  If the maximum
 * number of surface overlays is reached,
 * this method has no effect.
 */
void 
OverlaySet::addDisplayedOverlay()
{
    this->numberOfDisplayedOverlays++;
    if (this->numberOfDisplayedOverlays > BrainConstants::MAXIMUM_NUMBER_OF_OVERLAYS) {
        this->numberOfDisplayedOverlays = BrainConstants::MAXIMUM_NUMBER_OF_OVERLAYS;
    }
}

/**
 * @return Returns the number of displayed overlays. 
 */
int32_t 
OverlaySet::getNumberOfDisplayedOverlays() const
{
    return this->numberOfDisplayedOverlays;
}

/**
 * Sets the number of displayed overlays.
 * @param numberOfDisplayedOverlays
 *   Number of overlays for display.
 */
void 
OverlaySet::setNumberOfDisplayedOverlays(const int32_t numberOfDisplayedOverlays)
{
    this->numberOfDisplayedOverlays = numberOfDisplayedOverlays;
    if (this->numberOfDisplayedOverlays < BrainConstants::MINIMUM_NUMBER_OF_OVERLAYS) {
        this->numberOfDisplayedOverlays = BrainConstants::MINIMUM_NUMBER_OF_OVERLAYS;
    }
    if (this->numberOfDisplayedOverlays > BrainConstants::MAXIMUM_NUMBER_OF_OVERLAYS) {
        this->numberOfDisplayedOverlays = BrainConstants::MAXIMUM_NUMBER_OF_OVERLAYS;
    }
}


/**
 * Remove a displayed overlay.  This method will have
 * no effect if the minimum number of overlays are
 * displayed
 *
 * @param overlayIndex 
 *    Index of overlay for removal from display.
 */
void 
OverlaySet::removeDisplayedOverlay(const int32_t overlayIndex)
{
    if (this->numberOfDisplayedOverlays > BrainConstants::MINIMUM_NUMBER_OF_OVERLAYS) {
        this->numberOfDisplayedOverlays--;
        for (int32_t i = overlayIndex; i < this->numberOfDisplayedOverlays; i++) {
            this->overlays[i]->copyData(this->overlays[i+1]);
        }
    }
}

/**
 * Move the overlay at the given index up one level
 * (swap it with overlayIndex - 1).  This method will
 * have no effect if the overlay is the top-most overlay.
 *
 * @param overlayIndex 
 *    Index of overlay that is to be moved up.
 */
void 
OverlaySet::moveDisplayedOverlayUp(const int32_t overlayIndex)
{
    if (overlayIndex > 0) {
        this->overlays[overlayIndex]->swapData(this->overlays[overlayIndex - 1]);
    }
}

/**
 * Move the overlay at the given index down one level
 * (swap it with overlayIndex + 1).  This method will
 * have no effect if the overlay is the bottom-most overlay.
 *
 * @param overlayIndex 
 *    Index of overlay that is to be moved down.
 */
void 
OverlaySet::moveDisplayedOverlayDown(const int32_t overlayIndex)
{
    const int32_t nextOverlayIndex = overlayIndex + 1;
    if (nextOverlayIndex < this->numberOfDisplayedOverlays) {
        this->overlays[overlayIndex]->swapData(this->overlays[nextOverlayIndex]);
    }
}

/**
 * Initialize the overlays for the model display controller.
 * @param mdc
 *    Model Display Controller.
 */
void 
OverlaySet::initializeOverlays()
{
    
    Brain* brain = NULL;
    if (this->modelDisplayController != NULL) {
        brain = this->modelDisplayController->getBrain();
    }
    else if (this->brainStructure != NULL) {
        brain = this->brainStructure->getBrain();
    }
    if (brain == NULL) {
        return;
    }
    
    /*
     * Find connectivity files giving preferense to dense over dense time series
     */
    std::vector<ConnectivityLoaderFile*> connFiles;
    if (brain != NULL) {
        brain->getConnectivityFilesOfAllTypes(connFiles);
    }
//    const int32_t numConnFiles = static_cast<int32_t>(connFiles.size());
    
    
    std::deque<CaretMappableDataFile*> shapeMapFiles;
    std::deque<int32_t> shapeMapFileIndices;
    
    std::deque<CaretMappableDataFile*> overlayMapFiles;
    std::deque<int32_t> overlayMapFileIndices;
    
    ModelVolume* mdcv = dynamic_cast<ModelVolume*>(this->modelDisplayController);
    ModelWholeBrain* mdcwb = dynamic_cast<ModelWholeBrain*>(this->modelDisplayController);
    ModelSurfaceMontage* mdcsm = dynamic_cast<ModelSurfaceMontage*>(this->modelDisplayController);

    if (this->brainStructure != NULL) {
        /*
         * Look for a shape map in metric
         */
        MetricFile* shapeMetricFile = NULL;
        int32_t     shapeMapIndex;
        if (brainStructure->getMetricShapeMap(shapeMetricFile, shapeMapIndex)) {
            shapeMapFiles.push_back(shapeMetricFile);
            shapeMapFileIndices.push_back(shapeMapIndex);
        }
        
        if (brainStructure->getNumberOfLabelFiles() > 0) {
            overlayMapFiles.push_back(brainStructure->getLabelFile(0));
            overlayMapFileIndices.push_back(0);
        }
        int32_t numMetricFiles = brainStructure->getNumberOfMetricFiles();
        for (int32_t i = 0; i < numMetricFiles; i++) {
            MetricFile* mf = brainStructure->getMetricFile(i);
            if (mf != shapeMetricFile) {
                overlayMapFiles.push_back(mf);
                overlayMapFileIndices.push_back(0);
            }
        }
        
        
    }
    else if (mdcv != NULL) {
        const int32_t numVolumes = brain->getNumberOfVolumeFiles();
        for (int32_t i = 0; i < numVolumes; i++) {
            VolumeFile* vf = brain->getVolumeFile(i);
            if ((vf->getType() == SubvolumeAttributes::ANATOMY)
                || (vf->getType() == SubvolumeAttributes::UNKNOWN)) {
                shapeMapFiles.push_back(vf);
                shapeMapFileIndices.push_back(0);
            }
            else if (vf->getType() == SubvolumeAttributes::FUNCTIONAL) {
                overlayMapFiles.push_back(vf);
                overlayMapFileIndices.push_back(0);
            }
            else if (vf->getType() == SubvolumeAttributes::LABEL) {
                overlayMapFiles.push_back(vf);
                overlayMapFileIndices.push_back(0);
            }
        }
    }
    else if ((mdcwb != NULL)
             || (mdcsm != NULL)){
        BrainStructure* leftBrainStructure = brain->getBrainStructure(StructureEnum::CORTEX_LEFT, false);
        BrainStructure* rightBrainStructure = brain->getBrainStructure(StructureEnum::CORTEX_RIGHT, false);
        
        /*
         * Look for a shape map in metric for left and right
         */
        MetricFile* leftShapeMetricFile = NULL;
        int32_t     leftShapeMapIndex;
        if (leftBrainStructure != NULL) {
            if (leftBrainStructure->getMetricShapeMap(leftShapeMetricFile, leftShapeMapIndex)) {
                shapeMapFiles.push_back(leftShapeMetricFile);
                shapeMapFileIndices.push_back(leftShapeMapIndex);
            }
        }
        MetricFile* rightShapeMetricFile = NULL;
        int32_t     rightShapeMapIndex;
        if (rightBrainStructure != NULL) {
            if (rightBrainStructure->getMetricShapeMap(rightShapeMetricFile, rightShapeMapIndex)) {
                shapeMapFiles.push_back(rightShapeMetricFile);
                shapeMapFileIndices.push_back(rightShapeMapIndex);
            }
        }
        
        if (leftBrainStructure != NULL) {
            const int numMetricFiles = leftBrainStructure->getNumberOfMetricFiles();
            const int numLabelFiles  = leftBrainStructure->getNumberOfLabelFiles();
            if (numLabelFiles > 0) {
                overlayMapFiles.push_back(leftBrainStructure->getLabelFile(0));
                overlayMapFileIndices.push_back(0);
            }
            if (numMetricFiles > 0) {
                for (int32_t i = 0; i < numMetricFiles; i++) {
                    MetricFile* mf = leftBrainStructure->getMetricFile(i);
                    if (mf != leftShapeMetricFile) {
                        if (leftShapeMetricFile != NULL) {
                            overlayMapFiles.push_back(mf);
                            overlayMapFileIndices.push_back(0);
                        }
                        else {
                            overlayMapFiles.push_front(mf);
                            overlayMapFileIndices.push_front(0);
                        }
                    }
                }
            }
        }

        if (rightBrainStructure != NULL) {
            const int numMetricFiles = rightBrainStructure->getNumberOfMetricFiles();
            const int numLabelFiles  = rightBrainStructure->getNumberOfLabelFiles();
            if (numLabelFiles > 0) {
                overlayMapFiles.push_back(rightBrainStructure->getLabelFile(0));
                overlayMapFileIndices.push_back(0);
            }
            if (numMetricFiles > 0) {
                for (int32_t i = 0; i < numMetricFiles; i++) {
                    MetricFile* mf = rightBrainStructure->getMetricFile(i);
                    if (mf != rightShapeMetricFile) {
                        if (rightShapeMetricFile != NULL) {
                            overlayMapFiles.push_back(mf);
                            overlayMapFileIndices.push_back(0);
                        }
                        else {
                            overlayMapFiles.push_front(mf);
                            overlayMapFileIndices.push_front(0);
                        }
                    }
                }
            }
        }
        
        if (mdcwb != NULL) {
            const int32_t numVolumes = brain->getNumberOfVolumeFiles();
            for (int32_t i = 0; i < numVolumes; i++) {
                VolumeFile* vf = brain->getVolumeFile(i);
                if ((vf->getType() == SubvolumeAttributes::ANATOMY)
                    || (vf->getType() == SubvolumeAttributes::UNKNOWN)) {
                    shapeMapFiles.push_back(vf);
                    shapeMapFileIndices.push_back(0);
                }
                else if (vf->getType() == SubvolumeAttributes::FUNCTIONAL) {
                    overlayMapFiles.push_back(vf);
                    overlayMapFileIndices.push_back(0);
                }
                else if (vf->getType() == SubvolumeAttributes::LABEL) {
                    overlayMapFiles.push_back(vf);
                    overlayMapFileIndices.push_back(0);
                }
            }
        }
    }
    else {
        CaretAssertMessage(0, "Invalid model controller: " + this->modelDisplayController->getNameForGUI(false));
    }
    
    /*
     * Place shape at bottom, overlay files in middle, and connectivity on top
     */
    const int32_t numShapeFiles = static_cast<int32_t>(shapeMapFiles.size());
    int32_t numOverlayMapFiles = static_cast<int32_t>(overlayMapFiles.size());
    
    /*
     * Limit to two connectivity files if there are overlay files
     * and put them in the front of the overlay map files
     */
// DISABLE adding connectivity files as of 17 May 2012
//    int32_t maxConnFiles = numConnFiles;
//    if (numOverlayMapFiles > 0) {
//        maxConnFiles = std::min(maxConnFiles, 2);
//    }
//    for (int32_t i = (maxConnFiles - 1); i >= 0; i--) {
//        overlayMapFiles.push_front(connFiles[i]);
//        overlayMapFileIndices.push_front(0);
//    }
    /* update count */
    numOverlayMapFiles = static_cast<int32_t>(overlayMapFiles.size()); 
    
    /*
     * Number of overlay that are displayed.
     */
    const int32_t numDisplayedOverlays = this->getNumberOfDisplayedOverlays();
    
    std::deque<CaretMappableDataFile*> mapFiles;
    std::deque<int32_t> mapFileIndices;

    /*
     * Find out how many overlay files may be used and add them
     */
    const int32_t maxOverlayFiles = std::min((numDisplayedOverlays - numShapeFiles),
                                             numOverlayMapFiles);
    for (int32_t i = 0; i < maxOverlayFiles; i++) {
        mapFiles.push_back(overlayMapFiles[i]);
        mapFileIndices.push_back(overlayMapFileIndices[i]);
    }
    
    /*
     * Put in the shape files
     */
    for (int32_t i = 0; i < numShapeFiles; i++) {
        if (static_cast<int32_t>(mapFiles.size()) >= numDisplayedOverlays) {
            break;
        }
        mapFiles.push_back(shapeMapFiles[i]);
        mapFileIndices.push_back(shapeMapFileIndices[i]);
    }
    
    /*
     * Set the overlays
     */
    const int32_t numMapFiles = std::min(static_cast<int32_t>(mapFiles.size()), 
                                         this->getNumberOfDisplayedOverlays());
    for (int32_t i = 0; i < numMapFiles; i++) {
        this->getOverlay(i)->setSelectionData(mapFiles[i], mapFileIndices[i]);
    }
}

/**
 * Get any label files that are selected and applicable for the given surface.
 * @param surface
 *    Surface for which label files are desired.
 * @param labelFilesOut
 *    Label files that are applicable to the given surface.
 * @param labelMapIndicesOut
 *    Selected map indices in the output label files.
 */
void 
OverlaySet::getLabelFilesForSurface(const Surface* surface,
                                    std::vector<LabelFile*>& labelFilesOut,
                                    std::vector<int32_t>& labelMapIndicesOut)
{
    CaretAssert(surface);
    
    labelFilesOut.clear();
    labelMapIndicesOut.clear();
    
    const int32_t numberOfOverlays = this->getNumberOfDisplayedOverlays();
    for (int32_t i = 0; i < numberOfOverlays; i++) {
        Overlay* overlay = this->getOverlay(i);
        if (overlay->isEnabled()) {
            CaretMappableDataFile* mapFile;
            int32_t mapIndex;
            overlay->getSelectionData(mapFile, mapIndex);
            if (mapFile != NULL) {
                if (mapFile->getDataFileType() == DataFileTypeEnum::LABEL) {
                    if (mapFile->getStructure() == surface->getStructure()) {
                        LabelFile* labelFile = dynamic_cast<LabelFile*>(mapFile);
                        labelFilesOut.push_back(labelFile);
                        labelMapIndicesOut.push_back(mapIndex);
                    }
                }
            }
        }
    }
}



