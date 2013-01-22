#ifndef __OVERLAY__H_
#define __OVERLAY__H_

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

#include "CaretObject.h"
#include "DataFileTypeEnum.h"
#include "EventListenerInterface.h"
#include "SceneableInterface.h"
#include "WholeBrainVoxelDrawingMode.h"

namespace caret {
    class BrainStructure;
    class CaretMappableDataFile;
    class Model;
    class ModelSurface;
    class ModelSurfaceMontage;
    class ModelVolume;
    class ModelWholeBrain;
    class ModelYokingGroup;
    class SceneClassAssistant;
    
    class Overlay : public CaretObject, public EventListenerInterface, public SceneableInterface {
        
    public:
        Overlay(BrainStructure* brainStructure);
        
        Overlay(ModelVolume* modelDisplayControllerVolume);
        
        Overlay(ModelWholeBrain* modelDisplayControllerWholeBrain);
        
        Overlay(ModelYokingGroup* modelDisplayControllerYokingGroup);
        
        Overlay(ModelSurfaceMontage* modelDisplayControllerSurfaceMontage);
        
        virtual ~Overlay();
        
        virtual void receiveEvent(Event* event);
        
        float getOpacity() const;
        
        void setOpacity(const float opacity);
        
        AString getName() const;
        
        void setOverlayNumber(const int32_t overlayIndex);
        
        virtual AString toString() const;
        
        bool isEnabled() const;
        
        void setEnabled(const bool enabled);
        
        WholeBrainVoxelDrawingMode::Enum getWholeBrainVoxelDrawingMode() const;
        
        void setWholeBrainVoxelDrawingMode(const WholeBrainVoxelDrawingMode::Enum wholeBrainVoxelDrawingMode);
        
        void copyData(const Overlay* overlay);
        
        void swapData(Overlay* overlay);
        
        void getSelectionData(DataFileTypeEnum::Enum& mapFileTypeOut,
                              AString& selectedMapUniqueIDOut);
        
        void getSelectionData(std::vector<CaretMappableDataFile*>& mapFilesOut,
                              CaretMappableDataFile* &selectedMapFileOut,
                              AString& selectedMapUniqueIDOut,
                              int32_t& selectedMapIndexOut);
        
        void getSelectionData(CaretMappableDataFile* &selectedMapFileOut,
                              int32_t& selectedMapIndexOut);
        
        void setSelectionData(CaretMappableDataFile* selectedMapFile,
                              const int32_t selectedMapIndex);
        
        bool isPaletteDisplayEnabled() const;
        
        void setPaletteDisplayEnabled(const bool enabled);
        
        virtual SceneClass* saveToScene(const SceneAttributes* sceneAttributes,
                                        const AString& instanceName);
        
        virtual void restoreFromScene(const SceneAttributes* sceneAttributes,
                                      const SceneClass* sceneClass);
    private:
        Overlay(const Overlay&);

        Overlay& operator=(const Overlay&);

        void initializeOverlay(Model* modelDisplayController,
                               BrainStructure* brainStructure);
        
        /** Brain structure in this overlay (NULL if this overlay is not assigned to a brain structure */
        BrainStructure* m_brainStructure;
        
        /** Volume controller using this overlay (NULL if this overlay is not assigned to a volume controller) */
        ModelVolume* m_volumeController;
        
        /** Whole brain controller using this overlay (NULL if this overlay is not assigned to a whole brain controller) */
        ModelWholeBrain* m_wholeBrainController;
        
        /** Surfaced Montage controller using this overlay (NULL if this overlay is not assigned to a surface montage controller) */
        ModelSurfaceMontage* m_surfaceMontageController;
        
        /** Name of overlay (DO NOT COPY)*/
        AString m_name;
        
        /** Index of this overlay (DO NOT COPY)*/
        int32_t m_overlayIndex;
        
        /** opacity for overlay */
        float m_opacity;
        
        /** enabled status */
        bool m_enabled;
        
        /** available mappable files */
        //std::vector<CaretMappableDataFile*> m_mapFiles;
        
        /** selected mappable file */
        CaretMappableDataFile* m_selectedMapFile;
        
        /** selected data file map unique id */
        AString m_selectedMapUniqueID;
        
        /** Display palette in graphics window */
        bool m_paletteDisplayedFlag;
        
        /** Voxel drawing mode in Whole Brain View */
        WholeBrainVoxelDrawingMode::Enum m_wholeBrainVoxelDrawingMode;
        
        /** helps with scene save/restore */
        SceneClassAssistant* m_sceneAssistant;
    };
    
#ifdef __OVERLAY_DECLARE__
    // <PLACE DECLARATIONS OF STATIC MEMBERS HERE>
#endif // __OVERLAY_DECLARE__

} // namespace
#endif  //__OVERLAY__H_
