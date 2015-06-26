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
#include <QtSerialPort/QSerialPort>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    console = new Console;
    console->setEnabled(false);

    widget = new QWidget(this);
    QGridLayout* centralLayout = new QGridLayout(widget);

    consoleGroupBox = new QGroupBox(tr("Console"),this);
    QVBoxLayout* consoleLayout = new QVBoxLayout(consoleGroupBox);
    consoleLayout->addWidget(console);
    consoleGroupBox->setLayout(consoleLayout);

    createInfoGroupBox();
    createMediaGroupBox();
    createMapGroupBox();
    createControlSlidersGroupBox();
    createFlightModeControlGroupBox();

    centralLayout->addWidget(infoGroupBox,0,0);
    centralLayout->addWidget(mediaGroupBox,1,0);
    centralLayout->addWidget(mapGroupBox,2,1,2,1);
    centralLayout->addWidget(controlSlidersGroupBox,2,0,2,1);
    centralLayout->addWidget(flightModeControlGroupBox,0,1,2,2);
    centralLayout->addWidget(consoleGroupBox,2,2,2,1);

    widget->setLayout(centralLayout);
    setCentralWidget(widget);

    serial = new QSerialPort(this);

    settings = new SettingsDialog;

    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionQuit->setEnabled(true);
    ui->actionConfigure->setEnabled(true);

    initActionsConnections();

    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this,
            SLOT(handleError(QSerialPort::SerialPortError)));
    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(console, SIGNAL(getData(QByteArray)), this, SLOT(writeData(QByteArray)));

    resize(800,700);
    setWindowTitle(QApplication::translate("toplevel", "UAV Control 1.0"));

}

/** Manual control sliders */
void MainWindow::onSetThrottle(int t){
    char tsldvalue[1024];
    sprintf(tsldvalue,"%d",t);
    throttleValue->setText(tsldvalue);
    throttle_control = t;
    throttleSlider->setValue(t);
}
void MainWindow::onSetYaw(int y){
    char ysldvalue[1024];
    sprintf(ysldvalue,"%d",y);
    yawValue->setText(ysldvalue);
    yaw_control = y;
    yawSlider->setValue(y);
}
void MainWindow::onSetPitch(int p){
    char psldvalue[1024];
    sprintf(psldvalue,"%d",p);
    pitchValue->setText(psldvalue);
    pitch_control = p;
    pitchSlider->setValue(p);
}
void MainWindow::onSetRoll(int r){
    char rsldvalue[1024];
    sprintf(rsldvalue,"%d",r);
    rollValue->setText(rsldvalue);
    roll_control = r;
    rollSlider->setValue(r);
}

/** Arming state */
void MainWindow::onSetArming(){
    if(disarmRadioButton->isChecked())
         armingStateLabel->setText("State: Disarmed");
    else
         armingStateLabel->setText("State: Armed");
}

void MainWindow::onSetFlightMode(){
    if(returnOn->isChecked()){
        flight_mode = RETURN;
        flightModeLabel->setText("Flight Mode: Return");
        modeSwitch->setEnabled(false);
        assistSwitch->setEnabled(false);
        autoSwitch->setEnabled(false);
    }
    else{
        modeSwitch->setEnabled(true);
        if(manualRadioButton->isChecked()){
            flight_mode = MANUAL;
            flightModeLabel->setText("Flight Mode: Manual");
            assistSwitch->setEnabled(false);
            autoSwitch->setEnabled(false);
        }
        else if(assistRadioButton->isChecked()){
            assistSwitch->setEnabled(true);
            autoSwitch->setEnabled(false);
            if(altctl->isChecked()){
                flight_mode = ALTCTL;
                flightModeLabel->setText("Flight Mode: Altctl");
            }
            else{
                flight_mode = POSCTL;
                flightModeLabel->setText("Flight Mode: Posctl");
            }
        }
        else{
            assistSwitch->setEnabled(false);
            autoSwitch->setEnabled(true);
            if(mission->isChecked()){
                flight_mode = MISSION;
                flightModeLabel->setText("Flight Mode: Mission");
            }
            else{
                flight_mode = LOITER;
                flightModeLabel->setText("Flight Mode: Loiter");
            }
        }
    }

}

 void MainWindow::onTriggerSafety(){
    returnOff->setChecked(true);
    autoRadioButton->setChecked(true);
    loiter->setChecked(true);
    onSetFlightMode();
}

void MainWindow::createInfoGroupBox(){
    infoGroupBox = new QGroupBox(tr("Position"),this);
    QGridLayout* layout = new QGridLayout(infoGroupBox);

    xPosLabel = new QLabel(this);
    xPosLabel->setBaseSize(50,20);
    xPosValue = 0;
    char xvalue[1024];
    sprintf(xvalue,"xPos:%d",xPosValue);
    xPosLabel->setText(xvalue);

    yPosLabel = new QLabel(this);
    yPosLabel->setBaseSize(50,20);
    yPosValue = 0;
    char yvalue[1024];
    sprintf(yvalue,"yPos:%d",yPosValue);
    yPosLabel->setText(yvalue);

    zPosLabel = new QLabel(this);
    zPosLabel->setBaseSize(50,20);
    zPosValue = 0;
    char zvalue[1024];
    sprintf(zvalue,"zPos:%d",zPosValue);
    zPosLabel->setText(zvalue);

    xAcce = new QLabel(this);
    xAcce->setBaseSize(50,20);
    xAcceValue = 1;
    char xaccevalue[1024];
    sprintf(xaccevalue,"xAcce:%d",xAcceValue);
    xAcce->setText(xaccevalue);

    yAcce = new QLabel(this);
    yAcce->setBaseSize(50,20);
    yAcceValue = 1;
    char yaccevalue[1024];
    sprintf(yaccevalue,"yAcce:%d",yAcceValue);
    yAcce->setText(yaccevalue);

    zAcce = new QLabel(this);
    zAcce->setBaseSize(50,20);
    zAcceValue = 10;
    char zaccevalue[1024];
    sprintf(zaccevalue,"zAcce:%d",zAcceValue);
    zAcce->setText(zaccevalue);

    xGyro = new QLabel(this);
    xGyro->setBaseSize(50,20);
    xGyroValue = 0;
    char xgyrovalue[1024];
    sprintf(xgyrovalue,"xGyro:%d",xGyroValue);
    xGyro->setText(xgyrovalue);

    yGyro = new QLabel(this);
    yGyro->setBaseSize(50,20);
    yGyroValue = 0;
    char ygyrovalue[1024];
    sprintf(ygyrovalue,"yGyro:%d",yGyroValue);
    yGyro->setText(ygyrovalue);

    zGyro = new QLabel(this);
    zGyro->setBaseSize(50,20);
    zGyroValue = 0;
    char zgyrovalue[1024];
    sprintf(zgyrovalue,"zGyro:%d",zGyroValue);
    zGyro->setText(zgyrovalue);

    xMag = new QLabel(this);
    xMag->setBaseSize(50,20);
    xMagValue = 0;
    char xmagvalue[1024];
    sprintf(xmagvalue,"xGyro:%d",xMagValue);
    xMag->setText(xmagvalue);

    yMag = new QLabel(this);
    yMag->setBaseSize(50,20);
    yMag->setText("yMag");
    yMagValue = 0;
    char ymagvalue[1024];
    sprintf(ymagvalue,"yGyro:%d",yMagValue);
    yMag->setText(ymagvalue);

    zMag = new QLabel(this);
    zMag->setBaseSize(50,20);
    zMag->setText("zMag");
    zMagValue = 0;
    char zmagvalue[1024];
    sprintf(zmagvalue,"zGyro:%d",zMagValue);
    zMag->setText(zmagvalue);

    layout->addWidget(xPosLabel,0,0);
    layout->addWidget(yPosLabel,1,0);
    layout->addWidget(zPosLabel,2,0);

    layout->addWidget(xAcce,0,2);
    layout->addWidget(yAcce,1,2);
    layout->addWidget(zAcce,2,2);

    layout->addWidget(xGyro,3,0);
    layout->addWidget(yGyro,4,0);
    layout->addWidget(zGyro,5,0);

    layout->addWidget(xMag,3,2);
    layout->addWidget(yMag,4,2);
    layout->addWidget(zMag,5,2);

    infoGroupBox->setLayout(layout);
}

void MainWindow::createMediaGroupBox(){
    mediaGroupBox = new QGroupBox(tr("Media"),this);
    QGridLayout* layout  = new QGridLayout(mediaGroupBox);

    songListLabel = new QLabel(tr("Song List: "),this);
    songListBox = new QComboBox(this);

    songListBox->addItem("Ma Ya Hi");
    songListBox->addItem("Immortals");
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
    throttleSlider->setBaseSize(50,200);
    throttleSlider->setTickPosition(QSlider::TicksBelow);
    throttleSlider->setTickInterval(5);

    yawSlider = new QSlider(Qt::Vertical,this);
    yawSlider->setRange(-180,180);
    yawSlider->setValue(yaw_control);
    yawSlider->setBaseSize(50,200);
    yawSlider->setTickPosition(QSlider::TicksBelow);
    yawSlider->setTickInterval(10);

    pitchSlider = new QSlider(Qt::Vertical,this);
    pitchSlider->setRange(-90,90);
    pitchSlider->setValue(pitch_control);
    pitchSlider->setBaseSize(50,200);
    pitchSlider->setTickPosition(QSlider::TicksBelow);
    pitchSlider->setTickInterval(10);

    rollSlider = new QSlider(Qt::Vertical,this);
    rollSlider->setRange(-90,90);
    rollSlider->setValue(yaw_control);
    rollSlider->setBaseSize(50,200);
    rollSlider->setTickPosition(QSlider::TicksBelow);
    rollSlider->setTickInterval(10);

    throttleLabel = new QLabel(this);
    throttleLabel->setBaseSize(50,10);
    throttleLabel->setText("PWM");

    throttleValue = new QLabel(this);
    throttleValue->setBaseSize(50,10);
    char tslidvalue[1024];
    sprintf(tslidvalue,"%d",throttleSlider->value());
    throttleValue->setText(tslidvalue);

    yawLabel = new QLabel(this);
    yawLabel->setBaseSize(50,10);
    yawLabel->setText("Yaw");

    yawValue = new QLabel(this);
    yawValue->setBaseSize(50,10);
    char yslidvalue[1024];
    sprintf(yslidvalue,"%d",yawSlider->value());
    yawValue->setText(yslidvalue);

    pitchLabel = new QLabel(this);
    pitchLabel->setBaseSize(50,10);
    pitchLabel->setText("Pitch");

    pitchValue = new QLabel(this);
    pitchValue->setBaseSize(50,10);
    char pslidvalue[1024];
    sprintf(pslidvalue,"%d",pitchSlider->value());
    pitchValue->setText(yslidvalue);

    rollLabel = new QLabel(this);
    rollLabel->setBaseSize(50,10);
    rollLabel->setText("Roll");

    rollValue = new QLabel(this);
    rollValue->setBaseSize(50,10);
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

    map1->setBaseSize(20,20);//setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    map2->setBaseSize(20,20);//setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

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

//! [4]
void MainWindow::openSerialPort()
{
    SettingsDialog::Settings p = settings->settings();
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
//! [4]

//! [5]
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
//! [5]

void MainWindow::about()
{  //"The <b>UAV Control v1.0</b> is developed by "
    QMessageBox::about(this, tr("About UAV Control Version 1.0"),
                       tr("\tUAV Ground Control 1.0\n"
                          "developed on 2015 summer in Boston and Hong Kong\n"
                          "by Harvard-HKUST Research Team"));
}

//! [6]
void MainWindow::writeData(const QByteArray &data)
{
    serial->write(data);
}
//! [6]

//! [7]
void MainWindow::readData()
{
    QByteArray data = serial->readAll();
    console->putData(data);
}
//! [7]

//! [8]
void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        closeSerialPort();
    }
}
//! [8]

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
