// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_RESOURCE_DATABASE_H_
#define _BUTANE_RESOURCE_DATABASE_H_

class Database final {
  __foundation_trait(Database, non_copyable);

  public:
    struct Record {
    };

  public:
    Database();

  private:
    HashTable<Resource::Id, Record> _entries;
};

#endif // _BUTANE_RESOURCE_DATABASE_H_
