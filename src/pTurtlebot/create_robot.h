#ifndef CREATE_ROBOT_H
#define CREATE_ROBOT_H

/** C includes **/
#include "oi.h" //iRobot's Open Interface
#include <stdio.h>   /* Standard input/output definitions */
#include <stdlib.h>   /* Standard C functions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */

/** C++ includes **/
#include <iostream>

/** Macro definitions **/
#define HIGH(A) (A & 0xFF00)>>8
#define LOW(A) (A & 0xFF)

/** Power LED colors**/
#define POWER_GREEN 	0x00
#define POWER_RED 	0xFF
#define POWER_ORANGE	0x7F
#define POWER_YELLOW	0x0B


using namespace std;

class CreateRobot
{
	private:
		int 	fd_create; //Sending serial commands to Create
		int 	fd_avr; 
		bool 	started;
		char	playAdvanceLEDsByte;
		char	powerLEDColor;	
		char	powerLEDIntensity;			
		
		int		openSerialCom(const char* port);	
		
		bool 	writeToCreate(char* command, size_t numBytes);
		bool 	readFromCreate(char* incoming, size_t numBytes);
		
		void 	updateLEDS();
			
	public:		
		CreateRobot(); 				
		
		bool 	openCreateSerial(const char* port);
		bool 	openAVRSerial(const char* port);
		
		bool 	createInit(const char* create_port, const char* avr_port);
		void 	createTerminate();
				
		bool 	sendCommand(char* str_command);
		void 	readData();		
		
		bool 	isStarted();
		char 	getPowerLEDColor();
		char 	getPowerLEDIntensity();
		char 	getPlayLEDState();
		char 	getAdvanceLEDState();
		
		void	setPlayLED(bool update);
		void	setAdvanceLED(bool update);
		void 	setPowerLED(char intensity, bool update);
		void 	setPowerLEDColor(char color, bool update);
		void	clearPlayLED(bool update);
		void	clearAdvanceLED(bool update);
		void	clearPowerLED(bool update);
		void 	togglePlayLED(bool update);
		void	toggleAdvanceLED(bool update);
		void	togglePowerLED(bool update);
};
#endif
