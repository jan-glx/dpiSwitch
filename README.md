# dpiSwitcher
[![GitHub license](https://img.shields.io/github/license/jan-glx/dpiswitch.svg)](LICENSE)
[![AppVeyor](https://img.shields.io/appveyor/ci/jan-glx/dpiswitch.svg)](https://ci.appveyor.com/project/jan-glx/dpiswitch)
[![Github All Releases](https://img.shields.io/github/downloads/jan-glx/dpiswitch/total.svg)](https://github.com/jan-glx/dpiSwitch/releases/latest)

dpiSwitcher is a minimalistic tool with no user interface. It *automatically* sets the DPI scaling of the internal monitor to 250% whenever an external display is connected, and back to 175% when the external display is disconnected. 
To run the tool, execute `dpiSwitch.exe`.

## Usage:
Place `dpiSwitcher.exe` in the `shell:startup` folder to launch it automatically upon login. Alternatively, you can start it manually with a double-click for a single session. To terminate the tool, use the task manager.

## Roadmap:
- [ ] Implement functionality to remember specific DPI scaling settings for different situations, instead of using hardcoded values.

## Attribution
This project includes and is entirely based on the work by Sahil Singh found at https://github.com/lihas/windows-DPI-scaling-sample.
An alternative command-line version is available at: https://github.com/imniko/SetDPI.
