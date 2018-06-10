#!/bin/bash

# Clean up after ourselves if something fails.
error() {
  rm -rf _site
  rm -f nginx.conf
}
trap error EXIT

# Switch to root so we can touch all the things!
sudo su

# Strict mode.
set -euo pipefail

SITE=yeti
INSTALL=/var/www/$SITE

# Extract bundle.
echo "    | Extracting bundle"
tar xzfv bundle.tar.gz | sed 's/^/    /' && rm bundle.tar.gz

echo "    | Installing web server configuration"

# Move configuration to correct folder.
mv nginx.conf /etc/nginx/sites-available/$SITE.conf

echo "    | Enabling website if it isn't already"

# Make sure site is enabled just in case it was disabled for some reason.
ln -sf /etc/nginx/sites-available/$SITE.conf /etc/nginx/sites-enabled/$SITE.conf

echo "    | Moving content into $INSTALL"

# Move site to correct folder.
rm -rf $INSTALL
mv _site $INSTALL

# Set appropriate permissions and ownership.
chmod -R 550 $INSTALL
chown -R www:www $INSTALL

echo "    | Restarting web server"

# Restart to reflect any changes to configuration.
systemctl restart nginx

# Report an error if nginx failed to restart
systemctl is-active --quiet nginx || echo "    | Web server failed to restart! Double check your configuration."
