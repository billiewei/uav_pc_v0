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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "console.h"
#include "settingsdialog.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    console = new Console;
    console->setEnabled(false);

    serial = new MavSerialPort(this);
    settings = new SettingsDialog(this);

    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionQuit->setEnabled(true);
    ui->actionConfigure->setEnabled(true);

    initActionsConnections();
    initSerialConnections();

    widget = new QWidget(this);
    QGridLayout* centralLayout = new QGridLayout(widget);

    consoleGroupBox = new QGroupBox(tr("Console"),this);
    QVBoxLayout* consoleLayout = new QVBoxLayout(consoleGroupBox);
    consoleLayout->addWidget(console);
    consoleGroupBox->setLayout(consoleLayout);

    createInfoGroupBox();
    createMapGroupBox();
    createControlSlidersGroupBox();
    createFlightModeControlGroupBox();

    centralLayout->addWidget(infoGroupBox,0,0,2,1);
    centralLayout->addWidget(mapGroupBox,2,1,2,1);
    centralLayout->addWidget(controlSlidersGroupBox,2,0,2,1);
    centralLayout->addWidget(flightModeControlGroupBox,0,1,2,2);
    centralLayout->addWidget(consoleGroupBox,2,2,2,1);

    initUpdateConnections();
    initCommandConnections();

    widget->setLayout(centralLayout);
    setCentralWidget(widget);

    resize(800,700);
    setWindowTitle(QApplication::translate("toplevel", "UAV Control 1.1"));
}

/** Manual control sliders */
void MainWindow::onSetThrottle(int t){
    char tsldvalue[1024];
    sprintf(tsldvalue,"%d",t);
    throttleValue->setText(tsldvalue);
    throttle_control = t;
    throttleSlider->setValue(t);
    //serial sends out throttle target command
}

void MainWindow::onSetYaw(int y){
    char ysldvalue[1024];
    sprintf(ysldvalue,"%d",y);
    yawValue->setText(ysldvalue);
    yaw_control = y;
    yawSlider->setValue(y);
    //serial sends out yaw target command
}

void MainWindow::onSetPitch(int p){
    char psldvalue[1024];
    sprintf(psldvalue,"%d",p);
    pitchValue->setText(psldvalue);
    pitch_control = p;
    pitchSlider->setValue(p);
    //serial sends out pitch target command
}
void MainWindow::onSetRoll(int r){
    char rsldvalue[1024];
    sprintf(rsldvalue,"%d",r);
    rollValue->setText(rsldvalue);
    roll_control = r;
    rollSlider->setValue(r);
    //serial sends out roll target command
}

/** update Info */
void MainWindow::onUpdateTime(){
    char time[1024];
    sprintf(time,"time since boot: %d ms",serial->attitude.time_boot_ms);
    timeLabel->setText(time);
}

void MainWindow::onUpdateLocal(){
    char xvalue[1024];
    sprintf(xvalue,"xPos: %3.2fm",serial->local_position_ned.x);
    xPosLabel->setText(xvalue);
    char yvalue[1024];
    sprintf(yvalue,"yPos: %3.2fm",serial->local_position_ned.y);
    yPosLabel->setText(yvalue);
    char zvalue[1024];
    sprintf(zvalue,"zPos: %3.2fm",serial->local_position_ned.z);
    zPosLabel->setText(zvalue);

    char vxvalue[1024];
    sprintf(vxvalue,"V_x: %4.3fm/s",serial->local_position_ned.vx);
    vxLabel->setText(vxvalue);
    char vyvalue[1024];
    sprintf(vyvalue,"V_y: %4.3fm/s",serial->local_position_ned.vy);
    vyLabel->setText(vyvalue);
    char vzvalue[1024];
    sprintf(vzvalue,"V_z: %4.3fm/s",serial->local_position_ned.vz);
    vzLabel->setText(vzvalue);
}

void MainWindow::onUpdateGlobal(){
    char lat_value[1024];
    sprintf(lat_value,"Latitude: %3.2f",(float)serial->global_position_int.lat / 1e7);
    latitudeLabel->setText(lat_value);
    char lon_value[1024];
    sprintf(lon_value,"Longitude: %3.2f",(float)serial->global_position_int.lon/1e7);
    longitudeLabel->setText(lon_value);
    char alt_value[1024];
    sprintf(alt_value,"Altitude: %3.2f",(float)serial->global_position_int.alt/1000);
    altitudeLabel->setText(alt_value);
}

void MainWindow::onUpdateBattery(){
    char v_value[1024];
    sprintf(v_value,"Voltage: %d mV",serial->sys_status.voltage_battery);
    voltageLabel->setText(v_value);

    char remaining_value[1024];
    sprintf(remaining_value,"Remaining: %d",serial->sys_status.battery_remaining);
    remainingLabel->setText(remaining_value);
}

void MainWindow::onUpdateIMU(){
    char xacc_value[1024];
    sprintf(xacc_value,"xAcce: %3.2f",serial->highres_imu.xacc);
    xAcceLabel->setText(xacc_value);
    char yacc_value[1024];
    sprintf(yacc_value,"yAcce: %3.2f",serial->highres_imu.yacc);
    yAcceLabel->setText(yacc_value);
    char zacc_value[1024];
    sprintf(zacc_value,"zAcce: %3.2f",serial->highres_imu.zacc);
    zAcceLabel->setText(zacc_value);

    char xgyro_value[1024];
    sprintf(xgyro_value,"xGyro: %3.2f",serial->highres_imu.xgyro);
    xGyroLabel->setText(xgyro_value);
    char ygyro_value[1024];
    sprintf(ygyro_value,"yGyro: %3.2f",serial->highres_imu.ygyro);
    yGyroLabel->setText(ygyro_value);
    char zgyro_value[1024];
    sprintf(zgyro_value,"zGyro: %3.2f",serial->highres_imu.zgyro);
    zGyroLabel->setText(zgyro_value);

    char xmag_value[1024];
    sprintf(xmag_value,"xMag: %3.2f",serial->highres_imu.xmag);
    xMagLabel->setText(xmag_value);
    char ymag_value[1024];
    sprintf(ymag_value,"yMag: %3.2f",serial->highres_imu.ymag);
    yMagLabel->setText(ymag_value);
    char zmag_value[1024];
    sprintf(zmag_value,"zMag: %3.2f",serial->highres_imu.zmag);
    zMagLabel->setText(zmag_value);

    char pa_value[1024];
    sprintf(pa_value,"Pressure_Altitude: %3.2f",serial->highres_imu.pressure_alt);
    pressure_altitudeLabel->setText(pa_value);
    char tem_value[1024];
    sprintf(tem_value,"temperature: %3.2f",serial->highres_imu.temperature);
    temperatureLabel->setText(tem_value);
}

void MainWindow::onUpdateAttitude(){
    char roll_angle[1024];
    sprintf(roll_angle,"Roll: %3.2f",serial->attitude.roll);
    rollangleLabel->setText(roll_angle);
    char pitch_angle[1024];
    sprintf(pitch_angle,"Pitch: %3.2f",serial->attitude.pitch);
    pitchangleLabel->setText(pitch_angle);
    char yaw_angle[1024];
    sprintf(yaw_angle,"Yaw: %3.2f",serial->attitude.yaw);
    yawangleLabel->setText(yaw_angle);

    char roll_speed[1024];
    sprintf(roll_speed,"Roll_speed: %3.2f",serial->attitude.rollspeed);
    rollspeedLabel->setText(roll_speed);
    char  pitch_speed[1024];
    sprintf( pitch_speed,"Pitch_speed: %3.2f",serial->attitude.pitchspeed);
    pitchspeedLabel->setText( pitch_speed);
    char yaw_speed[1024];
    sprintf(yaw_speed,"Yaw_speed: %3.2f",serial->attitude.yawspeed);
    yawspeedLabel->setText(yaw_speed);
}

/** Arming state
 * use this slot to send out serial command
 */
void MainWindow::onSetArming(){

    if(disarmRadioButton->isChecked()){
         armingStateLabel->setText("State: Disarmed");
         serial->set_mode_arm();
    }
    else{
         armingStateLabel->setText("State: Armed");
         serial->set_mode_disarm();
    }

}

/** Flight mode
 * use this slot to send out serial command
 */
void MainWindow::onSetFlightMode(){
    if(returnOn->isChecked()){
        flight_mode = RETURN;
        flightModeLabel->setText("Flight Mode: Return");
        modeSwitch->setEnabled(false);
        assistSwitch->setEnabled(false);
        autoSwitch->setEnabled(false);
        serial->set_mode_return();
    }
    else{
        modeSwitch->setEnabled(true);
        if(manualRadioButton->isChecked()){
            flight_mode = MANUAL;
            flightModeLabel->setText("Flight Mode: Manual");
            assistSwitch->setEnabled(false);
            autoSwitch->setEnabled(false);
            serial->set_mode_manual();
        }
        else if(assistRadioButton->isChecked()){
            assistSwitch->setEnabled(true);
            autoSwitch->setEnabled(false);
            if(altctl->isChecked()){
                flight_mode = ALTCTL;
                flightModeLabel->setText("Flight Mode: Altctl");
                serial->set_mode_assist_altctl();
                //Need to do altitude control
            //    serial->cmd_do_set_mode(MAV_MODE_GUIDED_ARMED);
            }
            else{
                flight_mode = POSCTL;
                flightModeLabel->setText("Flight Mode: Posctl");
                serial->set_mode_assist_posctl();
                //Need to do position control
            //    serial->cmd_do_set_mode(MAV_MODE_GUIDED_ARMED);
            }
        }
        else{
            assistSwitch->setEnabled(false);
            autoSwitch->setEnabled(true);
            if(mission->isChecked()){
                flight_mode = MISSION;
                flightModeLabel->setText("Flight Mode: Mission");
                serial->set_mode_auto_mission();

            }
            else{
                flight_mode = LOITER;
                flightModeLabel->setText("Flight Mode: Loiter");
                serial->set_mode_auto_loiter();
            }
        }
    }

}

/** Safety
 * turn to auto loiter mode
 */
 void MainWindow::onTriggerSafety(){
    returnOff->setChecked(true);
    autoRadioButton->setChecked(true);
    loiter->setChecked(true);
    onSetFlightMode();
}

 void MainWindow::createTimeGroupBox(){
     timeGroupBox = new QGroupBox(tr("Real-time Display"),this);
     QHBoxLayout* layout  = new QHBoxLayout(timeGroupBox);

     timeLabel = new QLabel(this);//time since boot(ms)
     timeLabel->setBaseSize(80,30);
     timeLabel->setText("Time since boot(ms): 0");

     layout->addWidget(timeLabel);
     timeGroupBox->setLayout(layout);
 }

 void MainWindow::createLocalGroupBox(){
     localGroupBox = new QGroupBox(tr("Local Position"),this);
     QGridLayout* layout  = new QGridLayout(localGroupBox);

     xPosLabel = new QLabel(this);
     xPosLabel->setBaseSize(50,30);
     xPosLabel->setText("xPos: Unknown");

     yPosLabel = new QLabel(this);
     yPosLabel->setBaseSize(50,30);
     yPosLabel->setText("yPos: Unknown");

     zPosLabel = new QLabel(this);
     zPosLabel->setBaseSize(50,30);
     zPosLabel->setText("zPos: Unknown");

     vxLabel = new QLabel(this);
     vxLabel->setBaseSize(50,30);
     vxLabel->setText("V_x: Unknown");

     vyLabel = new QLabel(this);
     vyLabel->setBaseSize(50,30);
     vyLabel->setText("V_y: Unknown");

     vzLabel = new QLabel(this);
     vzLabel->setBaseSize(50,30);
     vzLabel->setText("V_z: Unknown");

     layout->addWidget(xPosLabel,0,0);
     layout->addWidget(yPosLabel,1,0);
     layout->addWidget(zPosLabel,2,0);
     layout->addWidget(vxLabel,0,1);
     layout->addWidget(vyLabel,1,1);
     layout->addWidget(vzLabel,2,1);

     localGroupBox->setLayout(layout);
 }

 void MainWindow::createGlobalGroupBox(){
     globalGroupBox = new QGroupBox(tr("Global Position"),this);
     QGridLayout* layout  = new QGridLayout(globalGroupBox);

     latitudeLabel = new QLabel(this);
     latitudeLabel->setBaseSize(50,40);
     latitudeLabel->setText("Latitude: Unknown");

     longitudeLabel = new QLabel(this);
     longitudeLabel->setBaseSize(50,40);
     longitudeLabel->setText("Longitude: Unknown");

     altitudeLabel = new QLabel(this);
     altitudeLabel->setBaseSize(50,40);
     altitudeLabel->setText("Altitude: Unknown");

     layout->addWidget(latitudeLabel,0,0);
     layout->addWidget(longitudeLabel,1,0);
     layout->addWidget(altitudeLabel,2,0);

     globalGroupBox->setLayout(layout);
 }


 void MainWindow::createMediaGroupBox(){
     mediaGroupBox = new QGroupBox(tr("Media"),this);
     QGridLayout* layout  = new QGridLayout(mediaGroupBox);

     songListLabel = new QLabel(tr("Song List: "),this);
     songListBox = new QComboBox(this);

     songListBox->addItem("Immortals");
     songListBox->addItem("Ma Ya Hi");
     songListBox->addItem("Chariots of Fire");
     songListBox->addItem("All I Do is Win");

     toggleButton = new QPushButton(tr("&Play this song"));
     toggleButton->setCheckable(true);
     toggleButton->setChecked(false);

     layout->addWidget(songListLabel,0,0);
     layout->addWidget(songListBox,0,1);
     layout->addWidget(toggleButton,1,0,1,2);

     mediaGroupBox->setLayout(layout);
 }

 void MainWindow::createBatteryGroupBox(){
     batteryGroupBox = new QGroupBox(tr("Battery Info"),this);
     QVBoxLayout* layout  = new QVBoxLayout(batteryGroupBox);

     voltageLabel = new QLabel(this);
     voltageLabel->setBaseSize(50,40);
     voltageLabel->setText("Voltage: Unknown");

     remainingLabel= new QLabel(this);
     remainingLabel->setBaseSize(50,40);
     remainingLabel->setText("Remaining: Unknown");

     layout->addWidget(voltageLabel);
     layout->addWidget(remainingLabel);

     batteryGroupBox->setLayout(layout);
 }

 void MainWindow::createIMUGroupBox(){
     IMUGroupBox = new QGroupBox(tr("High Resolution IMU"),this);
     QGridLayout* layout  = new QGridLayout(IMUGroupBox);

     xAcceLabel = new QLabel(this);
     xAcceLabel->setBaseSize(50,40);
     xAcceLabel->setText("xAcce: Unknown");

     yAcceLabel = new QLabel(this);
     yAcceLabel->setBaseSize(50,40);
     yAcceLabel->setText("yAcce: Unknown");

     zAcceLabel = new QLabel(this);
     zAcceLabel->setBaseSize(50,40);
     zAcceLabel->setText("zAcce: Unknown");

     xGyroLabel = new QLabel(this);
     xGyroLabel->setBaseSize(50,40);
     xGyroLabel->setText("xGyro: Unknown");

     yGyroLabel = new QLabel(this);
     yGyroLabel->setBaseSize(50,40);
     yGyroLabel->setText("yGyro: Unknown");

     zGyroLabel = new QLabel(this);
     zGyroLabel->setBaseSize(50,40);
     zGyroLabel->setText("zGyro: Unknown");

     xMagLabel = new QLabel(this);
     xMagLabel->setBaseSize(50,40);
     xMagLabel->setText("xMag: Uknown");

     yMagLabel = new QLabel(this);
     yMagLabel->setBaseSize(50,40);
     yMagLabel->setText("yMag: Unknown");

     zMagLabel = new QLabel(this);
     zMagLabel->setBaseSize(50,40);
     zMagLabel->setText("zMag: Unknown");

     pressure_altitudeLabel = new QLabel(this);
     pressure_altitudeLabel->setBaseSize(50,40);
     pressure_altitudeLabel->setText("Pressure_Altitude: Uknown");

     temperatureLabel = new QLabel(this);
     temperatureLabel->setBaseSize(50,40);
     temperatureLabel->setText("temperature: Unknown");

     layout->addWidget(xAcceLabel,0,0);
     layout->addWidget(yAcceLabel,1,0);
     layout->addWidget(zAcceLabel,2,0);
     layout->addWidget(xGyroLabel,3,0);
     layout->addWidget(yGyroLabel,4,0);
     layout->addWidget(zGyroLabel,5,0);
     layout->addWidget(xMagLabel,0,1);
     layout->addWidget(yMagLabel,1,1);
     layout->addWidget(zMagLabel,2,1);
     layout->addWidget(pressure_altitudeLabel,3,1);
     layout->addWidget(temperatureLabel,4,1);
     IMUGroupBox->setLayout(layout);
 }

 void MainWindow::createAttitudeGroupBox(){
     attitudeGroupBox = new QGroupBox(tr("Attitude"),this);
     QGridLayout* layout  = new QGridLayout(attitudeGroupBox);

     rollangleLabel = new QLabel(this);
     rollangleLabel->setBaseSize(50,40);
     rollangleLabel->setText("Roll: Unknown");

     pitchangleLabel = new QLabel(this);
     pitchangleLabel->setBaseSize(50,40);
     pitchangleLabel->setText("Pitch: Unknown");

     yawangleLabel = new QLabel(this);
     yawangleLabel->setBaseSize(50,40);
     yawangleLabel->setText("Yaw: Unknown");

     rollspeedLabel = new QLabel(this);
     rollspeedLabel->setBaseSize(50,40);
     rollspeedLabel->setText("Roll_speed: Unknown");

     pitchspeedLabel = new QLabel(this);
     pitchspeedLabel->setBaseSize(50,40);
     pitchspeedLabel->setText("Pitch_speed: Unknown");

     yawspeedLabel = new QLabel(this);
     yawspeedLabel->setBaseSize(50,40);
     yawspeedLabel->setText("Yaw_speed: Unknown");

     layout->addWidget(rollangleLabel,0,0);
     layout->addWidget(pitchangleLabel,1,0);
     layout->addWidget(yawangleLabel,2,0);
     layout->addWidget(rollspeedLabel,3,0);
     layout->addWidget(pitchspeedLabel,4,0);
     layout->addWidget(yawspeedLabel,5,0);

     attitudeGroupBox->setLayout(layout);
 }

void MainWindow::createInfoGroupBox(){
    infoGroupBox = new QGroupBox(tr("Position"),this);
    QGridLayout* layout = new QGridLayout(infoGroupBox);

    createTimeGroupBox();
    createLocalGroupBox();
  //createGlobalGroupBox();
    createMediaGroupBox();
    createBatteryGroupBox();
    createIMUGroupBox();
    createAttitudeGroupBox();

    layout->addWidget(timeGroupBox,0,0,1,2);
    layout->addWidget(localGroupBox,1,0,3,2);
  //layout->addWidget(globalGroupBox,1,1,3,1);
    layout->addWidget(mediaGroupBox,0,2,2,1);
    layout->addWidget(batteryGroupBox,2,2,2,1);
    layout->addWidget(IMUGroupBox,4,0,4,2);
    layout->addWidget(attitudeGroupBox,4,2,4,1);

    infoGroupBox->setLayout(layout);
}

void MainWindow::createControlSlidersGroupBox(){

    controlSlidersGroupBox = new QGroupBox(tr("Control"),this);
    QGridLayout* layout = new QGridLayout(controlSlidersGroupBox);

    //set default value
    throttle_control = 0;
    yaw_control = 0;
    pitch_control = 0;
    roll_control = 0;

    throttleSlider = new QSlider(Qt::Vertical,this);
    throttleSlider->setRange(0,100);
    throttleSlider->setValue(throttle_control);
    throttleSlider->setBaseSize(60,200);
    throttleSlider->setTickPosition(QSlider::TicksBelow);
    throttleSlider->setTickInterval(5);

    yawSlider = new QSlider(Qt::Vertical,this);
    yawSlider->setRange(-180,180);
    yawSlider->setValue(yaw_control);
    yawSlider->setBaseSize(60,200);
    yawSlider->setTickPosition(QSlider::TicksBelow);
    yawSlider->setTickInterval(10);

    pitchSlider = new QSlider(Qt::Vertical,this);
    pitchSlider->setRange(-90,90);
    pitchSlider->setValue(pitch_control);
    pitchSlider->setBaseSize(60,200);
    pitchSlider->setTickPosition(QSlider::TicksBelow);
    pitchSlider->setTickInterval(10);

    rollSlider = new QSlider(Qt::Vertical,this);
    rollSlider->setRange(-90,90);
    rollSlider->setValue(yaw_control);
    rollSlider->setBaseSize(60,200);
    rollSlider->setTickPosition(QSlider::TicksBelow);
    rollSlider->setTickInterval(10);

    throttleLabel = new QLabel(this);
    throttleLabel->setBaseSize(60,10);
    throttleLabel->setText("Throttle");

    throttleValue = new QLabel(this);
    throttleValue->setBaseSize(60,10);
    char tslidvalue[1024];
    sprintf(tslidvalue,"%d",throttleSlider->value());
    throttleValue->setText(tslidvalue);

    yawLabel = new QLabel(this);
    yawLabel->setBaseSize(60,10);
    yawLabel->setText("Yaw");

    yawValue = new QLabel(this);
    yawValue->setBaseSize(60,10);
    char yslidvalue[1024];
    sprintf(yslidvalue,"%d",yawSlider->value());
    yawValue->setText(yslidvalue);

    pitchLabel = new QLabel(this);
    pitchLabel->setBaseSize(60,10);
    pitchLabel->setText("Pitch");

    pitchValue = new QLabel(this);
    pitchValue->setBaseSize(60,10);
    char pslidvalue[1024];
    sprintf(pslidvalue,"%d",pitchSlider->value());
    pitchValue->setText(yslidvalue);

    rollLabel = new QLabel(this);
    rollLabel->setBaseSize(60,10);
    rollLabel->setText("Roll");

    rollValue = new QLabel(this);
    rollValue->setBaseSize(60,10);
    char rslidvalue[1024];
    sprintf(rslidvalue,"%d",rollSlider->value());
    rollValue->setText(rslidvalue);

    connect(throttleSlider, SIGNAL(valueChanged(int)),this,SLOT(onSetThrottle(int)));
    connect(yawSlider, SIGNAL(valueChanged(int)),this,SLOT(onSetYaw(int)));
    connect(pitchSlider, SIGNAL(valueChanged(int)),this,SLOT(onSetPitch(int)));
    connect(rollSlider, SIGNAL(valueChanged(int)),this,SLOT(onSetRoll(int)));

    layout->addWidget(throttleValue,0,0);
    layout->addWidget(yawValue,0,1);
    layout->addWidget(pitchValue,0,2);
    layout->addWidget(rollValue,0,3);

    layout->addWidget(throttleSlider,1,0);
    layout->addWidget(yawSlider,1,1);
    layout->addWidget(pitchSlider,1,2);
    layout->addWidget(rollSlider,1,3);

    layout->addWidget(throttleLabel,2,0);
    layout->addWidget(yawLabel,2,1);
    layout->addWidget(pitchLabel,2,2);
    layout->addWidget(rollLabel,2,3);

    controlSlidersGroupBox->setLayout(layout);
    controlSlidersGroupBox->setAlignment(Qt::AlignHCenter);
}


void MainWindow::createMapGroupBox(){

    /** make two maps: Harvard and HKUST */

    mapGroupBox = new QGroupBox(tr("Map"),this);
    QVBoxLayout* layout = new QVBoxLayout(mapGroupBox);

    tabWidget = new QTabWidget(this);
    map1 = new QLabel(this);
    map2 = new QLabel(this);
    map1->setScaledContents(true);
    map2->setScaledContents(true);

    QImage image1("Harvard.jpg");
    QImage image2("HKUST.jpg");

    map1->setPixmap(QPixmap::fromImage(image1));
    map2->setPixmap(QPixmap::fromImage(image2));
    map1->setText("Harvard University\n\nMap Loading");
    map2->setText("Hong Kong University of\nScience and Technology\n\nMap Loading");

    map1->setBaseSize(20,20);
    map2->setBaseSize(20,20);

    tabWidget->addTab(map1, "Harvard");
    tabWidget->addTab(map2, "HKUST");

    layout->addWidget(tabWidget);
  //  layout->addWidget(map1);
  //  layout->addWidget(map2);

    mapGroupBox->setLayout(layout);
    mapGroupBox->setAlignment(Qt::AlignHCenter);
}

void MainWindow::createFlightModeControlGroupBox(){
    flightModeControlGroupBox = new QGroupBox(tr("Flight Mode Control"),this);
    QGridLayout* layout = new QGridLayout(flightModeControlGroupBox);

    flight_mode = RETURN;

    createArmingGroupBox();
    createWaypointGroupBox();
    createFlightModeGroupBox();

    layout->addWidget(armingGroupBox,0,0);
    layout->addWidget(waypointGroupBox,1,0,2,1);
    layout->addWidget(flightModeGroupBox,0,1,3,1);

    flightModeControlGroupBox->setLayout(layout);
    flightModeControlGroupBox->setAlignment(Qt::AlignHCenter);
}


/** Inside the flight mode control*/

void MainWindow::createArmingGroupBox(){

    armingGroupBox = new QGroupBox(tr("Arming State"),this);
    QGridLayout* layout = new QGridLayout(armingGroupBox);

    disarmRadioButton = new QRadioButton("Disarm",this);
    armRadioButton = new QRadioButton("Arm",this);
    disarmRadioButton->setChecked(true);

    armingStateLabel = new QLabel(this);
    armingStateLabel->setText("State: Disarmed");

    connect(disarmRadioButton,SIGNAL(clicked()),this, SLOT(onSetArming()));
    connect(armRadioButton,SIGNAL(clicked()),this, SLOT(onSetArming()));

    layout->addWidget(disarmRadioButton,0,0);
    layout->addWidget(armRadioButton,0,1);
    layout->addWidget(armingStateLabel,1,0,1,2);

    armingGroupBox->setLayout(layout);
    armingGroupBox->setAlignment(Qt::AlignHCenter);
}


void MainWindow::createWaypointGroupBox(){
    waypointGroupBox = new QGroupBox(tr("Waypoint"),this);
    QGridLayout* layout = new QGridLayout(waypointGroupBox);

    waypointGroupBox->setLayout(layout);
    waypointGroupBox->setAlignment(Qt::AlignHCenter);
}


void MainWindow::createFlightModeGroupBox(){
    flightModeGroupBox = new QGroupBox(tr("Flight Mode"));
    QVBoxLayout* layout = new QVBoxLayout(flightModeGroupBox);

    returnSwitch = new QGroupBox(tr("Return"),this);
    returnLayout = new QHBoxLayout(returnSwitch);
    returnOn = new QRadioButton("Return On",this);
    returnOff = new QRadioButton("Other Modes",this);
    returnOn->setChecked(true);

    returnLayout->addWidget(returnOn);
    returnLayout->addWidget(returnOff);
    returnSwitch->setLayout(returnLayout);

    modeSwitch = new QGroupBox(tr("Mode"),this);
    modeSwitch->setEnabled(false);

    modeLayout = new QHBoxLayout(modeSwitch);
    manualRadioButton = new QRadioButton("Manual",this);
    assistRadioButton = new QRadioButton("Assist",this);
    autoRadioButton = new QRadioButton("Auto",this);
    manualRadioButton->setChecked(true);
    modeLayout->addWidget(manualRadioButton);
    modeLayout->addWidget(assistRadioButton);
    modeLayout->addWidget(autoRadioButton);
    modeSwitch->setLayout(modeLayout);

    assistSwitch = new QGroupBox(tr("Assist"),this);
    assistSwitch->setEnabled(false);

    assistLayout = new QHBoxLayout(assistSwitch);
    altctl = new QRadioButton("Altctl",this);
    posctl = new QRadioButton("Posctl",this);
    altctl->setChecked(true);
    assistLayout->addWidget(altctl);
    assistLayout->addWidget(posctl);
    assistSwitch->setLayout(assistLayout);

    autoSwitch = new QGroupBox(tr("Auto"),this);
    autoSwitch->setEnabled(false);

    autoLayout = new QHBoxLayout(autoSwitch);
    mission = new QRadioButton("Mission",this);
    loiter = new QRadioButton("Loiter",this);
    mission->setChecked(true);
    autoLayout->addWidget(mission);
    autoLayout->addWidget(loiter);
    autoSwitch->setLayout(autoLayout);

    flightModeLabel = new QLabel(this);
    flightModeLabel->setText("Flight Mode: Return" );

    layout->addWidget(returnSwitch);
    layout->addWidget(modeSwitch);
    layout->addWidget(assistSwitch);
    layout->addWidget(autoSwitch);
    layout->addWidget(flightModeLabel);

    connect(returnOn,SIGNAL(clicked()),this,SLOT(onSetFlightMode()));
    connect(returnOff,SIGNAL(clicked()),this,SLOT(onSetFlightMode()));
    connect(manualRadioButton,SIGNAL(clicked()),this,SLOT(onSetFlightMode()));
    connect(assistRadioButton,SIGNAL(clicked()),this,SLOT(onSetFlightMode()));
    connect(autoRadioButton,SIGNAL(clicked()),this,SLOT(onSetFlightMode()));
    connect(altctl,SIGNAL(clicked()),this,SLOT(onSetFlightMode()));
    connect(posctl,SIGNAL(clicked()),this,SLOT(onSetFlightMode()));
    connect(mission,SIGNAL(clicked()),this,SLOT(onSetFlightMode()));
    connect(loiter,SIGNAL(clicked()),this,SLOT(onSetFlightMode()));

    flightModeGroupBox->setLayout(layout);
    flightModeGroupBox->setAlignment(Qt::AlignHCenter);
}


MainWindow::~MainWindow()
{
    delete settings;
    delete ui;
}


void MainWindow::openSerialPort()
{
    SettingsDialog::Settings p = settings->settings();
    //mac uses absolute address
    //windows uses relative address
    serial->setPortName("/dev/cu."+p.name);
    serial->setBaudRate(p.baudRate);
    serial->setDataBits(p.dataBits);
    serial->setParity(p.parity);
    serial->setStopBits(p.stopBits);
    serial->setFlowControl(p.flowControl);
    if (serial->open(QIODevice::ReadWrite)) {
            console->setEnabled(true);
            console->setLocalEchoEnabled(p.localEchoEnabled);
            ui->actionConnect->setEnabled(false);
            ui->actionDisconnect->setEnabled(true);
            ui->actionConfigure->setEnabled(false);
            ui->statusBar->showMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                                       .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                                       .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));
    } else {
        QMessageBox::critical(this, tr("Error"), serial->errorString());

        ui->statusBar->showMessage(tr("Open error"));
    }
}


void MainWindow::closeSerialPort()
{
    if (serial->isOpen())
        serial->close();
    console->setEnabled(false);
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionConfigure->setEnabled(true);
    ui->statusBar->showMessage(tr("Disconnected"));
}


void MainWindow::about()
{  //"The <b>UAV Control v1.0</b> is developed by "
    QMessageBox::about(this, tr("About UAV Control Version 1.0"),
                       tr("\tUAV Ground Control 1.0\n"
                          "\tdeveloped on 2015 summer\n"
                          "\tby Harvard-HKUST Research Team"));
}


void MainWindow::writeData(const QByteArray &data)
{
  //  serial->write(data);
 //   serial->send_test_urob();
  //  serial->send_manual_setpoint();
    serial->cmd_do_set_mode();//MAV_MODE_STABILIZE_ARMED);
  //  serial->send_manual_control();
}


void MainWindow::readData()
{
    QByteArray data = serial->readAll();
    serial->mavRead(&data);
    console->putData(data);
}


void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        closeSerialPort();
    }
}


void MainWindow::initActionsConnections()
{
    connect(ui->actionConnect, SIGNAL(triggered()), this, SLOT(openSerialPort()));
    connect(ui->actionDisconnect, SIGNAL(triggered()), this, SLOT(closeSerialPort()));
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionConfigure, SIGNAL(triggered()), settings, SLOT(show()));
    connect(ui->actionClear, SIGNAL(triggered()), console, SLOT(clear()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    connect(ui->actionSafety, SIGNAL(triggered()), this, SLOT(onTriggerSafety()));
    connect(ui->actionLogo, SIGNAL(triggered()), this, SLOT(about()));
}

void MainWindow::initSerialConnections(){

    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this,
        SLOT(handleError(QSerialPort::SerialPortError)));
    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(console, SIGNAL(getData(QByteArray)), this, SLOT(writeData(QByteArray)));
}

void MainWindow::initUpdateConnections(){
    connect(serial,SIGNAL(timeChanged()),this,SLOT(onUpdateTime()));
    connect(serial,SIGNAL(localChanged()),this,SLOT(onUpdateLocal()));
 //   connect(serial,SIGNAL(globalChanged()),this,SLOT(onUpdateGlobal()));
    connect(serial,SIGNAL(batteryChanged()),this,SLOT(onUpdateBattery()));
    connect(serial,SIGNAL(IMUChanged()),this,SLOT(onUpdateIMU()));
    connect(serial,SIGNAL(attitudeChanged()),this,SLOT(onUpdateAttitude()));
}

void MainWindow::initCommandConnections(){
 //   connect(this,SIGNAL(armingStateChanged(ARM_STATE)),this,SLOT());
 //   connect(this,SIGNAL(flightModeChanged(FLIGHT_MODE)),serial,SLOT());
  //  connect(toggleButton,SIGNAL(clicked()),serial,SLOT(send_manual_setpoint()));
}
