#include "mavserialport.h"
#include "mavlink/v1.0/mavlink_msg_test_uorb.h"

int MavSerialPort::packageDrops = 0;
uint8_t MavSerialPort::msgReceived = false;

MavSerialPort::MavSerialPort(QObject* parent):
    QSerialPort(parent),system_id(0),component_id(0),
    target_system(1),target_component(0)
{

}


//11
void MavSerialPort::send_set_mode(){
    QByteArray ba;
  //  uint32_t custom_mode; ///< The new autopilot-specific mode. This field can be ignored by an autopilot.
  //  uint8_t target_system; ///< The system setting the mode
  //  uint8_t base_mode; ///< The new base mode

    write(ba);
}
//69
void MavSerialPort::send_manual_control(){
    mavlink_message_t msg;
    uint8_t buffer[2048];

    int16_t x = 1000; ///< X-axis, normalized to the range [-1000,1000]. A value of INT16_MAX indicates that this axis is invalid. Generally corresponds to forward(1000)-backward(-1000) movement on a joystick and the pitch of a vehicle.
    int16_t y = 200; ///< Y-axis, normalized to the range [-1000,1000]. A value of INT16_MAX indicates that this axis is invalid. Generally corresponds to left(-1000)-right(1000) movement on a joystick and the roll of a vehicle.
    int16_t z = 350; ///< Z-axis, normalized to the range [-1000,1000]. A value of INT16_MAX indicates that this axis is invalid. Generally corresponds to a separate slider movement with maximum being 1000 and minimum being -1000 on a joystick and the thrust of a vehicle.
    int16_t r = -900; ///< R-axis, normalized to the range [-1000,1000]. A value of INT16_MAX indicates that this axis is invalid. Generally corresponds to a twisting of the joystick, with counter-clockwise being 1000 and clockwise being -1000, and the yaw of a vehicle.
    uint16_t buttons = 1; ///< A bitfield corresponding to the joystick buttons' current state, 1 for pressed, 0 for released. The lowest bit corresponds to Button 1.
    uint8_t target = 0; ///< The system to be controlled.

    mavlink_msg_manual_control_pack(0, 0, &msg, target, x, y, z, r, buttons);
    int size = mavlink_msg_to_send_buffer(buffer, &msg);

    QByteArray ba((char*)buffer,size);

    write(ba);
     qDebug() << "send out manual control\n";
}
//76
void MavSerialPort::send_command_long(uint16_t CMD_ID, uint8_t confirmation, float f1, float f2, float f3, float f4, float f5, float f6, float f7){
    mavlink_message_t msg;
    uint8_t buffer[2048];

    mavlink_msg_command_long_pack(system_id, component_id, &msg, target_system, target_component,
                                  CMD_ID, confirmation, f1, f2, f3, f4, f5, f6, f7);


    int size = mavlink_msg_to_send_buffer(buffer, &msg);
    QByteArray ba((char*)buffer,size);
    write(ba);
}

//81
void MavSerialPort::send_manual_setpoint(){

    mavlink_message_t msg;
    uint8_t buffer[2048];
//    char* buffer;

//todo: timestamp
    uint32_t time_boot_ms = 100000; ///< Timestamp in milliseconds since system boot
    float roll = 0; ///< Desired roll rate in radians per second
    float pitch = 0; ///< Desired pitch rate in radians per second
    float yaw = 1; ///< Desired yaw rate in radians per second
    float thrust = 0.2; ///< Collective thrust, normalized to 0 .. 1
    uint8_t mode_switch = 0; ///< Flight mode switch position, 0.. 255
    uint8_t manual_override_switch = 0; ///< Override mode switch position, 0.. 255
  //  mavlink_msg_manual_setpoint_send(0, time_boot_ms, roll, pitch, yaw, thrust, mode_switch, manual_override_switch);

    //system_id = 0
    //component_id = 0 //COMP_ID_ALL
    mavlink_msg_manual_setpoint_pack(0,0, &msg, time_boot_ms, roll, pitch, yaw, thrust, mode_switch, manual_override_switch);


    // Pack a message to send it over a serial byte stream
    // MAVLINK_HELPER uint16_t
    // return MAVLINK_NUM_NON_PAYLOAD_BYTES + (uint16_t)msg->len;
    //(uint8_t *buffer, const mavlink_message_t *msg)

    int size = mavlink_msg_to_send_buffer(buffer, &msg);

    QByteArray ba((char*)buffer,size);

    write(ba);

    qDebug() << "send out manual setpoint\n";
}
//83
void MavSerialPort::send_attitude_target(){
    mavlink_message_t msg;
    uint8_t buffer[2048];


    uint32_t time_boot_ms = 1000000; ///< Timestamp in milliseconds since system boot
    float q[4] = {1.1,0.0,0.0,0.0}; ///< Attitude quaternion (w, x, y, z order, zero-rotation is 1, 0, 0, 0)
    float body_roll_rate = 0; ///< Body roll rate in radians per second
    float body_pitch_rate = 0; ///< Body roll rate in radians per second
    float body_yaw_rate = 0; ///< Body roll rate in radians per second
    float thrust = 0.5; ///< Collective thrust, normalized to 0 .. 1 (-1 .. 1 for vehicles capable of reverse trust)
    uint8_t type_mask = 0;///< Mappings: If any of these bits are set, the corresponding input should be ignored: bit 1: body roll rate, bit 2: body pitch rate, bit 3: body yaw rate. bit 4-bit 7: reserved, bit 8: attitude

    //uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
//    uint32_t time_boot_ms, uint8_t type_mask, const float *q, float body_roll_rate, float body_pitch_rate, float body_yaw_rate, float thrust)

    mavlink_msg_attitude_target_pack(system_id, component_id, &msg, time_boot_ms,type_mask, q, body_roll_rate, body_pitch_rate, body_yaw_rate, thrust);

    int size = mavlink_msg_to_send_buffer(buffer, &msg);
    QByteArray ba((char*)buffer,size);
    write(ba);
}

//84
void MavSerialPort::send_position_target_local_ned(){
    mavlink_message_t msg;
    uint8_t buffer[2048];

    uint32_t time_boot_ms = 10000; ///< Timestamp in milliseconds since system boot
    float x = 1; ///< X Position in NED frame in meters
    float y = 0; ///< Y Position in NED frame in meters
    float z = 0; ///< Z Position in NED frame in meters (note, altitude is negative in NED)
    float vx = 0; ///< X velocity in NED frame in meter / s
    float vy = 1; ///< Y velocity in NED frame in meter / s
    float vz = 0; ///< Z velocity in NED frame in meter / s
    float afx = 0; ///< X acceleration or force (if bit 10 of type_mask is set) in NED frame in meter / s^2 or N
    float afy = 0; ///< Y acceleration or force (if bit 10 of type_mask is set) in NED frame in meter / s^2 or N
    float afz = 1; ///< Z acceleration or force (if bit 10 of type_mask is set) in NED frame in meter / s^2 or N
    float yaw =1; ///< yaw setpoint in rad
    float yaw_rate = 3; ///< yaw rate setpoint in rad/s
    uint16_t type_mask = 0; ///< Bitmask to indicate which dimensions should be ignored by the vehicle: a value of 0b0000000000000000 or 0b0000001000000000 indicates that none of the setpoint dimensions should be ignored. If bit 10 is set the floats afx afy afz should be interpreted as force instead of acceleration. Mapping: bit 1: x, bit 2: y, bit 3: z, bit 4: vx, bit 5: vy, bit 6: vz, bit 7: ax, bit 8: ay, bit 9: az, bit 10: is force setpoint, bit 11: yaw, bit 12: yaw rate
    uint8_t coordinate_frame = 0; ///

    mavlink_msg_set_position_target_local_ned_pack(system_id, component_id, &msg, time_boot_ms, target_system, target_component, coordinate_frame, type_mask, x, y, z, vx, vy, vz, afx, afy, afz, yaw, yaw_rate);

    int size = mavlink_msg_to_send_buffer(buffer, &msg);

    QByteArray ba((char*)buffer,size);

    write(ba);
    qDebug() << "send out set position target local ned\n";
}

// #177
void MavSerialPort::send_test_urob(){

    mavlink_message_t msg;
    uint8_t buffer[1024];

    uint64_t timestamp = 2015;
    uint64_t shuai = 2016;

    mavlink_msg_test_uorb_pack(2,2, &msg, timestamp,shuai);

    int size = mavlink_msg_to_send_buffer(buffer, &msg);
    QByteArray ba((char*)buffer,size);
    write(ba);
    qDebug() << "send out test_uorb\n";
}


/** CMD*/
//16 MAV_CMD_NAV_WAYPOINT
void MavSerialPort::cmd_nav_waypoint(){
    // Navigate to MISSION.
    float f1 = 15; //Hold time in decimal seconds. (ignored by fixed wing, time to stay at MISSION for rotary wing)
    float f2 = 0; //Acceptance radius in meters (if the sphere with this radius is hit, the MISSION counts as reached)
    float f3 = 0; //0 to pass through the WP, if > 0 radius in meters to pass by WP. Positive value for clockwise orbit, negative value for counter-clockwise orbit. Allows trajectory control.
    float f4 = 0; //Desired yaw angle at MISSION (rotary wing)
    float f5 = 22.33; // Latitude22.3371147,114.268395
    float f6 = 114.26; // Longitude
    float f7 = 5; // Altitude
    //confirmation 0
    //go back later
    send_command_long(MAV_CMD_NAV_WAYPOINT, 0, f1, f2, f3, f4, f5, f6, f7);
}

//17 MAV_CMD_NAV_LOITER_UNLIM
void MavSerialPort::cmd_nav_loiter_unlim(){

    //confirmation 0
    //go back later
//    send_command_long(MAV_CMD_NAV_LOITER_UNLIM, 0, f1, f2, f3, f4, f5, f6, f7);
}

//19 MAV_CMD_NAV_LOITER_TIME
void MavSerialPort::cmd_nav_loiter_time(){

}

//20 MAV_CMD_NAV_RETURN_TO_LAUNCH
void MavSerialPort::cmd_nav_return_to_launch(){

}

//21 MAV_CMD_NAV_LAND
void MavSerialPort::cmd_nav_land(){

}

//22 MAV_CMD_NAV_TAKEOFF
void MavSerialPort::cmd_nav_takeoff(){

}

//24 MAV_CMD_NAV_TAKEOFF_LOCAL
void MavSerialPort::cmd_nav_takeoff_local(){

}

//176 MAV_CMD_DO_SET_MODE
void MavSerialPort::cmd_do_set_mode(){//MAV_MODE mode){
    /* Set system mode.*/
    /** MAV_MODE_PREFLIGHT=0,       // System is not ready to fly, booting, calibrating, etc. No flag is set.
    MAV_MODE_MANUAL_DISARMED=64,    // System is allowed to be active, under manual (RC) control, no stabilization
    MAV_MODE_TEST_DISARMED=66,      // UNDEFINED mode. This solely depends on the autopilot - use with caution, intended for developers only.
    MAV_MODE_STABILIZE_DISARMED=80, // System is allowed to be active, under assisted RC control.
    MAV_MODE_GUIDED_DISARMED=88,    // System is allowed to be active, under autonomous control, manual setpoint
    MAV_MODE_AUTO_DISARMED=92,      // System is allowed to be active, under autonomous control and navigation (the trajectory is decided onboard and not pre-programmed by MISSIONs)
    MAV_MODE_MANUAL_ARMED=192,      // System is allowed to be active, under manual (RC) control, no stabilization
    MAV_MODE_TEST_ARMED=194,        // UNDEFINED mode. This solely depends on the autopilot - use with caution, intended for developers only.
    MAV_MODE_STABILIZE_ARMED=208,   // System is allowed to be active, under assisted RC control.
    MAV_MODE_GUIDED_ARMED=216,      // System is allowed to be active, under autonomous control, manual setpoint
    MAV_MODE_AUTO_ARMED=220,        // System is allowed to be active, under autonomous control and navigation (the trajectory is decided onboard and not pre-programmed by MISSIONs)
    MAV_MODE_ENUM_END=221
    */

    // Mode, as defined by ENUM MAV_MODE, will be ignored if custom main mode is set
    // Custom mode - this is system specific, please refer to the individual autopilot specifications for details.
    // Empty| Empty| Empty| Empty| Empty
    // leave the confirmation as 0
    send_command_long(MAV_CMD_DO_SET_MODE,0,194,0,0,0,0,0,0);
    qDebug() << "send do set mode";
}

/** Set Mode */
void MavSerialPort::set_mode_disarm(){
    send_command_long(MAV_CMD_DO_SET_MODE,0,MAV_MODE_PREFLIGHT,0,0,0,0,0,0);
    qDebug() << "MODE_PREFLIGHT";
}

void MavSerialPort::set_mode_arm(){
    send_command_long(MAV_CMD_DO_SET_MODE,0,MAV_MODE_MANUAL_ARMED,0,0,0,0,0,0);
    qDebug() << "MODE_MANUAL_ARMED";
}

void MavSerialPort::set_mode_return(){
    send_command_long(MAV_CMD_NAV_RETURN_TO_LAUNCH,0,0,0,0,0,0,0,0);
    qDebug() << "SEND RETURN TO LAUNCH";

}

void MavSerialPort::set_mode_manual(){
    send_command_long(MAV_CMD_DO_SET_MODE,0,MAV_MODE_MANUAL_ARMED,0,0,0,0,0,0);
    qDebug() << "MODE_MANUAL_ARMED";

}

void MavSerialPort::set_mode_assist_altctl(){
    send_command_long(MAV_CMD_DO_SET_MODE,0,MAV_MODE_STABILIZE_ARMED,0,0,0,0,0,0);
    qDebug() << "MODE_STABILIZE_ARMED";
}

void MavSerialPort::set_mode_assist_posctl(){
    send_command_long(MAV_CMD_DO_SET_MODE,0,MAV_MODE_STABILIZE_ARMED,0,0,0,0,0,0);
    qDebug() << "MODE_STABILIZE_ARMED";
}

void MavSerialPort::set_mode_auto_mission(){
    send_command_long(MAV_CMD_DO_SET_MODE,0,MAV_MODE_AUTO_ARMED,0,0,0,0,0,0);
    qDebug() << "MODE_AUTO_ARMED";
}

void MavSerialPort::set_mode_auto_loiter(){
    send_command_long(MAV_CMD_DO_SET_MODE,0,MAV_MODE_PREFLIGHT,0,0,0,0,0,0);
    qDebug() << "MODE_AUTO_ARMED";
}

//178 MAV_CMD_DO_CHANGE_SPEED
void MavSerialPort::cmd_do_change_speed(){

}

//179 MAV_CMD_DO_SET_HOME
void MavSerialPort::cmd_do_set_home(){

}

//0
void MavSerialPort::heartbeat_handler(){
  //  qDebug() << "MAVLINK_MSG_ID_HEARTBEAT\n";
    mavlink_msg_heartbeat_decode(&message, &heartbeat);

    qDebug() << "base mode: " << heartbeat.base_mode;
    qDebug() << "custom mode: " << heartbeat.custom_mode;
    qDebug() << "system status: " << heartbeat.system_status;

}

//1
void MavSerialPort::sys_status_handler(){
  //  qDebug() << "MAVLINK_MSG_ID_SYS_STATUS\n";
    mavlink_msg_sys_status_decode(&message, &sys_status);

    emit batteryChanged();
}

//24
void MavSerialPort::gps_raw_int_handler(){
    qDebug() << "MAVLINK_MSG_ID_GPS_RAW_INT\n";
    mavlink_msg_gps_raw_int_decode(&message, &gps_raw_int);
}

//30
void MavSerialPort::attitude_handler(){
  //  qDebug() << "MAVLINK_MSG_ID_ATTITUDE\n";
    mavlink_msg_attitude_decode(&message, &attitude);
    emit timeChanged();
    emit attitudeChanged();
}

//32
void MavSerialPort::local_position_ned_handler(){
   // qDebug() << "MAVLINK_MSG_ID_LOCAL_POSITION_NED\n";
    mavlink_msg_local_position_ned_decode(&message, &local_position_ned);
    emit localChanged();

}

//33
void MavSerialPort::global_position_int_handler(){
  //  qDebug() << "MAVLINK_MSG_ID_GLOBAL_POSITION_INT\n";
    mavlink_msg_global_position_int_decode(&message, &global_position_int);
    emit globalChanged();
}

//42
void MavSerialPort::mission_current_handler(){
  //  qDebug() << "MAVLINK_MSG_ID_MISSION_CURRENT\n";
    mavlink_msg_mission_current_decode(&message, &mission_current);

}

//74
void MavSerialPort::vfr_hud_handler(){
  //  qDebug() << "MAVLINK_MSG_ID_VFR_HUD\n";
    mavlink_msg_vfr_hud_decode(&message, &vfr_hud);

}
//81
void MavSerialPort::manual_setpoint_handler(){
     qDebug() << "MAVLINK_MSG_ID_MANUAL_SETPOINT\n";
     mavlink_msg_manual_setpoint_decode(&message,&manual_setpoint);
     qDebug() << "time_boot_ms: " << manual_setpoint.time_boot_ms <<'\n';
     qDebug() << "roll: "<< manual_setpoint.roll << '\n';
     qDebug() << "roll: "<< manual_setpoint.roll << '\n';

}

//83
void MavSerialPort::attitude_target_handler(){
  //  qDebug() << "MAVLINK_MSG_ID_ATTITUDE_TARGET\n";
    mavlink_msg_attitude_target_decode(&message, &attitude_target);

}

//85
void MavSerialPort::position_target_local_ned_handler(){
   // qDebug() << "MAVLINK_MSG_ID_POSITION_TARGET_LOCAL_END\n";
    mavlink_msg_position_target_local_ned_decode(&message, &position_target_local_ned);

}

//87
void MavSerialPort::position_target_global_int_handler(){
   // qDebug() << "MAVLINK_MSG_ID_POSITION_TARGET_GLOBAL_INT\n";
    mavlink_msg_position_target_global_int_decode(&message, &position_target_global_int);

}

//105
void MavSerialPort::highres_imu_handler(){
  //  qDebug() << "MAVLINK_MSG_HIGHRES_IMU";
    mavlink_msg_highres_imu_decode(&message, &highres_imu);
    emit IMUChanged();

  //  qDebug() << "imu: " << highres_imu.xgyro << " " << highres_imu.ygyro << "  " << highres_imu.zgyro << endl;
}


//147
void MavSerialPort::battery_status_handler(){
  //  qDebug() << "MAVLINK_MSG_ID_BATTERY_STATUS\n";
    mavlink_msg_battery_status_decode(&message, &battery_status);
}

void MavSerialPort::mavRead(QByteArray* ba){
    unsigned char *buf;
    buf = (unsigned char*)ba->data();

    //kernel part of the code
    for(int i = 0 ; i < ba->size(); i++){
        //does it matter if i change it to COMM_0 ?
        msgReceived = mavlink_parse_char(MAVLINK_COMM_1, buf[i], &message, &status);
        if(msgReceived){
            mavDecode(message);
            msgReceived = false;
        }
    }
}

void MavSerialPort::mavDecode(mavlink_message_t &message){
    switch (message.msgid){
    //0
    case MAVLINK_MSG_ID_HEARTBEAT:
        heartbeat_handler();
        break;

    //1
    case MAVLINK_MSG_ID_SYS_STATUS:
        sys_status_handler();
        break;

    //24
    case MAVLINK_MSG_ID_GPS_RAW_INT:
        gps_raw_int_handler();
        break;

    //30
    case MAVLINK_MSG_ID_ATTITUDE:
        attitude_handler();
        break;

    //32
    case MAVLINK_MSG_ID_LOCAL_POSITION_NED:
        local_position_ned_handler();
        break;

    //33
    case MAVLINK_MSG_ID_GLOBAL_POSITION_INT:
        global_position_int_handler();
        break;

    //42
    case MAVLINK_MSG_ID_MISSION_CURRENT:
        mission_current_handler();
        break;

    //74
    case MAVLINK_MSG_ID_VFR_HUD:
        vfr_hud_handler();
        break;

    //81
    case MAVLINK_MSG_ID_MANUAL_SETPOINT:
        manual_setpoint_handler();
        break;

    //83
    case MAVLINK_MSG_ID_ATTITUDE_TARGET:
        attitude_target_handler();
        break;

    //85
    case MAVLINK_MSG_ID_POSITION_TARGET_LOCAL_NED:
        position_target_local_ned_handler();
        break;

    //87
    case MAVLINK_MSG_ID_POSITION_TARGET_GLOBAL_INT:
        position_target_global_int_handler();
        break;

    //105
    case MAVLINK_MSG_ID_HIGHRES_IMU:
        highres_imu_handler();
        break;

    //147
    case MAVLINK_MSG_ID_BATTERY_STATUS:
        battery_status_handler();
        break;

    default:
        qDebug() << "new message:" << (int)message.msgid;
        break;

    } // end of switch
}
