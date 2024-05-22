AC_PREREQ([2.69])
AC_INIT([casamonitor], [1.0], [justin.popescu1605@stud.acs.upb.ro])
AM_INIT_AUTOMAKE([foreign subdir-objects])
AC_PROG_CC
AC_PROG_CXX

# Check for avr-gcc and avr-g++
AC_CHECK_TOOL([AVR_CC], [avr-gcc])
AC_CHECK_TOOL([AVR_CXX], [avr-g++])

# Set environment variables for AVR
AC_SUBST([CC], [$AVR_CC])
AC_SUBST([CXX], [$AVR_CXX])
AC_SUBST([CFLAGS], ['-mmcu=atmega328p -Os'])
AC_SUBST([CXXFLAGS], ['-mmcu=atmega328p -Os'])
AC_SUBST([LDFLAGS], ['-mmcu=atmega328p'])

AC_CONFIG_FILES([Makefile include/Makefile src/Makefile])
AC_OUTPUT
