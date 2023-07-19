$ESP_DEVICE="COM11" # Adjust this to your actual ESP device
$FIRMWARE_BIN_FILE="../build/firmware.bin" # Path to your firmware binary file
$SERIAL_FILE="serial_number.txt" # File that contains the serial number
$BIN_FILE="serial_number.bin" # Binary file to be created

# Get the current script path
$SCRIPT_PATH = Split-Path -Parent -Path $MyInvocation.MyCommand.Definition

# Create full paths for the serial and binary files
$SERIAL_FILE_FULL = Join-Path -Path $SCRIPT_PATH -ChildPath $SERIAL_FILE
$BIN_FILE_FULL = Join-Path -Path $SCRIPT_PATH -ChildPath $BIN_FILE

# Read the current serial number
$CURRENT_SERIAL = Get-Content -Path $SERIAL_FILE_FULL

# Convert the current serial number to a binary format and write it to the binary file
$BYTES = [System.Text.Encoding]::UTF8.GetBytes($CURRENT_SERIAL.ToString().Trim())

if ($BYTES.Length -gt 32) {
    Write-Host "Error: Byte representation of serial number is too large for eFuse block."
    exit 1
}

[System.IO.File]::WriteAllBytes($BIN_FILE_FULL, $BYTES)

Write-Host "Flashing firmware and updating serial number..."

# Flash the firmware binary
idf.py -p $ESP_DEVICE -b 115200 flash

# Write the serial number to eFuse
# Adjust this line to reflect the correct path to espefuse.py if necessary
# and specify the absolute path to the binary file
espefuse.py burn_block_data --port $ESP_DEVICE --offset 28 BLOCK3 $BIN_FILE_FULL

# Increment the serial number and write it back to the file
$NEW_SERIAL = [int]$CURRENT_SERIAL + 1
$NEW_SERIAL | Out-File -FilePath $SERIAL_FILE_FULL

# Delete the bin file
Remove-Item -Path $BIN_FILE_FULL

Write-Host "Serial number updated and firmware flashed successfully!"

# At the end of your script, add:
Read-Host -Prompt "Press any key to continue . . ."
