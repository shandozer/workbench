#ifndef __MAP_SETTINGS_FIBER_TRAJECTORY_WIDGET__H_
#define __MAP_SETTINGS_FIBER_TRAJECTORY_WIDGET__H_

/*LICENSE_START*/
/*
 * Copyright 2012 Washington University,
 * All rights reserved.
 *
 * Connectome DB and Connectome Workbench are part of the integrated Connectome 
 * Informatics Platform.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * Neither the names of Washington University nor the
 *      names of its contributors may be used to endorse or promote products
 *      derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR  
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 */
/*LICENSE_END*/

#include <set>

#include <QWidget>

#include "FiberTrajectoryDisplayModeEnum.h"

class QButtonGroup;
class QComboBox;
class QDoubleSpinBox;
class QRadioButton;

namespace caret {
    class CaretMappableDataFile;
    class CiftiFiberTrajectoryFile;

    class MapSettingsFiberTrajectoryWidget : public QWidget {
        
        Q_OBJECT

    public:
        MapSettingsFiberTrajectoryWidget(QWidget* parent = 0);
        
        virtual ~MapSettingsFiberTrajectoryWidget();
        
        void updateEditor(CiftiFiberTrajectoryFile* fiberTrajectoryFile);
        
        void updateWidget();
        // ADD_NEW_METHODS_HERE
        
    private slots:
        void processAttributesChanges();
        
    private:
        MapSettingsFiberTrajectoryWidget(const MapSettingsFiberTrajectoryWidget&);

        MapSettingsFiberTrajectoryWidget& operator=(const MapSettingsFiberTrajectoryWidget&);
        
        QWidget* createAttributesWidget();
        
        QWidget* createDisplayModeWidget();
        
        QWidget* createDataMappingWidget();
        
        CiftiFiberTrajectoryFile* m_fiberTrajectoryFile;
        
        QComboBox* m_colorSelectionComboBox;
        
        std::vector<QRadioButton*> m_displayModeRadioButtons;
        std::vector<FiberTrajectoryDisplayModeEnum::Enum> m_displayModeRadioButtonData;
        QButtonGroup* m_displayModeButtonGroup;
        
        QDoubleSpinBox* m_proportionStreamlineSpinBox;
        
        QDoubleSpinBox* m_proportionMinimumSpinBox;
        
        QDoubleSpinBox* m_proportionMaximumSpinBox;
        
        QDoubleSpinBox* m_countStreamlineSpinBox;
        
        QDoubleSpinBox* m_countMinimumSpinBox;
        
        QDoubleSpinBox* m_countMaximumSpinBox;
        
        QDoubleSpinBox* m_distanceStreamlineSpinBox;
        
        QDoubleSpinBox* m_distanceMinimumSpinBox;
        
        QDoubleSpinBox* m_distanceMaximumSpinBox;
        
        bool m_updateInProgress;
        
        // ADD_NEW_MEMBERS_HERE
        
    };
    
#ifdef __MAP_SETTINGS_FIBER_TRAJECTORY_WIDGET_DECLARE__
#endif // __MAP_SETTINGS_FIBER_TRAJECTORY_WIDGET_DECLARE__

} // namespace
#endif  //__MAP_SETTINGS_FIBER_TRAJECTORY_WIDGET__H_