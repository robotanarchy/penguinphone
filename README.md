![penguinphone-n900](https://cloud.githubusercontent.com/assets/7833187/13034752/d69e3ff8-d33d-11e5-9b85-42511ccc71ce.jpg)

Wallpaper by [Dablim](http://www.deviantart.com/art/Simple-GNU-Linux-Wallpaper-336558602) ([CC-BY](http://creativecommons.org/licenses/by/3.0/)) - **[Screenshots](https://github.com/robotanarchy/penguinphone/issues/1)**

# penguinphone

### Motivation
Dissatisfaction with the current world of smartphone software. This project is slowly heading towards a full GNU/Linux userland for smartphones with the following objectives.
* Compatibility with normal Linux programs. Firefox for example *should just work* out of the box.
* Minimalism: Minimal dependencies, minimal lines of code. More Lines of Code always mean more bugs!
* Fast Prototyping: Get a working system fast. Use more bloated components and replace them later, if necessary.
* Forward Compatibility: X11 is slowly dying, make everything Wayland compatible
* Tested Code: Try to use tested code, where possible. If feasible, write testsuites for componentes, that we build on.
* Modularity
* Lots of optional configuration options for the user
* Contributing to upstream OSS projects
* Security: Provide an as-secure-as-possible system. Enjoy these buzzwords: Full disk encryption, support existing state-of-the-art cryptographic communication software, reproducible builds, [libreboot](https://libreboot.org/) running (this should only take a few hundred years), warning the user that phone calls and SMS are fully unencrypted etc. (Please note, that no security related software has been written yet for penguinphone.)

### Status
Right now, i3 is somewhat extended to be usable on a smartphone. The code is tested, highly efficient and we have a nice environment for further development. See the [screenshots](https://github.com/robotanarchy/penguinphone/issues/1) for details.

### Requirements
* [X.org](x.org) (to be replaced with [Wayland](https://wayland.freedesktop.org/))
* [i3](http://i3wm.org/) >= 4.11 (to be replaced with [Sway](https://github.com/SirCmpwn/sway) once it is compatible enough)
* [Font Awesome](http://fontawesome.io/) (...because it was packaged in Debian, Arch and Void and the Unicode codes can be looked up easily)
* [Elementary](https://www.enlightenment.org/about-efl) (Toolkit from the Enlightenment Desktop)


### Test Environment
Unlike most modern smartphone operating systems, you don't need to run a fully bloated virtual machine for development.
Just install all the dependencies from above on your desktop Linux, and [Xephyr](https://en.wikipedia.org/wiki/Xephyr) (which is packaged on most distributions) to run another X server inside your regular X server. Then clone and compile:
```shell
git clone https://github.com/robotanarchy/penguinphone
cd penguinphone
make testenv # compiles everything and runs Xephyr
```

For serious development, installing [valgrind](http://valgrind.org/) is necessary. Inside the running penguinphone environment, you can run the testsuite with `make testsuite`.


### Real Hardware Installation
* Install an up-to-date, **real** Linux distribution (not you, Android! Check the *Requirements* from above) on your smartphone. For the N900, that would be [debian900](https://github.com/dderby/debian900) (If you get it working with others, let me know!). The installation is not that easy, you can **brick your device** and what not, so you should probably run the *Test Environment* (see above) first.

* Install the dependencies. For debian900:
```shell
# enable backports (see http://backports.debian.org/ for more info)
# this is required, because we need the latest i3 version
echo 'deb http://http.debian.net/debian jessie-backports main' \
  > /etc/apt/sources.list.d/jessie-backports.list
apt-get update

apt-get -t jessie-backports install git i3-wm fonts-font-awesome \
  xinit libelementary-dev libelementary-bin terminology feh

# recommended programs (optional; psmisc contains killall)
apt-get install tmux htop xinput-calibrator psmisc ntp unclutter scrot
```

* Clone the source code and compile it on the device (takes a few seconds only!)
```
su user
cd ~
git clone https://github.com/robotanarchy/penguinphone
cd penguinphone
make
```

* Copy the `config/xinitrc` to `~/.xinitrc` and customize it (keyboard layout for example)
* Start X11: `startx`
* If your touchscreen is inverted or otherwise not working, run `xinput-calibrator` and copy the output to your `xorg.conf`
* Run `elementary_config` (when the X server is started with penguinphone, click on the rocket on the top right and choose Elementary Config). Increase the finger size and scale to what you like.
