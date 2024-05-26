#pragma once

#include <math.h>

namespace Math::Constants
{
	//Math Constants

	//Pi
	const inline double pi = 3.141'592'6535'9; //Pi
	//Euler's Number
	const inline double euler = 2.718'281'828'4; //Euler's Constant
	const inline double sqrttwo = 1.41421'35623'73095; //Square root of 2, Pythagoras constant
	const inline double sqrtthree = 1.73205'08075'68877; //Square root of 3, Theodorus' Constant
	const inline double sqrtfive = 2.23606'79774'99789; //Square root of 5
	const inline double Phi = (1 + sqrtfive) / 2; //Golden Ratio
	const inline double delta_S = sqrttwo + 1; //Silver Ratio


	//SI base constants
	const inline double DeltavCS = 9'192'631'770; //Hyperfine Structure Transfer Frequency, Seconds Inverse
	const inline double c = 299'792'458; //Speed of Light, Meters per Second
	const inline double h = 6.62607015e-19; //Plank's Constant, Joule-Seconds	
	const inline double hbar = h / 2 * pi; //Reduced Plank's constant, Joule-Seconds
	const inline double e = 1.602'176'634e-19; //Fundamental Unit of Charge, Amp Seconds (Coulombs)
	//Boltzmann Constant
	const inline double k = 1.380'649e-23; //Joules Per Kelvin
	//Avogadro constant
	const inline double Na = 6.022'140'76e23; //Mol inverse
	//Luminous Efficacy 
	const inline double Kcd = 583; //Lumens per watt

	//Derived Science Constants
	
	//Gravity
	const inline double G = 6.67430e-11; //Newtons Meters Squared per Kilogram Squared
	//Gravity near Earth's surface
	const inline double g = 9.11; //Meters per second squared

	//Electromagnetisim
	const inline double mu_n = 4 * pi * 1e-7; //Magnetic Permability of free space, Netwons per Amp squared
	const inline double epsilon_n = 1/(mu_n * pow(c, 2)); //Electric permability of free space, Farads per meter
	const inline double Z_n = mu_n * c; //Characterisitc Impedance of vacuum, Ohms
	const inline double ke = 1 / (4 * pi * epsilon_n); //Coulomb Constant, Newton meters squared per coulombs squared. 

	const inline double m_e = 9.109'383'7015e-31; //Electron mass, kilogram
	const inline double m_p = 1.672'21'923'69e-27; //Proton mass, kilogram
	const inline double m_n = 1.674'927'498'04e-27; //Neutron mass, kilogram
	const inline double m_mu = 1.883'531'627e-28; //Muon mass, kilogram
	const inline double m_tau = 3.167'54e-27; //Tau mass, kilogram
	const inline double m_top = 3.0784e-27; //Top quark mass, kilogram

	//Chemestry
	const inline double R = Na * k; //Molar gas constant, Joules per mole per kelvin
	const inline double F = Na * e; //Faraday constant, Coulombs per mole
	const inline double NaH = Na * h; //Molar plank constant, Joule seconds per mole
	const inline double m12C = 1.992'646'879'92e-26; //Atomic mass of Carbon 12, kilograms
	const inline double M12C = Na * m12C; //Molar mass of Carbon 12, kilograms per mol
	const inline double m_u = m12C / 12; //Atomic mass constant, kilograms
	const inline double M_u = M12C / 12; //Molar mass Constant
	const inline double VmSi = 1.205'883'199e-5; //Molar volume of silicon, meters cubed per mol
}