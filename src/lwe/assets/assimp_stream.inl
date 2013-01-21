// =============================================================================
// This file is part of LWE. See readme.md for details.
//
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// =============================================================================

#include <assimp/IOSystem.hpp>
#include <assimp/IOStream.hpp>

class IOStream :
  public Assimp::IOStream
{
  public:
    IOStream();
    ~IOStream();

  public:
    size_t Read( void* out_buffer, size_t size, size_t count );
    size_t Write( const void* buffer, size_t size, size_t count );
    aiReturn Seek( size_t offset, aiOrigin origin );
    size_t Tell() const;
    size_t FileSize() const;
    void Flush();

  public:
    FILE* fh;
};

IOStream::IOStream() {
}

IOStream::~IOStream() {
  fclose(fh);
}

size_t IOStream::Read( void* out_buffer, size_t size, size_t count ) {
  return fread(out_buffer, size, count, fh);
}

size_t IOStream::Write( const void* buffer, size_t size, size_t count ) {
  return fwrite(buffer, size, count, fh);
}

aiReturn IOStream::Seek( size_t offset, aiOrigin origin ) {

  switch (origin) {
    case aiOrigin_SET: fseek(fh, offset, SEEK_SET); break;
    case aiOrigin_CUR: fseek(fh, offset, SEEK_CUR); break;
    case aiOrigin_END: fseek(fh, offset, SEEK_END); break;
    default: return aiReturn_FAILURE; break;
  }

  return aiReturn_SUCCESS;
}

size_t IOStream::Tell() const {
  return ftell(fh);
}

size_t IOStream::FileSize() const {
  const long orig = ftell(fh);
  fseek(fh, 0, SEEK_END);
  const size_t len = ftell(fh);
  fseek(fh, orig, SEEK_SET);
  return len;
}

void IOStream::Flush() {
  lwe_fail("IOStream::Flush is not implemented.");
}

class IOSystem
  : public Assimp::IOSystem
{
  public:
    IOSystem();
    ~IOSystem();

  public:
    bool Exists( const char* path ) const;
    char getOsSeparator() const;
    Assimp::IOStream* Open( const char* path, const char* mode );
    void Close( Assimp::IOStream* stream );

  public:
    lwe_asset_compile_data_t* acd;
};

IOSystem::IOSystem() {
}

IOSystem::~IOSystem() {
}

bool IOSystem::Exists( const char* path ) const {
  return (strcmp(acd->path, path) == 0);
}

char IOSystem::getOsSeparator() const {
  return '/';
}

Assimp::IOStream* IOSystem::Open( const char* path, const char* mode ) {
  if (strcmp(acd->path, path) != 0)
    return NULL;

  IOStream* stream = new IOStream();
  stream->fh = fopen(path, mode);
  return stream;
}

void IOSystem::Close(
  Assimp::IOStream* stream )
{
  fclose(((IOStream*)stream)->fh);
  delete stream;
}