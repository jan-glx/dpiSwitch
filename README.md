# dpiSwitcher
[![GitHub license](https://img.shields.io/github/license/jan-glx/dpiswitch.svg)](LICENSE)
[![AppVeyor](https://img.shields.io/appveyor/ci/jan-glx/dpiswitch.svg)](https://ci.appveyor.com/project/jan-glx/dpiswitch)
[![Github All Releases](https://img.shields.io/github/downloads/jan-glx/dpiswitch/total.svg)](https://github.com/jan-glx/dpiSwitch/releases/latest)

dpiSwitcher is a minimalistic tool with no user interface. It automatically adjusts the DPI scaling of the *internal dispaly* whenever an external display is connected or disconnected to the last value set for the given number of dispalys. These settings are stored in the Windows registry (HKCU/software/dpiSwitchr).

## Usage:
1. Place [`dpiSwitcher.exe`](https://github.com/jan-glx/dpiSwitch/releases/latest/download/dpiSwitcher.exe) in the `shell:startup` folder to have it launch automatically upon login. Alternatively, you can start it manually (e.g., with a double-click) for a single session. If you want to terminate the tool, use the task manager.

2. Whenever the scaling of the internal monitor is not to your liking, adjust it as usual (Windows settings -> Display settings -> Scale). <br>
  => dpiSwitcher will be notified of this change and store the current dpi scaling setting for the current number of displays in the Windows registry. And whenever you connect or disconnect an external display leading to this number of monitors in the future, dpiSwitcher will load these settings from the registry and apply them.

## Roadmap:
- [x] Implemented functionality to remember specific DPI scaling settings for different situations.
- [x] Fixed issue with multiple external displays.

## Attribution
This project includes and is entirely based on the work by Sahil Singh found at https://github.com/lihas/windows-DPI-scaling-sample.
An alternative command-line version is available at: https://github.com/imniko/SetDPI.
