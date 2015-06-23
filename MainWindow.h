#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QSlider>
#include <QLabel>
#include <QKeyEvent>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QMenuBar>
#include <QPixmap>
#include <QPicture>
#include <QTabWidget>


#include <QHBoxLayout>  // horizontal layout
#include <QVBoxLayout>  // vertical layout
#include <QGroupBox>
#include <QGridLayout>

#include <QTime>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

QT_BEGIN_NAMESPACE
class QMainWindow;

class QLabel;
class QSlider;
class QKeyEvent;
class QLineEdit;
class QComboBox;
class QSpinBox;
class QPushButton;
class QMenuBar;
class QPixmap;
class QPicture;

class QHBoxLayout;  // horizontal layout
class QVBoxLayout;  // vertical layout
class QGroupBox;
class QGridLayout;

class QTime;

QT_END_NAMESPACE

const int BAUD1200 = 1200;
const int BAUD2400 = 2400;
const int BAUD4800 = 4800;
const int BAUD9600 = 9600;
const int BAUD19200 = 19200;
const int BAUD38400 = 38400;
const int BAUD57600 = 57600;
const int BAUD115200 = 115200;
const int BAUD128000 = 128000;
const int BAUD256000 = 256000;
const int BAUD921600 = 921600;
const int BAUD1000000 = 1000000;
const int BAUD1500000 = 1500000;
const int BAUD2000000 = 2000000;
const int BAUD2500000 = 2500000;
const int BAUD3000000 = 3000000;
const int BAUD3500000 = 3500000;
const int BAUD4000000 = 4000000;

namespace Ui {
class MainWindow;
}

class MainWindow: public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    int throttle_control;
    int yaw_control;
    int pitch_control;
    int roll_control;

public slots:
    void onSetThrottle(int t);
    void onSetYaw(int y);
    void onSetPitch(int p);
    void onSetRoll(int r);

protected:
    bool fullScreen;
 /**   void keyPressEvent(QKeyEvent *e); */

private:
    Ui::MainWindow *ui;

 //   QMenuBar* menuBar;  
    QGroupBox* infoGroupBox;
    QGroupBox* controlSlidersGroupBox;
    QGroupBox* mapGroupBox;
    QGroupBox* serialPortGroupBox;
    QGroupBox* commandTerminalGroupBox;

 //   QAction* exitAction;
    QLabel* xPosLabel;
    QLabel* yPosLabel;
    QLabel* heightLabel;

    QLabel* xPosValue;
    QLabel* yPosValue;
    QLabel* heightValue;

    QTabWidget* tabWidget;
    QLabel* map1;
    QLabel* map2;
    QPixmap* pixmap1;
    QPixmap* pixmap2;

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

    QString portName;
//    QSerialPort* port;

    QLabel* serialPortLabel;
    QComboBox* serialPortComboBox;

    QLabel* baudRateLabel;
    QComboBox* baudRateBox;

    QLabel* waitResponseLabel;
    QSpinBox* waitResponseSpinBox;

    QLabel* parityLabel;
    QComboBox* parityBox;

    QPushButton *runButton;

    void createMenu();
    void createInfoGroupBox();
    void createMapGroupBox();
    void createControlSlidersGroupBox();
    void createSerialPortGroupBox();
    void createCommandTerminalGroupBox();

};

#endif //MAINWINDOW_H
