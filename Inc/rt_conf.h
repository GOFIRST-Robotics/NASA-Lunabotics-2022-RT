//
// Created by julia on 11/5/19.
//

#ifndef NASA_RMC_RT_RT_CONF_H
#define NASA_RMC_RT_RT_CONF_H
/*
 * This file should be a central location that all interacting IDs can be
 * placed, including VESC IDs and system IDs, since all of these have to be
 * unique between each other. Verify that this information is reflected in the
 * README
 */

// System IDs
#define DRIVETRAIN_SYS_ID 100u
#define EXCAVATE_SYS_ID 102u
#define INTAKE_SYS_ID 101u

// Controller IDs

#define DRIVE_MOTOR_BL 2u
#define DRIVE_MOTOR_FL 1u
#define DRIVE_MOTOR_BR 4u
#define DRIVE_MOTOR_FR 3u
#define EXCAVATE_MOTOR_L 5u
#define EXCAVATE_MOTOR_R 6u
#define INTAKE_MOTOR 7u

#endif  // NASA_RMC_RT_RT_CONF_H
