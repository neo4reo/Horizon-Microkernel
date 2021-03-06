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

/*! \file spec/multiboot/mmap.h
 *  \date June 2014
 */

#pragma once

#include <spec/multiboot.h>
#include <stddef.h>

size_t multiboot_mem_size_get(const multiboot_info_t* mbi);

const multiboot_mmap_t* multiboot_mmap_get(const multiboot_info_t* mbi, const multiboot_mmap_t* prev);
