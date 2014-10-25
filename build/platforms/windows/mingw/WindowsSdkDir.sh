#!/usr/bin/env bash

if [ -z "$WindowsSdkDir" ]; then
  keys=("HKEY_CURRENT_USER\SOFTWARE\Wow6432Node\Microsoft\Microsoft SDKs\Windows"
  "HKEY_CURRENT_USER\SOFTWARE\Microsoft\Microsoft SDKs\Windows"
  "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Microsoft SDKs\Windows"
  "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Microsoft SDKs\Windows")
  for key in "${keys[@]}"; do
    value=$(REG QUERY "$key" -v CurrentInstallFolder 2>&1)
    if [ $? -eq 0 ]; then
      echo $(echo "$(echo $(echo "$value" | cut -s -d $'\n' --complement -f 2) | cut -s -d $' ' --complement -f 1-2)" | sed 's/\\$//')
      exit 0
    fi
  done
  exit 1
else
  echo "$WindowsSdkDir"
  exit 0
fi
