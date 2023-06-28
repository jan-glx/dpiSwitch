# dpiSwitch & dpiSwitcher
[![GitHub license](https://img.shields.io/github/license/jan-glx/dpiswitch.svg)](LICENSE)
[![AppVeyor](https://img.shields.io/appveyor/ci/jan-glx/dpiswitch.svg)](https://ci.appveyor.com/project/jan-glx/dpiswitch)
[![Github All Releases](https://img.shields.io/github/downloads/jan-glx/dpiswitch/total.svg)](https://github.com/jan-glx/dpiSwitch/releases/latest)

Website: [jan-glx.github.io/dpiSwitch](http://jan-glx.github.io/dpiSwitch)

Two tiny tools to turn toggling the internal displays DPI settings every time you are stuck without an external display into an easy tasks.
## dpiSwitch
A small tool to manually  on Windows.

### Usage:

- set dpi scaling:  ````dpiSwitch.exe 1````     

## dpiSwitcher
A small tool with no user interface that will *automatically* set dpi scaling to one value when an external display is plugged in and to another when the last external dispaly is removed. 
````dpiSwitch.exe 200 150````     

### Usage:
Put ````dpiSwitcher.exe```` into the folder ````shell:startup```` to run it automatically at login or start it manually with a double click once for a session. Ending it is possible via the task manager only.

## Attribution
This includes / is completely based on work by Sahil Singh https://github.com/lihas/windows-DPI-scaling-sample
There is an anlternative command line version: https://github.com/imniko/SetDPI
