
/*LICENSE_START*/
/*
 *  Copyright (C) 2014  Washington University School of Medicine
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
/*LICENSE_END*/

#include <iostream>

#define __WU_Q_FACTORY_DECLARE__
#include "WuQFactory.h"
#undef __WU_Q_FACTORY_DECLARE__

#include "CaretLogger.h"

using namespace caret;


    
/**
 * \class caret::WuQFactory 
 * \brief A factory for creating Qt Widgets with desired attributes.
 * \ingroup GuiQt
 *
 * This factory creates Qt Widgets with some attributes set to desired
 * values.  For QSpinBox, by default, issues value changed events as 
 * each character is entered into the spin box.  So, if the user enters
 * five numbers, five events are issued, which at times, can cause
 * undesired updates to the user-interface.  In addition, the methods in
 * this factory can simplify the creation of new widgets.
 * <p>
 * Qt3 contained overloaded constructors that contained parameters for
 * widget attributes but these were removed in Qt4 most likely because
 * there was no way to clearly indicate the purpose of the parameters other
 * than the documentation.  Thus, in Qt4 a widget must be created and then
 * methods called to set the widget's attributes.  Using the a naming
 * convention in these factory methods removes the ambiguity about any
 * parameters.
 * <p>
 * Rules for the method naming:<br>
 * <ol><li> use the pattern "new<WidgetType>[OptionalParameterNames][WithSignal[Type]]<br>
 * <ul>
 * <li> A method name always begins with <b>new</b>
 * <li> <b><i>WidgetType</i></b> is the name of the Qt widget without the <b>Q</b>
 * <li> <b><i>OptionalParameterNames</i></b> describe any additional parameters
 * <li> <b><i>WithSignal</i></b> is at the end of the method name for those methods
 * that connect a value changed signal to a receiving class and slot.  If a parameter
 * is passed, the type should also be indicated (WithSignalInt, WithSignalString, etc.). 
 * Keep in mind that the receiving slot does not need to contain any parameters even
 * if the signal does contain parameters.  In most cases, the signal is only issued
 * if the user changes the value.  However, some widgets issue a signal when either the
 * user changes the value or the value is changed programmatically and this should be 
 * noted in the documentation.
 * </ul>
 * <li> Do not use default parameters.
 * <li> Do not overload method names (identically named methods containing different parameters).
 * <li> This naming convention may result in long names.  However, the names will make the usage fairly obvious.
 * <li> These naming patterns is based off that for Objective-C.
 * </ol>
 */

#include <limits>

#include <QComboBox>
#include <QDoubleSpinBox>
#include <QEvent>
#include <QSpinBox>
#include <QStyleFactory>

#include "WuQEventBlockingFilter.h"

/**
 * Constructor.
 */
WuQFactory::WuQFactory()
{
    
}

/**
 * Destructor.
 */
WuQFactory::~WuQFactory()
{
    
}

/**
 * Create a combo box.
 *
 * On Apple computers, the mouse wheel can cause unintended changes
 * to a combo box even if the combo box does not have focus.  So,
 * block the wheel event on Apple computers.  In addition, on Apple 
 * computers, every item that is in the combo box is displayed in 
 * the pop-up menu (as large as the vertical size of the screen) 
 * so use a Windows Style combo box to improve the usability.
 *
 * @return
 *   A combo box.
 */
QComboBox*
WuQFactory::newComboBox()
{
    QComboBox* cb = new QComboBox();
    
#ifdef CARET_OS_MACOSX
    /*
     * Attach an event filter that blocks wheel events in the combo box if Mac
     */
    WuQEventBlockingFilter* comboBoxWheelEventBlockingFilter = new WuQEventBlockingFilter(cb);
    comboBoxWheelEventBlockingFilter->setEventBlocked(QEvent::Wheel,
                                                                true);
    cb->installEventFilter(comboBoxWheelEventBlockingFilter);
    
    //setWindowsStyleForApple(cb);
#endif // CARET_OS_MACOSX
    
    return cb;
}

/**
 * Create a combo box.
 *
 * On Apple computers, the mouse wheel can cause unintended changes
 * to a combo box even if the combo box does not have focus.  So,
 * block the wheel event on Apple computers.  In addition, on Apple
 * computers, every item that is in the combo box is displayed in
 * the pop-up menu (as large as the vertical size of the screen)
 * so use a Windows Style combo box to improve the usability.
 *
 * @param receiver
 *   Object that received the signal when the value is changed by the user.
 * @param method
 *   Method that is connected to the spin box's valueChanged(int)
 *   signal.
 * @return
 *   A combo box.
 */
QComboBox*
WuQFactory::newComboBoxSignalInt(QObject* receiver,
                                       const char* method)
{
    QComboBox* cb = newComboBox();
    QObject::connect(cb, SIGNAL(activated(int)),
                     receiver, method);
    return cb;
}

/**
 * Create a spin box.
 *
 * The minimum value is the most negative 32-bit integer, the maximum
 * values is the most positive 32-bit integer, step size is one, and 
 * the default value is zero.
 * Keyboard tracking is disabled so that signal are NOT issued when
 * the user changes the text contained in the spin box.
 *
 * @return
 *   A QSpinBox initialized with the default parameters.
 */
QSpinBox*
WuQFactory::newSpinBox()
{
    QSpinBox* sb = newSpinBoxWithMinMaxStep(std::numeric_limits<int>::min(),
                                            std::numeric_limits<int>::max(),
                                            1);
    return sb;
}

/**
 * Create a spin box.
 *
 * The minimum value is the most negative 32-bit integer, the maximum
 * values is the most positive 32-bit integer, step size is one, and
 * the default value is zero.
 * Keyboard tracking is disabled so that signal are NOT issued when
 * the user changes the text contained in the spin box.
 * <p>
 * NOTE: The signal contains an integer parameters that is the new value
 * contained in the spin box.  The signal is emitted when the user
 * changes the value AND when the value is changed programatically.
 *
 * @param receiver
 *   Object that received the signal when the value is changed.
 * @param method
 *   Method that is connected to the spin box's valueChanged(int)
 *   signal.
 * @return
 *   A QSpinBox initialized with the default parameters.
 */
QSpinBox*
WuQFactory::newSpinBoxWithSignalInt(QObject* receiver,
                                    const char* method)
{
    QSpinBox* sb = newSpinBoxWithMinMaxStepSignalInt(std::numeric_limits<int>::min(),
                                                     std::numeric_limits<int>::max(),
                                                     1,
                                                     receiver,
                                                     method);
    return sb;
}



/**
 * Create a spin box with the given minimum, maximum, and step values.
 * Keyboard tracking is disabled so that signal are NOT issued when
 * the user changes the text contained in the spin box.
 * <p>
 * The default value is zero.  If zero is not within the given 
 * minimum and maximum values, the default value is the minimum value.
 * 
 * @param minimumValue
 *   Minimum value for spin box.
 * @param maximumValue
 *   Maximum value for spin box.
 * @param stepSize
 *   Step (change in value) when the user increments the spin box.
 * @return 
 *   A QSpinBox initialized with the given parameters.
 */
QSpinBox*
WuQFactory::newSpinBoxWithMinMaxStep(const int minimumValue,
                                     const int maximumValue,
                                     const int stepSize)
{
    QSpinBox* sb = new QSpinBox();
    sb->setMinimum(minimumValue);
    sb->setMaximum(maximumValue);
    sb->setSingleStep(stepSize);
    sb->setKeyboardTracking(false);
    if ((0 >= minimumValue)
        && (0 <= maximumValue)) {
        sb->setValue(0);
    }
    else {
        sb->setValue(minimumValue);
    }
    return sb;
}


/**
 * Create a spin box with the given minimum, maximum, and step values.
 * Keyboard tracking is disabled so that signal are NOT issued when
 * the user changes the text contained in the spin box.
 * <p>
 * The default value is zero.  If zero is not within the given
 * minimum and maximum values, the default value is the minimum value.
 * <p>
 * NOTE: The signal contains an integer parameters that is the new value
 * contained in the spin box.  The signal is emitted when the user 
 * changes the value AND when the value is changed programatically.
 *
 * @param minimumValue
 *   Minimum value for spin box.
 * @param maximumValue
 *   Maximum value for spin box.
 * @param stepSize
 *   Step (change in value) when the user increments the spin box.
 * @param receiver
 *   Object that received the signal when the value is changed.
 * @param method
 *   Method that is connected to the spin box's valueChanged(int)
 *   signal.
 * @return
 *   A QSpinBox initialized with the given parameters.
 */
QSpinBox*
WuQFactory::newSpinBoxWithMinMaxStepSignalInt(const int minimumValue,
                                              const int maximumValue,
                                              const int stepSize,
                                              QObject* receiver,
                                              const char* method)
{
    QSpinBox* sb = newSpinBoxWithMinMaxStep(minimumValue,
                                            maximumValue,
                                            stepSize);
    QObject::connect(sb,
                     SIGNAL(valueChanged(int)),
                     receiver,
                     method);
    return sb;
}

/**
 * Create a double spin box with the minimum value set to the most negative
 * float (not double) value, the maximum value set to the most positive
 * float (not double) value, the step size set to one and the number of 
 * digits right of the decimal set to two.  The default value is zero.
 * Keyboard tracking is disabled so that signal are NOT issued when
 * the user changes the text contained in the spin box.
 * <p>
 * Since workbench stores most data as 32-bit floats, the default minimum and
 * maximum values are those for float, not double.
 *
 * @return
 *   A QDoubleSpinBox initialized with the default parameters.
 */
QDoubleSpinBox*
WuQFactory::newDoubleSpinBox()
{
    QDoubleSpinBox* sb = newDoubleSpinBoxWithMinMaxStepDecimals(-std::numeric_limits<float>::max(),
                                                                std::numeric_limits<float>::max(),
                                                                1.0,
                                                                2);
    return sb;
}

/**
 * Create a double spin box with the minimum value set to the most negative
 * float (not double) value, the maximum value set to the most positive
 * float (not double) value, the step size set to one and the number of
 * digits right of the decimal set to two.  The default value is zero.
 * Keyboard tracking is disabled so that signal are NOT issued when
 * the user changes the text contained in the spin box.
 * <p>
 * Since workbench stores most data as 32-bit floats, the default minimum and
 * maximum values are those for float, not double.
 *
 * <p>
 * NOTE: The signal contains an double parameter that is the new value
 * contained in the spin box.  The signal is emitted when the user
 * changes the value AND when the value is changed programatically.
 *
 * @param receiver
 *   Object that received the signal when the value is changed.
 * @param method
 *   Method that is connected to the spin box's valueChanged(double)
 *   signal.
 * @return
 *   A QDoubleSpinBox initialized with the default parameters.
 */
QDoubleSpinBox*
WuQFactory::newDoubleSpinBoxWithSignalDouble(QObject* receiver,
                                             const char* method)
{
    QDoubleSpinBox* sb = newDoubleSpinBoxWithMinMaxStepDecimalsSignalDouble(-std::numeric_limits<float>::max(),
                                                                std::numeric_limits<float>::max(),
                                                                1.0,
                                                                2,
                                                                receiver,
                                                                method);
    return sb;
}


/**
 * Create a double spin box with the given minimum, maximum, step, and
 * digits right of decimal values.
 * Keyboard tracking is disabled so that signal are NOT issued when
 * the user changes the text contained in the spin box.
 * <p>
 * The default value is zero.  If zero is not within the given
 * minimum and maximum values, the default value is the minimum value.
 *
 * @param minimumValue
 *   Minimum value for spin box.
 * @param maximumValue
 *   Maximum value for spin box.
 * @param stepSize
 *   Step (change in value) when the user increments the spin box.
 * @param digitsRightOfDecimal
 *   Number of digits to right of decimal shown in the spin box.
 * @return
 *   A QDoubleSpinBox initialized with the given parameters.
 */
QDoubleSpinBox*
WuQFactory::newDoubleSpinBoxWithMinMaxStepDecimals(const double minimumValue,
                                                              const double maximumValue,
                                                              const double stepSize,
                                                              const int digitsRightOfDecimal)
{
    QDoubleSpinBox* sb = new QDoubleSpinBox();
    sb->setMinimum(minimumValue);
    sb->setMaximum(maximumValue);
    sb->setSingleStep(stepSize);
    sb->setDecimals(digitsRightOfDecimal);
    sb->setKeyboardTracking(false);
    if ((0 >= minimumValue)
        && (0 <= maximumValue)) {
        sb->setValue(0);
    }
    else {
        sb->setValue(minimumValue);
    }
    return sb;
}

/**
 * Create a double spin box with the given minimum, maximum, step, and 
 * digits right of decimal values.
 * Keyboard tracking is disabled so that signal are NOT issued when
 * the user changes the text contained in the spin box.
 * <p>
 * The default value is zero.  If zero is not within the given
 * minimum and maximum values, the default value is the minimum value.
 * <p>
 * NOTE: The signal contains an double parameter that is the new value
 * contained in the spin box.  The signal is emitted when the user
 * changes the value AND when the value is changed programatically.
 *
 * @param minimumValue
 *   Minimum value for spin box.
 * @param maximumValue
 *   Maximum value for spin box.
 * @param stepSize
 *   Step (change in value) when the user increments the spin box.
 * @param digitsRightOfDecimal
 *   Number of digits to right of decimal shown in the spin box.
 * @param receiver
 *   Object that received the signal when the value is changed.
 * @param method
 *   Method that is connected to the spin box's valueChanged(double)
 *   signal.
 * @return
 *   A QDoubleSpinBox initialized with the given parameters.
 */
QDoubleSpinBox*
WuQFactory::newDoubleSpinBoxWithMinMaxStepDecimalsSignalDouble(const double minimumValue,
                                                                          const double maximumValue,
                                                                          const double stepSize,
                                                                          const int digitsRightOfDecimal,
                                                                          QObject* receiver,
                                                                          const char* method)
{
    QDoubleSpinBox* sb = newDoubleSpinBoxWithMinMaxStepDecimals(minimumValue,
                                                                maximumValue,
                                                                stepSize,
                                                                digitsRightOfDecimal);
    QObject::connect(sb,
                     SIGNAL(valueChanged(double)),
                     receiver,
                     method);
    return sb;
}

/**
 * Sets the style of the given windows to windows.
 */
void
WuQFactory::setWindowsStyleForApple(QWidget* w)
{
//    w->setStyle(new WorkbenchMacStyle());
//    return;
    
    /*
     * Only try creating once
     */
    if (s_windowsStyleForAppleWasCreated == false) {
        s_windowsStyleForAppleWasCreated = true;
        
        s_windowsStyleForApple = QStyleFactory::create("Windows");
        if (s_windowsStyleForApple == NULL) {
            CaretLogSevere("Failed to create Windows Style");
        }
    }
    
    if (s_windowsStyleForApple != NULL) {
        w->setStyle(s_windowsStyleForApple);
    }
}

//int
//WorkbenchMacStyle::styleHint ( StyleHint sh, const QStyleOption * opt, const QWidget * w, QStyleHintReturn * hret) const
//{
//    int value = QMacStyle::styleHint(sh, opt, w, hret);
//    
//    if (sh == QStyle::SH_ComboBox_Popup) {
//        value = 0;
//    }
//    
//    return value;
//}

