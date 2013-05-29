// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_RESOURCE_DATABASE_H_
#define _BUTANE_RESOURCE_DATABASE_H_

class BUTANE_EXPORT Database final {
  __foundation_trait(Database, non_copyable);

  public:
    struct Record {
      time_t last_modified;
    };

  private:
    typedef HashTable<Resource::Id, Record>::Pair Entry;

  private:
    Database(
      size_t size = BUTANE_RESOURCE_DATABASE_HASH_TABLE_INITAL_SIZE );

    ~Database();

  public:
    static Database* open(
      const char* path );

    bool save(
      const char* path );

    void close();

  public:
    bool find(
      const Resource::Id id,
      Record& record );

    bool update(
      const Resource::Id id,
      const Record& record );

  private:
    HashTable<Resource::Id, Record> _entries;
};

#endif // _BUTANE_RESOURCE_DATABASE_H_
