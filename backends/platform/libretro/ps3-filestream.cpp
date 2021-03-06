#include "ps3-filestream.h"
#include "common/config-manager.h"
#include "common/file.h"
#include "engines/engine.h"
#include <cell/cell_fs.h>

PS3FileStream *PS3FileStream::makeFromPath(const Common::String &path, bool writeMode) {
	int mode = writeMode ? (CELL_FS_O_WRONLY | CELL_FS_O_CREAT) : CELL_FS_O_RDONLY;
	int fd=0;

	CellFsErrno err = cellFsOpen(path.c_str(),mode,&fd,NULL,0);
	
	if(err == CELL_FS_SUCCEEDED)
		return new PS3FileStream(fd);

	return 0;
}

PS3FileStream::PS3FileStream(int fd)
{
	_fd=fd;
	_err=0;
	_eos=false;
}

PS3FileStream:: ~PS3FileStream()
{
	flush();
	_err = cellFsClose(_fd);
}

bool PS3FileStream::err() const
{
	return _err != 0;
}

void PS3FileStream::clearErr()
{
	_err = 0;
}

bool PS3FileStream::eos() const
{
	return _eos;
}

uint32 PS3FileStream::write(const void *dataPtr, uint32 dataSize)
{
	uint64_t written=0;
	_err = cellFsWrite(_fd,dataPtr,dataSize,&written);

	return written;
}

bool PS3FileStream::flush()
{
	_err = cellFsFsync(_fd);
	
	return err();
}

int32 PS3FileStream::pos() const
{
	uint64_t l_pos=0;
	cellFsLseek(_fd,0,CELL_FS_SEEK_CUR,&l_pos);
	return l_pos;
}

int32 PS3FileStream::size() const
{
	uint64_t old_pos=0;
	uint64_t l_size=0;
	int l_err;

	l_err = cellFsLseek(_fd,0,CELL_FS_SEEK_CUR,&old_pos);
	l_err = cellFsLseek(_fd,0,CELL_FS_SEEK_END,&l_size);
	l_err = cellFsLseek(_fd,old_pos,CELL_FS_SEEK_SET,&old_pos);

	return l_size;
}

bool PS3FileStream::seek(int32 offs, int whence)
{
	uint64_t l_pos=0;
	_err = cellFsLseek(_fd,offs,whence,&l_pos);
	return err();
}

uint32 PS3FileStream::read(void *dataPtr, uint32 dataSize)
{
	if(dataSize<1)
		return 0;

	uint64_t l_read=0;
	_err = cellFsRead(_fd,dataPtr,dataSize,&l_read);

	if(l_read==0)
		_eos=true;

	return l_read;
}
