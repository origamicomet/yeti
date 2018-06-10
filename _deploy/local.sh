#!/bin/bash

#

# Strict mode.
set -euo pipefail

WEBSITE="yeti.origamicomet.com"
REMOTE="ssh.origamicomet.com"

# You'll need to create a deploy user and add keys.
# useradd -s /usr/sbin/nologin -r -M -U -d /home/deploy deploy
# usermod -aG sudo deploy

# Determine top-level directory.
ROOT=$(cd "${0%/*}/.."; pwd)

# Temporary file for bundling.
BUNDLE="$ROOT/_deploy/bundle.tar"

echo "[1/5] Building..."

pushd $ROOT
JEKYLL_ENV=production make build > /dev/null
popd

echo "[2/5] Bundling site and configuration..."

# Bundle site the configuration then compress it.
tar cf "$BUNDLE" -C "$ROOT/_site" . --transform='s,.,_site,'
tar rf "$BUNDLE" -C "$ROOT/_deploy" nginx.conf
gzip "$BUNDLE"

echo "[3/5] Copying bundle to $REMOTE..."

# Copy the bundle over.
scp -q "$BUNDLE.gz" "deploy@$REMOTE:~/bundle.tar.gz"

echo "[4/5] Running setup script on remote..."

# Run setup script to move everything where it needs to be and restart the
# web server to reflect any changes to configuration.
ssh "deploy@$REMOTE" 'bash -s' < "$ROOT/_deploy/remote.sh"

echo "[5/5] Emptying cache..."

if [ -z "${CFAPI_ZONE:-}" ]  [ -z "${CFAPI_KEY:-}" ] || [ -z "${CFAPI_EMAIL:-}" ] ; then
  echo "    | CFAPI_ZONE, CFAPI_KEY, and CFAPI_EMAIL must be defined to purge caches!"
else
  # Get a list of all files so we can purge 'em from caches.
  FILES="$( (cd "$ROOT/_site" && find . -type f) | sed "s/^.\//"https:\/\/$WEBSITE\//; s/$/"/" | xargs -d'\n' | sed 's/[[:space:]]/, /g')"

  # Evict everything from Cloudflare's caches.
  curl -X DELETE "https://api.cloudflare.com/client/v4/zones/$CFAPI_ZONE/purge_cache" \
       -H "X-Auth-Email: $CFAPI_EMAIL" \
       -H "X-Auth-Key: $CFAPI_KEY" \
       -H "Content-Type: application/json" \
       --data "{\"files\":[$FILES]}"
fi

echo "Done!"
