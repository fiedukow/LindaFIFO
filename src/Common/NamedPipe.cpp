#include "NamedPipe.h"
#include <cassert>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstdio>
#include <memory>
#include <iostream>

NamedPipe::NamedPipe(const std::string& pipeName)
  : name_(pipeName),
    descriptor_(-1)
{}

bool NamedPipe::open(PipeMode mode)
{
  descriptor_ = ::open(name_.c_str(), mode);
  return (descriptor_ > 0);
}

void NamedPipe::close()
{
  int toClose = descriptor_;
  descriptor_ = -1;
  ::close(toClose);
}

struct flock NamedPipe::buildFileLock(int type)
{
  struct flock fl;

  fl.l_type   = type;     
  fl.l_whence = SEEK_SET; 
  fl.l_start  = 0;        
  fl.l_len    = 0;        
  fl.l_pid    = getpid(); 

  return fl;
}

void NamedPipe::readLock()
{
  assert(getFileDescriptor() > 0);
  struct flock fl = buildFileLock(F_RDLCK);

  fcntl(getFileDescriptor(), F_SETLKW, &fl); 
}

void NamedPipe::readUnlock()
{
  assert(getFileDescriptor() > 0);
  struct flock fl = buildFileLock(F_UNLCK);

  fcntl(getFileDescriptor(), F_SETLKW, &fl);
}

pid_t NamedPipe::getReadLockingPid() const
{
  int chkDescriptor = ::open(name_.c_str(), READ_NONBLOCKING);
  if(chkDescriptor <= 0)
    return -1;

  struct flock fl = buildFileLock(F_WRLCK);
  fl.l_pid = -1;
  
  fcntl(chkDescriptor, F_GETLK, &fl);

  ::close(chkDescriptor);
  return fl.l_pid;
}

int NamedPipe::getFileDescriptor() const
{
  return descriptor_;
}

OwnedNamedPipe::OwnedNamedPipe(const std::string& pipeName)
  : NamedPipe(pipeName)
{
  mkfifo(name_.c_str(), 0666);
}

OwnedNamedPipe::~OwnedNamedPipe() noexcept
{
  unlink(name_.c_str());
}

NamedPipeReader::NamedPipeReader(NamedPipePtr pipe)
  : pipe_(pipe)
{}

NamedPipeWriter::NamedPipeWriter(NamedPipePtr pipe)
  : pipe_(pipe)
{}

bool NamedPipeReader::open()
{
  pipe_->open(READ_BLOCKING);
}

bool NamedPipeWriter::open()
{
  pipe_->open(WRITE_BLOCKING);
}

bool NamedPipeReader::tryOpen()
{
  pipe_->open(READ_NONBLOCKING);
}

bool NamedPipeWriter::tryOpen()
{
  pipe_->open(WRITE_NONBLOCKING);
}

void NamedPipeReader::close()
{
  pipe_->close();
}

void NamedPipeWriter::close()
{
  pipe_->close();
}

std::string NamedPipeReader::read()
{
  pipe_->readLock();
  ::read(pipe_->getFileDescriptor(), buffer, BUFSIZ);
  pipe_->readUnlock();
  return std::string(buffer);
}

std::string NamedPipeReader::waitRead()
{
  return read();
}

void NamedPipeWriter::write(const std::string& data)
{
  assert(data.size() < BUFSIZ);
  ::write(pipe_->getFileDescriptor(), data.c_str(), BUFSIZ);
}

