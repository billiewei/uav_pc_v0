#ifndef MAVSERIALPORT_H
#define MAVSERIALPORT_H

#include <QSerialPort>
#include <QByteArray>
#include <QString>
#include <QDebug>


#include "mavlink/v1.0/common/mavlink.h"
#include "mavlink/v1.0/pixhawk/mavlink.h"


class MavSerialPort: public QSerialPort {
    Q_OBJECT

public:
    MavSerialPort(QObject* parent);

    void mavRead(QByteArray* ba);
    void mavDecode(mavlink_message_t &message);


    //0
    inline void heartbeat_handler();
    //1
    inline void sys_status_handler();
    //24
    inline void gps_raw_int_handler();
    //30
    inline void attitude_handler();
    //32
    inline void local_position_ned_handler();
    //33
    inline void global_position_int_handler();
    //42
    inline void mission_current_handler();
    //74
    inline void vfr_hud_handler();
    //81
    inline void manual_setpoint_handler();
    //83
    inline void attitude_target_handler();
    //85
    inline void position_target_local_ned_handler();
    //87
    inline void position_target_global_int_handler();
    //105
    inline void highres_imu_handler();
    //147
    inline void battery_status_handler();

    static int packageDrops;
    static uint8_t msgReceived;

    mavlink_message_t message;
    mavlink_status_t status;

    //0
    mavlink_heartbeat_t heartbeat;
    //1
    mavlink_sys_status_t sys_status;
    //24
    mavlink_gps_raw_int_t gps_raw_int;
    //30
    mavlink_attitude_t attitude;
    //32
    mavlink_local_position_ned_t local_position_ned;
    //33
    mavlink_global_position_int_t global_position_int;
    //42
    mavlink_mission_current_t mission_current;
    //74
    mavlink_vfr_hud_t vfr_hud;
    //81
    mavlink_manual_setpoint_t manual_setpoint;
    //83
    mavlink_attitude_target_t attitude_target;

    //85
    mavlink_position_target_local_ned_t position_target_local_ned;
    //87
    mavlink_position_target_global_int_t position_target_global_int;
    //105
    mavlink_highres_imu_t highres_imu;
    //147
    mavlink_battery_status_t battery_status;

signals:
    void timeChanged();
    void localChanged();
    void globalChanged();
    void batteryChanged();
    void IMUChanged();
    void attitudeChanged();

public slots:
    /** Set Mode */
    void set_mode_disarm();
    void set_mode_arm();
    void set_mode_return();
    void set_mode_manual();
    void set_mode_assist_altctl();
    void set_mode_assist_posctl();
    void set_mode_auto_mission();
    void set_mode_auto_loiter();

    //11
    void send_set_mode();
    //69
    void send_manual_control();
    //81
    void send_manual_setpoint();
    //83
    void send_attitude_target();
    //84
    void send_position_target_local_ned();
    //177
    void send_test_urob();

    /** CMD*/
    //16 MAV_CMD_NAV_WAYPOINT
    void cmd_nav_waypoint();
    //17 MAV_CMD_NAV_LOITER_UNLIM
    void cmd_nav_loiter_unlim();
    //19 MAV_CMD_NAV_LOITER_TIME
    void cmd_nav_loiter_time();
    //20 MAV_CMD_NAV_RETURN_TO_LAUNCH
    void cmd_nav_return_to_launch();
    //21 MAV_CMD_NAV_LAND
    void cmd_nav_land();
    //22 MAV_CMD_NAV_TAKEOFF
    void cmd_nav_takeoff();
    //24 MAV_CMD_NAV_TAKEOFF_LOCAL
    void cmd_nav_takeoff_local();
    //31 MAV_CMD_NAV_LOITER_TO_ALT
//    void cmd_nav_loiter_to_alt();
    //80 MAV_CMD_NAV_ROI
 //   void cmd_nav_loi();
    //176 MAV_CMD_DO_SET_MODE
    void cmd_do_set_mode();//MAV_MODE mode);
    //178 MAV_CMD_DO_CHANGE_SPEED
    void cmd_do_change_speed();
    //179 MAV_CMD_DO_SET_HOME
    void cmd_do_set_home();
    //185 MAV_CMD_DO_FLIGHTTERMINATION
 //   void cmd_do_flighttermination();

private:
    uint8_t system_id;//ID of the sending system
    uint8_t component_id;//ID of the sending component
    uint8_t target_system;//ID of receiving system. px4 by default is 1
    uint8_t target_component;//ID of receiving component. 0 for all component

    //76
    void send_command_long(uint16_t CMD_ID, uint8_t confirmation, float f1, float f2, float f3, float f4, float f5, float f6, float f7);

};

//< sample code for using mavlink library
//! somewhere is changed for understanding

/**
 * @brief sample code using mavlink

mavlink_status_t status;
uint8_t msgReceived = false;

void Dialog::onReadyRead()
{
   QByteArray ba;
   QString str;
   unsigned char *buf;

   if (port->bytesAvailable())
   {
        ba = port->readAll();
        buf = (unsigned char*)ba.data();

        //kernel part of the code
        for(int i = 0 ; i < ba.size(); i++)
        {
            msgReceived = mavlink_parse_char(MAVLINK_COMM_1, buf[i], &message, &status);

            if(msgReceived)
            {
                deocode_messages(message);//TODO
                msgReceived = false;
            }
        }
    }
}



uint64_t Dialog::get_time_usec()
{
    uint64_t  _time_stamp;
    _time_stamp = GetTickCount();
    return _time_stamp*1000;

}
*/

#endif // MAVSERIALPORT_H

