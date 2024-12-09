<p align="center">
<img src="https://github.com/user-attachments/assets/9494c07a-1433-44f5-8ea2-fa535fb48f23" width="500">
</p>

<h1 align="center"> 
Circuit Simulator
</h1>

<p align="center">
  <img src="https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white" >
  <img src="https://img.shields.io/badge/Visual_Studio-5C2D91?style=for-the-badge&logo=visual%20studio&logoColor=white" >
  <img src="https://img.shields.io/badge/Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white" >

</p>

<p align="center"> 
  A circuit simulator that provides an intuitive platform for designing, modifying, and solving simple DC circuits. 
</p>

## Features

- ✅ **Interactive Circuit Design:** Create circuit elements like resistors, voltage sources, current sources, wires, and ground on a user-friendly grid interface.

- ✅ **Efficient Value Input Navigation:** Update circuit element values directly with support for metric prefixes (e.g., k, M, µ) for precision.

- ✅ **Seamless Input Navigation:** Quickly switch between circuit elements to modify values.

- ✅ **Multiple Circuits:** Design and solve multiple circuits simultaneously within a single canvas.

- ✅  **Easy Circuit Solving:** Solve the circuit and hover over nodes and elements to view detailed information like voltage and current values.

- ✅ **Error Detection:** Automatically identifies and alerts you to invalid circuit configurations.

- ✅ **Camera Control and Zoom:** Adjust the view and zoom level to easily navigate and manage large circuits.

- ✅ **Undo/Redo Actions:** Effortlessly correct mistakes with undo functionality.

## User Guide

## 1. Creating a Circuit Element  
- **Start Drawing**: Click on any grid intersection to begin creating a circuit element. The element will extend from the clicked intersection to the mouse cursor position.  
- **Cancel Creation**: Right-click or press `Ctrl + Z` to cancel the creation.  
- **Complete Element**: Click on another grid intersection to finalize the circuit element.  
- **Switch Elements**: Press the first character of a circuit element to switch to its drawing mode:
  - `R`: Resistor
  - `V`: Voltage Source
  - `C`: Current Source
  - `W`: Wire
  - `G`: Ground

---

## 2. Modifying Circuit Element Values  
- **Single Element Input**:
  - Left-click on the element to enter input mode.  
  - Type the desired value and press `Enter` (metrix prefixes can be added at the end).
  - Supported metric prefixes include:
    - `p`: $`10^{-12}`$
    - `n`: $`10^{-9}`$
    - `µ`: $`10^{-6}`$ (write `u` instead of µ)
    - `m`: $`10^{-3}`$
    - `k`: $`10^3`$
    - `M`: $`10^6`$
    - `G`: $`10^9`$

- **Multiple Elements Input**:
  - Press `Tab` to navigate to the next element.
  - Press `Shift + Tab` to navigate to the previous element.
  - Input mode exits when:
    - `Enter` is pressed.
    - `Shift + Tab` is pressed on the first element.
    - `Tab` is pressed on the last element.

---

## 3. Solving the Circuit  
- **Solve Command**: Press `Ctrl + Enter` to solve the circuit (multiple circuits can be solved in one canvas).  
- **View Results**:
  - Hover over nodes to see nodal voltages.  
  - Hover over elements to view voltage differences and currents.  
- **Error Handling**: Invalid circuits will display an error message.

---

## 4. Extra Features  
- **Camera Controls**:
  - Move the view: Hold `Right Click` and drag the mouse.  
  - Zoom in/out: Use the mouse wheel or press `Ctrl + =` / `Ctrl + -`.  
- **Undo Actions**: Press `Ctrl + Z` to undo the last action.  

---

## 5. Shortcuts Summary  

| **Action**                   | **Shortcut**           |  
|-------------------------------|------------------------|  
| Draw Resistor                | `R`                   |  
| Draw Voltage Source          | `V`                   |  
| Draw Current Source          | `C`                   |  
| Draw Wire                    | `W`                   |  
| Draw Ground                  | `G`                   |  
| Cancel Element Creation      | `Right Click`         |  
| Enter Input Mode             | `Left Click on Element` |  
| Enter Input Mode for First Element  | `Tab` |  
| Navigate to Next Element     | `Tab`                 |  
| Navigate to Previous Element | `Shift + Tab`         |  
| Solve Circuit                | `Ctrl + Enter`        |  
| Move Camera                  | `Right Click + Drag`  |  
| Zoom In/Out                  | `Mouse Wheel` / `Ctrl + =` / `Ctrl + -` |  
| Undo Last Action             | `Ctrl + Z`            |  

---

## Project Setup

### Steps

**1.	Install Raylib on your system:**

One of the easiest ways to install Raylib is using `vcpkg`
Follow this video for instructions: [here](https://youtu.be/UiZGTIYld1M?si=jz9CSbnuvEvrdDks)

**2.	Clone the  repository:**

<img src="https://github.com/user-attachments/assets/29d99323-fb76-4faf-a792-754b7ee34da0" alt="Example Image" width="400">
<img src="https://github.com/user-attachments/assets/a52fa04c-52dc-4ca4-8096-5a4ddb12b2d8" alt="Example Image" width="400">

**3. You’re done!:**

Run the  project via  `Ctrl + F5`.

<img src="https://github.com/user-attachments/assets/5b9cef7b-7ec0-4cc5-b673-1fb0f2caa122" alt="Example Image" width="400">
