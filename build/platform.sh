#!/usr/bin/env bash

case "`$(echo uname -s) | tr '[:upper:]' '[:lower:]'`" in
  mingw*)
    echo "mingw"
    ;;
  cygwin*)
    echo "cygwin"
    ;;
  darwin*)
    echo "macosx"
    ;;
  linux*)
    echo "linux"
    ;;
  *bsd*)
    echo "bsd"
    ;;
  *)
    exit 1
    ;;
esac

exit 0
