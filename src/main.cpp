#include "main.h"

int main(int argc, char** argv)
{
    // declaration of some important variables
    int particle1 = 99, particle2 = 99; // 99 means error
    double detDist = 17.467 * cm2mm;    //  10 cm
    double detSolAngle = 0.0;
    double detTheta = 0.0; // in radian
    double detThetaby2 = 0.0;
    double solAngF1 = 0.0, solAngF2 = 0.0;
    double theta = 999.9, phi = 999.9;
    double xpos = 0.0, ypos = 0.0;
    long NumParticles = 20; //  1e+8;  // total number of pairs to see
    long pDetected = 0;     // particles detected by detector

    // Initialize product particle from target
    particle alpha("alpha", 4.00150646649, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    particle tritium("triton", 3.0160492, 0.0, 0.0, 0.0, 0.0, 14949.794e+3, 8481.821e+3);
    particle deuteron("deuteron", 2.01410178, 0.0, 0.0, 0.0, 0.0, 13135.720e+3, 2224.52e+3);
    particle proton("proton", 1.00727646687991, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0);

    // initialize detector
    detArray={0, 0, 9, 9, 1, 0, 1, 1};
    makeDetectors(detArray, detDist);
    stripdetector strip("strip", dets1, dets2, dets3, detStripGap, 16, 16, 0.0, 0.0);     // 5 cm X 5 cm X 0.155 cm and 16 X 16 lines bias = 0.0V
    stripdetector eDet("eDet", dets1, dets2, dets3, detStripGap, 16, 16, 0.0, 0.0);        // 5cm X 5cm X 0.155cm 16 horizontal 16 vert
    stripdetector delEDet("delEDet", edets1, edets2, edets3, edetStripGap, 16, 0, 0.0, 0.0); // 5cm X 5cm X 50e-4cm 16 vertical
    detNodeCompute(strip);
    detNodeCompute(eDet);
    detNodeCompute(delEDet);

    // std::cout << " strip area = " << strip.SArea() << std::endl;
    std::cout << " Strip limit :->  X = " << -strip.halfX << " mm : " << strip.halfX
              << " mm    and  Y = " << -strip.halfY << " mm : " << strip.halfY << " mm " << std::endl;

    // detSolAngle = strip.SArea()/x2(detDist);    // steradian
    solAngF1 = 0.5 * sqrt(4.0 * x2(detDist) + x2(strip.lenX) + x2(strip.lenY)); // physically slanting length
    solAngF2 = strip.SArea() / sqrt((x2(strip.lenX) + x2(solAngF1)) * (x2(strip.lenY) + x2(solAngF1)));
    detSolAngle = 4.0 * asin(solAngF2);
    // detTheta = 0.5 * detSolAngle;
    detTheta =
        asin(strip.halfX / solAngF1); // acos(detDist / solAngF1);                     // acos(1.0 - detSolAngle/PI2);
    detThetaby2 = 0.5 * detTheta;

    //  std::cout << " solid angle = " << detSolAngle << "   theta = " << detTheta << "  phi = " << PI2 << std::endl;

    // generate particles with specific energy
    // and
    // within a certain solid angle
    for(int icnt = 0; icnt < NumParticles; icnt++) {
        do {
            gen_Particle(particle1, particle2, detTheta, strip.halfX, strip.halfY, solAngF1, detDist, xpos, ypos);
        } while(particle1 == 99 || particle2 == 99);
        // std::cout << particleArray[particle1] << " --- " << particleArray[particle2] << " x = " << xpos << " y = " <<
        // ypos << std::endl;
        checkDetection(xpos, ypos, eDet);
        checkDetection(xpos, ypos, delEDet);
        //   if (checkDetection(xpos, ypos, eDet))
        //  std::cout << particleArray[particle1] << " and " << particleArray[particle2] << " detected....." <<
        //  std::endl;
    }
    return 0;
}
