#pragma config(Sensor, dgtl1,  shoulderEncoder, sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  elbowEncoder,   sensorQuadEncoder)
#pragma config(Motor,  port1,           shoulderMotor, tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port10,          elbowMotor,    tmotorVex393_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

typedef struct
{
	float P, I, D, prevError, totalError;
	long prevTime;
	short setpoint;
	bool reverseOutput;
	tMotor plant;
	tSensors sensor;
} PIDData;

void calculatePID(PIDData pid)
{
	short error = SensorValue[pid.sensor] - pid.setpoint; //Calculate error
	short de = error - pid.prevError; //Calculate change in error (for integral and derivitive)
	pid.prevError = error; //prevError is now current error

	long dt = time1[T1] - pid.prevTime; //Change in time (milliseconds)

	float output = pid.P * error; //Add proportional term (current error)

	pid.totalError =+ error * dt; //Calculate integral (area under error curve)
	output =+ pid.I * pid.totalError; //Add integral term

	output =- pid.D * de / dt; //Subract derivative term (slope of error)

	motor[pid.plant] = output; //Set motor to output
}

void initPID(PIDData pid)
{
	pid.prevError = SensorValue[pid.sensor] - pid.setpoint;
	pid.prevTime = time1[T1];
}

task main()
{
	struct PIDData shoulderPID; //Construct shoulder PID
	shoulderPID.P = 0;
	shoulderPID.I = 0;
	shoulderPID.D = 0;
	shoulderPID.setpoint = 200;
	shoulderPID.reverseOutput = false;
	shoulderPID.plant = shoulderMotor;
	shoulderPID.sensor = shoulderEncoder;

	struct PIDData elbowPID; //Construct elbow PID
	elbowPID.P = 0;
	elbowPID.I = 0;
	elbowPID.D = 0;
	elbowPID.setpoint = 200;
	elbowPID.reverseOutput = false;
	elbowPID.plant = elbowMotor;
	elbowPID.sensor = elbowEncoder;

	initPID(shoulderPID);
	initPID(elbowPID);

	while(true)
	{
		calculatePID(shoulderPID);
		calculatePID(elbowPID);
	}
}