
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSerialPort/QSerialPortInfo>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createInfoGroupBox();
    createMapGroupBox();
    createControlSlidersGroupBox();
    createSerialPortGroupBox();
    createCommandTerminalGroupBox();

    infoGroupBox->setGeometry(5,10,285,175);
    mapGroupBox->setGeometry(210,190,260,300);
    controlSlidersGroupBox->setGeometry(5,190,200,300);
    serialPortGroupBox->setGeometry(475,190,320,300);
    commandTerminalGroupBox->setGeometry(295,10,500,175);

 //   QVBoxLayout* mainLayout = new QVBoxLayout;
 //   QHBoxLayout* h1 = new QHBoxLayout;
//    QHBoxLayout* h2 = new QHBoxLayout;

 //   h1->addWidget(infoGroupBox);
 //   h1->addWidget(commandTerminalGroupBox);
 //   h2->addWidget(controlSlidersGroupBox);
 //   h2->addWidget(mapGroupBox);
 //   h2->addWidget(serialPortComboBox);

 //   mainLayout->addLayout(h1);
 //   mainLayout->addLayout(h2);
 //   widget->setLayout(mainLayout);
}

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

void MainWindow::createInfoGroupBox(){
    infoGroupBox = new QGroupBox(tr("Position"),this);
    QGridLayout* layout = new QGridLayout();

    xPosLabel = new QLabel(this);
    xPosLabel->setBaseSize(50,10);
    xPosLabel->setText("xPos: ");

    xPosValue = new QLabel(this);
    xPosValue->setBaseSize(50,10);
    char xvalue[1024];
   // sprintf(xvalue,"%d",throttleSlider->value());
    xPosValue->setText("0");


    yPosLabel = new QLabel(this);
    yPosLabel->setBaseSize(50,10);
    yPosLabel->setText("yPos: ");

    yPosValue = new QLabel(this);
    yPosValue->setBaseSize(50,10);
    char yvalue[1024];
   // sprintf(xvalue,"%d",throttleSlider->value());
    yPosValue->setText("0");

    heightLabel = new QLabel(this);
    heightLabel->setBaseSize(50,10);
    heightLabel->setText("height: ");

    heightValue = new QLabel(this);
    heightValue->setBaseSize(50,10);
    char hvalue[1024];
   // sprintf(xvalue,"%d",throttleSlider->value());
    heightValue->setText("0");

    QLabel* xAcce = new QLabel(this);
    xAcce->setBaseSize(50,10);
    xAcce->setText("xAcce: ");
    QLabel* xAcceValue = new QLabel(this);
    xAcceValue->setBaseSize(50,10);
    xAcceValue->setText("0");

    QLabel* yAcce = new QLabel(this);
    yAcce->setBaseSize(50,10);
    yAcce->setText("yAcce: ");
    QLabel* yAcceValue = new QLabel(this);
    yAcceValue->setBaseSize(50,10);
    yAcceValue->setText("0");

    QLabel* zAcce = new QLabel(this);
    zAcce->setBaseSize(50,10);
    zAcce->setText("zAcce: ");
    QLabel* zAcceValue = new QLabel(this);
    zAcceValue->setBaseSize(50,10);
    zAcceValue->setText("9.81918");

    QLabel* xGyro = new QLabel(this);
    xGyro->setBaseSize(50,10);
    xGyro->setText("xGyro: ");
    QLabel* xGyroValue = new QLabel(this);
    xGyroValue->setBaseSize(50,10);
    xGyroValue->setText("0");

    QLabel* yGyro = new QLabel(this);
    yGyro->setBaseSize(50,10);
    yGyro->setText("yGyro: ");
    QLabel* yGyroValue = new QLabel(this);
    yGyroValue->setBaseSize(50,10);
    yGyroValue->setText("0");

    QLabel* zGyro= new QLabel(this);
    zGyro->setBaseSize(50,10);
    zGyro->setText("zGyro: ");

    QLabel* zGyroValue= new QLabel(this);
    zGyroValue->setBaseSize(50,10);
    zGyroValue->setText("0");

    layout->addWidget(xPosLabel,0,0);
    layout->addWidget(xPosValue,0,1);

    layout->addWidget(yPosLabel,1,0);
    layout->addWidget(yPosValue,1,1);

    layout->addWidget(heightLabel,2,0);
    layout->addWidget(heightValue,2,1);

    layout->addWidget(xAcce,0,2);
    layout->addWidget(xAcceValue,0,3);

    layout->addWidget(yAcce,1,2);
    layout->addWidget(yAcceValue,1,3);

    layout->addWidget(zAcce,2,2);
    layout->addWidget(zAcceValue,2,3);

    layout->addWidget(xGyro,0,4);
    layout->addWidget(xGyroValue,0,5);

    layout->addWidget(yGyro,1,4);
    layout->addWidget(yGyroValue,1,5);

    layout->addWidget(zGyro,2,4);
    layout->addWidget(zGyroValue,2,5);

    infoGroupBox->setLayout(layout);
}

void MainWindow::createMapGroupBox(){

    /** make two maps: Harvard and HKUST */

    mapGroupBox = new QGroupBox(tr("Map"),this);
    QVBoxLayout* layout = new QVBoxLayout();

    tabWidget = new QTabWidget();
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

void MainWindow::createControlSlidersGroupBox(){

    controlSlidersGroupBox = new QGroupBox(tr("Control"),this);
    QGridLayout* layout = new QGridLayout();


    fullScreen = true;

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
   // yawSlider->setFocusPolicy(Qt::StrongFocus);

    pitchSlider = new QSlider(Qt::Vertical,this);
    pitchSlider->setRange(-90,90);
    pitchSlider->setValue(pitch_control);
    pitchSlider->setBaseSize(50,200);
    pitchSlider->setTickPosition(QSlider::TicksBelow);
    pitchSlider->setTickInterval(10);
   // pitchSlider->setFocusPolicy(Qt::StrongFocus);

    rollSlider = new QSlider(Qt::Vertical,this);
    rollSlider->setRange(-90,90);
    rollSlider->setValue(yaw_control);
    rollSlider->setBaseSize(50,200);
    rollSlider->setTickPosition(QSlider::TicksBelow);
    rollSlider->setTickInterval(10);
  //  rollSlider->setFocusPolicy(Qt::StrongFocus);

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

    //set default value
    throttle_control = 0;
    yaw_control = 0;
    pitch_control = 0;
    roll_control = 0;


/**
    connect(throttleSlider, SIGNAL(valueChanged(int)),this,SLOT(onSetThrottle(int)));
    connect(yawSlider, SIGNAL(valueChanged(int)),this,SLOT(onSetYaw(int)));
    connect(pitchSlider, SIGNAL(valueChanged(int)),this,SLOT(onSetPitch(int)));
    connect(rollSlider, SIGNAL(valueChanged(int)),this,SLOT(onSetRoll(int)));
*/
    controlSlidersGroupBox->setLayout(layout);
    controlSlidersGroupBox->setAlignment(Qt::AlignHCenter);
}



void MainWindow::createSerialPortGroupBox(){

   //   serialPortLabel     serialPortComboBox
   //   baudRateLabel       baudRateBox
   //   waitResponseLabel   waitResponseSpinBox
   //   parityLabel         parityBox
   //   runButton

    serialPortGroupBox = new QGroupBox(tr("Serial Port"),this);
    QGridLayout* layout = new QGridLayout();

    serialPortLabel = new QLabel(tr("Serial port:"),this);
    serialPortComboBox = new QComboBox(this);



    //make sure user can input their own port name!
    serialPortComboBox->setEditable(true);
    //check all available serial ports
   foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
          serialPortComboBox->addItem(info.portName());

    baudRateLabel = new QLabel(tr("Baudrate"),this);
    baudRateBox = new QComboBox(this);
    baudRateBox->addItem("1200", QSerialPort::Baud1200);
    baudRateBox->addItem("2400", QSerialPort::Baud2400);
    baudRateBox->addItem("4800", BAUD4800);
    baudRateBox->addItem("9600", BAUD9600);
    baudRateBox->addItem("19200", BAUD19200);
    baudRateBox->addItem("38400", BAUD38400);
    baudRateBox->addItem("57600", BAUD57600);
    baudRateBox->addItem("115200", BAUD115200);
    baudRateBox->addItem("128000", BAUD128000);
    baudRateBox->addItem("256000", BAUD256000);
    baudRateBox->addItem("921600", BAUD921600);
    baudRateBox->addItem("1000000", BAUD1000000);

    waitResponseLabel = new QLabel(tr("Wait response, msec:"),this);
    waitResponseSpinBox = new QSpinBox(this);
    waitResponseSpinBox->setRange(0, 10000);
    waitResponseSpinBox->setValue(1000);

    parityLabel = new QLabel(tr("Parity"), this);
    parityBox = new QComboBox(this);
    parityBox->addItem("NONE", QSerialPort::NoParity);
    parityBox->addItem("ODD", QSerialPort::OddParity);
    parityBox->addItem("EVEN", QSerialPort::EvenParity);

    runButton = new QPushButton(tr("Send heartbeat packages"));

    layout->addWidget(serialPortLabel,0,0);
    layout->addWidget(serialPortComboBox,0,1);
    layout->addWidget(baudRateLabel,1,0);
    layout->addWidget(baudRateBox,1,1);
    layout->addWidget(waitResponseLabel,2,0);
    layout->addWidget(waitResponseSpinBox,2,1);
    layout->addWidget(parityLabel,3,0);
    layout->addWidget(parityBox,3,1);
    layout->addWidget(runButton,4,0,1,2);

    serialPortGroupBox->setLayout(layout);
    serialPortGroupBox->setAlignment(Qt::AlignHCenter);
}

void MainWindow::createCommandTerminalGroupBox(){
    commandTerminalGroupBox = new QGroupBox(tr("Command Terminal"),this);
    QGridLayout* layout = new QGridLayout();
    commandTerminalGroupBox->setLayout(layout);
    commandTerminalGroupBox->setAlignment(Qt::AlignHCenter);
}

/**
void MainWindow::keyPressEvent(QKeyEvent *e){
    switch ( e->key() )
      {
      case Qt::Key_F2:
        fullScreen = !fullScreen;
        if ( fullScreen )
        {
          showFullScreen();
        }
        else
        {
          showNormal();
        }
      neheWidget->updateGL();
        break;
      case Qt::Key_Escape:
       close();
        break;
     }
}
*/
MainWindow::~MainWindow()
{
    delete infoGroupBox;
    delete mapGroupBox;
    delete controlSlidersGroupBox;
    delete serialPortGroupBox;
    delete commandTerminalGroupBox;

    delete xPosLabel;
    delete yPosLabel;
    delete heightLabel;

    delete xPosValue;
    delete yPosValue;
    delete heightValue;

    delete map1;
    delete map2;
    delete pixmap1;
    delete pixmap2;

    delete throttleSlider;
    delete yawSlider;
    delete pitchSlider;
    delete rollSlider;

    delete throttleLabel;
    delete yawLabel;
    delete pitchLabel;
    delete rollLabel;

    delete throttleValue;
    delete yawValue;
    delete pitchValue;
    delete rollValue;

    //delete port;

    delete serialPortLabel;
    delete serialPortComboBox;

    delete baudRateLabel;
    delete baudRateBox;

    delete waitResponseLabel;
    delete waitResponseSpinBox;

    delete parityLabel;
    delete parityBox;

    delete runButton;

    delete ui;


}
