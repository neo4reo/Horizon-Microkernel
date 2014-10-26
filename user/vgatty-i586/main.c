// main.c

#include <horizon/ipc.h>
#include <sys/proc.h>
#include <sys/sched.h>
#include <sys/mman.h>
#include <sys/svc.h>
#include <sys/msg.h>
#include <stdint.h>
#include <memory.h>

char device_name[] = "tty";

uint16_t* const video_mem = (uint16_t*)0xA0000000;
size_t cursor_x = 0, cursor_y = 0;

void putc(char c)
{
	switch (c)
	{
		case '\b':
		{
			if (cursor_x)
				cursor_x -= 1;

			c = ' ';
			break;
		}
		case '\t':
		{
			size_t new_x = (cursor_x + 4) & ~(4-1);
			cursor_x = (new_x < 80) ? new_x : (++cursor_y, 0);

			break;
		}
		case '\r':
		{
			cursor_x = 0;
			break;
		}
		case '\n':
		{
			cursor_x = 0;
			cursor_y += 1;
			break;
		}
		default:
			break;
	}

	if (c >= ' ')
	{
		video_mem[cursor_y * 80 + cursor_x] = (c | (0x7 << 8));
		cursor_x += 1;
	}

	if (cursor_x >= 80)
	{
		cursor_x = 0;
		cursor_y += 1;
	}

	if (cursor_y >= 25)
	{
		for (size_t i = 0; i != 80*24; ++i)
			video_mem[i] = video_mem[i + 80];

		for (size_t i = 80*24; i != 80*25; ++i)
			video_mem[i] = (' ' | (0x7 << 8));

		cursor_y -= 1;
	}
}

void puts(const char* s)
{
	while (*s != '\0')
		putc(*s++);
}

int main()
{
	if (pmap(video_mem, 0xB8000, 4096) == NULL)
		return 1;

	memset(video_mem, 0, 80*25*sizeof(uint16_t));
	puts("[tty] Initialized VGA driver.\n");

	ipcport_t devmgr;
	while ((devmgr = svcid(SVC_DEVMGR)) == 0);

	puts("[tty] Registering with device manager... ");

	struct msg request = {{0}};
	request.to = devmgr;
	request.code = 1;
	request.payload.buf = device_name;
	request.payload.size = 4;

	send(&request);
	wait(devmgr);

	struct msg response;
	recv(&response);
	if (response.code != 0)
		return 1;

	puts("OK!\n");

	return 0;
}
