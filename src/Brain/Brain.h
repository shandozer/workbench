#ifndef __BRAIN_H__
#define __BRAIN_H__

/*
 *  Copyright 1995-2002 Washington University School of Medicine
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
/*LICENSE_END*/

#include <vector>
#include <stdint.h>

#include "CaretObject.h"
#include "DataFileTypeEnum.h"
#include "DataFileException.h"
#include "DisplayGroupEnum.h"
#include "EventListenerInterface.h"
#include "SceneableInterface.h"
#include "StructureEnum.h"

namespace caret {
    
    class Border;
    class BorderFile;
    class FociFile;
    class BrainStructure;
    class CaretDataFile;
    class CiftiScalarFile;
    class ConnectivityLoaderFile;
    class ConnectivityLoaderManager;
    class DisplayProperties;
    class DisplayPropertiesBorders;
    class DisplayPropertiesFiberOrientation;
    class DisplayPropertiesFoci;
    class DisplayPropertiesInformation;
    class DisplayPropertiesLabels;
    class DisplayPropertiesSurface;
    class DisplayPropertiesVolume;
    class EventDataFileRead;
    class EventSpecFileReadDataFiles;
    class IdentificationManager;    
    class LabelFile;
    class MetricFile;
    class ModelSurfaceMontage;
    class ModelVolume;
    class ModelWholeBrain;
    class PaletteFile;
    class RgbaFile;
    class SceneClassAssistant;
    class SceneFile;
    class Surface;
    class SurfaceFile;
    class SurfaceProjectedItem;
    class SpecFile;
    class VolumeFile;
    
    class Brain : public CaretObject, public EventListenerInterface, public SceneableInterface {

    public:
        Brain();
        
        ~Brain();
        
    private:
        Brain(const Brain&);
        Brain& operator=(const Brain&);
        
    public:
        int getNumberOfBrainStructures() const;
        
        void addBrainStructure(BrainStructure* brainStructure);
        
        BrainStructure* getBrainStructure(const int32_t indx);

        const BrainStructure* getBrainStructure(const int32_t indx) const;
        
        BrainStructure* getBrainStructure(StructureEnum::Enum structure,
                                          bool createIfNotFound);

        int32_t getNumberOfBorderFiles() const;
        
        BorderFile* getBorderFile(const int32_t indx);
        
        const BorderFile* getBorderFile(const int32_t indx) const;
        
        BorderFile* addBorderFile();
        
        enum NearestBorderTestMode {
            NEAREST_BORDER_TEST_MODE_ALL_POINTS,
            NEAREST_BORDER_TEST_MODE_ENDPOINTS
        };
        bool findBorderNearestBorder(const DisplayGroupEnum::Enum displayGroup,
                                     const int32_t browserTabIndex,
                                     const SurfaceFile* surfaceFile,
                                    const Border* border,
                                    const NearestBorderTestMode borderTestMode,
                                    const float maximumDistance,
                                    BorderFile*& borderFileOut,
                                    int32_t& borderFileIndexOut,
                                    Border*& borderOut,
                                    int32_t& borderIndexOut,
                                    SurfaceProjectedItem*& borderPointOut,
                                    int32_t& borderPointIndexOut,
                                    float& distanceToBorderPointOut) const;
                                    
        bool findBorderNearestXYZ(const DisplayGroupEnum::Enum displayGroup,
                                  const int32_t browserTabIndex,
                                  const SurfaceFile* surfaceFile,
                                 const float xyz[3],
                                 const float maximumDistance,
                                 BorderFile*& borderFileOut,
                                 int32_t& borderFileIndexOut,
                                 Border*& borderOut,
                                 int32_t& borderIndexOut,
                                 SurfaceProjectedItem*& borderPointOut,
                                 int32_t& borderPointIndexOut,
                                 float& distanceToBorderPointOut) const;
        
        int32_t getNumberOfFociFiles() const;
        
        FociFile* getFociFile(const int32_t indx);
        
        const FociFile* getFociFile(const int32_t indx) const;
        
        FociFile* addFociFile();
        
        PaletteFile* getPaletteFile();
        
        const PaletteFile* getPaletteFile() const;

        SceneFile* addSceneFile();
        
        int32_t getNumberOfSceneFiles() const;
        
        SceneFile* getSceneFile(const int32_t indx);
        
        const SceneFile* getSceneFile(const int32_t indx) const;
        
        AString getSpecFileName() const;
        
        bool isSpecFileValid() const;
        
        Surface* getSurfaceWithName(const AString& surfaceFileName,
                                    const bool useAbsolutePath);
        
        std::vector<const Surface*> getVolumeInteractionSurfaces() const;
        
        std::vector<const SurfaceFile*> getVolumeInteractionSurfaceFiles() const;
        
        int32_t getNumberOfVolumeFiles() const;
        
        VolumeFile* getVolumeFile(const int32_t volumeFileIndex);
        
        const VolumeFile* getVolumeFile(const int32_t volumeFileIndex) const;
        
        void resetBrain();
        
        void resetBrainKeepSceneFiles();
        
        void receiveEvent(Event* event);
        
        ConnectivityLoaderManager* getConnectivityLoaderManager();
        
        const ConnectivityLoaderManager* getConnectivityLoaderManager() const;
        
        void getMappableConnectivityFilesOfAllTypes(std::vector<ConnectivityLoaderFile*>& connectivityFilesOfAllTypes) const;
        
        int32_t getNumberOfConnectivityDenseFiles() const;
        
        ConnectivityLoaderFile* getConnectivityDenseFile(int32_t indx);
        
        const ConnectivityLoaderFile* getConnectivityDenseFile(int32_t indx) const;
        
        void getConnectivityDenseFiles(std::vector<ConnectivityLoaderFile*>& connectivityDenseFilesOut) const;
        
        int32_t getNumberOfConnectivityDenseLabelFiles() const;
        
        ConnectivityLoaderFile* getConnectivityDenseLabelFile(int32_t indx);
        
        const ConnectivityLoaderFile* getConnectivityDenseLabelFile(int32_t indx) const;
        
        void getConnectivityDenseLabelFiles(std::vector<ConnectivityLoaderFile*>& connectivityDenseLabelFilesOut) const;
        
        int32_t getNumberOfConnectivityDenseScalarFiles() const;
        
        CiftiScalarFile* getConnectivityDenseScalarFile(int32_t indx);
        
        const CiftiScalarFile* getConnectivityDenseScalarFile(int32_t indx) const;
        
        void getConnectivityDenseScalarFiles(std::vector<CiftiScalarFile*>& connectivityDenseScalarFilesOut) const;
        
        int32_t getNumberOfConnectivityFiberOrientationFiles() const;
        
        ConnectivityLoaderFile* getConnectivityFiberOrientationFile(int32_t indx);
        
        const ConnectivityLoaderFile* getConnectivityFiberOrientationFile(int32_t indx) const;
        
        void getConnectivityFiberOrientationFiles(std::vector<ConnectivityLoaderFile*>& connectivityFiberOrientationFilesOut) const;
        
        int32_t getNumberOfConnectivityTimeSeriesFiles() const;
        
        ConnectivityLoaderFile* getConnectivityTimeSeriesFile(int32_t indx);
        
        const ConnectivityLoaderFile* getConnectivityTimeSeriesFile(int32_t indx) const;
        
        void getConnectivityTimeSeriesFiles(std::vector<ConnectivityLoaderFile*>& connectivityTimeSeriesFilesOut) const;
        
        AString getCurrentDirectory() const;
        
        void setCurrentDirectory(const AString& currentDirectory);
        
        void getAllDataFiles(std::vector<CaretDataFile*>& allDataFilesOut) const;
        
        bool isFileValid(const CaretDataFile* caretDataFile) const;

        void determineDisplayedDataFiles();
        
        bool areFilesModified(const bool excludeConnectivityFiles,
                              const bool excludeSceneFiles);
        
        void writeDataFile(CaretDataFile* caretDataFile,
                           const bool isAddToSpecFile) throw (DataFileException);
        
        bool removeDataFile(CaretDataFile* caretDataFile);
        
        DisplayPropertiesBorders* getDisplayPropertiesBorders();
        
        const DisplayPropertiesBorders* getDisplayPropertiesBorders() const;
        
        DisplayPropertiesFiberOrientation* getDisplayPropertiesFiberOrientation();
        
        const DisplayPropertiesFiberOrientation* getDisplayPropertiesFiberOrientation() const;
        
        DisplayPropertiesFoci* getDisplayPropertiesFoci();
        
        const DisplayPropertiesFoci* getDisplayPropertiesFoci() const;
        
        DisplayPropertiesVolume* getDisplayPropertiesVolume();
        
        const DisplayPropertiesVolume* getDisplayPropertiesVolume() const;
        
        DisplayPropertiesSurface* getDisplayPropertiesSurface();
        
        const DisplayPropertiesSurface* getDisplayPropertiesSurface() const;
        
        
        DisplayPropertiesLabels* getDisplayPropertiesLabels();
        
        const DisplayPropertiesLabels* getDisplayPropertiesLabels() const;
        
        DisplayPropertiesInformation* getDisplayPropertiesInformation();
        
        const DisplayPropertiesInformation* getDisplayPropertiesInformation() const;
        
        void copyDisplayProperties(const int32_t sourceTabIndex,
                                   const int32_t targetTabIndex);
        
        virtual SceneClass* saveToScene(const SceneAttributes* sceneAttributes,
                                        const AString& instanceName);
        
        virtual void restoreFromScene(const SceneAttributes* sceneAttributes,
                                      const SceneClass* sceneClass);
        
        IdentificationManager* getIdentificationManager();
        
    private:
        enum ResetBrainKeepSceneFiles {
            RESET_BRAIN_KEEP_SCENE_FILES_NO,
            RESET_BRAIN_KEEP_SCENE_FILES_YES
        };
        
        enum ResetBrainKeepSpecFile {
            RESET_BRAIN_KEEP_SPEC_FILE_NO,
            RESET_BRAIN_KEEP_SPEC_FILE_YES
        };
        
        void loadFilesSelectedInSpecFile(EventSpecFileReadDataFiles* readSpecFileDataFilesEvent);
        
        void loadSpecFileFromScene(const SceneAttributes* sceneAttributes,
                                   SpecFile* specFile,
                          const ResetBrainKeepSceneFiles keepSceneFile,
                          const ResetBrainKeepSpecFile keepSpecFile);
        
        void resetBrain(const ResetBrainKeepSceneFiles keepSceneFiles,
                        const ResetBrainKeepSpecFile keepSpecFile);
        
        void processReadDataFileEvent(EventDataFileRead* readDataFileEvent);
        
        void readDataFile(const DataFileTypeEnum::Enum dataFileType,
                          const StructureEnum::Enum structure,
                          const AString& dataFileName,
                          const bool addDataFileToSpecFile) throw (DataFileException);
        
        LabelFile* readLabelFile(const AString& filename,
                           const StructureEnum::Enum structure) throw (DataFileException);
        
        MetricFile* readMetricFile(const AString& filename,
                            const StructureEnum::Enum structure) throw (DataFileException);
        
        RgbaFile* readRgbaFile(const AString& filename,
                          const StructureEnum::Enum structure) throw (DataFileException);
        
        Surface* readSurfaceFile(const AString& filename,
                             const StructureEnum::Enum structure) throw (DataFileException);
        
        void readVolumeFile(const AString& filename) throw (DataFileException);
                            
        void readBorderFile(const AString& filename) throw (DataFileException);
        
        void readConnectivityDenseFile(const AString& filename) throw (DataFileException);
        
        void readConnectivityDenseLabelFile(const AString& filename) throw (DataFileException);
        
        void readConnectivityDenseScalarFile(const AString& filename) throw (DataFileException);
        
        void readConnectivityFiberOrientationFile(const AString& filename) throw (DataFileException);
        
        void readConnectivityTimeSeriesFile(const AString& filename) throw (DataFileException);
        
        void validateConnectivityFile(const ConnectivityLoaderFile* clf) throw (DataFileException);
        
        void readFociFile(const AString& filename) throw (DataFileException);
        
        void readPaletteFile(const AString& filename) throw (DataFileException);
        
        void readSceneFile(const AString& filename) throw (DataFileException);
        
        AString updateFileNameForReading(const AString& filename);
        
        AString updateFileNameForWriting(const AString& filename) throw (DataFileException);
        
        void updateVolumeSliceController();
        
        void updateWholeBrainController();
        
        void updateSurfaceMontageController();
        
        std::vector<BrainStructure*> m_brainStructures;
        
        std::vector<BorderFile*> m_borderFiles;
        
        std::vector<FociFile*> m_fociFiles;
        
        std::vector<SceneFile*> m_sceneFiles;
        
        PaletteFile* m_paletteFile;
        
        std::vector<ConnectivityLoaderFile*> m_connectivityDenseFiles;
        
        std::vector<ConnectivityLoaderFile*> m_connectivityDenseLabelFiles;
        
        std::vector<CiftiScalarFile*> m_connectivityDenseScalarFiles;
        
        std::vector<ConnectivityLoaderFile*> m_connectivityFiberOrientationFiles;
        
        std::vector<ConnectivityLoaderFile*> m_connectivityTimeSeriesFiles;
        
        mutable AString m_currentDirectory;
        
        AString m_specFileName;
        
        std::vector<VolumeFile*> m_volumeFiles;
        
        ModelVolume* m_volumeSliceController;
        
        ModelWholeBrain* m_wholeBrainController;
        
        ModelSurfaceMontage* m_surfaceMontageController;
        
        ConnectivityLoaderManager* m_connectivityLoaderManager;
        
        /** contains all display properties */
        std::vector<DisplayProperties*> m_displayProperties;
        
        /**
         * Display properties for volume - DO NOT delete since this
         * is also in the displayProperties std::vector.
         */
        DisplayPropertiesVolume* m_displayPropertiesVolume;
        
        /**
         * Display properties for surface - DO NOT delete since this
         * is also in the displayProperties std::vector.
         */
        DisplayPropertiesSurface* m_displayPropertiesSurface;
        
        /**
         * Display properties for labels - DO NOT delete since this
         * is also in the displayProperties std::vector.
         */
        DisplayPropertiesLabels* m_displayPropertiesLabels;
        
        /**
         * Display properties for borders - DO NOT delete since this
         * is also in the displayProperties std::vector.
         */
        DisplayPropertiesBorders* m_displayPropertiesBorders;
        
        /**
         * Display properties for fiber orientation - DO NOT delete since this
         * is also in the displayProperties std::vector.
         */
        DisplayPropertiesFiberOrientation* m_displayPropertiesFiberOrientation;
        
        /**
         * Display properties for foci - DO NOT delete since this
         * is also in the displayProperties std::vector.
         */
        DisplayPropertiesFoci* m_displayPropertiesFoci;
        
        /**
         * Display properties for information - DO NOT delete since this
         * is also in the displayProperties std::vector.
         */
        DisplayPropertiesInformation* m_displayPropertiesInformation;
        
        /** true when a spec file is being read */
        bool m_isSpecFileBeingRead;
        
        AString m_fileReadingUsername;
        
        AString m_fileReadingPassword;
        
        SceneClassAssistant* m_sceneAssistant;
        
        /** Identification manager */
        IdentificationManager* m_identificationManager;
    };

} // namespace

#endif // __BRAIN_H__

