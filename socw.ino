#define DELAY_MS (20)
#define CONSTANT_SPEED_PERIOD_MS (20000)

#define MAX_SPEED_PWM (200)

const uint8_t dirA = 12;
const uint8_t pwmA = 3;
const uint8_t brakeA = 8;

enum state_enum
{
	STOPPED,
	ACCEL,
	AT_SPEED,
	DECEL
};
typedef enum state_enum STATE_ENUM;

enum dir_enum
{
	CW,
	CCW
};
typedef enum dir_enum DIR_ENUM;

STATE_ENUM s_state = STOPPED;
DIR_ENUM s_direction = CCW;
uint8_t s_pwm = 0;
uint16_t speed_ms_count = 0;

void setup()
{
  	pinMode(dirA, OUTPUT);
	pinMode(pwmA, OUTPUT);
	pinMode(brakeA, OUTPUT);

  	digitalWrite(dirA, LOW);
	digitalWrite(pwmA, LOW);
	digitalWrite(brakeA, LOW);
}

void loop()
{
	switch(s_state)
	{
	case STOPPED:
		s_direction = (s_direction == CW) ? CCW : CW;
                digitalWrite(dirA, (s_direction == CW) ? LOW : HIGH);
		s_state = ACCEL;
		break;
		
	case ACCEL:
		if (s_pwm < MAX_SPEED_PWM)
		{
			analogWrite(pwmA, ++s_pwm);
		}
		else
		{
			s_state = AT_SPEED;
		}
		break;
		
	case AT_SPEED:
		speed_ms_count += DELAY_MS;
		if (speed_ms_count >= CONSTANT_SPEED_PERIOD_MS)
		{
			speed_ms_count = 0;
			s_state = DECEL;
		}
		break;
		
	case DECEL:
		if (s_pwm > 0)
		{
			analogWrite(pwmA, --s_pwm);
		}
		else
		{
			s_state = STOPPED;
		}
		break;
	}
	
	delay(DELAY_MS);
}
