greaterThan(QT_MAJOR_VERSION, 4) {
    QT       += widgets serialport
} else {
    include($$QTSERIALPORT_PROJECT_ROOT/src/serialport/qt4support/serialport.prf)
}

TARGET = terminal
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    settingsdialog.cpp \
    console.cpp \
    mavserialport.cpp

HEADERS += \
    mainwindow.h \
    settingsdialog.h \
    console.h \
    mavlink/v1.0/common/common.h \
    mavlink/v1.0/common/mavlink.h \
    mavlink/v1.0/common/mavlink_msg_actuator_control_target.h \
    mavlink/v1.0/common/mavlink_msg_att_pos_mocap.h \
    mavlink/v1.0/common/mavlink_msg_attitude.h \
    mavlink/v1.0/common/mavlink_msg_attitude_quaternion.h \
    mavlink/v1.0/common/mavlink_msg_attitude_quaternion_cov.h \
    mavlink/v1.0/common/mavlink_msg_attitude_target.h \
    mavlink/v1.0/common/mavlink_msg_auth_key.h \
    mavlink/v1.0/common/mavlink_msg_autopilot_version.h \
    mavlink/v1.0/common/mavlink_msg_battery_status.h \
    mavlink/v1.0/common/mavlink_msg_camera_trigger.h \
    mavlink/v1.0/common/mavlink_msg_change_operator_control.h \
    mavlink/v1.0/common/mavlink_msg_change_operator_control_ack.h \
    mavlink/v1.0/common/mavlink_msg_command_ack.h \
    mavlink/v1.0/common/mavlink_msg_command_int.h \
    mavlink/v1.0/common/mavlink_msg_command_long.h \
    mavlink/v1.0/common/mavlink_msg_data_stream.h \
    mavlink/v1.0/common/mavlink_msg_data_transmission_handshake.h \
    mavlink/v1.0/common/mavlink_msg_debug.h \
    mavlink/v1.0/common/mavlink_msg_debug_vect.h \
    mavlink/v1.0/common/mavlink_msg_distance_sensor.h \
    mavlink/v1.0/common/mavlink_msg_encapsulated_data.h \
    mavlink/v1.0/common/mavlink_msg_file_transfer_protocol.h \
    mavlink/v1.0/common/mavlink_msg_global_position_int.h \
    mavlink/v1.0/common/mavlink_msg_global_position_int_cov.h \
    mavlink/v1.0/common/mavlink_msg_global_vision_position_estimate.h \
    mavlink/v1.0/common/mavlink_msg_gps2_raw.h \
    mavlink/v1.0/common/mavlink_msg_gps2_rtk.h \
    mavlink/v1.0/common/mavlink_msg_gps_global_origin.h \
    mavlink/v1.0/common/mavlink_msg_gps_inject_data.h \
    mavlink/v1.0/common/mavlink_msg_gps_raw_int.h \
    mavlink/v1.0/common/mavlink_msg_gps_rtk.h \
    mavlink/v1.0/common/mavlink_msg_gps_status.h \
    mavlink/v1.0/common/mavlink_msg_heartbeat.h \
    mavlink/v1.0/common/mavlink_msg_highres_imu.h \
    mavlink/v1.0/common/mavlink_msg_hil_controls.h \
    mavlink/v1.0/common/mavlink_msg_hil_gps.h \
    mavlink/v1.0/common/mavlink_msg_hil_optical_flow.h \
    mavlink/v1.0/common/mavlink_msg_hil_rc_inputs_raw.h \
    mavlink/v1.0/common/mavlink_msg_hil_sensor.h \
    mavlink/v1.0/common/mavlink_msg_hil_state.h \
    mavlink/v1.0/common/mavlink_msg_hil_state_quaternion.h \
    mavlink/v1.0/common/mavlink_msg_local_position_ned.h \
    mavlink/v1.0/common/mavlink_msg_local_position_ned_cov.h \
    mavlink/v1.0/common/mavlink_msg_local_position_ned_system_global_offset.h \
    mavlink/v1.0/common/mavlink_msg_log_data.h \
    mavlink/v1.0/common/mavlink_msg_log_entry.h \
    mavlink/v1.0/common/mavlink_msg_log_erase.h \
    mavlink/v1.0/common/mavlink_msg_log_request_data.h \
    mavlink/v1.0/common/mavlink_msg_log_request_end.h \
    mavlink/v1.0/common/mavlink_msg_log_request_list.h \
    mavlink/v1.0/common/mavlink_msg_manual_control.h \
    mavlink/v1.0/common/mavlink_msg_manual_setpoint.h \
    mavlink/v1.0/common/mavlink_msg_memory_vect.h \
    mavlink/v1.0/common/mavlink_msg_mission_ack.h \
    mavlink/v1.0/common/mavlink_msg_mission_clear_all.h \
    mavlink/v1.0/common/mavlink_msg_mission_count.h \
    mavlink/v1.0/common/mavlink_msg_mission_current.h \
    mavlink/v1.0/common/mavlink_msg_mission_item.h \
    mavlink/v1.0/common/mavlink_msg_mission_item_int.h \
    mavlink/v1.0/common/mavlink_msg_mission_item_reached.h \
    mavlink/v1.0/common/mavlink_msg_mission_request.h \
    mavlink/v1.0/common/mavlink_msg_mission_request_list.h \
    mavlink/v1.0/common/mavlink_msg_mission_request_partial_list.h \
    mavlink/v1.0/common/mavlink_msg_mission_set_current.h \
    mavlink/v1.0/common/mavlink_msg_mission_write_partial_list.h \
    mavlink/v1.0/common/mavlink_msg_named_value_float.h \
    mavlink/v1.0/common/mavlink_msg_named_value_int.h \
    mavlink/v1.0/common/mavlink_msg_nav_controller_output.h \
    mavlink/v1.0/common/mavlink_msg_optical_flow.h \
    mavlink/v1.0/common/mavlink_msg_optical_flow_rad.h \
    mavlink/v1.0/common/mavlink_msg_param_map_rc.h \
    mavlink/v1.0/common/mavlink_msg_param_request_list.h \
    mavlink/v1.0/common/mavlink_msg_param_request_read.h \
    mavlink/v1.0/common/mavlink_msg_param_set.h \
    mavlink/v1.0/common/mavlink_msg_param_value.h \
    mavlink/v1.0/common/mavlink_msg_ping.h \
    mavlink/v1.0/common/mavlink_msg_position_target_global_int.h \
    mavlink/v1.0/common/mavlink_msg_position_target_local_ned.h \
    mavlink/v1.0/common/mavlink_msg_power_status.h \
    mavlink/v1.0/common/mavlink_msg_radio_status.h \
    mavlink/v1.0/common/mavlink_msg_raw_imu.h \
    mavlink/v1.0/common/mavlink_msg_raw_pressure.h \
    mavlink/v1.0/common/mavlink_msg_rc_channels.h \
    mavlink/v1.0/common/mavlink_msg_rc_channels_override.h \
    mavlink/v1.0/common/mavlink_msg_rc_channels_raw.h \
    mavlink/v1.0/common/mavlink_msg_rc_channels_scaled.h \
    mavlink/v1.0/common/mavlink_msg_request_data_stream.h \
    mavlink/v1.0/common/mavlink_msg_safety_allowed_area.h \
    mavlink/v1.0/common/mavlink_msg_safety_set_allowed_area.h \
    mavlink/v1.0/common/mavlink_msg_scaled_imu.h \
    mavlink/v1.0/common/mavlink_msg_scaled_imu2.h \
    mavlink/v1.0/common/mavlink_msg_scaled_imu3.h \
    mavlink/v1.0/common/mavlink_msg_scaled_pressure.h \
    mavlink/v1.0/common/mavlink_msg_scaled_pressure2.h \
    mavlink/v1.0/common/mavlink_msg_serial_control.h \
    mavlink/v1.0/common/mavlink_msg_servo_output_raw.h \
    mavlink/v1.0/common/mavlink_msg_set_actuator_control_target.h \
    mavlink/v1.0/common/mavlink_msg_set_attitude_target.h \
    mavlink/v1.0/common/mavlink_msg_set_gps_global_origin.h \
    mavlink/v1.0/common/mavlink_msg_set_mode.h \
    mavlink/v1.0/common/mavlink_msg_set_position_target_global_int.h \
    mavlink/v1.0/common/mavlink_msg_set_position_target_local_ned.h \
    mavlink/v1.0/common/mavlink_msg_sim_state.h \
    mavlink/v1.0/common/mavlink_msg_statustext.h \
    mavlink/v1.0/common/mavlink_msg_sys_status.h \
    mavlink/v1.0/common/mavlink_msg_system_time.h \
    mavlink/v1.0/common/mavlink_msg_terrain_check.h \
    mavlink/v1.0/common/mavlink_msg_terrain_data.h \
    mavlink/v1.0/common/mavlink_msg_terrain_report.h \
    mavlink/v1.0/common/mavlink_msg_terrain_request.h \
    mavlink/v1.0/common/mavlink_msg_timesync.h \
    mavlink/v1.0/common/mavlink_msg_v2_extension.h \
    mavlink/v1.0/common/mavlink_msg_vfr_hud.h \
    mavlink/v1.0/common/mavlink_msg_vicon_position_estimate.h \
    mavlink/v1.0/common/mavlink_msg_vision_position_estimate.h \
    mavlink/v1.0/common/mavlink_msg_vision_speed_estimate.h \
    mavlink/v1.0/common/testsuite.h \
    mavlink/v1.0/common/version.h \
    mavlink/v1.0/pixhawk/mavlink.h \
    mavlink/v1.0/pixhawk/mavlink_msg_attitude_control.h \
    mavlink/v1.0/pixhawk/mavlink_msg_brief_feature.h \
    mavlink/v1.0/pixhawk/mavlink_msg_detection_stats.h \
    mavlink/v1.0/pixhawk/mavlink_msg_image_available.h \
    mavlink/v1.0/pixhawk/mavlink_msg_image_trigger_control.h \
    mavlink/v1.0/pixhawk/mavlink_msg_image_triggered.h \
    mavlink/v1.0/pixhawk/mavlink_msg_marker.h \
    mavlink/v1.0/pixhawk/mavlink_msg_onboard_health.h \
    mavlink/v1.0/pixhawk/mavlink_msg_pattern_detected.h \
    mavlink/v1.0/pixhawk/mavlink_msg_point_of_interest.h \
    mavlink/v1.0/pixhawk/mavlink_msg_point_of_interest_connection.h \
    mavlink/v1.0/pixhawk/mavlink_msg_position_control_setpoint.h \
    mavlink/v1.0/pixhawk/mavlink_msg_raw_aux.h \
    mavlink/v1.0/pixhawk/mavlink_msg_set_cam_shutter.h \
    mavlink/v1.0/pixhawk/mavlink_msg_set_position_control_offset.h \
    mavlink/v1.0/pixhawk/mavlink_msg_watchdog_command.h \
    mavlink/v1.0/pixhawk/mavlink_msg_watchdog_heartbeat.h \
    mavlink/v1.0/pixhawk/mavlink_msg_watchdog_process_info.h \
    mavlink/v1.0/pixhawk/mavlink_msg_watchdog_process_status.h \
    mavlink/v1.0/pixhawk/pixhawk.h \
    mavlink/v1.0/pixhawk/testsuite.h \
    mavlink/v1.0/pixhawk/version.h \
    mavlink/v1.0/test/mavlink.h \
    mavlink/v1.0/test/mavlink_msg_test_types.h \
    mavlink/v1.0/test/test.h \
    mavlink/v1.0/test/testsuite.h \
    mavlink/v1.0/test/version.h \
    mavlink/v1.0/checksum.h \
    mavlink/v1.0/mavlink_conversions.h \
    mavlink/v1.0/mavlink_helpers.h \
    mavlink/v1.0/mavlink_types.h \
    mavlink/v1.0/protocol.h \
    mavserialport.h

FORMS += \
    mainwindow.ui \
    settingsdialog.ui

RESOURCES += \
    terminal.qrc
