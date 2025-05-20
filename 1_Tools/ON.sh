#!/bin/bash

echo ""
echo "List of all TTY devices (avoid 'NOT USB' devices)"
echo ""

ls /dev/tty*

echo ""
read -p "Board to be attached? :" DEVICE


if [ ! -e "$DEVICE" ]; then

  echo "[ERROR] $DEVICE not found!"
  exit 1

fi

sudo setfacl -m u:$USER:rw "$DEVICE"
echo "✅ '$DEVICE' ADDED for $USER "
echo "Now, you can serial it with you actual user!"
exit 0
