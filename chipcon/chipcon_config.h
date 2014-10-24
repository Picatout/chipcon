/*
* Copyright 2014, Jacques Deschênes
* This file is part of CHIPcon.
*
*     CHIPcon is free software: you can redistribute it and/or modify
*     it under the terms of the GNU General Public License as published by
*     the Free Software Foundation, either version 3 of the License, or
*     (at your option) any later version.
*
*     CHIPcon is distributed in the hope that it will be useful,
*     but WITHOUT ANY WARRANTY; without even the implied warranty of
*     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*     GNU General Public License for more details.
*
*     You should have received a copy of the GNU General Public License
*     along with CHIPcon.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 * chipcon_config.h
 *
 * Created: 2014-10-14 13:17:40
 *  Author: Jacques Deschênes
 *  
 * Ce fichier doit-être inclus au début de hardware.h
 */ 


#ifndef CHIPCON_CONFIG_H_
#define CHIPCON_CONFIG_H_

//définir SDC_SUPPORT à 1
//pour une console avec
//carte SD
//sinon mettre à 0
#define SDC_SUPPORT 1

// pour déboguage firware seulement
#define FW_DEBUG  0

#endif /* CHIPCON_CONFIG_H_ */