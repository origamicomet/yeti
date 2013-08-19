// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_RESOURCE_DATABASE_H_
#define _BUTANE_RESOURCE_DATABASE_H_

class BUTANE_EXPORT Database final {
  __foundation_trait(Database, non_copyable);

  public:
    class Record final {
      __foundation_trait(Record, non_assignable);

      private:
        friend class Database;

      public:
        class Variation final {
          __foundation_trait(Variation, non_assignable);

          private:
            friend class Database;

          public:
            Variation();
            Variation( const Variation& variation );
            ~Variation();

          public:
            const char* source() const;
            void set_source( const char* source );

            Array<Resource::Property>& properties();
            const Array<Resource::Property>& properties() const;

            uint64_t compiled_at() const;
            void set_compiled_at( const uint64_t at );

          private:
            String _source;
            Array<Resource::Property> _properties;
            uint64_t _compiled_at;
        };

      public:
        Record();
        Record( const Record& resource );
        ~Record();

      public:
        const char* path() const;
        void set_path( const char* path );

      public:
        const Variation* find_variation_by_id(
          const Resource::Variation id ) const;

        Variation* find_or_add_variation_by_id(
          const Resource::Variation id );

        bool remove_variation_by_id(
          const Resource::Variation id );

        void for_each_variation(
          bool (*callback)(
            void* closure,
            const Resource::Variation id,
            const Variation* variation ),
          void* closure = nullptr ) const;

      private:
        String _path;
        typedef Pair<butane::Resource::Variation, Variation> _Variation;
        Array<_Variation> _variations;
    };

  private:
    Database(
      size_t size = BUTANE_RESOURCE_DATABASE_HASH_TABLE_INITAL_SIZE );

    ~Database();

  public:
    static Database* create(
      const char* path );

    static Database* load(
      const char* path );

    static Database* create_or_load(
      const char* path );

    bool save(
      const char* path ) const;

    void close();

  public:
    Record* add(
      const Resource::Id id );

    Record* find(
      const Resource::Id id );

    Record* find_or_add(
      const Resource::Id id );

    void remove(
      const Resource::Id id );

  public:
    void for_each(
      bool (*callback)(
        void* closure,
        const Resource::Id id,
        const Resource::Database::Record* record ),
      void* closure = nullptr ) const;

  private:
    HashTable<Resource::Id, Record> _records;
};

#endif // _BUTANE_RESOURCE_DATABASE_H_
