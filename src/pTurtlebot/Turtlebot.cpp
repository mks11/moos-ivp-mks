/************************************************************/
/*    NAME: Mukul K Singh                                              */
/*    ORGN: MIT                                             */
/*    FILE: Turtlebot.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "Turtlebot.h"

// create robot interface 
#include "create_robot.h"

CreateRobot theBot;

using namespace std;

//---------------------------------------------------------
// Constructor

Turtlebot::Turtlebot()
{
  // initilizing the robot interface
  m_iterations = 0;
  m_timewarp   = 1;
  
}

//---------------------------------------------------------
// Destructor


Turtlebot::~Turtlebot()
{
  theBot.createTerminate();
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool Turtlebot::OnNewMail(MOOSMSG_LIST &NewMail)
{
  cout << "in OnNewMail " <<endl;
  MOOSMSG_LIST::iterator p;
   
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;

    //#if 0 // Keep these around just for template
    string key   = msg.GetKey();
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString(); 
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
    //#endif
    
    if(key == "SEND_CMD")
      {
	m_current_cmd = toupper(sval); // for simplicity example format is like "move,1,2"
	cout <<"in the key conditional; current cmd:" <<m_current_cmd<<endl;
      }
  }
	
  return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool Turtlebot::OnConnectToServer()
{
   // register for variables here
   // possibly look at the mission file?
   // m_MissionReader.GetConfigurationParam("Name", <string>);
   // m_Comms.Register("VARNAME", 0);
	
   RegisterVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool Turtlebot::Iterate()
{
  m_iterations++;
 
  // //place to handle cmd
  //sendCommand requires a NON-CONST pointer to char
  char * p_current_cmd = const_cast<char*>(m_current_cmd.c_str());//c_str();
  cout<<"in Iterate(), current cmd: "<< p_current_cmd<<endl;
  theBot.sendCommand(p_current_cmd); //creates a pointer to the array that string object represensts 
  
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool Turtlebot::OnStartUp()
{
  list<string> sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(m_MissionReader.GetConfiguration(GetAppName(), sParams)) {
    list<string>::iterator p;
    for(p=sParams.begin(); p!=sParams.end(); p++) {
      string original_line = *p;
      string param = stripBlankEnds(toupper(biteString(*p, '=')));
      string value = stripBlankEnds(*p);
      
      if(param == "FOO") {
        //handled
      }
      else if(param == "BAR") {
        //handled
      }
    }
  }
  m_timewarp = GetMOOSTimeWarp();
  cout <<"in startup "<<endl;
  
  std::string str = "/dev/ttyUSB0";
  char * pPort = const_cast<char*>(str.c_str());
  theBot.createInit(pPort,NULL);

 
 //initlize theBot
  // if (theBot->createInit("/dev/ttyUSB0", NULL)){
  //  cout<<"initlizing..."<<endl;
  // return 0;
  //} //NULL since no AVR connected.
  
  RegisterVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void Turtlebot::RegisterVariables()
{
  // m_Comms.Register("FOOBAR", 0);
  m_Comms.Register("SEND_CMD",0);
}

