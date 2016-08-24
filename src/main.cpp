#include "main.h"

int main ( int argc, char** argv )
{
  // declaration of some important variables
  int particle1=99, particle2=99;  // 99 means error
  double detDist = 30.0 * cm2m; // cm
  double detSolAngle = 0.0;
  double detTheta = 0.0; // in radian
  double theta = 999.9, phi = 999.9;



    // Initialize product particle from target
  particle alpha("alpha", 4.00150646649, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0);
  particle tritium("triton", 3.0160492, 0.0, 0.0, 0.0, 0.0, 14949.794e+3, 8481.821e+3);
  particle deuteron ("deuteron", 2.01410178, 0.0, 0.0, 0.0, 0.0, 13135.720e+3, 2224.52e+3);
  particle proton("proton", 1.00727646687991, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0);

  // initialize detector
  detector strip("strip", 0.05, 0.05, 0.00155, 16, 16); // 5 cm X 5 cm X 0.155 cm and 16 X 16 lines
  detSolAngle = strip.SArea()/x2(detDist);    // steradian
  detTheta = 0.5 * acos(1.0 - detSolAngle/(2.0 * PI));

    // generate particles with specific energy
    // and
    // within a certain solid angle

  do {
    gen_Particle (particle1, particle2, detTheta, theta, phi);
  } while(particle1 == 99 || particle2 == 99);

  // setPos(double tht, double phi, double dist)

    return 0;
}
