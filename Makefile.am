AUTOMAKE_OPTIONS = foreign subdir-objects

# Define the subdirectories
SUBDIRS = include src

# Include directories
AM_CPPFLAGS = -I$(srcdir)/include

# Define the program to be built
bin_PROGRAMS = casamonitor

# Additional flags and libraries
casamonitor_CXXFLAGS = -Wall -g $(AM_CPPFLAGS)
casamonitor_LDADD = -lm
