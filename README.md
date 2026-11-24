# 🔐 Smart Safe — ATmega32 Password-Protected Safe

A digital safe lock built on the **ATmega32** microcontroller in bare-metal C. It uses a 4x4 keypad for input, a 16x2 LCD for feedback, and the internal EEPROM to keep the password and failed-attempt counter even after power loss.

> Simulated in **Proteus** and tested with a working demo.

---

## 📺 Demo

https://github.com/user-attachments/assets/92c03d15-3b47-4c76-9dd2-cf6e0522f77a

---

## ✨ Features

- **First-time setup**: on the very first power-up, the user is asked to set a 4-digit password.
- **Persistent storage**: the password is saved in the internal EEPROM and survives power cycles.
- **Safe write protection**: a validity flag (`0xA5`) is written *last*, so an interrupted write is never mistaken for a valid password.
- **Masked input**: each digit is shown briefly, then replaced with `*`.
- **Backspace**: press `C` to delete the last entered digit.
- **Brute-force protection**: after **3 wrong attempts**, the safe locks for **30 seconds** with a live countdown on the LCD.
- **Power-cycle proof**: the failed-attempt counter is stored in EEPROM, so turning the device off and on does not reset the lockout.
- **Change password**: after opening the safe, press `C` within 2 seconds to set a new password.

---

## 🧰 Hardware

| Component | Details |
|-----------|---------|
| Microcontroller | ATmega32 @ 8 MHz |
| Display | LCD 16x2 (4-bit mode) |
| Input | 4x4 Keypad |
| Storage | Internal EEPROM |

### Pin Connections

| Peripheral | Port / Pins |
|------------|-------------|
| LCD data (D4–D7) | PORTA high nibble (PA4–PA7) |
| LCD RS | PA1 |
| LCD E | PA2 |
| Keypad rows | PORTC high nibble (PC4–PC7) |
| Keypad columns | PORTC low nibble (PC0–PC3) |

> Pins and ports can be changed from `lcd_Config.h` and `KPD_Config.h`.

---

## 🗂️ Project Structure

The project follows a **layered architecture**:

```
Smart_Safe/
├── Smart_Safe.c          # Application layer (main logic)
├── Proteus/              # Proteus simulation project
├── docs/                 # Screenshots / schematic
├── HAL/
│   ├── LCD/              # lcd.c, lcd.h, lcd_Config.h
│   └── KPD/              # KPD.c, KPD.h, KPD_Config.h
├── MCAL/
│   ├── DIO/              # DIO.c, DIO.h, DIO_REG.h
│   └── EEPROM/           # EEP.c, EEP.h, EEP_REG.h
└── SERV/
    ├── STD_TYPES.h       # Standard data types, F_CPU
    └── STD_Macros.h      # Bit manipulation macros
```

| Layer | Responsibility |
|-------|----------------|
| **APP** | Password logic, lockout, user flow |
| **HAL** | LCD and keypad drivers |
| **MCAL** | Direct register access (DIO, EEPROM) |
| **SERV** | Types and macros shared across layers |

---

## 🗃️ EEPROM Memory Map

| Address | Purpose |
|---------|---------|
| `0x00` | Valid-password flag (`0xA5` = password is set) |
| `0x01 – 0x04` | Stored password (4 bytes) |
| `0x10` | Failed attempts counter |

---

## ⚙️ How It Works

```
Power ON
   │
   ├── Flag != 0xA5? ──► Set new password ──► Save to EEPROM
   │
   ├── Failed tries >= 3? ──► 30 s lockout
   │
   └── Loop:
         Enter password
            ├── Correct ──► "Safe Opened" (press C to reset password), tries = 0
            └── Wrong   ──► tries++ (saved to EEPROM)
                              └── tries == 3 ──► 30 s lockout
```

---

## 🚀 Getting Started

### Requirements
- Atmel Studio (Microchip Studio)
- Proteus (for simulation)

### Build & Run
1. Open the project in Atmel Studio and build it (`F7`) to generate the `.hex` file.
2. Open the Proteus design from the `Proteus/` folder.
3. Double-click the ATmega32, load the generated `.hex` as the program file, and set the clock to **8 MHz**.
4. Run the simulation.

### Usage
| Key | Action |
|-----|--------|
| `0–9` | Enter password digits |
| `C` | Delete last digit / reset password when the safe is open |

---

## 🔧 Configuration

Edit these in `Smart_Safe.c`:

```c
#define PASS_LEN      4    // Password length
#define MAX_TRIES     3    // Allowed wrong attempts
#define LOCK_SEC      30   // Lockout duration (seconds)
```

---

## 🔮 Future Improvements

- Servo/relay to drive a real lock mechanism
- Buzzer alarm on lockout
- Admin master password
- Hash the stored password instead of saving it as plain text

---
