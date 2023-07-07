# dpiSwitcher
[![GitHub license](https://img.shields.io/github/license/jan-glx/dpiswitch.svg)](LICENSE)
[![AppVeyor](https://img.shields.io/appveyor/ci/jan-glx/dpiswitch.svg)](https://ci.appveyor.com/project/jan-glx/dpiswitch)
[![Github All Releases](https://img.shields.io/github/downloads/jan-glx/dpiswitch/total.svg)](https://github.com/jan-glx/dpiSwitch/releases/latest)

dpiSwitcher is a minimalistic tool with no user interface. It automatically adjusts the DPI scaling of the *internal dispaly* whenever an external display is connected or disconnected to the last value set for the given number of dispalys. These settings are stored in the Windows registry (HKCU/software/dpiSwitchr).

## Usage:
Place [`dpiSwitcher.exe`](https://github.com/jan-glx/dpiSwitch/releases/latest/download/dpiSwitcher.exe) in the `shell:startup` folder to have it launch automatically upon login. Alternatively, you can start it manually (e.g., with a double-click) for a single session. If you want to terminate the tool, you will need to use the task manager.
Then, whenever the scaling of the internal monitor is not as you want, adjust it using the usual method (Right-click on desktop -> Display settings -> Scale). dpiSwitcher will be notified of this change and store your current dpi scaling setting for the current number of displays in te windows registry. Whenever you connect or disconnect an external display, dpiSwitcher loads the setting for the new number of dispalys from the registry and apply themn.

## Roadmap:
- [x] Implemented functionality to remember specific DPI scaling settings for different situations.
- [x] Fixed issue with multiple external displays.

## Attribution
This project includes and is entirely based on the work by Sahil Singh found at https://github.com/lihas/windows-DPI-scaling-sample.
An alternative command-line version is available at: https://github.com/imniko/SetDPI.
