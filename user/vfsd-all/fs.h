#pragma once

enum VFS_FTYPE
{
	VFS_FILE,
	VFS_DIR,
	VFS_VIRTDIR,
	VFS_MOUNTPT
};

enum VFS_REQUEST
{
	VFS_MOUNT,
	VFS_UNMOUNT,

	VFS_OPEN,
	VFS_DOPEN,
	VFS_CLOSE,

	VFS_READ,
	VFS_WRITE
};

enum VFS_RESPONSE
{
	VFS_FSINDEX,
	VFS_FSFIND,

	VFS_FSREAD,
	VFS_FSWRITE
};
