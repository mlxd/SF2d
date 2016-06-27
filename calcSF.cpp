/*
 * 	This program takes a space delimited text file of
 * 	x and y positions and returns a file containing
 *  the structure factor - SFplotdata.txt.
 *
 *  The calculations are based on real space lattice
 *  spacings of a0=1e-7m
 *
 *  The data set must be at least as large as
 *  10e-7 < xval < 60e-7
 *  2e-7  < yval < 50e-7
 *  for everything outside this range is trimmed before
 *  calculation.
 *
 *  The range/resolution in reciprocal space is
 *  -3pi to 3pi in qx and qy in steps of pi/20.0
 *
 *
 *  Modifications need to be made to this program to allow
 *  for different densities and parameters to be specified.
 *
 *
 *
 * 	This was written by Jon Watkins - 2012
 *
 */

#include <iostream>
#include <fstream>
#include <cmath>
#include <list>
#include <sstream>
#include <iomanip>
#include <string.h>
#include <stdio.h>
#include <vector>

using namespace std;

double pi = 3.14159;
double a0=2.044094e-05;

class CVortex{
    double x, y, z;

    public:
	CVortex(){
		x=0; y=0; z=0;
	}

	~CVortex(){}

	void set_pos (double a,double b,double c){
		x = a; y = b; z = c;
	}

    	double get_x(){
			return x;
	}

    	double get_y(){
			return y;
	}

    	double get_z(){
			return z;
	}
};

void initialiseVortices(list<CVortex>& vorticesList, bool& file){
	ostringstream oss;
	char renderChar[100];
	string renderStr;
	oss.str("");
	oss << "vort_ord_2000.csv";
	cout <<"oss: " << oss.str() << endl;
	renderStr = oss.str();
	cout << renderStr << endl;
	strcpy(renderChar,renderStr.c_str());

	ifstream myfile (renderChar);
	cout << "initialise Vortices" << endl;


	if (myfile.is_open()) {
		cout << "Initial Vortex Positions From File" << endl;
		file = true;
		double xval;
		double yval;
		int a,b,c;
		char delim;
		while ( myfile.good() ){
       		 	myfile >> xval;
			xval *= 6.821230e-07;
			myfile >> delim;

            		myfile >> yval;
			yval *= 6.821230e-07;
			myfile >> delim;

			myfile >> a;
			myfile >> delim;

			myfile >> b;
			myfile >> delim;

			myfile >> c;
			CVortex newVortex;
			if (xval > 200*6.821230e-07 && xval < 800*6.821230e-07 && yval > 200*6.821230e-07 && yval < 800*6.821230e-07 ) {
				newVortex.set_pos(xval/a0,yval/a0,0);
				vorticesList.push_back(newVortex);
			}
       		}
        	myfile.close();
	}
	cout << "initialised" << endl;
}

double S(double qx, double qy, list<CVortex> posdata) {
	double sumpq=0;

	for (list<CVortex>::iterator p=posdata.begin(); p!=posdata.end(); ++p ){
		double sumq=0;

		for (list<CVortex>::iterator q=posdata.begin(); q!=posdata.end(); ++q) {
			sumq +=
			cos( qx * (q->get_x() - p->get_x())
			+
			qy * (q->get_y() - p->get_y())
		);
		}
		sumpq += sumq;
	}
	return sumpq;
}


int main() {
	int ngX=128;
	double numRecip=2*pi;
	list<CVortex> posdata;

	bool file;
	initialiseVortices(posdata, file);
	double sf,qx,qy;
	int qi,qj;
	std::vector<double> vx(ngX);
	std::vector<double> vy(ngX);
	std::vector<double> vz(ngX*ngX);
	#pragma omp parallel for private(qj)
	for (qi = 0; qi<ngX; qi++) {
		for (qj = 0; qj<ngX; qj++) {

			qx = ((qi-ngX/2.)/ngX)*numRecip*pi;
			vx[qi] = qx;
			qy = ((qj-ngX/2.)/ngX)*numRecip*pi;
			vy[qj] = qy;
			vz[qi*ngX+qj] = S(qx,qy,posdata);

			//plotdata << setw(20) << qx << setw(20) << qy << setw(20) << vz[qi*256+qj] << endl;
			//sf=S(qx,qy,posdata);


	//for (qx = -numRecip*pi; qx<=numRecip*pi; qx += 2*pi/20) {
	//	for (qy = -numRecip*pi; qy<=numRecip*pi; qy += 2*pi/20) {
			//sf=S(qx,qy,posdata);
			//cout << setw(10) << qx << setw(10) << qy << setw(20) << sf << endl;
			//plotdata << setw(20) << qx << setw(20) << qy << setw(20) << sf << endl;
		}
		//plotdata << endl;
	}
	
	ofstream dataX, dataY, dataZ;
	dataX.open("x.csv");
	dataY.open("y.csv");
	dataZ.open("z.csv");
	dataX.precision(6);
	dataY.precision(6);
	dataZ.precision(6);
	
	for (int ii=0; ii<ngX; ++ii){

		for (int jj=0; jj<ngX; ++jj){
		
			dataX << setw(20) << vx[ii] << endl;
			dataY << setw(20) << vy[jj] << endl;
			dataZ << vx[ii] << setw(20) << vy[jj] << setw(20) << vz[ii*ngX + jj] << endl;

		}
		dataZ << endl;
	}

	dataX.close();
	dataY.close();
	dataZ.close();
	cout << "Structure Factor plot data written to SFplotdata.txt" << endl;

	return 0;
}
