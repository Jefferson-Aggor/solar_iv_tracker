# Solar IV Tracker

## Description

**Solar IV Tracker** is an Arduino-based system designed to monitor the performance of solar panels by measuring their current-voltage (I-V) characteristics. It collects real-time voltage and current data using the INA219 sensor and logs the information to an SD card with timestamps for future analysis.

## Hardware Used
- **Arduino Uno**: The main microcontroller.
- **INA219**: Sensor for measuring voltage and current.
- **RTC (Real-Time Clock)**: Used to timestamp data logs.
- **LCD Display**: Shows live voltage and current values.
- **SD Card Module**: Logs the collected data in a `.txt` file.

## How It Works

1. **Voltage Measurement**: 
   - The INA219 sensor measures both the bus and shunt voltage of the solar panel.
   - The system also measures the voltage using an analog pin (A1) to cross-check the input voltage.
   
2. **Current Measurement**: 
   - The current flowing through the solar panel is calculated by the INA219 sensor.
   
3. **Real-Time Data Display**:
   - The LCD screen shows live readings of the voltage and current.
   
4. **Data Logging**:
   - Data (timestamp, voltage, and current) is logged into a `.txt` file on the SD card for future analysis.
