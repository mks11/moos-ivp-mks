/************************************************************/
/*    NAME: Mukul K Singh                                              */
/*    ORGN: MIT                                             */
/*    FILE: Turtlebot.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef Turtlebot_HEADER
#define Turtlebot_HEADER

#include "create_robot.h"
#include "MOOS/libMOOS/MOOSLib.h"
#include <string>

using namespace std;

class Turtlebot : public CMOOSApp
{
 public:
   Turtlebot();
   ~Turtlebot();
   
   // CreateRobot theBot;
 protected:
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();
   void RegisterVariables();
   
 private: // Configuration variables
   string m_incoming_cmd; // subscribe this as SEND_CMD
   string m_states;  // publish this TURTLEBOT_STATE
   
 private: // State variables
   unsigned int m_iterations;
   double       m_timewarp;
   std::string  m_current_cmd;
};

#endif 
