#include <iostream>
#include <cmath>

#define PI 3.14159265

using namespace std;

double julian_day(int year, int month, int day, int hour, int minute, int second)
{
double time = 0;
if(month < 3) {
        year  = year - 1;
        month = month + 12;
}

time = hour + minute / 60 + second / 3600;

return 365.25*year + 30.6001*(month+1) - 15 + 1720996.5 + day + time / 24.0 ;
}

double mean_anomaly(double ju)
{
//m0 in degrees, m1 in degrees per day from a table, j2000 is julian 2000. 
double m0 = 357.5291, m1 = 0.98560028, j2000 = 2451545,  m = 0;

m = m0+m1 * (ju - j2000);

	while(m > 180){
	m = m - 180;
	}
return m;
}

double anomaly_correction(double mean_anom)
{
double cor = 0;
double c1 = 1.9148, c2 = 0.02, c3 = 0.0003;
cor = c1 * sin(mean_anom*PI/180) + c2 * sin(2*mean_anom*PI/180) + c3 * sin(3*mean_anom*PI/180);
return cor;
}


double ecliptical_longitude(double tru_anom)
{
//perihelion of earth is 102.9372
double per = 102.9372;
double e = per + tru_anom + 180;
	while (e > 360){ e = e - 360; }
return e;
}

double sun_ra(double e)
{
// coefficients a2 a4 a6 are from a table from the approximation
// maximum error 0.0003
double a2 = -2.468, a4 = 0.053, a6 = -0.0014;
double sunra = e + a2 * sin(2*e*PI/180) + a4 * sin(4*e*PI/180) + a6 * sin(6*e*PI/180);
return sunra; 
}

double sun_dec(double e)
{
// coefficients d1 d3 d5 are from a table from the approximation
// maximum error 0.0003
double d1 = 22.8008, d3 = 0.5999, d5 = 0.0493;
double sin_eclip = sin(e*PI/180);
double sundec =   d1 * sin_eclip +
		  d3 * sin_eclip * sin_eclip * sin_eclip + 
		  d5 * sin_eclip * sin_eclip * sin_eclip * sin_eclip * sin_eclip;
return sundec;
}


int main()
{
for(;;){
// aa.quae.nl/en/reken/zonpositie.html#mjx-eqn-eqdelta-zon
int ye = 2004, mo = 4, da = 1, ho = 12 , mi = 0 , se = 0;
cout << "enter year( yyyy )\n month( m )\n day( d )\n the hour is set default 12:00:00 \n";
cin >> ye;
cin >> mo;
cin >> da;

// find julian day
double julian = julian_day(ye,mo,da,ho,mi,se);

cout << ye << "/" << mo << "/" << da << "-"<< ho << ":"<< mi << ":"<< se 
<<" julian day: " << julian << endl;


//find mean anomaly on that julian day
double mean_an = mean_anomaly(julian);
cout << "mean anomaly: " << mean_an << endl;

// correct the anomaly for the fact that the orbit is an ellipse
// for a circular orbit, mean and true anomaly are the same
// but ellipse so let's convert the meaan anomaly into true anomaly
// using a correction variable c, and the true anomaly variable v.
// correction variable c is calculated using the following equation
// c = c1 sin(m) + c2 sin(2m) + c3 sin(3m) where m is 
// calculated mean anomaly anc coefficients c1 c2 c3 are looked from a table

double anomaly_cor = anomaly_correction(mean_an);
double true_anomaly = mean_an + anomaly_cor;
cout << "anomaly correction: " << anomaly_cor << endl;
cout << "true anomaly: 	     " << true_anomaly << endl;

// ecliptical coordinates
// perihelion( per ) and obliquity( obq ) of the ecliptic ( planetocentric ) 
// ecliptical longitude (lmd) = true anomaly + perihelion +  180( back to back )
// fun fact: when ecliptical longitude = 0, begining of spring in northern hemisphere,
// every 90 degrees change marks the start of the next season

double ecliptical_long = ecliptical_longitude(true_anomaly);
cout << "ecliptical longitude: " << ecliptical_long << endl;

// convert to equatorial coordinates right ascension (alpha) and declination ( delta )
// an approximation takes place for obq close to 0 or 180

double sunRA = sun_ra(ecliptical_long);
double sunDE = sun_dec(ecliptical_long);
cout << "sun coordinates: RA   |    DEC \n";
cout << "	       :" << sunRA << "  " << sunDE << endl;

// the observer's position on earth effectts the
}
	return 0;
}

