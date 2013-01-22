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

#include <algorithm>
#include <cmath>

#include <QContextMenuEvent>
#include <QMouseEvent>
#include <QWheelEvent>

#define __BRAIN_OPENGL_WIDGET_DEFINE__
#include "BrainOpenGLWidget.h"
#undef __BRAIN_OPENGL_WIDGET_DEFINE__

#include "Border.h"
#include "Brain.h"
#include "BrainOpenGLFixedPipeline.h"
#include "BrainOpenGLShape.h"
#include "BrainOpenGLWidgetContextMenu.h"
#include "BrainOpenGLWidgetTextRenderer.h"
#include "BrainOpenGLViewportContent.h"
#include "BrainStructure.h"
#include "BrowserTabContent.h"
#include "CaretAssert.h"
#include "CaretLogger.h"
#include "CursorManager.h"
#include "EventModelGetAll.h"
#include "EventManager.h"
#include "EventBrowserWindowContentGet.h"
#include "EventGraphicsUpdateAllWindows.h"
#include "EventGraphicsUpdateOneWindow.h"
#include "EventGetOrSetUserInputModeProcessor.h"
#include "EventUserInterfaceUpdate.h"
#include "GuiManager.h"
#include "SelectionManager.h"
#include "SelectionItemSurfaceNode.h"
#include "MathFunctions.h"
#include "Matrix4x4.h"
#include "Model.h"
#include "ModelYokingGroup.h"
#include "MouseEvent.h"
#include "Surface.h"
#include "UserInputModeBorders.h"
#include "UserInputModeFoci.h"
#include "UserInputModeView.h"

using namespace caret;

/**
 * Constructor.
 * 
 * @param
 *   The parent widget.
 */
BrainOpenGLWidget::BrainOpenGLWidget(QWidget* parent,
                                     const int32_t windowIndex)
: QGLWidget(parent)
{
    this->openGL = NULL;
    this->borderBeingDrawn = new Border();
    this->borderBeingDrawn->setColor(CaretColorEnum::RED);
    this->textRenderer = new BrainOpenGLWidgetTextRenderer(this);
    this->windowIndex = windowIndex;
    this->userInputBordersModeProcessor = new UserInputModeBorders(this->borderBeingDrawn,
                                                                   windowIndex);
    this->userInputFociModeProcessor = new UserInputModeFoci(windowIndex);
    this->userInputViewModeProcessor = new UserInputModeView();
    this->selectedUserInputProcessor = this->userInputViewModeProcessor;
    this->selectedUserInputProcessor->initialize();
    this->mousePressX = -10000;
    this->mousePressY = -10000;
    EventManager::get()->addEventListener(this, EventTypeEnum::EVENT_GRAPHICS_UPDATE_ALL_WINDOWS);
    EventManager::get()->addEventListener(this, EventTypeEnum::EVENT_GRAPHICS_UPDATE_ONE_WINDOW);
    EventManager::get()->addEventListener(this, EventTypeEnum::EVENT_GET_OR_SET_USER_INPUT_MODE);
}

/**
 * Destructor.
 */
BrainOpenGLWidget::~BrainOpenGLWidget()
{
    makeCurrent();
    
    this->clearDrawingViewportContents();
    
    if (this->textRenderer != NULL) {
        delete this->textRenderer;
        this->textRenderer = NULL;
    }
    
    if (this->openGL != NULL) {
        delete this->openGL;
        this->openGL = NULL;
    }
    delete this->userInputViewModeProcessor;
    delete this->userInputBordersModeProcessor;
    delete this->userInputFociModeProcessor;
    this->selectedUserInputProcessor = NULL; // DO NOT DELETE since it does not own the object to which it points
    
    delete this->borderBeingDrawn;
    
    EventManager::get()->removeAllEventsFromListener(this);
}

/**
 * Initializes graphics.
 */
void 
BrainOpenGLWidget::initializeGL()
{
    if (this->openGL == NULL) {
        this->openGL = new BrainOpenGLFixedPipeline(this->textRenderer); //GuiManager::get()->getBrainOpenGL();
    }
    this->openGL->initializeOpenGL();
    
    this->lastMouseX = 0;
    this->lastMouseY = 0;
    this->isMousePressedNearToolBox = false;

    this->setFocusPolicy(Qt::StrongFocus);
    
    QGLFormat format = this->format();
    
    AString msg = ("OpenGL Context:"
                   "\n   Accum: " + AString::fromBool(format.accum())
                   + "\n   Accum size: " + AString::number(format.accumBufferSize())
                   + "\n   Alpha: " + AString::fromBool(format.alpha())
                   + "\n   Alpha size: " + AString::number(format.alphaBufferSize())
                   + "\n   Depth: " + AString::fromBool(format.depth())
                   + "\n   Depth size: " + AString::number(format.depthBufferSize())
                   + "\n   Direct Rendering: " + AString::fromBool(format.directRendering())
                   + "\n   Red size: " + AString::number(format.redBufferSize())
                   + "\n   Green size: " + AString::number(format.greenBufferSize())
                   + "\n   Blue size: " + AString::number(format.blueBufferSize())
                   + "\n   Double Buffer: " + AString::fromBool(format.doubleBuffer())
                   + "\n   RGBA: " + AString::fromBool(format.rgba())
                   + "\n   Samples: " + AString::fromBool(format.sampleBuffers())
                   + "\n   Samples size: " + AString::number(format.samples())
                   + "\n   Stencil: " + AString::fromBool(format.stencil())
                   + "\n   Stencil size: " + AString::number(format.stencilBufferSize())
                   + "\n   Swap Interval: " + AString::number(format.swapInterval())
                   + "\n   Stereo: " + AString::fromBool(format.stereo())
                   + "\n   Major Version: " + AString::number(format.majorVersion())
                   + "\n   Minor Version: " + AString::number(format.minorVersion()));
            
    msg += ("\n\n" + BrainOpenGL::getOpenGLInformation());

    CaretLogConfig(msg);
    
    if (s_openGLVersionInformation.isEmpty()) {
        s_openGLVersionInformation = msg;
    }
    
    if (s_defaultGLFormatInitialized == false) {
        CaretLogSevere("PROGRAM ERROR: The default QGLFormat has not been set.\n"
                       "Need to call BrainOpenGLWidget::initializeDefaultGLFormat() prior to "
                       "instantiating an instance of this class.");
    }
}

/**
 * @return Information about OpenGL.
 */
QString
BrainOpenGLWidget::getOpenGLInformation()
{
    return s_openGLVersionInformation;
}

/**
 * Called when widget is resized.
 */
void 
BrainOpenGLWidget::resizeGL(int w, int h)
{
    this->windowWidth[this->windowIndex] = w;
    this->windowHeight[this->windowIndex] = h;
}

void
BrainOpenGLWidget::getViewPortSize(int &w, int &h)
{
    w = this->windowWidth[this->windowIndex];
    h = this->windowHeight[this->windowIndex];
}

/**
 * @return Pointer to the border that is being drawn.
 */
Border* 
BrainOpenGLWidget::getBorderBeingDrawn()
{
    return this->borderBeingDrawn;
}

/**
 * Clear the contents for drawing into the viewports.
 */
void 
BrainOpenGLWidget::clearDrawingViewportContents()
{
    const int32_t num = static_cast<int32_t>(this->drawingViewportContents.size());
    for (int32_t i = 0; i < num; i++) {
        delete this->drawingViewportContents[i];
    }
    this->drawingViewportContents.clear();
}

/**
 * Paints the graphics.
 */
void 
BrainOpenGLWidget::paintGL()
{
    /*
     * Set the cursor to that requested by the user input processor
     */
    CursorEnum::Enum cursor = this->selectedUserInputProcessor->getCursor();
    
    GuiManager::get()->getCursorManager()->setCursorForWidget(this,
                                                              cursor);
    
    this->clearDrawingViewportContents();
    
    int windowViewport[4] = {
        0,
        0,
        this->windowWidth[this->windowIndex],
        this->windowHeight[this->windowIndex]
    };
    
    EventBrowserWindowContentGet getModelEvent(this->windowIndex);
    EventManager::get()->sendEvent(getModelEvent.getPointer());

    if (getModelEvent.isError()) {
        return;
    }
    
    const int32_t numToDraw = getModelEvent.getNumberOfItemsToDraw();
    if (numToDraw == 1) {
        BrainOpenGLViewportContent* vc = new BrainOpenGLViewportContent(windowViewport,
                                                                        windowViewport,
                                                                        GuiManager::get()->getBrain(),
                                                                        getModelEvent.getTabContentToDraw(0));
        this->drawingViewportContents.push_back(vc);
    }
    else if (numToDraw > 1) {
        /**
         * Determine the number of rows and columns for the montage.
         * Since screen width typically exceeds height, always have
         * columns greater than or equal to rows.
         */
        int32_t numRows = (int)std::sqrt((double)numToDraw);
        int32_t numCols = numRows;
        int32_t row2 = numRows * numRows;
        if (row2 < numToDraw) {
            numCols++;
        }
        if ((numRows * numCols) < numToDraw) {
            numRows++;
        }
        
        int32_t vpX = 0;
        int32_t vpY = 0;
        const int32_t vpWidth = this->windowWidth[this->windowIndex] / numCols;
        const int32_t vpHeight = this->windowHeight[this->windowIndex] / numRows;
        
        int32_t iModel = 0;
        for (int32_t i = 0; i < numRows; i++) {
            vpX = 0;
            for (int32_t j = 0; j < numCols; j++) {
                if (iModel < numToDraw) {
                    const int modelViewport[4] = {
                        vpX,
                        vpY,
                        vpWidth,
                        vpHeight
                    };
                    BrainOpenGLViewportContent* vc = 
                       new BrainOpenGLViewportContent(modelViewport,
                                                      modelViewport,
                                                      GuiManager::get()->getBrain(),
                                                      getModelEvent.getTabContentToDraw(iModel));
                    this->drawingViewportContents.push_back(vc);
                }
                iModel++;
                vpX += vpWidth;
            }
            vpY += vpHeight;
        }
    }
    
    if (this->selectedUserInputProcessor == userInputBordersModeProcessor) {
        this->openGL->setBorderBeingDrawn(this->borderBeingDrawn);
    }
    else {
        this->openGL->setBorderBeingDrawn(NULL);
    }
    this->openGL->drawModels(this->drawingViewportContents);
}

/**
 * Receive Content Menu events from Qt.
 * @param contextMenuEvent
 *    The context menu event.
 */
void 
BrainOpenGLWidget::contextMenuEvent(QContextMenuEvent* contextMenuEvent)
{
    const int x = contextMenuEvent->x();
    const int y1 = contextMenuEvent->y();
    const int y = this->height() - y1;
    
    BrainOpenGLViewportContent* viewportContent = this->getViewportContentAtXY(x, y);
    if (viewportContent == NULL) {
        return;
    }
    BrowserTabContent* tabContent = viewportContent->getBrowserTabContent();
    if (tabContent == NULL) {
        return;
    }
    
    SelectionManager* idManager = this->performIdentification(x,
                                                                   y,
                                                                   false);
    
    BrainOpenGLWidgetContextMenu contextMenu(idManager,
                                             tabContent,
                                             this);
    contextMenu.exec(contextMenuEvent->globalPos());
}

/**
 * Receive Mouse Wheel events from Qt.
 * @param we
 *   The wheel event.
 */
void 
BrainOpenGLWidget::wheelEvent(QWheelEvent* we)
{
    const Qt::KeyboardModifiers keyModifiers = we->modifiers();
    
    const int wheelX = we->x();
    const int wheelY = this->windowHeight[this->windowIndex] - we->y();
    int delta = we->delta();
    delta = MathFunctions::limitRange(delta, -2, 2);
    
    MouseEvent mouseEvent(this->windowIndex,
                          MouseEventTypeEnum::WHEEL_MOVED,
                          keyModifiers,
                          wheelX,
                          wheelY,
                          0,
                          delta);
    this->processMouseEvent(&mouseEvent);
    
    we->accept();
}

/*
 * If there is a middle button and it is pressed with not keys depressed,
 * set mouse action to left button with shift key down to perform
 * panning in some mouse modes.
 *
 * @param mouseButtons
 *     Button state when event was generated
 * @param button
 *     Button that caused the event.
 * @param keyModifiers
 *     Keys that are down, may be modified.
 * @param isMouseMoving
 *     True if mouse is moving, else false.
 */
void
BrainOpenGLWidget::checkForMiddleMouseButton(Qt::MouseButtons& mouseButtons,
                                             Qt::MouseButton& button,
                                             Qt::KeyboardModifiers& keyModifiers,
                                             const bool isMouseMoving)
{
    if (isMouseMoving) {
        if (button == Qt::NoButton) {
            if (mouseButtons == Qt::MiddleButton) {
                if (keyModifiers == Qt::NoButton) {
                    mouseButtons = Qt::LeftButton;
                    button = Qt::NoButton;
                    keyModifiers = Qt::ShiftModifier;
                }
            }
        }
    }
    else {
        if (button == Qt::MiddleButton) {
            if (keyModifiers == Qt::NoButton) {
                button = Qt::LeftButton;
                keyModifiers = Qt::ShiftModifier;
            }
        }
    }
}

/**
 * Receive mouse press events from Qt.
 * @param me
 *    The mouse event.
 */
void 
BrainOpenGLWidget::mousePressEvent(QMouseEvent* me)
{
    Qt::MouseButton button = me->button();
    Qt::KeyboardModifiers keyModifiers = me->modifiers();
    Qt::MouseButtons mouseButtons = me->buttons();
    
    checkForMiddleMouseButton(mouseButtons,
                              button,
                              keyModifiers,
                              false);
    
    this->isMousePressedNearToolBox = false;
    
    if (button == Qt::LeftButton) {
        const int mouseX = me->x();
        const int mouseY = this->windowHeight[this->windowIndex] - me->y();

        this->mousePressX = mouseX;
        this->mousePressY = mouseY;
        
        MouseEvent mouseEvent(this->windowIndex,
                              MouseEventTypeEnum::LEFT_PRESSED,
                              keyModifiers,
                              mouseX,
                              mouseY,
                              0,
                              0);
        this->processMouseEvent(&mouseEvent);
        
        this->lastMouseX = mouseX;
        this->lastMouseY = mouseY;

        this->mouseMovementMinimumX = mouseX;
        this->mouseMovementMaximumX = mouseX;
        this->mouseMovementMinimumY = mouseY;
        this->mouseMovementMaximumY = mouseY;
        
        /*
         * The user may intend to increase the size of a toolbox
         * but instead misses the edge of the toolbox when trying
         * to drag the toolbox and make it larger.  So, indicate
         * when the user is very close to the edge of the graphics
         * window.
         */
        const int nearToolBoxDistance = 5;
        if ((mouseX < nearToolBoxDistance) 
            || (mouseX > (this->windowWidth[this->windowIndex] - 5))
            || (mouseY < nearToolBoxDistance) 
            || (mouseY > (this->windowHeight[this->windowIndex] - 5))) {
            this->isMousePressedNearToolBox = true;
        }
    }
    else {
        this->mousePressX = -10000;
        this->mousePressY = -10000;
    }
    
    me->accept();
}

/**
 * Receive mouse button release events from Qt.
 * @param me
 *    The mouse event.
 */
void 
BrainOpenGLWidget::mouseReleaseEvent(QMouseEvent* me)
{
    Qt::MouseButton button = me->button();
    Qt::KeyboardModifiers keyModifiers = me->modifiers();
    Qt::MouseButtons mouseButtons = me->buttons();

    checkForMiddleMouseButton(mouseButtons,
                              button,
                              keyModifiers,
                              false);
    
    if (button == Qt::LeftButton) {
        const int mouseX = me->x();
        const int mouseY = this->windowHeight[this->windowIndex] - me->y();
        
        this->mouseMovementMinimumX = std::min(this->mouseMovementMinimumX, mouseX);
        this->mouseMovementMaximumX = std::max(this->mouseMovementMaximumX, mouseX);
        this->mouseMovementMinimumY = std::min(this->mouseMovementMinimumY, mouseY);
        this->mouseMovementMaximumY = std::max(this->mouseMovementMaximumY, mouseY);
        
        const int dx = this->mouseMovementMaximumX - this->mouseMovementMinimumX;
        const int dy = this->mouseMovementMaximumY - this->mouseMovementMinimumY;
        const int absDX = (dx >= 0) ? dx : -dx;
        const int absDY = (dy >= 0) ? dy : -dy;

        if ((absDX <= BrainOpenGLWidget::MOUSE_MOVEMENT_TOLERANCE) 
            && (absDY <= BrainOpenGLWidget::MOUSE_MOVEMENT_TOLERANCE)) {
            MouseEvent mouseEvent(this->windowIndex,
                                  MouseEventTypeEnum::LEFT_CLICKED,
                                  keyModifiers,
                                  mouseX,
                                  mouseY,
                                  dx,
                                  dy);
            this->processMouseEvent(&mouseEvent);
        }
        else {
            MouseEvent mouseEvent(this->windowIndex,
                                  MouseEventTypeEnum::LEFT_RELEASED,
                                  keyModifiers,
                                  mouseX,
                                  mouseY,
                                  dx,
                                  dy);
            this->processMouseEvent(&mouseEvent);
        }
    }
    
    this->mousePressX = -10000;
    this->mousePressY = -10000;
    this->isMousePressedNearToolBox = false;
    
    me->accept();
}

/**
 * Get the viewport content at the given location.
 * @param x
 *    X-coordinate.
 * @param y
 *    Y-coordinate.
 */
BrainOpenGLViewportContent* 
BrainOpenGLWidget::getViewportContentAtXY(const int x,
                                          const int y)
{
    BrainOpenGLViewportContent* viewportContent = NULL;
    const int32_t num = static_cast<int32_t>(this->drawingViewportContents.size());
    for (int32_t i = 0; i < num; i++) {
        int viewport[4];
        this->drawingViewportContents[i]->getModelViewport(viewport);
        if ((x >= viewport[0])
            && (x < (viewport[0] + viewport[2]))
            && (y >= viewport[1])
            && (y < (viewport[1] + viewport[3]))) {
            viewportContent = this->drawingViewportContents[i];
            break;
        }
    }
    return viewportContent;
}

/**
 * Perform identification.
 *
 * @param x
 *    X-coordinate for identification.
 * @param y
 *    Y-coordinate for identification.
 * @param applySelectionBackgroundFiltering
 *    If true (which is in most cases), if there are multiple items
 *    selected, those items "behind" other items are not reported.
 *    For example, suppose a focus is selected and there is a node
 *    the focus.  If this parameter is true, the node will NOT be
 *    selected.  If this parameter is false, the node will be
 *    selected.
 * @return
 *    SelectionManager providing identification information.
 */
SelectionManager* 
BrainOpenGLWidget::performIdentification(const int x,
                                         const int y,
                                         const bool applySelectionBackgroundFiltering)
{
    BrainOpenGLViewportContent* idViewport = this->getViewportContentAtXY(x, y);

    this->makeCurrent();
    CaretLogFine("Performing selection");
    SelectionManager* idManager = GuiManager::get()->getBrain()->getSelectionManager();
    idManager->reset();
//    idManager->getSurfaceTriangleIdentification()->setEnabledForSelection(true);
//    idManager->getSurfaceNodeIdentification()->setEnabledForSelection(true);
    
    if (idViewport != NULL) {
        /*
         * ID coordinate needs to be relative to the viewport
         *
        int vp[4];
        idViewport->getViewport(vp);
        const int idX = x - vp[0];
        const int idY = y - vp[1];
         */
        this->openGL->selectModel(idViewport, 
                                  x, 
                                  y,
                                  applySelectionBackgroundFiltering);
    }
    return idManager;
}

void 
BrainOpenGLWidget::performProjection(const int x,
                                     const int y,
                                     SurfaceProjectedItem& projectionOut)
{
    BrainOpenGLViewportContent* projectionViewport = this->getViewportContentAtXY(x, y);
    
    this->makeCurrent();
    CaretLogFine("Performing projection");
    
    if (projectionViewport != NULL) {
        /*
         * ID coordinate needs to be relative to the viewport
         *
         int vp[4];
         idViewport->getViewport(vp);
         const int idX = x - vp[0];
         const int idY = y - vp[1];
         */
        this->openGL->projectToModel(projectionViewport,
                                     x,
                                     y,
                                     projectionOut);
    }
}


/** 
 * Receive mouse move events from Qt.
 * @param me
 *    The mouse event.
 */
void 
BrainOpenGLWidget::mouseMoveEvent(QMouseEvent* me)
{
    Qt::MouseButton button = me->button();
    Qt::KeyboardModifiers keyModifiers = me->modifiers();
    Qt::MouseButtons mouseButtons = me->buttons();
    
    checkForMiddleMouseButton(mouseButtons,
                              button,
                              keyModifiers,
                              true);
    
    if (button == Qt::NoButton) {
        if (mouseButtons == Qt::LeftButton) {
            const int mouseX = me->x();
            const int mouseY = this->windowHeight[this->windowIndex] - me->y();
            
            this->mouseMovementMinimumX = std::min(this->mouseMovementMinimumX, mouseX);
            this->mouseMovementMaximumX = std::max(this->mouseMovementMaximumX, mouseX);
            this->mouseMovementMinimumY = std::min(this->mouseMovementMinimumY, mouseY);
            this->mouseMovementMaximumY = std::max(this->mouseMovementMaximumY, mouseY);
            
            const int dx = mouseX - this->lastMouseX;
            const int dy = mouseY - this->lastMouseY;
            const int absDX = (dx >= 0) ? dx : -dx;
            const int absDY = (dy >= 0) ? dy : -dy;
            
            if ((absDX > 0) 
                || (absDY > 0)) { 
                MouseEvent mouseEvent(this->windowIndex,
                                      MouseEventTypeEnum::LEFT_DRAGGED,
                                      keyModifiers,
                                      mouseX,
                                      mouseY,
                                      dx,
                                      dy);
                this->processMouseEvent(&mouseEvent);
            }
            
            this->lastMouseX = mouseX;
            this->lastMouseY = mouseY;
        }
    }
    
    me->accept();
}

/**
 * Process a mouse event by sending it to the current
 * user input processor.
 *
 * @param mouseEvent
 *    Mouse event for processing.
 */
void 
BrainOpenGLWidget::processMouseEvent(MouseEvent* mouseEvent)
{
    CaretLogFiner(mouseEvent->toString());
    
    if (mouseEvent->isValid()) {        
        /*
         * Ignore mouse movement near edge of graphics
         */
        if (this->isMousePressedNearToolBox == false) {
            /*
             * Use location of mouse press so that the model
             * being manipulated does not change if mouse moves
             * out of its viewport without releasing the mouse
             * button.
             */
            BrainOpenGLViewportContent* viewportContent = NULL;
            
            if (mouseEvent->getMouseEventType() == MouseEventTypeEnum::WHEEL_MOVED) {
                viewportContent = this->getViewportContentAtXY(mouseEvent->getX(), 
                                                               mouseEvent->getY());
            }
            else {
                viewportContent = this->getViewportContentAtXY(this->mousePressX, 
                                                               this->mousePressY);
            }
            
            if (viewportContent != NULL) {
                BrowserTabContent* browserTabContent = viewportContent->getBrowserTabContent();
                if (browserTabContent != NULL) {
                    this->selectedUserInputProcessor->processMouseEvent(mouseEvent,
                                                                        viewportContent,
                                                                        this);
                }
            }
        }
    }
}

/**
 * Receive events from the event manager.
 * 
 * @param event
 *   Event sent by event manager.
 */
void 
BrainOpenGLWidget::receiveEvent(Event* event)
{
    if (event->getEventType() == EventTypeEnum::EVENT_GRAPHICS_UPDATE_ALL_WINDOWS) {
        EventGraphicsUpdateAllWindows* updateAllEvent =
            dynamic_cast<EventGraphicsUpdateAllWindows*>(event);
        CaretAssert(updateAllEvent);
        
        updateAllEvent->setEventProcessed();
        
        if (updateAllEvent->isRepaint()) {
            this->repaint();
        }
        else {
            this->updateGL();
        }
    }
    else if (event->getEventType() == EventTypeEnum::EVENT_GRAPHICS_UPDATE_ONE_WINDOW) {
        EventGraphicsUpdateOneWindow* updateOneEvent =
        dynamic_cast<EventGraphicsUpdateOneWindow*>(event);
        CaretAssert(updateOneEvent);
        
        if (updateOneEvent->getWindowIndex() == this->windowIndex) {
            updateOneEvent->setEventProcessed();
            
            this->updateGL();
        }
        else {
            /*
             * If a window is yoked, update its graphics.
             */
            EventBrowserWindowContentGet getModelEvent(this->windowIndex);
            EventManager::get()->sendEvent(getModelEvent.getPointer());
            
            if (getModelEvent.isError()) {
                return;
            }
            
            const int32_t numItemsToDraw = getModelEvent.getNumberOfItemsToDraw();
            bool needUpdate = false;
            if (numItemsToDraw > 0) {
                for (int32_t i = 0; i < numItemsToDraw; i++) {
                    BrowserTabContent* btc = getModelEvent.getTabContentToDraw(0);
                    if (btc != NULL) {
                        Model* mdc = btc->getModelControllerForDisplay();
                        ModelYokingGroup* myg = btc->getSelectedYokingGroupForModel(mdc);
                        if (myg != NULL) {
                            needUpdate = true;
                            break;
                        }
                    }
                }
            }
            if (needUpdate) {
                this->updateGL();
            }
        }
    }
    else if (event->getEventType() == EventTypeEnum::EVENT_GET_OR_SET_USER_INPUT_MODE) {
        EventGetOrSetUserInputModeProcessor* inputModeEvent =
        dynamic_cast<EventGetOrSetUserInputModeProcessor*>(event);
        CaretAssert(inputModeEvent);
        
        if (inputModeEvent->getWindowIndex() == this->windowIndex) {
            if (inputModeEvent->isGetUserInputMode()) {
                inputModeEvent->setUserInputProcessor(this->selectedUserInputProcessor);
            }
            else if (inputModeEvent->isSetUserInputMode()) {
                UserInputReceiverInterface* newUserInputProcessor = NULL;
                switch (inputModeEvent->getUserInputMode()) {
                    case UserInputReceiverInterface::INVALID:
                        CaretAssertMessage(0, "INVALID is NOT allowed for user input mode");
                        break;
                    case UserInputReceiverInterface::BORDERS:
                        newUserInputProcessor = this->userInputBordersModeProcessor;
                        break;
                    case UserInputReceiverInterface::FOCI:
                        newUserInputProcessor = this->userInputFociModeProcessor;
                        break;
                    case UserInputReceiverInterface::VIEW:
                        newUserInputProcessor = this->userInputViewModeProcessor;
                        break;
                }
                
                if (newUserInputProcessor != NULL) {
                    if (newUserInputProcessor != this->selectedUserInputProcessor) {
                        this->selectedUserInputProcessor->finish();
                        this->selectedUserInputProcessor = newUserInputProcessor;
                        this->selectedUserInputProcessor->initialize();
                    }
                }
            }
            inputModeEvent->setEventProcessed();
        }
    }
    else {
        
    }
}

/**
 * Capture an image of the window's graphics area using 
 * the given image size.  If either of the image dimensions
 * is zero, the image will be the size of the graphcis 
 * area.
 *
 * @param imageSizeX
 *    Desired X size of image.
 * @param imageSizeY
 *    Desired X size of image.
 * @return
 *    An image of the graphics area.
 */
QImage 
BrainOpenGLWidget::captureImage(const int32_t imageSizeX,
                                const int32_t imageSizeY)
{
    const int oldSizeX = this->windowWidth[this->windowIndex];
    const int oldSizeY = this->windowHeight[this->windowIndex];
    
    /*
     * Force immediate mode since problems with display lists
     * in image capture.
     */
    BrainOpenGLShape::setImmediateModeOverride(true);
    
    QPixmap pixmap = this->renderPixmap(imageSizeX,
                                        imageSizeY);
    QImage image = pixmap.toImage();
    BrainOpenGLShape::setImmediateModeOverride(false);
    
    this->resizeGL(oldSizeX, oldSizeY);
    
    return image;
}

/**
 * Initialize the OpenGL format.  This must be called
 * prior to initializing an instance of this class so
 * that the OpenGL is setup properly.
 */
void
BrainOpenGLWidget::initializeDefaultGLFormat()
{
    QGLFormat glfmt;
    glfmt.setAccum(false);
    glfmt.setAlpha(true);
    glfmt.setAlphaBufferSize(8);
    glfmt.setDepth(true);
    glfmt.setDepthBufferSize(24);
    glfmt.setDirectRendering(true);
    glfmt.setDoubleBuffer(true);
    glfmt.setOverlay(false);
    glfmt.setSampleBuffers(false);
    glfmt.setStencil(false);
    glfmt.setStereo(false);
    
    glfmt.setRgba(true);
    glfmt.setRedBufferSize(8);
    glfmt.setGreenBufferSize(8);
    glfmt.setBlueBufferSize(8);
    QGLFormat::setDefaultFormat(glfmt);
    
    s_defaultGLFormatInitialized = true;
}


