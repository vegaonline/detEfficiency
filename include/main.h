#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <iterator>
#include <cstdio>
#include <cstring>
#include <random>
#include <chrono>

#define PI 2.0 * asin ( 1.0 )
#define x2( x ) ( x * x )
#define ev2J 1.60217656535e-19
#define amu 1.66053904020e-27     // kg
#define eCharge 1.60217656535e-19 // Coulomb
#define cm2m 0.01; // cm to meter

unsigned seedRD = std::chrono::system_clock::now ().time_since_epoch ().count ();
std::mt19937_64 mySeed ( seedRD );

unsigned int numParticle = 4;
std::string particleArray [] = {"alpha", "triton", "deuteron", "proton"};

// create uniform distribution for physical parameters
std::uniform_int_distribution<> partDist(0, (int)numParticle-1);
std::uniform_real_distribution<double> angleDist(-PI, PI);



class Vector3 {
  double v1;
  double v2;
  double v3;
 public:
 Vector3(double a, double b, double c) : v1(a), v2(b), v3(c){}
};

class particle
{
    std::string name;
    double massNumU;
    double charge;
    double energy;
    double angle_theta;  // in radian
    double angle_phi;    // in radian
    double excessEnergy; // eV
    double BE;           // eV
    double massNumG = massNumU * amu;
    double chargeCoul = charge * eCharge;
    double posX;
    double posY;
 public:
 particle(std::string name, double mU, double ch, double en, double tht, double phi, double ex, double binEn) 
   : name(name),massNumU(mU), charge(ch), energy(en), angle_theta(tht), angle_phi(phi), excessEnergy(ex), BE(binEn){}
    void setPos(double, double, double );
};

class beam
{
    std::string name;
    double ELAB;   // energy
    double ECM;
    Vector3 PLAB;  // 3 momentum
    Vector3 PCM;
    Vector3 VLAB;  // 3 velocity
    Vector3 VCM;
    Vector3 pos;   // position
    double beamRadius;  // radius of beam
    double charge;      // charge
};

class target
{
    std::string name;
    double massNumU;
    double atNum;
    double density;
    double length;
    double breadth;
    double thick;
    double SArea = length * breadth; // surface area

 public:
 target(std::string name, double mU, double atN, double den, double len, double br, double thk) 
   : name(name), massNumU(mU), atNum(atN), density(den), length(len), breadth(br), thick(thk){}
};

class detector
{
    std::string name;
    double lenX;
    double lenY;
    double thick;
    int numStripX;
    int numStripY;
 public:
    double SArea(){return lenX * lenY;} // surface area;
    detector(std::string name, double lx, double ly, double thk, int nx, int ny) 
   : name(name), lenX(lx), lenY(ly), thick(thk), numStripX(nx), numStripY(ny){}
};


// generate particle pair within a solid angle

void particle::setPos(double tht, double phi, double dist) {
}

void gen_Particle (int& p1, int& p2, double tht, double& thistht, double& thisphi) { 
  do {
    p1 = partDist(mySeed);
    p2 = partDist(mySeed);
  } while (p1 !=0);
  
  do {
    thistht = angleDist(mySeed);
    thisphi = angleDist(mySeed);
  } while (thistht < -tht || thistht > tht ||thisphi < -tht || thisphi > tht);

  
  std::cout << " Allowed angle range = " << -tht << " : " << tht << std::endl;
   std::cout << particleArray[p1] << "  " << particleArray[p2] << " theta = " << thistht << "  phi = " << thisphi << std::endl;

}
