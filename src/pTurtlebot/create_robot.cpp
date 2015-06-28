#include "create_robot.h"

//Default constructor
CreateRobot::CreateRobot()
{	
	fd_create=-1;
	fd_avr=-1;
	started = false;
	
	playAdvanceLEDsByte = 0; 	//Play and Advance LEDs are off
	powerLEDIntensity = 0; 		//Power LED is off
	powerLEDColor = 0; 			//Power LED is green	
}

int CreateRobot::openSerialCom(const char* port)
{
	int fd; //File descriptor
	struct termios options; //Struct that holds configuration data
	
	//O_RDWR: Open for Read and Write
	//O_NOCTTY: this process doesn't control the terminal
	//O_NDELAY: this process doesn't sleep waiting the serial port be ready
	fd = open(port,O_RDWR | O_NOCTTY | O_NDELAY);
	
	//Early return if couldn't open the port
	if(fd == -1)
		return -1;

	fcntl(fd, F_SETFL, FNDELAY); //Sets non-blocking read mode

	tcgetattr(fd,&options); //Reads in current port configuration (default)
	cfsetospeed(&options,B57600); //Output baud rate
	cfsetispeed(&options,B57600); //Inut baud rate
	options.c_cflag |=(CLOCAL | CREAD); //Enables receiver and sets local mode bits
	
	options.c_cflag &= ~CSIZE; //Clears all size bits;
	options.c_cflag |= CS8; //8 data bits

	options.c_cflag &= ~PARENB; //No parity

	options.c_cflag &= ~CSTOPB; //One stop bit

	options.c_cflag &= ~CRTSCTS; //Disables hardware flow control

	//Returns error if failed to apply configurations
	if(tcsetattr(fd,TCSANOW,&options) == -1)
		return -1;

	return fd;
}

bool CreateRobot::openCreateSerial(const char* port)
{
	fd_create = openSerialCom(port);
	
	if(fd_create!=-1)
		return true;
	else
		return false;
}

bool CreateRobot::openAVRSerial(const char* port)
{
	fd_avr = openSerialCom(port);
	
	if(fd_avr!=-1)
		return true;
	else
		return false;
}


bool CreateRobot::writeToCreate(char* command, size_t numBytes)
{
  std::cout<<"writing to create.."<<std::endl;
	return (write(fd_create, command, numBytes)>0? true:false);	
}

bool CreateRobot::readFromCreate(char* incoming, size_t numBytes)
{
	return (read(fd_create,incoming, numBytes)>0? true:false);
}

bool CreateRobot::sendCommand(char* str_command)
{	
	char* token; 
	
	
	//Extracts the command
	if((token = strtok(str_command," \t")) == NULL)
		return false; //no command

	//Sends the appropriate bytes depending on the command type
	
	//Toggles between START and PASSIVE with the same code
	if((strcmp(token,"START") == 0)||(strcmp(token,"PASSIVE") == 0))
	{
		char command = CmdStart;
		writeToCreate(&command, 1);
	}
	else if(strcmp(token,"INIT_SAFE") == 0)
	{
		char command = CmdSafe;
		writeToCreate(&command, 1);
	}
	else if(strcmp(token,"INIT_FULL") == 0)
	{
		char command = CmdFull;
		writeToCreate(&command, 1);
	}
	else if(strcmp(token,"DRIVE") == 0) //Requires two integer arguments
	{
		int vel, radius;
		char *arg1, *arg2;

		arg1 = strtok(NULL," \t");
		arg2 = strtok(NULL," \t");
		
		if((arg1 == NULL) || (arg2 == NULL)) return false;

		vel = atoi(arg1); //Velocity in mm/s
		radius = atoi(arg2); //Turn radius in mm
		std::cout<<"DRIVE ..."<<arg1<<" "<<arg2<<std::endl;
		char command[] = {CmdDrive,HIGH(vel),LOW(vel),HIGH(radius),LOW(radius)};
		
		if (writeToCreate(command, 5)== true)
		  {std::cout<<" writing DRIVE to create"<<std::endl;}
		
	}
	else if(strcmp(token,"DRIVE_DIRECT") == 0) //Requires two integer arguments
	{
		int vel_right, vel_left;
		char *arg1, *arg2;

		arg1 = strtok(NULL," \t");
		arg2 = strtok(NULL," \t");
		
		if((arg1 == NULL) || (arg2 == NULL)) return false;

		vel_right = atoi(arg1); //Velocity in mm/s for the right wheel
		vel_left  = atoi(arg2); //Velocity in mm/s for the left wheel

		char command[] = {CmdDriveWheels,HIGH(vel_right),LOW(vel_right),HIGH(vel_left),LOW(vel_left)};
		writeToCreate(command, 5);
	}
	else if(strcmp(token,"STRAIGHT") == 0) //Requires one integer argument
	{
		int vel;
		char* arg;

		arg = strtok(NULL," \t");

		if(arg == NULL) return false;		

		vel = atoi(arg); //Velocity in mm/s
		
		char command[] = {CmdDrive,HIGH(vel),LOW(vel),0x80,0x00};		
		writeToCreate(command, 5);
	}
	else if(strcmp(token,"BACKWARDS") == 0) //Requires one integer argument
	{
		int vel;
		char* arg;

		arg = strtok(NULL," \t");

		if(arg == NULL) return false;		

		vel = atoi(arg); //Velocity in mm/s
		vel = -vel;
		
		char command[] = {CmdDrive,HIGH(vel),LOW(vel),0x80,0x00};		
		writeToCreate(command, 5);
	}
	
	else if(strcmp(token,"CW") == 0) //Requires one integer argument
	{
		int vel;
		char* arg;

		arg = strtok(NULL," \t");

		if(arg == NULL) return false;		

		vel = atoi(arg); //Velocity in mm/s
		
		char command[] = {CmdDrive,HIGH(vel),LOW(vel),0xFF,0xFF};
		writeToCreate(command, 5);
	}
	else if(strcmp(token,"CCW") == 0) //Requires one integer argument
	{
		int vel;
		char* arg;

		arg = strtok(NULL," \t");

		if(arg == NULL) return false;		

		vel = atoi(arg); //Velocity in mm/s
		
		char command[] = {CmdDrive,HIGH(vel),LOW(vel),0x00,0x01};
		writeToCreate(command, 5);
	}
	else if(strcmp(token,"STOP") == 0)
	{
		char command[] = {CmdDrive,0,0,0,0};
		writeToCreate(command, 5);
	}
	else if(strcmp(token,"LEDS") == 0) //Requires 4 byte arguments
	{		
		char *playState, *advanceState, *powerColor, *powerIntensity;

		playState = strtok(NULL," \t");
		advanceState = strtok(NULL," \t");
		powerColor = strtok(NULL," \t");
		powerIntensity = strtok(NULL," \t");
		
		if((playState == NULL) || (advanceState == NULL)
			|| (powerColor == NULL) || (powerIntensity == NULL)) 
			return false;
		
		playAdvanceLEDsByte = (char) (atoi(playState)<<1)+(atoi(advanceState)<<3);
		
		powerLEDColor = (char) atoi(powerColor);
		powerLEDIntensity = (char) atoi (powerIntensity);
		
		updateLEDS();
	}
	else
		return false; //Unknown command
	
	std::cout << "inside Create:sendCommand--token: "<<token<<std::endl;
	return true;
}

void CreateRobot::readData()
{}

bool CreateRobot::createInit(const char *create_port="/dev/ttyUSB0",
	const char *avr_port="/dev/ttyUSB1")
{
	char start[] = "START";
	char init_safe[] = "INIT_SAFE";

	bool flagSuccess = true;

	if(openCreateSerial(create_port))
	{
		cout<<"\n\nCreate serial connection successfully opened!\n";
		
		cout<<"Sending Create the START command...";		
		sendCommand(start);
		cout<<"done!\n";
		
		started = true; //Robot is in START state
		
		cout<<"Initializing Create in Safe mode...";		
		sendCommand(init_safe);
		cout<<"done!\n";
		
		//Uses the LEDs to show that the robot is initialized
		setPowerLED(0xFF, false);
		setPowerLEDColor(POWER_YELLOW,false);
		clearPlayLED(false);
		clearAdvanceLED(false);		
		updateLEDS();		
	}		
	else
	{
		cout<<"\n\nCouldn't open serial connection with Create.\n\n";	
		flagSuccess = false;
	}

	if(avr_port != NULL)
	{
		if(openAVRSerial(avr_port))	
			cout<<"\n\nAVR serial connection successfully opened!\n";
		else
		{
			cout<<"\n\nCouldn't open serial connection with AVR.\n\n";
			flagSuccess = false;
		}
	}
	else
		cout<<"\n\nAVR serial connection skipped.\n\n";	
		
	return flagSuccess;
}

void CreateRobot::createTerminate()
{
	char stop[] = "STOP";
	char passive[] = "PASSIVE";
	
	//Robot stops moving
	sendCommand(stop);
	
	//Puts it back in passive mode if the robot has been started
	if(started)	
		sendCommand(passive);
		
	started = false;

	/*Closes all open serial communication interfaces*/
	if(fd_create != -1)
	{
		cout<<"\n\nClosing Create's serial connection";
		close(fd_create);
		fd_create = -1;
	}

	if(fd_avr != -1)
	{
		cout<<"\n\nClosing AVR's serial connection";
		close(fd_avr);
		fd_avr = -1;
	}	
	
	cout<<"\n\nCreate terminated. Hasta la vista, baby.";
}

bool CreateRobot::isStarted()
{
	return started;
}

char CreateRobot::getPowerLEDColor()
{
	return powerLEDColor;
}

char CreateRobot::getPowerLEDIntensity()
{
	return powerLEDIntensity;
}

char CreateRobot::getPlayLEDState()
{
	return (playAdvanceLEDsByte & LEDPlay)>>1;
}

char CreateRobot::getAdvanceLEDState()
{
	return (playAdvanceLEDsByte & LEDAdvance)>>3;
}

void CreateRobot::setPlayLED(bool update)
{
	playAdvanceLEDsByte |= LEDPlay;
	if (update) updateLEDS();
}

void CreateRobot::setAdvanceLED(bool update)
{
	playAdvanceLEDsByte |= LEDAdvance;
	if (update) updateLEDS();
}

void CreateRobot::setPowerLED(char intensity, bool update)
{
	powerLEDIntensity = intensity;
	if (update) updateLEDS();
}

void CreateRobot::setPowerLEDColor(char color, bool update)
{
	powerLEDColor = color;
	if (update) updateLEDS();
}

void CreateRobot::clearPlayLED(bool update)
{
	playAdvanceLEDsByte &= ~LEDPlay;
	if (update) updateLEDS();
}

void CreateRobot::clearAdvanceLED(bool update)
{
	playAdvanceLEDsByte &= ~LEDAdvance;
	if (update) updateLEDS();
}

void CreateRobot::clearPowerLED(bool update)
{
	powerLEDIntensity = 0x00;
	if (update) updateLEDS();
}

void CreateRobot::togglePlayLED(bool update)
{
	playAdvanceLEDsByte ^= LEDPlay;		
	if (update) updateLEDS();
}

void CreateRobot::toggleAdvanceLED(bool update)
{
	playAdvanceLEDsByte ^= LEDAdvance;
	if (update) updateLEDS();
}

void CreateRobot::togglePowerLED(bool update)
{
	powerLEDIntensity = 0xFF - powerLEDIntensity;
	if (update) updateLEDS();
}

void CreateRobot::updateLEDS()
{
	char command[] = {CmdLeds,playAdvanceLEDsByte,powerLEDColor,powerLEDIntensity};
	writeToCreate(command, 4);
}





