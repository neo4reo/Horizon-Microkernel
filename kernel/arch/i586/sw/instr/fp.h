/* Horizon Microkernel - Hobby Operating System
 * Copyright (C) 2014 Wyatt Lindquist
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*! \file arch/i586/sw/instr/fp.h
 *  \date June 2014
 */

#pragma once

#include <stdint.h>

//! Load default values into FPU.
static inline void fninit()
{
	__asm ("fninit" ::: "memory");
}

//! Store FPU status word.
//! \param stsw	The address to store the status word at.
static inline void fnstsw(uint16_t* stsw)
{
	__asm ("fnstsw (%0)" :: "p" (stsw));
}

//! Load FPU control word.
//! \param cw	The control word to load.
static inline void fldcw(uint16_t cw)
{
	__asm ("fldcw %0" :: "m" (cw) : "memory");
}