/****************************************************************************
**
** Copyright (C) 2012 Denis Shienkov <denis.shienkov@gmail.com>
** Copyright (C) 2012 Laszlo Papp <lpapp@kde.org>
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtSerialPort module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia. For licensing terms and
** conditions see http://qt.digia.com/licensing. For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights. These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QtGlobal>

#include <QMainWindow>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QComboBox>
#include <QSlider>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>

#include <QtSerialPort/QSerialPort>

QT_BEGIN_NAMESPACE

namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE

class Console;
class SettingsDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    enum FLIGHT_MODE {RETURN = 0, MANUAL = 1, ALTCTL = 2, POSCTL = 3, MISSION = 4, LOITER = 5};
    ~MainWindow();

private slots:
    /** Serial port dialog */
    void openSerialPort();
    void closeSerialPort();
    void about();
    void writeData(const QByteArray &data);
    void readData();

    void handleError(QSerialPort::SerialPortError error);

    /** Manual control sliders */
   void onSetThrottle(int t);
   void onSetYaw(int y);
   void onSetPitch(int p);
   void onSetRoll(int r);

   /** Arming state */
   void onSetArming();

   /** Flight mode swtich */
   void onSetFlightMode();

   /** Safety Mode */
   void onTriggerSafety();


private:
    void initActionsConnections();

    void createInfoGroupBox();
    void createMediaGroupBox();
    void createControlSlidersGroupBox();
    void createMapGroupBox();
    void createFlightModeControlGroupBox();

    void createArmingGroupBox();
    void createWaypointGroupBox();
    void createFlightModeGroupBox();

private:
    Ui::MainWindow *ui;

    /** Console */
    Console *console;
    SettingsDialog *settings;
    QSerialPort *serial;

    /** UAV Control main window */
    QWidget* widget;

    QGroupBox* infoGroupBox;
    QGroupBox* mediaGroupBox;
    QGroupBox* controlSlidersGroupBox;
    QGroupBox* mapGroupBox;
    QGroupBox* flightModeControlGroupBox;
    QGroupBox* consoleGroupBox;

    /** Info: real-time feedback from the UAV */
    QLabel* xPosLabel;
    QLabel* yPosLabel;
    QLabel* zPosLabel;
    int xPosValue;
    int yPosValue;
    int zPosValue;

    QLabel* xAcce;
    QLabel* yAcce;
    QLabel* zAcce;
    int xAcceValue;
    int yAcceValue;
    int zAcceValue;

    QLabel* xGyro;
    QLabel* yGyro;
    QLabel* zGyro;
    int xGyroValue;
    int yGyroValue;
    int zGyroValue;

    QLabel* xMag;
    QLabel* yMag;
    QLabel* zMag;
    int xMagValue;
    int yMagValue;
    int zMagValue;

    /** Media */
    QLabel* songListLabel;
    QComboBox* songListBox;
    QPushButton* toggleButton;


    /** Manual control sliders */
    int throttle_control;
    int yaw_control;
    int pitch_control;
    int roll_control;

    QSlider* throttleSlider;
    QSlider* yawSlider;
    QSlider* pitchSlider;
    QSlider* rollSlider;

    QLabel* throttleLabel;
    QLabel* yawLabel;
    QLabel* pitchLabel;
    QLabel* rollLabel;

    QLabel* throttleValue;
    QLabel* yawValue;
    QLabel* pitchValue;
    QLabel* rollValue;

    /** Tab and map */
    QTabWidget* tabWidget;
    QLabel* map1;
    QLabel* map2;

    /** Flight Mode Control */
    QGroupBox* armingGroupBox;
    QGroupBox* waypointGroupBox;
    QGroupBox* flightModeGroupBox;

 //   FLIGHT_MODE flight_mode;

    /** Inside armingGroupBox*/
    QRadioButton* disarmRadioButton;
    QRadioButton* armRadioButton;
    QLabel* armingStateLabel;

    /** Inside waypointGroupBox*/
    /** Now nothing */

    /** Inside flightModeGroupBox*/
    FLIGHT_MODE flight_mode;
    QGroupBox* returnSwitch;
    QHBoxLayout* returnLayout;
    QRadioButton* returnOn;
    QRadioButton* returnOff;

    QGroupBox* modeSwitch;
    QHBoxLayout* modeLayout;
    QRadioButton* manualRadioButton;
    QRadioButton* assistRadioButton;
    QRadioButton* autoRadioButton;

    QGroupBox* assistSwitch;
    QHBoxLayout* assistLayout;
    QRadioButton* altctl;
    QRadioButton* posctl;

    QGroupBox* autoSwitch;
    QHBoxLayout* autoLayout;
    QRadioButton* mission;
    QRadioButton* loiter;

    QLabel* flightModeLabel;



};

#endif // MAINWINDOW_H
