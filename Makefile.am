AUTOMAKE_OPTIONS = foreign subdir-objects

# Define the subdirectories
SUBDIRS = include src

# Include directories
AM_CPPFLAGS = -I$(srcdir)/include

# Define the program to be built
bin_PROGRAMS = main

# Additional flags and libraries
main_CXXFLAGS = -Wall -g $(AM_CPPFLAGS)
main_LDADD = -lm
