///////////////////////////////////////////////////////////////////////////////
// FILE:          FluidController.cpp
// PROJECT:       Micro-Manager
// SUBSYSTEM:     DeviceAdapters
//-----------------------------------------------------------------------------
// DESCRIPTION:   Controls microfluidics stimulus controller 
// LICENSE:       LGPL
// AUTHOR:        Daniel Merritt
//

#include "FluidController.h"

// Alias code used to simplify the usual function call and susequent error code checking.
#define RETURN_ON_MM_ERROR( result ) do { \
   int return_value = (result); \
   if (return_value != DEVICE_OK) { \
      return return_value; \
   } \
} while (0)



const char* g_DeviceFluidControllerName = "FluidController";



///////////////////////////////////////////////////////////////////////////////
// Exported MMDevice API
///////////////////////////////////////////////////////////////////////////////
MODULE_API void InitializeModuleData()
{
	RegisterDevice(g_DeviceFluidControllerName, MM::GenericDevice, "Stimulus controller for microfluidics applications");
}

MODULE_API MM::Device* CreateDevice(const char* deviceName)
{
	if (deviceName == 0)
		return 0;
	if(strcmp(deviceName, g_DeviceFluidControllerName) == 0)
		return new FluidController;

    return 0;
}

MODULE_API void DeleteDevice(MM::Device* pDevice)
{
   delete pDevice;
}

///////////////////////////////////////////////////////////////////////////////
// FluidController LBX DPSS
///////////////////////////////////////////////////////////////////////////////
FluidController::FluidController() :
	port_("Undefined"),
	initialized_(false),
	busy_(false)
{
	InitializeDefaultErrorMessages();
	SetErrorText(ERR_PORT_CHANGE_FORBIDDEN, "Port cannot be changed once after device has been initialized.");

	// Name
	CreateProperty(MM::g_Keyword_Name, g_DeviceFluidControllerName, MM::String, true);

	// Description
	CreateProperty(MM::g_Keyword_Description, "A stimulus controller for microfluidics.", MM::String, true);

	// Port
	CPropertyAction* pAct = new CPropertyAction (this, &FluidController::OnPort);
	CreateProperty(MM::g_Keyword_Port, "Undefined", MM::String, false, pAct, true);

}

FluidController::~FluidController()
{
     Shutdown();
}


void FluidController::GetName(char* Name) const
{
     CDeviceUtils::CopyLimitedString(Name, g_DeviceFluidControllerName);
}


int FluidController::Initialize(){
	initialized_ = true;
	return DEVICE_OK;
}


int FluidController::Shutdown()
{
/*	if (initialized_) {
		EmissionOnOff(0);		// Turns off the emssion on "Shutdown"
		initialized_ = false;
	}*/
	return DEVICE_OK;
}


bool FluidController::Busy()
{
	return busy_;
}


int FluidController::EmissionOnOff(int onoff)
{
	std::string command, answer;
/*
	if (onoff == 0) {
		command = "dl 0";
		emissionStatus_ = "Off";
	}
	else if (onoff == 1) {
		command = "dl 1";
		emissionStatus_ = "On";
	}
	RETURN_ON_MM_ERROR( SendAndReceive(command,answer) );*/

	return DEVICE_OK;
}

///////////////////////////////////////////////////////////////////////////////
// Action handlers
///////////////////////////////////////////////////////////////////////////////


int FluidController::OnPort(MM::PropertyBase* pProp, MM::ActionType eAct)
{
/*	if (eAct == MM::BeforeGet) {
		pProp->Set(port_.c_str());
	} else if (eAct == MM::AfterSet) {
		if (initialized_) {
			// revert
			pProp->Set(port_.c_str());
			return ERR_PORT_CHANGE_FORBIDDEN;
		}
		pProp->Get(port_);
	}*/
	return DEVICE_OK;
}


int FluidController::OnSerialNumber(MM::PropertyBase* pProp, MM::ActionType)
{
	std::string command, answer;
	command = "hid?";

	RETURN_ON_MM_ERROR( SendAndReceive(command,answer) );
	pProp->Set(answer.c_str());
     
	return DEVICE_OK;
}


int FluidController::OnSWVersion(MM::PropertyBase* pProp, MM::ActionType)
{
	std::string command;
	command = "?sv";
/*
	RETURN_ON_MM_ERROR( SendAndReceive(command,softVersion_) );
	pProp->Set(softVersion_.c_str());*/
     
	return DEVICE_OK;
}


int FluidController::OnModel(MM::PropertyBase* pProp, MM::ActionType)
{
 	std::string command, answer;
	command = "inf?";

	RETURN_ON_MM_ERROR( SendAndReceive(command,answer) );

	std::string::size_type Pos = answer.find_first_of("-", 0) + 1;
	Pos = answer.find_first_of("-", Pos);

	pProp->Set(answer.substr(0,Pos).c_str());

	return DEVICE_OK;
}


int FluidController::OnLaserStatus(MM::PropertyBase* pProp, MM::ActionType )
{
	int currentStatus = 0;
	std::string laserStatus = "";
/*	
	RETURN_ON_MM_ERROR( GetStatus(currentStatus) );

	switch(currentStatus) {
		case 1 : laserStatus = "Warm-up phase";
			break;
		case 2 : laserStatus = "Stand-by for emission";
			break;
		case 3 : laserStatus = "Emission is on";
			break;
		case 4 : laserStatus = "Alarm raised";
			break;
		case 5 : laserStatus = "Internal error raised";
			break;
		case 6 : laserStatus = "Sleep mode";
			break;
		case 7 : laserStatus = "Searching for SLM point";
			break;
		default	 : return DEVICE_UNKNOWN_POSITION;
	}
	pProp->Set(laserStatus.c_str());
	*/
	return DEVICE_OK;
}


int FluidController::OnEmissionOnOff(MM::PropertyBase* pProp, MM::ActionType eAct)
{
	/*std::string command, answer; 
	emissionStatus_ = "Undefined";

	if (eAct == MM::BeforeGet) {
		int currentStatus = 0;
		RETURN_ON_MM_ERROR( GetStatus(currentStatus) );

		if ( (currentStatus == 1) || (currentStatus == 3) || (currentStatus == 7) )
			emissionStatus_ = "On";
		else
			emissionStatus_ = "Off";
          
		pProp->Set(emissionStatus_.c_str());

	} else if (eAct == MM::AfterSet) { 
		pProp->Get(answer);
		
		if (answer == "Off") 
			EmissionOnOff(false);
		else 
			EmissionOnOff(true);
	
		CDeviceUtils::SleepMs(500);
	}*/
	return DEVICE_OK;
}



int FluidController::OnAlarm(MM::PropertyBase* pProp, MM::ActionType )
{
/*	std::string command, answer;
	int currentAlarm = 0;
     
	command = "?f";
    RETURN_ON_MM_ERROR( SendAndReceive(command,answer) );

	currentAlarm = atoi(answer.c_str());

	switch(currentAlarm) {
		case 0 : alarm_ = "No alarm";
			break;
		case 1 : alarm_ = "Out-of-bounds current";
			break;
		case 2 : alarm_ = "Out-of-bounds power";
			break;
		case 3 : alarm_ = "Out-of-bounds supply voltage";
			break;
		case 4 : alarm_ = "Out-of-bounds inner temperature";
			break;
		case 5 : alarm_ = "Out-of-bounds laser head temperature";
			break;
		case 7 : alarm_ = "Interlock circuit open";
			break;
		case 8 : alarm_ = "Manual reset";
			break;
		default	 : return DEVICE_UNKNOWN_POSITION;
	}


	pProp->Set(alarm_.c_str());
	*/
	return DEVICE_OK;
}



int FluidController::OnInterlocked(MM::PropertyBase* pProp, MM::ActionType )
{
	/*std::string command, answer, interlockStatus;
     
	command = "?int";
    RETURN_ON_MM_ERROR( SendAndReceive(command,answer) );

	if( answer.compare("1") == 0)
				interlockStatus = "Closed";
			else
				interlockStatus = "Open";

	pProp->Set(interlockStatus.c_str());
	*/
	return DEVICE_OK;
}


int FluidController::OnControlMode(MM::PropertyBase* pProp, MM::ActionType eAct)
{
	std::string command, query, answer, newMode;
	std::ostringstream newCommand;
/*
	if (eAct == MM::BeforeGet) {
		if( model_.compare("LBX") == 0) {
			command = "?acc";
			RETURN_ON_MM_ERROR( SendAndReceive(command,answer) );

			if( answer.compare("1") == 0)
				modeControl_ = "ACC";
			else
				modeControl_ = "APC";
		} else {
			modeControl_ = "APC";
		}
		pProp->Set(modeControl_.c_str());
	}
	else if (eAct == MM::AfterSet) {
		
		pProp->Get(newMode);
		if( (model_.compare("LBX") == 0) && (modeControl_.compare(newMode) != 0) ) {

			EmissionOnOff(0);		// Turns the emission off before changing the control mode

			if( newMode.compare("ACC") == 0)
				query = "1";
			else
				query = "0";
			newCommand << "acc " << query;
			command = newCommand.str();

			RETURN_ON_MM_ERROR( SendAndReceive(command,answer) );
		}
	}*/
	return DEVICE_OK;
}



int FluidController::OnPower(MM::PropertyBase* pProp, MM::ActionType )
{
	std::string command, answer;
               
	command = "?p";
	RETURN_ON_MM_ERROR( SendAndReceive(command,answer) );
	pProp->Set(atof(answer.c_str()));
     
	return DEVICE_OK;
}


int FluidController::OnPowerSP(MM::PropertyBase* pProp, MM::ActionType eAct, long )
{
	std::string command, answer;
	std::ostringstream newCommand;

	/*if (eAct == MM::BeforeGet) {
		if( model_.compare("LBX") == 0) {
			command = "?sp";
			RETURN_ON_MM_ERROR( SendAndReceive(command,answer) );
			pProp->Set( 100 * atof(answer.c_str()) / nominalPower_ );
		}
		else {  // assuming "LCX"
			command = "ip";
			RETURN_ON_MM_ERROR( SendAndReceive(command,answer) );
			answer = answer.substr( 0, std::string::npos-1);
			pProp->Set( atof(answer.c_str()) );
		}
	}
	else if (eAct == MM::AfterSet) {
          
		pProp->Get(powerSP_);
		if( model_.compare("LBX") == 0)
			newCommand << "p " << ( nominalPower_ * powerSP_ / 100 );
		else
			newCommand << "ip " << powerSP_ ;

		command = newCommand.str();

		RETURN_ON_MM_ERROR( SendAndReceive(command,answer) );
	}*/

	return DEVICE_OK;
}



int FluidController::OnCurrent(MM::PropertyBase* pProp, MM::ActionType )
{
/*	if( model_.compare("LMX") != 0) {
		std::string command, answer;
               
		command = "?c";
		RETURN_ON_MM_ERROR( SendAndReceive(command,answer) );

		pProp->Set(atof(answer.c_str()));
	}*/
	return DEVICE_OK;
}



int FluidController::OnCurrentSP(MM::PropertyBase* pProp, MM::ActionType eAct, long )
{
/*	std::string command, answer;
	std::ostringstream newCommand;

	if (eAct == MM::BeforeGet) {
		if( model_.compare("LBX") == 0) {
			command = "?sc";
			RETURN_ON_MM_ERROR( SendAndReceive(command,answer) );

			pProp->Set(atof(answer.c_str()));
		} else {
			pProp->Set(0.0);
		}
	}
	else if (eAct == MM::AfterSet) {
		if( model_.compare("LBX") == 0) {
			pProp->Get(currentSP_);
			newCommand << "c " << currentSP_;
			command = newCommand.str();
			
			RETURN_ON_MM_ERROR( SendAndReceive(command,answer) );
		}
	}*/
	return DEVICE_OK;
}


int FluidController::OnSleep(MM::PropertyBase* pProp, MM::ActionType eAct )
{
/*	std::string command, query, answer, Smod;
	std::ostringstream newCommand;

	if (eAct == MM::BeforeGet) {
		if( model_.compare("LMX") != 0) {
			command = "?t";
			RETURN_ON_MM_ERROR( SendAndReceive(command,answer) );

			if( answer.compare("0") == 0)
				pProp->Set("Sleep");
			else
				pProp->Set("Ready");
		} else {
			pProp->Set("Ready");
		}
	}
	else if (eAct == MM::AfterSet) {
		if( model_.compare("LMX") != 0) {

			pProp->Get(Smod);
			if( Smod.compare("Sleep") == 0)
				query = "0";
			else
				query = "1";
			newCommand << "t " << query;
			command = newCommand.str();

			RETURN_ON_MM_ERROR( SendAndReceive(command,answer) );
		} else {
			// do nothing for LMX
		}
	}*/
	return DEVICE_OK;
}


int FluidController::OnAnalogMod(MM::PropertyBase* pProp, MM::ActionType eAct )
{
	std::string command, query, answer, Amod;
	std::ostringstream newCommand;
/*
	if (eAct == MM::BeforeGet) {
		if( model_.compare("LBX") == 0) {
			command = "?am";
			RETURN_ON_MM_ERROR( SendAndReceive(command,answer) );
	
			if( answer.compare("1") == 0)
				pProp->Set("On");
			else
				pProp->Set("Off");
		} else {
			pProp->Set("Off");
		}
	}
	else if (eAct == MM::AfterSet) {
		if( model_.compare("LBX") == 0) {

			pProp->Get(Amod);
			if( Amod.compare("On") == 0)
				query = "1";
			else
				query = "0";
			newCommand << "am " << query;
			command = newCommand.str();

			RETURN_ON_MM_ERROR( SendAndReceive(command,answer) );
		} else {
			// do nothing for LMX or LCX
		}
	}*/
	return DEVICE_OK;
}


int FluidController::OnDigitalMod(MM::PropertyBase* pProp, MM::ActionType eAct )
{
/*	std::string command, query, answer, Dmod;
	std::ostringstream newCommand;

	if (eAct == MM::BeforeGet) {
		if( model_.compare("LBX") == 0) {
			command = "?ttl";
			RETURN_ON_MM_ERROR( SendAndReceive(command,answer) );

			if( answer.compare("1") == 0)
				pProp->Set("On");
			else
				pProp->Set("Off");
		} else {
			pProp->Set("Off");
		}
	}
	else if (eAct == MM::AfterSet) {
		if( model_.compare("LBX") == 0) {

			pProp->Get(Dmod);
			if( Dmod.compare("On") == 0)
				query = "1";
			else
				query = "0";
			newCommand << "ttl " << query;
			command = newCommand.str();

			RETURN_ON_MM_ERROR( SendAndReceive(command,answer) );
		} else {
			// do nothing for LMX or LCX
		}
	}*/
	return DEVICE_OK;
}


int FluidController::OnHours(MM::PropertyBase* pProp, MM::ActionType )
{
	std::string command, answer;
               
	command = "?hh";
	RETURN_ON_MM_ERROR( SendAndReceive(command,answer) );

	pProp->Set( atof(answer.c_str()) );

	return DEVICE_OK;
}


int FluidController::OnBaseTemp(MM::PropertyBase* pProp, MM::ActionType )
{
	std::string command, answer;
               
	command = "?bt";
	RETURN_ON_MM_ERROR( SendAndReceive(command,answer) );

	pProp->Set( atof(answer.c_str()) );
     
	return DEVICE_OK;
}

///////////////////////////////////////////////////////////////////////////////
// Generic--purpose, private methods
///////////////////////////////////////////////////////////////////////////////

int FluidController::GetStatus(int &status)
{
/*	std::string command,answer;
               
	command = "?sta";
	RETURN_ON_MM_ERROR( SendAndReceive(command,answer) );

	status = atoi(answer.c_str());*/
     
	return DEVICE_OK;
}


int FluidController::SendAndReceive(std::string& command, std::string& answer)
{
/*	RETURN_ON_MM_ERROR( SendSerialCommand(port_.c_str(), command.c_str(), "\n") );
	RETURN_ON_MM_ERROR( GetSerialAnswer(port_.c_str(), "\r\n", answer) );
     */
	return DEVICE_OK;
}