*lead* provides hot-corners for your desktop, notably for openbox, awesome, i3 and other window managers.

This fork adds support for specifying an interval to wait before the action is triggered. There may be more improvements in the future.

![Screenshot of lead](https://github.com/mlde/lead/blob/master/screenshot.png)
> Despite the screenshot, the sensors are only 1px in size and invisible.


### Features
- hot corners: sensors in the corners of the screen
- hot sides: sensors on center/middle edge of the screen
- freely configurable action for each sensor
- each screen is individually configurable


# Get it

- clone the repository with `$ git clone https://github.com/mlde/lead`

**or**

- download as zip and extract it


# Build it

*lead* uses [clang](http://clang.llvm.org/) as compiler, [g++](https://gcc.gnu.org/) as linker and [make](https://www.gnu.org/software/make/) as build-tool. 

To build *lead* open a terminal in the root folder of the repository and:

    $ make

### Dependencies
- Qt5Widgets 
- Qt5Gui 
- Qt5Core


# Install it

To install *lead* open a terminal in the root folder of the repository and:

    $ sudo make install


# Configure it

On the first run lead will look into these dirs for a conf-file. The first one found will be used.

    ~/.config/lead/lead.conf
    /etc/xdg/lead/lead.conf

If none of these exists, it will create `~/.config/lead/lead.conf` with default values for each screen, ie:

    [eDP1]
    bottom-action=
    bottom-interval=0
    bottomLeft-action=
    bottomLeft-interval=0
    bottomRight-action=
    bottomRight-interval=0
    left-action=
    left-interval=0
    right-action=
    right-interval=0
    top-action=
    top-interval=0
    topLeft-action=
    topLeft-interval=0
    topRight-action=
    topRight-interval=0

To enable a action for a sensor, simply add a command to the corner or side:

    bottomLeft-action=chromium

The intervals are set in milliseconds. For example, to set chromium to open only after the mouse has been in the corner for 1 second, do:

    bottomLeft-interval=1000

The configuration file is monitored and changes are applied automatically.


# Use it

*lead* provides the binary `lead` which creates sensors for each configured corner or side.

Start *lead* as background process:

    $ lead &

# Uninstall it

To uninstall *lead* open a terminal in the root folder of the repository and:

    $ sudo make uninstall

# Drawbacks

I didnt want to poll the mouse, so i created sensors which works with events. The sensors are transparent, but need a running compositor like compton for that. Without a compositor, they are black.

# TODO

Improvements that need to be made:

- Error checking of the conf file; e.g. check that an integer was set for the interval

Possible new features:

- Custom regions
