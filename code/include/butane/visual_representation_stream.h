// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_VISUAL_REPRESENTATION_STREAM_H_
#define _BUTANE_VISUAL_REPRESENTATION_STREAM_H_

#include <butane/butane.h>
#include <butane/visual_representation.h>

namespace butane {
  class BUTANE_EXPORT VisualRepresentationStream final {
    __foundation_trait(VisualRepresentationStream, non_copyable);

    public:
      class Request {
        public:
          enum Type {
            CREATE  = 1,
            UPDATE  = 2,
            DESTROY = 3
          };

        protected:
          Request(
            const Request::Type type
          ) : type(type)
          {}

        public:
          const Request::Type type;
      };

      class Requests final {
        __foundation_trait(Requests, non_constructable);
        __foundation_trait(Requests, non_copyable);

        public:
          class Create final
            : public Request
          {
            public:
              Create()
                : Request(Request::CREATE)
              {}

            public:
              VisualRepresentation::Type vrt;
              VisualRepresentation::Id* vrid;
          };

          class Update final
            : public Request
          {
            public:
              Update()
                : Request(Request::UPDATE)
              {}

            public:
              VisualRepresentation::Id vrid;
          };

          class Destroy final
            : public Request
          {
            public:
              Destroy()
                : Request(Request::DESTROY)
              {}

            public:
              VisualRepresentation::Id vrid;
          };
      };

    public:
      VisualRepresentationStream();

      ~VisualRepresentationStream();

    public:
      /*! */
      void create(
        const VisualRepresentation::Type type,
        VisualRepresentation::Id* id );

      /*! */
      VisualRepresentation* update(
        const VisualRepresentation::Id id );

      /*! */
      void destroy(
        const VisualRepresentation::Id id );

      /*! */
      // VisualRepresentation& create_and_or_update(
      //   const VisualRepresentation::Type type,
      //   VisualRepresentation::Id* id );

      /*! */
      void for_each_request(
        void (*callback)(
          void* closure,
          const Request* request ),
        void* closure ) const;

    private:
      void request(
        const void* request,
        const size_t request_len );

    private:
      size_t _stream_offs;
      Array<uint8_t> _stream;
  };
} // butane

#endif // _BUTANE_VISUAL_REPRESENTATION_STREAM_H_
