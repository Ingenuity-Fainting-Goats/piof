PHP_ARG_ENABLE(piof, whether to enable my extension,
[ --enable-piof  Enable piof])

#if test "$PHP_PIOF" = "yes"; then
#  AC_DEFINE(HAVE_PIOF, 1, [Whether you have piof])
#  PHP_NEW_EXTENSION(piof, piof.c hooks/system/hook.c hooks/md5/hook.c piof_globals.c, $ext_shared)
#  CFLAGS="$CFLAGS -ldl"
#fi
if test "$PHP_PIOF" = "yes"; then
  AC_DEFINE(HAVE_PIOF, 1, [Whether you have piof])
  PHP_NEW_EXTENSION(piof, piof.c piof_globals.c, $ext_shared)
  CFLAGS="$CFLAGS -ldl -Wl,-rpath,\$(srcdir)/hooks_build"
fi
