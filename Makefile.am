AUTOMAKE_OPTIONS = foreign subdir-objects

# Define the subdirectories
SUBDIRS = include src

# Include directories
AM_CPPFLAGS = -I$(srcdir)/include

# Define the program to be built
bin_PROGRAMS = casamonitor

# Define the source files
casamonitor_SOURCES = \
    src/buff_atomic.cpp \
    src/main.cpp \
    src/timers.cpp \
    src/uint8t.c \
    src/usart.c

# Additional flags and libraries
casamonitor_CXXFLAGS = -Wall -g $(AM_CPPFLAGS)
casamonitor_LDADD = -lm
