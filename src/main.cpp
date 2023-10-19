#include "main.h"

// Magic functions
void disabled() {}
void competition_initialize() {}
void autonomous() {}

void on_center_button() {
	static bool pressed = false;
	
	if (pressed =! pressed) lcd::set_text(2, "I was pressed!");
	else lcd::clear_line(2);
}

void initialize() {
	lcd::initialize();
	lcd::set_text(1, "Hello PROS User!");

	lcd::register_btn1_cb(on_center_button);
}

void opcontrol() {
	Controller master(E_CONTROLLER_MASTER);

	Motor_Group l_mtrs = Motor_Group({1,2,3});
	Motor_Group r_mtrs = Motor_Group({4,5,6});

	l_mtrs.set_brake_modes(E_MOTOR_BRAKE_HOLD);
	r_mtrs.set_brake_modes(E_MOTOR_BRAKE_HOLD);

	for (;;) {
		auto btn_vals = lcd::read_buttons();
		lcd::print(0, "%d %d %d",	(btn_vals & LCD_BTN_LEFT) >> 2,
		                			(btn_vals & LCD_BTN_CENTER) >> 1,
		                 			(btn_vals & LCD_BTN_RIGHT) >> 0);
		
		int32_t left = master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y),
				right = master.get_analog(E_CONTROLLER_ANALOG_RIGHT_Y);
		if (!left && !right) {
			l_mtrs.brake();
			r_mtrs.brake();
		} else {
			l_mtrs = left;
			r_mtrs = right;
		}
		
		delay(20);
	}
}
