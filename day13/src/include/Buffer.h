
#pragma once
#include <string>
#include "Macros.h"

class Buffer {
 public:
  Buffer() = default;
  ~Buffer() = default;

  DISALLOW_COPY_AND_MOVE(Buffer);

  void Append(const char *_str, int _size);
  ssize_t Size();
  const char *ToStr();
  void Clear();
  void Getline();
  void SetBuf(const char *buf);

 private:
  std::string buf_;
};
