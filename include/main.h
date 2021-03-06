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
#define PI 2.0 * asin(1.0)
#define PIby2 0.5 * PI
#define PI2 2.0 * PI

#define x2(x) (x * x)
#define sqrt(x) pow(x, 0.5)
#define SGN(x) (x > 0) - (x < 0)
#define isAT(x1, x2, da) ((x1 >= (x2 - da)) && (x1 <= (x2 + da))) //  search for X1 within x2+-da

#define ev2J 1.60217656535e-19
#define amu 1.66053904020e-24     // g
#define eCharge 1.60217656535e-19 // Coulomb
#define cm2m 0.01;                // cm to meter
#define cm2mm 10.0;               // cm to mm

std::default_random_engine rngen;
unsigned seedRD = std::chrono::system_clock::now().time_since_epoch().count();
std::mt19937_64 mySeed(seedRD);

// declare some important global variable
unsigned int numParticle = 4;
std::string particleArray[] = {"alpha", "triton", "deuteron", "proton"};

// create detector node vector
std::vector<std::vector<double>> detNodes;
std::vector<int> detArray;

// detector parameters
double edets1 = 49.5, edets2 = 49.5, edets3 = 65.0e-3, edetStripGap = 0.1;
double dets1 = 49.5, dets2 = 49.5, dets3 = 1.5, detStripGap = 0.1;

class Vector3 {
    double v1;
    double v2;
    double v3;

  public:
    Vector3(double a, double b, double c) : v1(a), v2(b), v3(c)
    {
    }
};

class particle {
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
    double posX; // x position on detector
    double posY; // y position on detector
  public:
    particle(std::string name, double mU, double ch, double en, double tht, double phi, double ex, double binEn)
        : name(name), massNumU(mU), charge(ch), energy(en), angle_theta(tht), angle_phi(phi), excessEnergy(ex),
          BE(binEn)
    {
    }
};

class beam {
    std::string name;
    double ELAB; // energy
    double ECM;
    Vector3 PLAB; // 3 momentum
    Vector3 PCM;
    Vector3 VLAB; // 3 velocity
    Vector3 VCM;
    Vector3 pos;       // position
    double beamRadius; // radius of beam
    double charge;     // charge
};

class target {
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
        : name(name), massNumU(mU), atNum(atN), density(den), length(len), breadth(br), thick(thk)
    {
    }
};

class stripdetectorG {
  public:
    std::string name;
    double lenX;
    double lenY;
    double halfX = 0.5 * lenX;
    double halfY = 0.5 * lenY;
    double thick;
    double gap;
    double dx;
    double dy;
    int numStripX;
    int numStripY;
    double biasfrontV;
    double biasbackV;

  public:
    double SArea()
    {
        return lenX * lenY;
    } // surface area;
 stripdetector(std::string name, double lx, double ly, double thk, double gp, int nx, int ny, double frontV, double backV)
   : name(name), lenX(lx), lenY(ly), thick(thk), gap(gp), numStripX(nx), numStripY(ny), biasfrontV(frontV), biasbackV(backV)
    {
    }
};

class stripDet :: public stripdetectorG {

}

// generate particle pair within a solid angle

void setPos(double tht, double phi, double hval, double dist, double& xval, double& yval)
{
    double tht1 = tht;
    int isgn = SGN(tht);

    tht = std::abs(tht);
    xval = dist * tan(tht) * cos(phi); // dist * tht * cos(phi);
    yval = dist * tan(tht) * sin(phi); // dist * tht * sin(phi);
    xval *= isgn;
    yval *= isgn;
}

void gen_Particle(int& p1,
                  int& p2,
                  double detTheta,
                  double lx,
                  double ly,
                  double hval,
                  double dist,
                  double& xval,
                  double& yval)
{
    double thistht = 9999.99, thisphi = 9999.99;
    std::uniform_int_distribution<> partDist(0, (int)numParticle - 1);
    std::uniform_real_distribution<double> angleDist2(0.0, PI2);
    std::uniform_real_distribution<double> angleDist1(-detTheta, detTheta);

    do {
        p1 = partDist(mySeed);
        p2 = partDist(mySeed);
    } while(p1 != 0);

    do {
        do {
            thistht = angleDist1(mySeed); // rngen);
        } while(std::abs(thistht) > detTheta);

        thisphi = angleDist2(mySeed); // rngen);
        setPos(thistht, thisphi, hval, dist, xval, yval);
    } while((std::abs(xval) > lx) || (std::abs(yval) > ly));
}

void detNodeCompute(stripdetector& mystrip)
{
    double xloc = 0.0, yloc = 0.0;
    double totGapX = mystrip.gap * (mystrip.numStripX - 1);
    double totGapY = mystrip.gap * (mystrip.numStripY - 1);
    mystrip.dx = (mystrip.numStripX) ? (mystrip.lenX - totGapX) / (double)(mystrip.numStripX) : 0.0; // horizontal
    mystrip.dy = (mystrip.numStripY) ? (mystrip.lenY - totGapY) / (double)(mystrip.numStripY) : 0.0; // vertical

    std::cout << mystrip.name << "  XNum = " << mystrip.numStripX << "  YNum = " << mystrip.numStripY << "  ";
    std::cout << "dx = " << mystrip.dx << " dy = " << mystrip.dy << std::endl;

    int icnt = 0;
    if(mystrip.dx == 0.0 && mystrip.dy != 0.0) { //   hozizontal
        icnt = 0;
        for(int ynode = 0; ynode < mystrip.numStripY; ynode++) {
            ++icnt;
            std::vector<double> temp;
            xloc = -999.99;
            yloc = -mystrip.halfY + (ynode + 1) * mystrip.dy;
            temp.push_back(icnt);
            temp.push_back(xloc);
            temp.push_back(yloc);
            detNodes.push_back(temp);
            temp.clear();
        }
    }
    else if(mystrip.dx != 0.0 && mystrip.dy == 0.0) { // vertical
        icnt = 0;
        for(int xnode = 0; xnode < mystrip.numStripX; xnode++) {
            ++icnt;
            std::vector<double> temp;
            xloc = -mystrip.halfX + (xnode + 1) * mystrip.dx;
            yloc = -999.99;
            temp.push_back(icnt);
            temp.push_back(xnode);
            temp.push_back(yloc);
            detNodes.push_back(temp);
            temp.clear();
        }
    }
    else if(mystrip.dx != 0.0 && mystrip.dy != 0.0) {
        icnt = 0;
        for(int xnode = 0; xnode < mystrip.numStripX; xnode++) {
            for(int ynode = 0; ynode < mystrip.numStripY; ynode++) {
                ++icnt;
                std::vector<double> temp;
                xloc = -mystrip.halfX + (xnode + 1) * mystrip.dx;
                yloc = -mystrip.halfY + (ynode + 1) * mystrip.dy;
                temp.push_back(icnt);
                temp.push_back(xloc);
                temp.push_back(yloc);
                detNodes.push_back(temp);
                temp.clear();
            }
        }
    }
}

// Here make detector series
// eDet = 0, Det = 1, verticleConnected = 0, horizontalConnected = 1, 9 :: Not Connected
// Array presented reports arrangement of the detectors 
// like {{0, 0, 0, 1}, {1, 0, 1, 1}} means {{eDetV, eDetH}, {DetV, DetH}}
//      {{0, 0, 9, 9}, {1, 0, 1, 1}} means {{eDetV, NC}, {DetV, DetH}}
//      {{9, 9, 9, 9}, {1, 0, 1, 1}} means only Det connected
void makeDetectors(std::vector<int> detArray, double distFirst) {

}

int checkDetection(double xpos, double ypos, stripdetector det)
{
    int chk = 0; // 0 means not detected 1 means detected
    
    double xx = 0.0, yy = 0.0;
    int detType = 0;
    if(det.dx == 0.0)
        detType = 0;
    if(det.dy == 0.0)
        detType = 1;
    if(det.dx != 0.0 && det.dy != 0.0)
        detType = 2;

    for(std::vector<std::vector<double>>::iterator it1 = detNodes.begin(); it1 != detNodes.end(); ++it1) {
        xx = (*it1)[1];
        yy = (*it1)[2];

        switch(detType) {
        case 0:
	  chk = isAT(ypos, yy, det.dy);
        case 1:
	  chk = isAT(xpos, xx, det.dx);
        case 2:
	  chk = (isAT(xpos, xx, det.dx) && isAT(ypos, yy, det.dy));
	  //if(chk)
          //      std::cout << det.name << "   " << detType << " " << chk << std::endl;
        }
    }
    return chk;
}
