/**
 ******************************************************************************
 * @addtogroup OpenPilotModules OpenPilot Modules
 * @{
 * @addtogroup State Estimation
 * @brief Acquires sensor data and computes state estimate
 * @{
 *
 * @file       filterstationary.c
 * @author     The OpenPilot Team, http://www.openpilot.org Copyright (C) 2013.
 * @brief      Provides "fake" stationary state data for indoor mode
 *
 * @see        The GNU Public License (GPL) Version 3
 *
 ******************************************************************************/
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

#include "inc/stateestimation.h"

// Private constants

// Private types

// Private variables

// Private functions

static int32_t init(void);
static int32_t filter(stateEstimation *state);


void filterStationaryInitialize(stateFilter *handle)
{
    handle->init   = &init;
    handle->filter = &filter;
}

static int32_t init(void)
{
    return 0;
}

static int32_t filter(stateEstimation *state)
{
    state->pos[0]   = 0.0f;
    state->pos[1]   = 0.0f;
    state->pos[2]   = 0.0f;
    state->updated |= pos_UPDATED;

    state->vel[0]   = 0.0f;
    state->vel[1]   = 0.0f;
    state->vel[2]   = 0.0f;
    state->updated |= vel_UPDATED;

    return 0;
}

/**
 * @}
 * @}
 */