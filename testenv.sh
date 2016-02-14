#!/bin/bash
# X/Xephyr/i3 will be replaced with wayland/sway later

WIDTH="800"
HEIGHT="480"
TESTENV_DISPLAY=":1337"
PROG="Xephyr"
ARGS="-br -noreset -screen ${WIDTH}x${HEIGHT} ${TESTENV_DISPLAY}"
PIDFILE=".xephyr.pid"
USERLAND_DEPENDENCIES="i3 feh"


# Validate display server binary
BIN="$(which $PROG 2> /dev/null)"
if [ ! -e "$BIN" ]; then
	echo "ERROR: $PROG not found!"
	exit 1
fi

# Validate userland
for dep in ${USERLAND_DEPENDENCIES}; do
	if ! command -pv "$dep" > /dev/null; then
		echo "ERROR: $dep not found!"
		echo "Full list of userland dependencies:"
		echo "${USERLAND_DEPENENCIES}"
		exit 1
	fi
done


# Validate PID
PID=$(cat $PIDFILE 2>/dev/null)
if ! readlink -q /proc/$PID/exe | grep -q "^$BIN"; then
	PID=
	rm $PIDFILE 2> /dev/null
fi


case "$1" in
	start)
		echo "Starting Test Environment ($PROG)"
		$BIN $ARGS &
		echo $! > $PIDFILE
		sleep 1
		$0 status &> /dev/null || exit 1
		$0 start-userland
		;;
	start-userland)
		HOST_KEYMAP="$(setxkbmap -query | grep layout | cut -d':' -f2)"
		DISPLAY=${TESTENV_DISPLAY}
		setxkbmap ${HOST_KEYMAP}
		source configs/xinitrc
		$0 stop
		;;
	stop)
		echo "Stopping Test Environment ($PROG)"
		[[ ! -z $PID ]] && kill $PID &> /dev/null
		;;
	status)
		[[ -z $PID ]] && echo "Test Environment ($PROG): stopped" && exit 1
		echo "Test Environment ($PROG): started"
		;;
	restart)
		$0 stop
		$0 start
		;;
	*)
		echo "Syntax: $0 {start|stop|status|restart}"
esac
