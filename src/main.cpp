#include "main.h"
#include <string.h>
#include <iostream> 

Motor_Group l_mtrs({12,2,1}),
			r_mtrs({16,14,3}); 
Motor_Group cplt_mtrs({Motor(20, false), Motor(10, true)});

double max(double a, double b) {
	return a > b ? a : b;
}

void autonomous() {
	// Note: Use pros::Task	
	l_mtrs = 64;
	r_mtrs = 127;

	delay(1000);

	l_mtrs = 0;
	r_mtrs = 0;
}

// 60 going negative

void initialize() {
	lcd::initialize();
	lcd::set_text(1, "Integral Integral");

	cplt_mtrs.set_encoder_units(E_MOTOR_ENCODER_DEGREES);

	l_mtrs.set_brake_modes(E_MOTOR_BRAKE_HOLD);
	r_mtrs.set_brake_modes(E_MOTOR_BRAKE_HOLD);
	r_mtrs.set_reversed(true);
	cplt_mtrs.set_brake_modes(E_MOTOR_BRAKE_HOLD);
}

void opcontrol() {
	Controller master(E_CONTROLLER_MASTER);

	const double lwr = 45, upr = 60, k = 1;

	for (;;) {
		const int32_t left = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y),
					  right = master.get_analog(E_CONTROLLER_ANALOG_RIGHT_Y);
		
		if (left || right) {
			l_mtrs = left;
			r_mtrs = right;
		} else {
			l_mtrs.brake();
			r_mtrs.brake();
		}
		
		const int32_t sht_fwd = master.get_digital(E_CONTROLLER_DIGITAL_R1),
					  sht_bck = master.get_digital(E_CONTROLLER_DIGITAL_R2);

		const double cplt_pos = cplt_mtrs.get_positions()[0];
		if ((cplt_pos > lwr || sht_fwd) && (cplt_pos < upr || sht_bck))
			cplt_mtrs = 127 * (sht_fwd*(1 - exp(k*(cplt_pos - upr))) - sht_bck*(1 - exp(-k*(cplt_pos - lwr))));
		else cplt_mtrs.brake();
		lcd::set_text(2,std::to_string(cplt_pos));
		
		delay(20);
	}
}
