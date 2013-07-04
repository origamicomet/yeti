// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_SCENE_GRAPH_EMPTY_H_
#define _BUTANE_SCENE_GRAPH_EMPTY_H_

/*! */
class Empty final {
  __foundation_trait(Empty, non_copyable);

  public:
    Empty();

    ~Empty();
};

#endif // _BUTANE_SCENE_GRAPH_EMPTY_H_
