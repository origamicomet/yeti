// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_RESOURCE_DATABASE_H_
#define _BUTANE_RESOURCE_DATABASE_H_

class BUTANE_EXPORT Database final {
  __foundation_trait(Database, non_copyable);

  public:
    class Record {
      public:
        struct Serialized {
          Resource::Id id;
          char path[256];
          // char properties[];
          time_t compiled;
        };

      public:
        Record()
          : path(Allocators::heap())
          , properties(Allocators::heap())
          , compiled(0)
        {}

        ~Record()
        {}

        Record(
          const Record& r
        ) : path(r.path)
          , properties(r.properties)
          , compiled(r.compiled)
        {}

        Record& operator= (
          const Record& r )
        {
          if (&r == this)
            return *this;
          path = r.path;
          properties = r.properties;
          compiled = r.compiled;
          return *this;
        }

      public:
        String path;
        Array<String> properties;
        time_t compiled;
    };

  private:
    typedef HashTable<Resource::Id, Record>::Pair Entry;

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
