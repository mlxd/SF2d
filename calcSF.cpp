/*
 * 	This program takes a space delimited text file of
 * 	x and y positions and returns a file containing
 *  the structure factor - SFplotdata.txt.
 *
 * 	Original version written by Jon Watkins - 2012
 *	Updated by Lee J. O'Riordan - 2016
 */
#include <cstdlib>
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

void initialiseVortices(list<CVortex>& vorticesList, bool& file, ostringstream &oss){
	char renderChar[100];
	string renderStr;
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
		string value;
		getline (myfile,value,'\n');
		while ( myfile.good() ){
			myfile >> a;
			myfile >> delim;

       		 	myfile >> xval;
			xval *= 6.821230e-07;
			myfile >> delim;

			myfile >> b;
			myfile >> delim;

            		myfile >> yval;
			yval *= 6.821230e-07;
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


int main(int argc, char *argv[]) {
	int ngX=128;
	double numRecip=2*pi;
	
	list<CVortex> posdata;
	ostringstream oss;
	
	bool file;
	double sf, qx, qy;
	int qi, qj;
	
	std::vector<double> vx(ngX);
	std::vector<double> vy(ngX);
	std::vector<double> vz(ngX*ngX);
	
	double eps = 7.0/3.0 -4.0/3.0 -1.0;
	
	for(int count=1; count < argc; ++count){
		oss.clear();
		oss.str("");
		oss << "vort_arr_" << argv[count];
		cout << oss.str() << endl;

		initialiseVortices(posdata, file, oss);
		#pragma omp parallel for private(qj)
		for (qi = 0; qi<ngX; qi++) {
			for (qj = 0; qj<ngX; qj++) {
	
				qx = ((qi-ngX/2.)/ngX)*numRecip*pi + eps;
				vx[qi] = qx;
				qy = ((qj-ngX/2.)/ngX)*numRecip*pi + eps;
				vy[qj] = qy;
				vz[qi*ngX+qj] = S(vx[qi],vy[qj],posdata) + eps;
			}
	
		}
		std::vector<std::string> filenames = {"_x.csv","_y.csv","_z.csv"};
		ofstream dataX, dataY, dataZ;
		dataX.open(argv[count] + filenames[0]);
		dataY.open(argv[count] + filenames[1]);
		dataZ.open(argv[count] + filenames[2]);
		dataX.precision(7);
		dataY.precision(7);
		dataZ.precision(7);
		
		for (int ii=0; ii<ngX; ii++){
	
			for (int jj=0; jj<ngX; jj++){
			
				dataX << setw(20) << vx[ii] << endl;
				dataY << setw(20) << vy[jj] << endl;
				dataZ << vx[ii] << setw(20) << vy[jj] << setw(20) << vz[ii*ngX + jj] << endl;
	
			}
			dataZ << endl;
		}
	
		dataX.close();
		dataY.close();
		dataZ.close();

		vx.clear();
		vy.clear();
		vz.clear();
	
		posdata.clear();
	}
	return 0;
}
