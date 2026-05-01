# Orbit Med

A private, offline medication tracker for Windows. Track medications, dosages, and schedules — everything stays on your machine.

## Features

- Medication and dosage management — add, edit, and remove medications
- Reminder and alert scheduling — set times and intervals for each medication
- Dose history and adherence log — see what was taken and when
- Refill tracking — get alerted before you run out
- Simple, distraction-free interface
- Fully offline — no account, no internet, no cloud sync

## Download

Grab the latest installer from [Releases](https://github.com/apikeys131/Orbit-Med/releases).

Run `OrbitMed-Setup-v1.0.0.exe` and follow the installer. Windows may show a SmartScreen warning — click **More info → Run anyway**.

## Requirements

- Windows 10 or 11 (64-bit)

## Building

```bash
# 1. Clone the repository
git clone https://github.com/apikeys131/Orbit-Med.git
cd Orbit-Med

# 2. Configure
cmake -B build -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Release

# 3. Build
cmake --build build --config Release --parallel

# 4. Run
.\build\Release\OrbitMed.exe
```

## Architecture

```
src/
├── main.cpp                Entry point
├── ui/
│   ├── MainWindow.h/.cpp   Main window and layout
│   ├── MedList.h/.cpp      Medication list panel
│   ├── DoseLog.h/.cpp      Dose history and adherence view
│   └── Reminders.h/.cpp    Alert scheduling UI
├── data/
│   ├── Database.h/.cpp     Local SQLite storage
│   └── Medication.h        Medication and dose data structures
└── utils/
    ├── Scheduler.h/.cpp    Background reminder timer
    └── Notifications.h     Windows toast notification wrapper
```

## Source

Source is available for review. See the repository for build instructions.

## License

Source-available. Free to use. Do not redistribute modified binaries without permission.
