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

/*! \file core/system/syscalls.h
 *  \date June 2014
 */

#pragma once

#include <arch.h>
#include <horizon/types.h>
#include <horizon/shm.h>
#include <horizon/msg.h>
#include <horizon/svc.h>
#include <horizon/io.h>
#include <stddef.h>
#include <stdint.h>

//! FIXME: Some syscall return values may inferfere with error codes.
/*! Have a return register and an error register to avoid conflicts? */

void syscall_spawn();
void syscall_launch(pid_t pid, uintptr_t entry);
void syscall_dispatch(uintptr_t entry, uintptr_t stack);
void syscall_release(tid_t tid);
void syscall_kill(pid_t pid);

void syscall_yield();
void syscall_wait(ipcport_t sender);

void syscall_vmap(uintptr_t dest, size_t size);
void syscall_pmap(uintptr_t dest, uintptr_t src, size_t size);
void syscall_unmap(uintptr_t addr, size_t size);

void syscall_grant(struct shm* info, uintptr_t dest);
void syscall_share(struct shm* info);
void syscall_accept(shmid_t sid, uintptr_t dest, size_t size);

void syscall_send(struct msg* src);
void syscall_recv(struct msg* dest);
void syscall_peek();
void syscall_drop(struct msg* info);

void syscall_svcown(svc_t svc);
void syscall_svcid(svc_t svc);

void syscall_sysio(sysop_t action, size_t arg, uintptr_t data);

SYSCALL_TABLE = 
{
	{ syscall_spawn,    0 },
	{ syscall_launch,   2 },
	{ syscall_dispatch, 2 },
	{ syscall_release,  1 },
	{ syscall_kill,     1 },

	{ syscall_yield,    0 },
	{ syscall_wait,     1 },

	{ syscall_vmap,     2 },
	{ syscall_pmap,     3 },
	{ syscall_unmap,    2 },

	{ syscall_grant,    2 },
	{ syscall_share,    1 },
	{ syscall_accept,   3 },

	{ syscall_send,     1 },
	{ syscall_recv,     1 },
	{ syscall_peek,     0 },
	{ syscall_drop,     1 },

	{ syscall_svcown,   1 },
	{ syscall_svcid,    1 },

	{ syscall_sysio,    3 },

	0
};
