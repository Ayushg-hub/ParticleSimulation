#pragma once
#include"UserIface.h"
class SimulationIface : public UserIface
{
	const IfaceType IFType = IfaceType::SIMULATION;
public:
	virtual IfaceType getIfaceType() { return IFType; }
	virtual void calcPosition() = 0;
	virtual void calcVelocity() = 0;
	virtual void calcAccelaration() = 0;
};

