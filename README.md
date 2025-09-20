# ⚙️ STM32 Stepper Motor Speed Control

This project demonstrates **stepper motor control** using an **STM32C0 microcontroller**.  
The motor’s **speed is controlled by a potentiometer (ADC input)**, while **two buttons** are used to control the **direction** (forward/reverse).  
Smooth acceleration and deceleration are implemented for stable motor motion.  

---

## 📷 Project Overview
- **Button 1 (PB4)** → Move in one direction (e.g., clockwise).  
- **Button 2 (PB5)** → Move in the opposite direction (e.g., counterclockwise).  
- **Potentiometer (PA0)** → Adjusts motor speed.  
- **Acceleration/Deceleration** → Prevents sudden speed changes for smooth motion.  

---

## 🛠️ Hardware Setup
- **STM32C0 MCU** (e.g., STM32C031C6)  
- **Stepper Motor Driver Module** (connected to `STP` and `DIR` pins)  
- **Stepper Motor** (e.g., NEMA 17)  
- **Potentiometer** (for speed control)  
- **Push Buttons** (for direction control)  
- **Connections**:  
  - **STP (Step Pin)** → PB3  
  - **DIR (Direction Pin)** → PB10  
  - **Button 1** → PB4 (with pull-up)  
  - **Button 2** → PB5 (with pull-up)  
  - **Potentiometer** → PA0 (ADC input)  

---

## ⚡ Working
1. **Button 1 pressed** → Motor rotates in one direction.  
2. **Button 2 pressed** → Motor rotates in the opposite direction.  
3. **No button pressed** → Motor gradually decelerates and stops.  
4. **Potentiometer** adjusts motor speed:  
   - Low ADC value → Faster stepping.  
   - High ADC value → Slower stepping.  
5. **Acceleration control** ensures smooth speed changes.  

---

## 📌 Notes
- Motor driver must be powered separately (depending on motor specs).  
- Delay-based step generation → can be optimized with **TIM peripherals (PWM)** for higher precision.  
- Project is a good introduction to **GPIO, ADC, and motor control with STM32**.  
