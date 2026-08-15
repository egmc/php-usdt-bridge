PHP_ARG_ENABLE([usdt_bridge],
  [whether to enable usdt_bridge support],
  [AS_HELP_STRING([--enable-usdt-bridge],
    [Enable usdt_bridge support])],
  [no])

if test "$PHP_USDT_BRIDGE" != "no"; then
  AC_CHECK_HEADERS([sys/sdt.h], [], [
    AC_MSG_ERROR([sys/sdt.h not found. Install the systemtap-sdt-devel (RHEL/CentOS/Fedora) or systemtap-sdt-dev (Debian/Ubuntu) package.])
  ])

  PHP_NEW_EXTENSION(usdt_bridge, usdt_bridge.c, $ext_shared)
fi
