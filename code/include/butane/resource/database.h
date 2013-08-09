// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_RESOURCE_DATABASE_H_
#define _BUTANE_RESOURCE_DATABASE_H_

class BUTANE_EXPORT Database final {
  __foundation_trait(Database, non_copyable);

  public:
    struct Record {
      Resource::Id id;
      char path[256];
      char source[256];
      size_t num_of_properties;
      Resource::Property properties[32];
      uint64_t compiled;
    };

  private:
    Database(
      size_t size = BUTANE_RESOURCE_DATABASE_HASH_TABLE_INITAL_SIZE );

    ~Database();

  public:
    static Database* create(
      const char* path );

    static Database* open(
      const char* path );

    bool save(
      const char* path );

    void close();

  public:
    bool insert(
      const Resource::Id id,
      const Record& record );

    bool find(
      const Resource::Id id,
      Record& record );

    bool update(
      const Resource::Id id,
      const Record& record );

    bool remove(
      const Resource::Id id );

  public:
    void for_each(
      bool (*callback)(
        void* closure,
        const Resource::Id id,
        const Record& record ),
      void* closure = nullptr );

  private:
    HashTable<Resource::Id, Record> _entries;
};

#endif // _BUTANE_RESOURCE_DATABASE_H_
