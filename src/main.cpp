#include "main.h"

int main ( int argc, char** argv )
{
  // declaration of some important variables
  int particle1=99, particle2=99;  // 99 means error
  double detDist = 10.0 * cm2mm; //  10 cm
  double detSolAngle = 0.0;
  double detTheta = 0.0; // in radian
  double theta = 999.9, phi = 999.9;
  long NumParticles = 20;  //  1e+8;  // total number of pairs to see



    // Initialize product particle from target
  particle alpha("alpha", 4.00150646649, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0);
  particle tritium("triton", 3.0160492, 0.0, 0.0, 0.0, 0.0, 14949.794e+3, 8481.821e+3);
  particle deuteron ("deuteron", 2.01410178, 0.0, 0.0, 0.0, 0.0, 13135.720e+3, 2224.52e+3);
  particle proton("proton", 1.00727646687991, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0);

  // initialize detector
  detector strip("strip", 50, 50, 1.55, 16, 16); // 5 cm X 5 cm X 0.155 cm and 16 X 16 lines
  std::cout << " strip area = " << strip.SArea() << std::endl;
  detSolAngle = strip.SArea()/x2(detDist);    // steradian
  detTheta = 0.5 * acos(1.0 - detSolAngle/PI2);

  std::cout << " solid angle = " << detSolAngle << "   theta = " << detTheta << std::endl;

    // generate particles with specific energy
    // and
    // within a certain solid angle
  for (int icnt = 0; icnt < NumParticles; icnt++) {
    do {
      particle1 = 99;
      particle2 = 99;
      gen_Particle (particle1, particle2, detTheta, theta, phi);
    } while(particle1 == 99 || particle2 == 99);

    setPos(theta, phi, detDist, strip.halfX, strip.halfY);
  }
    return 0;
}
