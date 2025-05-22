#!/bin/bash

echo ""
echo "List of all TTY devices (avoid 'NOT USB' devices)"
echo ""

ls /dev/tty*

echo ""
read -p "Board to be dettached? :" DEVICE

if [ ! -e "$DEVICE" ]; then

  echo "[ERROR] $DEVICE not found!"
  exit 1

fi

sudo setfacl -x u:$USER "$DEVICE"

echo "✅ '$DEVICE' REMOVED for $USER !"
exit 0
