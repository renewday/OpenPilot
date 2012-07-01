/**
 ******************************************************************************
 *
 * @file       slam.c
 * @author     The OpenPilot Team, http://www.openpilot.org Copyright (C) 2010.
 * @brief      Example module to be used as a template for actual modules.
 *             Threaded periodic version.
 *
 * @see        The GNU Public License (GPL) Version 3
 *
 *****************************************************************************/
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

/**
 * Input object: ExampleSettings
 * Output object: ExampleObject2
 *
 * This module will periodically update the value of the ExampleObject object.
 * The module settings can configure how the ExampleObject is manipulated.
 *
 * The module executes in its own thread in this example.
 *
 * UAVObjects are automatically generated by the UAVObjectGenerator from
 * the object definition XML file.
 *
 * Modules have no API, all communication to other modules is done through UAVObjects.
 * However modules may use the API exposed by shared libraries.
 * See the OpenPilot wiki for more details.
 * http://www.openpilot.org/OpenPilot_Application_Architecture
 *
 */


#include "opencv/cv.h"		// OpenCV library
#include "opencv/highgui.h"	// HighGUI offers video IO and debug output to screen when run on a PC

#include "openpilot.h"
#include "slamsettings.h"	// object holding module settings
#include "hwsettings.h"		// object holding module system configuration

#include "opencvslam.h"		// bridge to C++ part of module

// Private constants
#define STACK_SIZE 16386 // doesn't really mater as long as big enough
#define TASK_PRIORITY (tskIDLE_PRIORITY+1)

// Private variables
static xTaskHandle taskHandle;
static bool slamEnabled;
static SLAMSettingsData settings;

// Private functions
static void slamTask(void *parameters);
static void SettingsUpdatedCb(UAVObjEvent * ev);

/**
 * Initialise the module, called on startup
 * \returns 0 on success or -1 if initialisation failed
 */
int32_t SLAMStart()
{
	if (slamEnabled) {
		// register callback
		SLAMSettingsConnectCallback(SettingsUpdatedCb);

		// Start main task
		xTaskCreate(slamTask, (signed char *)"SLAM", STACK_SIZE, NULL, TASK_PRIORITY, &taskHandle);
	}

	return 0;
}
/**
 * Initialise the module, called on startup
 * \returns 0 on success or -1 if initialisation failed
 */
int32_t SLAMInitialize()
{

	HwSettingsInitialize();
	uint8_t optionalModules[HWSETTINGS_OPTIONALMODULES_NUMELEM];

	HwSettingsOptionalModulesGet(optionalModules);

	if (optionalModules[HWSETTINGS_OPTIONALMODULES_SLAM] == HWSETTINGS_OPTIONALMODULES_ENABLED)
		slamEnabled = true;
	else
		slamEnabled = false;

	return 0;
}
MODULE_INITCALL(SLAMInitialize, SLAMStart)


/**
 * Module thread, should not return.
 */
static void slamTask(void *parameters)
{
	SettingsUpdatedCb(SLAMSettingsHandle());

	// Run C++ class through wrapper
	opencvslam(&settings);
}

static void SettingsUpdatedCb(UAVObjEvent * ev)
{
	// update global variable according to changed settings
	SLAMSettingsGet(&settings);
}
