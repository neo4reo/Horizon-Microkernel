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

#include <system/syscalls.h>
#include <arch.h>
#include <ipc/service.h>
#include <horizon/svc.h>
#include <horizon/errno.h>

//! Instead of returning an error code, svcid will return 0.
/*! 0 can be considered ENOTAVAIL in all cases. */
void syscall_svcid(svc_t svc)
{
	if (svc >= SVCMAX)
		return syscall_return_set(0);

	ipcport_t port = service_get(svc);
	syscall_return_set(port);
}
