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

/*! \file arch/i586/sw/int/irq.h
 *  \date June 2014
 */

#pragma once

#include <sw/int/isr.h>
#include <stdint.h>

#define IRQ_MAX 128

typedef int8_t irq_t;

void irq_set_isr(irq_t irq, isr_t isr);

irq_t irq_from_isr(isr_t isr);
isr_t irq_to_isr(irq_t irq);
