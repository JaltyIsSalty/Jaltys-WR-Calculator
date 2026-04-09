#pragma once
#include <iomanip>

#include "Info.h"



//------------------
//Helper Functions |
//------------------
inline double percentToDec(double percVal)
{
	return percVal / 100;
}
inline double decToPercent(double decVal)
{
	return decVal * 100;
}


class robot
{
public:
	double HP;
	double greyResistance;
	double reflector;
	double defensePoints;

	robot
	(
		double HP, 
		double greyResistance = Info::defaults::greyResistance,
		double reflector      = Info::defaults::reflector,
		double defensePoints  = Info::defaults::defensePoints
	)
	{
		this->HP             = HP;
		this->greyResistance = greyResistance;
		this->reflector      = reflector;
		this->defensePoints  = defensePoints;
	}

		

	double CalculateEffectiveHP()
	{
		//Grey resistance uses the same logic as Reflector but applies directly to Hit Points (HP). We assume we heal instantly
		double HPAfterGrey = HP / Reflector_DmgReceived(greyResistance);

		//Apply DP, EffectiveHP after DP should be calced before reflector since game applies DP before Reflector
		double EHPAfterDP = HPAfterGrey / DP_DmgReceived(defensePoints);

		//apply Reflector
		double EffectiveHP = EHPAfterDP / Reflector_DmgReceived(reflector);

		return EffectiveHP;
	}

	double CalculateRealisticHP()
	{
		return CalculateEffectiveHP() * 0.5;
	}
private:
	double DP_DmgReceived(double DP)
	{
		return 100 / (100 + DP);
	}
	double Reflector_DmgReceived(double refPerc /*Ex: 85 for 85% reflector resistance*/)
	{
		return std::abs(percentToDec(refPerc) - 1);
	}
};