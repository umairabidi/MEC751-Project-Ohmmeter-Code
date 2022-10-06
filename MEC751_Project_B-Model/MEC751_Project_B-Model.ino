/*
 * This is the B-Model
 * It will have some changes compared to the A-model.
 *
 * Things I can add:
 * - Calibration procedure
 * - States
 * - PID
 * - Speed control (needed for PID)
 * - Resistor Ranges
 * - LEDs
 * - A start button
 * - Safety limit switches?
 * - Throw errors
 * - Statistics on position?
 * 
 * Parameters:
 * Shuttle Distance: 5 - 20 cm
 * Input Resistance: 0 - 20k Ω
 * Voltage Divider resistor R2 = 220 Ω
 * Reference Voltage = 5 V (Vcc as Vref)
 * Motor Voltage = 24 V
 */
 
#include <stdint.h>
#include <LiquidCrystal.h>



// ------------ Pins ------------
const int Vdiv_pin = A6;		// The Voltage Divider analog input pin
const int RS = A0, enable = A1, d4 = A2, d5 = A3, d6 = A4, d7 = A5;
// Ultrasonic Sensor
const int ECHO = 5;		// Green Wire
const int TRIG = 6;		// Yellow Wire
// Motor
const int Motor_in1 = 9;
const int Motor_in2 = 10;
const int Motor_enable = 11;


// ------------ Prototypes ------------
double read_distance();
void update_lcd();
void shuttle_forward();
void shuttle_backward();
void shuttle_stop();

// ------------ Others: objects, global variables ------------
LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);
double calculated_r1;
double required_position;

#define divider_r2 220		// The bottom resistor of the voltage divider
#define Vref 4.4				// Vref used for the Analog Input


void setup(){
	pinMode(Vdiv_pin, INPUT);
	pinMode(ECHO, INPUT);
	pinMode(TRIG, OUTPUT);
	pinMode(Motor_in1, OUTPUT);
	pinMode(Motor_in2, OUTPUT);
	pinMode(Motor_enable, OUTPUT);
	digitalWrite(Motor_enable, HIGH);	// Enable the motor, no reason to ever disable it, right???
	lcd.begin(16, 2);
	Serial.begin(115200);
}

void loop(){
	double Vdiv_Out;
	double current_position;
	bool position_reached = false;
	
	Vdiv_Out = analogRead(Vdiv_pin)*Vref/1023;
	calculated_r1 = divider_r2*(Vref - Vdiv_Out)/Vdiv_Out;
	
	required_position = calculated_r1*0.00075 + 5;		// Change this to a define (r2 and distance range)
	
	// Now move the shuttle to the required position
	// This might need some padding if it constantly readjusts itself (vibrates, can't settle).
	if(position_reached == false){
		current_position = read_distance();
		if (current_position < required_position){
			shuttle_forward();
		}
		else if (current_position > required_position){
			shuttle_backward();
		}
		else if (current_position == required_position){		// I can add padding just to this line (± 2 steps of the resolution, maybe?)
			shuttle_stop();
			position_reached = true;
		}
	}

	update_lcd(current_position, calculated_r1);
	
	_delay_ms(100);
}


double read_distance(){
	static double read_position;
	digitalWrite(TRIG, HIGH);
	_delay_us(10);
	digitalWrite(TRIG, LOW);
	read_position = pulseIn(ECHO, HIGH, 5000)/(20000/346.3);		// Convert ECHO pulse width to distance in cm, limited to 22 cm
	return read_position;
}


void update_lcd(double disp_position, double R1){
	char lcd_buffer1[16] = {0};
	char lcd_buffer2[16] = {0};
	
	// Resistance
	lcd.setCursor(0,0);
	lcd.print("Res: ");
	dtostrf(R1, 4, 2, lcd_buffer1);
	sprintf(lcd_buffer1, "%s \xF4", lcd_buffer1);
	lcd.setCursor(5,0);
	lcd.print(lcd_buffer1);
	
	// Position
	dtostrf(disp_position, 4, 2, lcd_buffer2);
	sprintf(lcd_buffer2, "%s cm  ", lcd_buffer2);
	lcd.setCursor(0,1);
	lcd.print("Pos: ");
	lcd.setCursor(5, 1);
	lcd.print(lcd_buffer2);
}


void shuttle_forward(){
	digitalWrite(Motor_in1, LOW);
	digitalWrite(Motor_in2, HIGH);
}
void shuttle_backward(){
	digitalWrite(Motor_in1, HIGH);
	digitalWrite(Motor_in2, LOW);
}
void shuttle_stop(){
	digitalWrite(Motor_in1, LOW);
	digitalWrite(Motor_in2, LOW);
	//digitalWrite(Motor_enable, LOW);
}