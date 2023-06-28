# dpiSwitcher
[![GitHub license](https://img.shields.io/github/license/jan-glx/dpiswitch.svg)](LICENSE)
[![AppVeyor](https://img.shields.io/appveyor/ci/jan-glx/dpiswitch.svg)](https://ci.appveyor.com/project/jan-glx/dpiswitch)
[![Github All Releases](https://img.shields.io/github/downloads/jan-glx/dpiswitch/total.svg)](https://github.com/jan-glx/dpiSwitch/releases/latest)

dpiSwitcher is a minimalistic tool with no user interface. It *automatically* sets the DPI scaling of the internal monitor whenever an external display is connected (/disconnected) to the setting used the last time that monitor was disconnected (/connected).
Settings are rembered in the windows registry (HKCU/software/dpiSwitchr) .


## Usage:
Place  [`dpiSwitcher.exe`](/releases/latest/download/dpiSwitcher.exe) in the `shell:startup` folder to launch it automatically upon login. Alternatively, you can start it manually (e.g. with a double-click) for a single session. In case you want to terminate the tool, use the task manager.
Then whenever scaling of the internal monitor is not how you want it change it by the usual means (Right click on desktop -> Display settings - > Scale). If you now connect/disconnect an external dispaly, dpiSwitcher will store the settings in the registry andrestore them the next time there is a connect/disconnect leading to this situation.


## Roadmap:
- [x] Implement functionality to remember specific DPI scaling settings for different situations, instead of using hardcoded values.
- [ ] Fix issue with multiple external dispalys 

## Attribution
This project includes and is entirely based on the work by Sahil Singh found at https://github.com/lihas/windows-DPI-scaling-sample.
An alternative command-line version is available at: https://github.com/imniko/SetDPI.
