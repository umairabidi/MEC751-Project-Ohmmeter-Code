#define EN_A	4
#define IN1 	5
#define IN2 	6


void shuttle_forward();
void shuttle_backward();
void shuttle_stop();


void setup() {
	pinMode(EN_A, OUTPUT);
	digitalWrite(EN_A, 1);
	pinMode(IN1, OUTPUT);
	pinMode(IN2, OUTPUT);
}

void loop() {
	shuttle_forward();
	delay(2200);
	shuttle_stop();
	delay(1000);
	shuttle_backward();
	delay(2200);
	shuttle_stop();
	delay(1000);
}


void shuttle_forward(){
	digitalWrite(IN1, LOW);
	digitalWrite(IN2, HIGH);
}
void shuttle_backward(){
	digitalWrite(IN1, HIGH);
	digitalWrite(IN2, LOW);
}
void shuttle_stop(){
	digitalWrite(IN1, LOW);
	digitalWrite(IN2, LOW);
}
