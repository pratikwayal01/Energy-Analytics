import time
import serial
import csv

arduinoData = serial.Serial('com11', 115200)
time.sleep(1)

# Open a CSV file for writing
with open('output_data.csv', 'w', newline='') as csvfile:
    fieldnames = ['Voltage', 'Current']
    writer = csv.DictWriter(csvfile, fieldnames=fieldnames)

    # Write the header to the CSV file
    writer.writeheader()

    try:
        while True:
            while arduinoData.inWaiting() == 0:
                pass
            dataPackage = arduinoData.readline()
            dataPackage = str(dataPackage, 'utf-8')
            dataPackage = dataPackage.strip('\r\n')

            # Split the data into voltage and current values
            voltage, current = map(float, dataPackage.split())

            # Print the data to the console
            print(f"Voltage: {voltage}V   Current: {current}A")

            # Write the data to the CSV file
            writer.writerow({'Voltage': voltage, 'Current': current})

    except KeyboardInterrupt:
        # Close the serial connection and CSV file when KeyboardInterrupt (Ctrl+C) is received
        arduinoData.close()