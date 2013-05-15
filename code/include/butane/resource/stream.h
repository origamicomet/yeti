// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_RESOURCE_STREAM_H_
#define _BUTANE_RESOURCE_STREAM_H_

class Resource::Stream final {
  __foundation_trait(Stream, non_copyable);

  public:
    Stream(
      const Resource::Type& type,
      const Resource::Id id );
  
  private:
    void* _memory_resident_data;
    size_t _memory_resident_data_len;
    FILE* _streaming_data;
};

#endif // _BUTANE_RESOURCE_STREAM_H_
