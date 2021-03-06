#include <horizon/ipc.h>
#include <sys/sched.h>
#include <sys/svc.h>
#include <sys/msg.h>
#include <sys/proc.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <stdbool.h>
#include <string.h>
#include <malloc.h>

#include "../util-i586/msg.h"
#include "../vfsd-all/fs.h"

typedef struct elf_file_header elf_file_header_t;
struct __packed elf_file_header
{
	uint8_t fid[4];

	uint16_t type;
	uint16_t arch;
	uint32_t version;

	uint32_t entry;
	uint32_t pheader_offset;
	uint32_t sheader_offset;
	uint32_t flags;

	uint16_t size;
	uint16_t pheader_size;
	uint16_t pheader_count;
	uint16_t sheader_size;
	uint16_t sheader_count;
	uint16_t sheader_strtbl;
};

typedef struct elf_program_header elf_program_header_t;
struct __packed elf_program_header
{
	uint32_t type;
	uint32_t offset;
	uint32_t vaddr, paddr;

	uint32_t file_size;
	uint32_t mem_size;

	uint32_t flags;
	uint32_t align;
};

ipcport_t filesystem;
int screen, keyboard;

static inline uintptr_t addr_align(uintptr_t addr, size_t bound)
{
	return (addr & ~(bound-1));
}

static inline uintptr_t addr_align_next(uintptr_t addr, size_t bound)
{
	return addr_align(addr + bound-1, bound);
}

int open(const char* path)
{
	struct msg req;
	msg_create(&req, filesystem, VFS_OPEN);

	msg_attach_payload(&req, (void*)path, strlen(path)+1);

	send(&req);
	wait(req.to);

	struct msg res;
	recv(&res);

	return res.code;
}

int read(int fd, char* buffer, size_t size, size_t off)
{
	struct msg req;
	msg_create(&req, filesystem, VFS_READ);

	msg_set_args(&req, 3, fd, size, off);

	send(&req);
	wait(req.to);

	struct msg res;
	msg_attach_payload(&res, buffer, size);

	recv(&res);
	return res.code;
}

int write(int fd, char* buffer, size_t size, size_t off)
{
	struct msg req;
	msg_create(&req, filesystem, VFS_WRITE);

	msg_set_args(&req, 3, fd, size, off);
	msg_attach_payload(&req, buffer, size);

	send(&req);
	wait(req.to);

	struct msg res;
	recv(&res);

	return res.code;
}

void print(char* msg)
{
	write(screen, msg, strlen(msg)+1, 0);
}

int main()
{
	while ((filesystem = svcid(SVC_VFS)) == 0);

	while ((screen = open("/dev/tty")) == -1);
	print("Shell connected to TTY.\n");

	while ((keyboard = open("/dev/kbd")) == -1);
	print("Shell connected to keyboard.\n");

	char input_buffer[256];
	size_t input_pos = 0;

	while (true)
	{
		memset(input_buffer, 0, 256);
		input_pos = 0;

		print("$> ");

		char last_key = '\0';
		while (last_key != '\n')
		{
			while (read(keyboard, &last_key, 1, 0) < 1);

			char show_key = last_key;
			if (show_key < 32)
			{
				if (show_key == '\b' && input_pos)
				{
					input_pos -= 1;
					print("\b");
				}

				show_key = '\0';
			}

			if (input_pos >= 255)
				continue;

			if (show_key != '\0')
			{
				input_buffer[input_pos++] = show_key;

				char key_str[] = { show_key, '\0' };
				print(key_str);
			}
		}

		input_buffer[input_pos] = '\0';
		char* cmd = input_buffer;

		char* arg = strchr(input_buffer, ' ');
		if (arg)
		{
			*arg = '\0';
			arg += 1;
		}

		print("\n");
		if (strcmp(cmd, "read") == 0)
		{
			if (arg)
			{
				int fd = open(arg);
				if (fd != -1)
				{
					char* buffer = malloc(2048);
					memset(buffer, 0, 2048);
					print("reading...\n");

					size_t res = read(fd, buffer, 2048, 0);
					if (res == -1)
						print("Error.");
					else if (!res)
						print("Nothing to read.");
					else
						print(buffer);

					free(buffer);
				}
				else
				{
					print("File not found.");
				}
			}
		}
		else if (strcmp(cmd, "load") == 0)
		{
			//
		}
		else if (strcmp(cmd, "clear") == 0)
		{
			print("\e[2J");
			continue;
		}
		else
		{
			print("Unknown command '");
			print(cmd);
			print("'.");
		}

		print("\n");
	}

	for (;;);
	return 0;
}
