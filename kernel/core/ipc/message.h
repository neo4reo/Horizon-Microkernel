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

/*! \file core/ipc/message.h
 *  \date September 2014
 */

#pragma once

#include <multitask/thread.h>
#include <horizon/types.h>
#include <horizon/msg.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct message message_t;
struct message
{
	ipcport_t from;
	msgdata_t code, arg;

	struct __packed
	{
		// Point to the next message in queue.
		/* 1 extra bit for storing null index as MESSAGE_MAX. */
		uint32_t next         : 11;
		// The payload information is stored in the thread.
		uint32_t payload_flag : 1;
		// FIXME: Something useful for reserved.
		uint32_t _reserved    : 20;
	};
};

// FIXME: Too many parameters need to be passed...
void message_add(thread_uid_t uid, ipcport_t from, struct msg* info, bool head);
bool message_remove(thread_uid_t uid, struct msg* dest);
bool message_peek(thread_uid_t, ipcport_t* from);

bool message_find(thread_uid_t, ipcport_t search);
