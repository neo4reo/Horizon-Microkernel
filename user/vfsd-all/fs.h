#pragma once

enum VFS_REQUEST
{
	VFS_MOUNT,

	VFS_OPEN,
	VFS_DIRECT,

	VFS_READ,
	VFS_WRITE
};

enum VFS_COMMAND
{
	VFS_FSFIND,

	VFS_FSREAD,
	VFS_FSWRITE
};
