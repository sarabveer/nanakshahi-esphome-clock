// Calculate the phase and position of the moon for a given date.
// The algorithm is simple and adequate for many purposes.
//
// This software was originally adapted to javascript by Stephen R. Schmitt
// from a BASIC program from the 'Astronomical Computing' column of Sky & Telescope,
// April 1994, page 86, written by Bradley E. Schaefer.
//
// Subsequently adapted from Stephen R. Schmitt's javascript to c++ for the Arduino
// by Cyrus Rahman, this work is subject to Stephen Schmitt's copyright:
//
// Copyright 2004 Stephen R. Schmitt
// You may use or modify this source code in any way you find useful, provided
// that you agree that the author(s) have no warranty, obligations or liability.  You
// must determine the suitability of this source code for your use.

#include <math.h>
#include "MoonPhase.h"

// Names of lunar phases
static const char *phaseNames[] = {"    New Moon   ", "Waxing Crescent", " First Quarter ",
				   " Waxing Gibbous", "   Full Moon   ", " Waning Gibbous",
				   "  Last Quarter ", "Waning Crescent"};

// Constructor initialization.
MoonPhase::MoonPhase() {
  jDate = 0;
  phase = 0;
  phaseVal = 0;
  age = 0;
  fraction = 0;
  distance = 0;
  latitude = 0;
  longitude = 0;
  phaseName = "";
}

// Determine the Moon Phase and orbital positions for the specified time.
void
MoonPhase::calculate(time_t t) {
  jDate = julianDate(t);

  // Calculate illumination (synodic) phase.
  // From number of days since new moon on Julian date MOON_SYNODIC_OFFSET
  // (1815UTC January 6, 2000), determine remainder of incomplete cycle.
  phase = (jDate - MOON_SYNODIC_OFFSET) / MOON_SYNODIC_PERIOD;
  phase -= floor(phase);

  // Calculate age and illumination fraction.
  age = phase * MOON_SYNODIC_PERIOD;
  fraction = (1.0 - cos(2 * M_PI * phase)) * 0.5;
  phaseVal = (int)(phase * 8 + 0.5) % 8;
  phaseName = phaseNames[phaseVal];

  // Calculate distance from anomalistic phase.
  double distancePhase = (jDate - MOON_DISTANCE_OFFSET) / MOON_DISTANCE_PERIOD;
  distancePhase -= floor(distancePhase);
  distance = 60.4 - 3.3 * cos(2 * M_PI * distancePhase)
		  - 0.6 * cos(2 * 2 * M_PI * phase - 2 * M_PI * distancePhase)
		  - 0.5 * cos(2 * 2 * M_PI * phase);

  // Calculate ecliptic latitude from nodal (draconic) phase.
  double latPhase = (jDate - MOON_LATITUDE_OFFSET) / MOON_LATITUDE_PERIOD;
  latPhase -= floor(latPhase);
  latitude = 5.1 * sin(2 * M_PI * latPhase);

  // Calculate ecliptic longitude from sidereal motion.
  double longPhase = (jDate - MOON_LONGITUDE_OFFSET) / MOON_LONGITUDE_PERIOD;
  longPhase -= floor(longPhase);
  longitude = 360 * longPhase
	      + 6.3 * sin(2 * M_PI * distancePhase)
	      + 1.3 * sin(2 * 2 * M_PI * phase - 2 * M_PI * distancePhase)
	      + 0.7 * sin(2 * 2 * M_PI * phase);
  if (longitude > 360)
    longitude -= 360;
}

// Determine Julian date from Unix time.
// Provides marginally accurate results with older Arduino 4-byte double.
double
MoonPhase::julianDate(time_t t) {
  return (t / 86400.0L + 2440587.5);
}
