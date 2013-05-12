#ifndef _NAMED_PIPE_H_
#define _NAMED_PIPE_H_

#include <string>
#include <memory>
#include <fcntl.h>

enum PipeMode { READ_BLOCKING        = O_RDONLY,
                WRITE_BLOCKING       = O_WRONLY,
                READ_NONBLOCKING     = O_RDONLY | O_NDELAY,
                WRITE_NONBLOCKING    = O_WRONLY | O_NDELAY
              };

class NamedPipe
{
public:
  NamedPipe(const std::string& pipeName); //throws when unable to create
  virtual ~NamedPipe() = default; //tries to unlink...

  bool open(PipeMode);
  bool tryOpen(PipeMode);
  void close();
  
  void readLock();
  void readUnlock();
  pid_t getReadLockingPid() const;

  int getFileDescriptor() const;

public:
  static struct flock buildFileLock(int type);

protected:
  const std::string name_;

private:
  int descriptor_;
  
};
typedef std::shared_ptr<NamedPipe> NamedPipePtr;

class OwnedNamedPipe : public NamedPipe
{
public:
  OwnedNamedPipe(const std::string& pipeName);
  virtual ~OwnedNamedPipe() noexcept;
};

class NamedPipeReader
{
public:
  NamedPipeReader(const NamedPipePtr pipe);
  bool open();
  bool tryOpen();
  void close();
  std::string read();
  std::string waitRead();

private:
  const NamedPipePtr pipe_;
  char buffer[BUFSIZ];
};

class NamedPipeWriter
{
public:
  NamedPipeWriter(const NamedPipePtr pipe);
  bool open();
  bool tryOpen();
  void close();
  void write(const std::string& data);

private:
  const NamedPipePtr pipe_;
};

#endif
