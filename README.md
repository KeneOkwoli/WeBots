# Webots Robot Controller

This repository contains a **C-based controller** designed for use with [Webots](https://cyberbotics.com/), an open-source robot simulator. The controller simulates an autonomous robot exhibiting homeostasis-driven behaviour, including thirst, hunger, and health management.

## ✨ Features

- **Homeostasis Simulation**: The robot monitors thirst, hunger, and health; if any reach critical levels, it "dies" and stops moving.
- **Sensor Integration**: Utilises 9 IR distance sensors for obstacle detection and navigation.
- **Camera Processing**: Detects water sources (blue colour) using three cameras (left, middle, right).
- **Random Movement**: Roams randomly when no target is detected.
- **Targeted Movement**: Moves toward detected water sources when thirsty.
- **Communication Handling**: Listens for incoming "Red" predator alerts, reducing health appropriately.

## 📂 Repository Structure

- `base.c`: Main controller file with all logic for sensors, cameras, homeostasis, movement, and communication.

## 🚀 How It Works

1. **Initialization**  
   Motors, distance sensors, cameras, and a communication receiver are initialised.

2. **Main Loop**  
   - **Homeostasis Monitoring**: Continuously checks health, hunger, and thirst.
   - **Movement**:
     - Free-roams if needs are stable.
     - Seeks blue (water) when thirst drops below a threshold.
   - **Colour Detection**: Processes images to determine which direction to move.
   - **Predator Detection**: If a \"Red\" signal is received, health is reduced.
   - **Drinking Mechanism**: Replenishes thirst upon detecting sufficient blue.

3. **Failure State**  
   If thirst, hunger, or health drop too low, the robot halts and prints a "death" message.


## ⚡ Notes

- The controller expects device names:
  - Motors: `left wheel motor`, `right wheel motor`
  - Cameras: `CameraLeft`, `CameraMiddle`, `CameraRight`
  - IR Sensors: `ds0`, `ds1`, ..., `ds8`
- Ensure device names match exactly, or adjust `base.c` accordingly.
- Camera processing currently uses basic summation; optimising this could improve performance.

## 🌟 Future Improvements

- Optimise camera data processing for faster runtime.
- Expand the communication protocol for more environmental awareness.
- Implement enhanced obstacle avoidance using IR sensor data.
- Modularise the code into separate files for maintainability.
- Implement a more sophisticated decision-making AI (e.g., finite state machine).

## 👤 Author

**Kene Okwoli**  
[GitHub Profile](https://github.com/KeneOkwoli)


