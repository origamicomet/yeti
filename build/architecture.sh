#!/usr/bin/env bash

case "`$(echo uname -m) | tr '[:upper:]' '[:lower:]'`" in
  i*86)
    echo "x86"
    ;;
  x86_64)
    echo "x86-64"
    ;;
  arm*)
    echo "arm"
    ;;
  *)
    exit 1
    ;;
esac

exit 0
