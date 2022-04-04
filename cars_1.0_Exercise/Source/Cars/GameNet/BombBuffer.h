#pragma once

#include "Net/buffer.h"
#include "Math/Vector.h"
#include "Math/Transform.h"

class CBombBuffer : public Net::CBuffer
{
  public:
    CBombBuffer(size_t initsize = 500, size_t delta = 100);
    ~CBombBuffer();

    using Net::CBuffer::read;
    using Net::CBuffer::write;

    void write(const FVector &_data);
    void read(FVector &_data);
};
