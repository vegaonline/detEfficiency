#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <iterator>
#include <cstdio>
#include <cstring>
#include <random>
#include <chrono>

#define EPSA 1.0e-1
#define PI 2.0 * asin ( 1.0 )
#define PIby2 0.5 * PI
#define PI2 2.0 * PI

#define x2( x ) ( x * x )
#define sqrt(x) pow(x, 0.5)
#define SGN(x) (x > 0) - (x < 0)
#define isAT(x1, x2) ((x1 >= (x2 - EPSA)) && (x1 <= (x2 + EPSA)))  //  search for X1 within x2+/ EPSA

#define ev2J 1.60217656535e-19
#define amu 1.66053904020e-24     // g
#define eCharge 1.60217656535e-19 // Coulomb
#define cm2m 0.01; // cm to meter
#define cm2mm 10.0; // cm to mm

std::default_random_engine rngen;
unsigned seedRD = std::chrono::system_clock::now ().time_since_epoch ().count ();
std::mt19937_64 mySeed ( seedRD );

// declare some important global variable
unsigned int numParticle = 4;
std::string particleArray [] = {"alpha", "triton", "deuteron", "proton"};

// createdetector node vector
std::vector<std::vector<double> > detNodes;



class Vector3 {
  double v1;
  double v2;
  double v3;
 public:
 Vector3(double a, double b, double c) : v1(a), v2(b), v3(c){}
};

class particle
{
 public:
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
    double posX;         // x position on detector
    double posY;         // y position on detector
 public:
 particle(std::string name, double mU, double ch, double en, double tht, double phi, double ex, double binEn) 
   : name(name),massNumU(mU), charge(ch), energy(en), angle_theta(tht), angle_phi(phi), excessEnergy(ex), BE(binEn){}
  
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
 public:
    std::string name;
    double lenX;
    double lenY;
    double halfX = 0.5 * lenX;
    double halfY = 0.5 * lenY;
    double thick;
    int numStripX;
    int numStripY;
 public:
    double SArea(){return lenX * lenY;} // surface area;
    detector(std::string name, double lx, double ly, double thk, int nx, int ny) 
   : name(name), lenX(lx), lenY(ly), thick(thk), numStripX(nx), numStripY(ny){}
};







// generate particle pair within a solid angle

void setPos(double tht, double phi, double hval, double dist, double& xval, double& yval) {
  double  tht1 = tht;
  int isgn = SGN(tht);

  tht = std::abs(tht);
  xval = dist * tan(tht) * cos(phi);    // dist * tht * cos(phi);
  yval = dist * tan(tht) * sin(phi);    // dist * tht * sin(phi);
  xval  *= isgn;
  yval *= isgn;
}

void gen_Particle (int& p1, int& p2,  double detTheta, double lx, double ly, double hval, double dist, double& xval, double& yval) { 
  double thistht = 9999.99, thisphi = 9999.99;
  std::uniform_int_distribution<> partDist(0, (int)numParticle-1);
  std::uniform_real_distribution<double> angleDist2(0.0, PI2);
  std::uniform_real_distribution<double> angleDist1(-detTheta, detTheta);

  do {
    p1 = partDist(mySeed);
    p2 = partDist(mySeed);
  } while (p1 !=0);
  
  do {
    do {
      thistht = angleDist1(mySeed);      // rngen);
    } while (std::abs(thistht ) > detTheta);

    thisphi = angleDist2(mySeed);    // rngen);
    setPos(thistht, thisphi, hval, dist, xval, yval);
  } while ( (std::abs(xval) > lx) || (std::abs(yval) > ly));
}

void detNodeCompute(detector strip) {
  double xloc = 0.0, yloc = 0.0;
  double dx = strip.lenX / (double)(strip.numStripX + 1);
  double dy = strip.lenY / (double)(strip.numStripY + 1);
  int icnt = 0;

  for (int xnode = 0; xnode < strip.numStripX; xnode++) {
    for (int ynode = 0 ; ynode < strip.numStripY; ynode++) {
      std::vector<double> temp;
      xloc = -strip.halfX + (xnode + 1) * dx;
      yloc = -strip.halfY + (ynode + 1) * dy;
      temp.push_back(icnt);
      temp.push_back(xloc);
      temp.push_back(yloc);
      detNodes.push_back(temp);
      temp.clear();
      ++icnt;
    }
  }
}


int checkDetection(double xpos, double ypos) {
  int chk = 0; // 0 means not detected 1 means detected
  double xx = 0.0, yy = 0.0;
 
  for (std::vector<std::vector<double> >::iterator it1 = detNodes.begin(); it1 !=detNodes.end(); ++it1) {
    xx = (*it1)[1];
    yy = (*it1)[2];
    chk = (isAT(xpos, xx) && isAT(ypos, yy));
  }

  return 0;
}
