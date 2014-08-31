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
#include <multitask/process.h>
#include <multitask/scheduler.h>
#include <horizon/ipc.h>
#include <horizon/errno.h>

void syscall_kill(pid_t pid)
{
	if (pid == IDST_KERNEL || pid == IDST_ANY)
		return syscall_return_set(-e_badparam);

	thread_t* caller = thread_get(scheduler_curr());
	if (pid == 0) //< FIXME: Macro.
		pid = caller->owner;

	process_t* target = process_get(pid);
	if (!target)
		return syscall_return_set(-e_notavail);
	if (target->priv > process_get(caller->owner)->priv)
		return syscall_return_set(-e_badpriv);

	if (pid == caller->owner)
		scheduler_lock();

	process_kill(pid);

	if (!scheduler_curr())
		scheduler_unlock();
	else
		syscall_return_set(-e_success);
}