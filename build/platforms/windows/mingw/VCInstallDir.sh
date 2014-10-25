#!/usr/bin/env bash

if [ -z "$VCInstallDir" ]; then
  keys=("HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\VisualStudio\12.0\Setup\VC"
  "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\VisualStudio\12.0\Setup\VC"
  "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\VCExpress\12.0\Setup\VC"
  "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\VCExpress\12.0\Setup\VC"
  "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\VisualStudio\11.0\Setup\VC"
  "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\VisualStudio\11.0\Setup\VC"
  "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\VCExpress\11.0\Setup\VC"
  "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\VCExpress\11.0\Setup\VC"
  "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\VisualStudio\10.0\Setup\VC"
  "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\VisualStudio\10.0\Setup\VC"
  "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\VCExpress\10.0\Setup\VC"
  "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\VCExpress\10.0\Setup\VC"
  "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\VisualStudio\9.0\Setup\VC"
  "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\VisualStudio\9.0\Setup\VC"
  "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\VCExpress\9.0\Setup\VC"
  "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\VCExpress\9.0\Setup\VC"
  "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\VisualStudio\8.0\Setup\VC"
  "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\VisualStudio\8.0\Setup\VC"
  "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\VCExpress\8.0\Setup\VC"
  "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\VCExpress\8.0\Setup\VC"
  "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\VisualStudio\7.1\Setup\VC"
  "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\VisualStudio\7.1\Setup\VC"
  "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\VCExpress\7.1\Setup\VC"
  "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\VCExpress\7.1\Setup\VC"
  "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\VisualStudio\7.0\Setup\VC"
  "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\VisualStudio\7.0\Setup\VC"
  "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\VCExpress\7.0\Setup\VC"
  "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\VCExpress\7.0\Setup\VC"
  "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\VisualStudio\6.0\Setup\VC"
  "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\VisualStudio\6.0\Setup\VC"
  "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\VCExpress\6.0\Setup\VC"
  "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\VCExpress\6.0\Setup\VC")
  for key in "${keys[@]}"; do
    value=$(REG QUERY "$key" -v ProductDir 2>&1)
    if [ $? -eq 0 ]; then
      echo $(echo "$(echo $(echo "$value" | cut -s -d $'\n' --complement -f 2) | cut -s -d $' ' --complement -f 1-2)" | sed 's/\\$//')
      exit 0
    fi
  done
  exit 1
else
  echo "$VCInstallDir"
  exit 0
fi
