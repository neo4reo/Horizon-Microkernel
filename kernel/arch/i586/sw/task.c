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

#include "task.h"
#include <sw/gdt.h>
#include <memory.h>

// FIXME: Must be changed for SMP.
static tss_t kernel_task;
extern uintptr_t __kernel_stack;

//! Initialize any specific task info.
void task_init(task_info_t* info)
{
	// Nothing to do for this platform.
}

//! Cold start a task from kernel mode.
/*! Remember to change address spaces first. */
void task_start(task_info_t* info)
{
	tss_init(&kernel_task, __kernel_stack); //< FIXME: set up a new stack somewhere?
	tss_load(&kernel_task);

	_ASM
	(
		"movw  0x20|0x3, %%ds; " //< Set the user data segment for each reg.
		"movw  0x20|0x3, %%es; "
		"movw  0x20|0x3, %%fs; "
		"movw  0x20|0x3, %%gs; "
		"pushl 0x20|0x3;       " //< Also place on stack as user_esp.
		"pushl %0;             " //< Push the specified stack pointer.
		"pushf;                "
		"popl  %%eax;          " //< Pop EFLAGS into EAX.
		"or    $0x200,   %%eax;" //< Enable interrupts.
		"pushl %%eax;          " //< Set the new EFLAGS.
		"pushl 0x18|0x3;       " //< Set the user code segment.
		"pushl %1;             " //< Push the new EIP.
		"iret;                 "
		:: "r" (info->stack),
		   "r" (info->entry)
		:  "memory"
	);
}

//! Switch to another task during an interrupt.
/*! The address space only needs to be changed prior if switching to new process. */
void task_switch(task_info_t* curr, task_info_t* next, int_frame_t* frame)
{
	// Save the old state.
	memcpy(&(curr->frame), frame, sizeof(int_frame_t));

	if (!(next->frame.EIP)) //< This is a new task.
	{
		// Set up the starting values.
		frame->EIP = next->entry;
		frame->user_esp = next->stack;
	}
	else
	{
		// Copy in the new state.
		memcpy(frame, &(next->frame), sizeof(int_frame_t));
	}
}

//! Initialize a TSS structure.
/*! Called somewhere internally in arch code. */
void tss_init(tss_t* task, uintptr_t kstack)
{
	memset(task, 0, sizeof(tss_t));

	// CS and DS are binary OR-ed with ring 3 access.
	task->CS = GDT_SEGMENT_OFFSET(GDT_SEGMENT_KCODE) | 0x3;
	task->DS = GDT_SEGMENT_OFFSET(GDT_SEGMENT_KDATA) | 0x3;
	task->SS = task->ES = task->FS = task->GS = task->DS;

	// Do not give the stack segment ring 3 access.
	task->SS0  = GDT_SEGMENT_OFFSET(GDT_SEGMENT_KDATA);
	task->ESP0 = kstack;
}

//! Load a TSS structure.
/*! Only happens once per CPU core. */
void tss_load(const tss_t* task)
{
	uintptr_t base  = (uintptr_t)task;
	uintptr_t limit = sizeof(tss_t) - 1;

	gdt_t* curr_gdt = kernel_gdt(); //< FIXME: Change for SMP.
	gdt_entry_set(curr_gdt, GDT_SEGMENT_TSS, base, limit, GDT_ENTRYTYPE_TSS, 0);
	gdt_load(curr_gdt);

	_ASM
	(
		"movw 0x28|0x3, %%ax;"
		"ltr  %%ax;          "
	    ::: "memory"
	);
}
