#!/bin/bash

ESP_DEVICE="COM13" # Adjust this to your actual ESP device
FIRMWARE_BIN_FILE="..\build\ecocomfort_essential.bin" # Path to your firmware binary file
VERSION_FILE="version.txt" # File that contains the version number

# Read the current version number
FIRMWARE_VERSION=$(cat "$VERSION_FILE")

# Increment the version number and write it back to the file
FIRMWARE_VERSION=$((FIRMWARE_VERSION + 1))
echo "$FIRMWARE_VERSION" > "$VERSION_FILE"

echo "Flashing firmware version $FIRMWARE_VERSION..."

# Flash the firmware binary
python -m esptool --chip esp32c3 --port "$ESP_DEVICE" --baud 115200 --before default_reset --after hard_reset write_flash --flash_mode dio --flash_size detect --flash_freq 40m 0x1000 ../build/bootloader/bootloader.bin 0x8000 ../build/partition_table/partition-table.bin 0x10000 "$FIRMWARE_BIN_FILE"

echo "Writing version number to flash..."

# Create a binary file that contains the version number
#echo -n -e $(printf "\\x%02x" $FIRMWARE_VERSION) > version.bin

# Write the version number to flash
#python -m esptool --chip esp32c3 --port "$ESP_DEVICE" --baud 115200 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 40m --flash_size detect 0x2000 version.bin
