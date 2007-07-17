/***************************************************************************
  This file is part of Project Apollo - NASSP
  Copyright 2004-2007

  Environmental Control System.

  Project Apollo is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  Project Apollo is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Project Apollo; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  See http://nassp.sourceforge.net/license/ for more details.

  **************************** Revision History ****************************
  *	$Log$
  **************************** Revision History ****************************/

#if !defined(_PA_ECS_H)
#define _PA_ECS_H

class Saturn;

///
/// This class simulates the cabin pressure regulator in the CSM.
/// \ingroup InternalSystems
/// \brief cabin pressure regulator.
///
class CabinPressureRegulator {

public:
	CabinPressureRegulator();
	virtual ~CabinPressureRegulator();

	void Init(h_Pipe *p);
	void SystemTimestep(double simdt);
	void Close();
	void Reset();
	void SetPressurePSI(double p);
	void SetMaxFlowLBH(double f);
	void ResetMaxFlow();
	void LoadState(char *line);
	void SaveState(FILEHANDLE scn);

protected:
	h_Pipe *pipe;

	bool closed;
};


///
/// This class simulates the oxygen demand regulator in the CSM.
/// \ingroup InternalSystems
/// \brief O2 demand regulator.
///
class O2DemandRegulator {

public:
	O2DemandRegulator();
	virtual ~O2DemandRegulator();

	void Init(h_Pipe *p, h_Pipe *s);
	void SystemTimestep(double simdt);
	void Close();
	void Reset();
	void OpenSuitReliefValve();
	void ResetSuitReliefValve();
	void LoadState(char *line);
	void SaveState(FILEHANDLE scn);

protected:
	h_Pipe *pipe;
	h_Pipe *suitReliefValve;

	bool closed;
	bool suitReliefValveOpen;
};


///
/// This class simulates the cabin pressure relief valve and the post landing vent in the CSM.
/// \ingroup InternalSystems
/// \brief Cabin pressure relief valve.
///
class CabinPressureReliefValve {

public:
	CabinPressureReliefValve(Sound &plventsound);
	virtual ~CabinPressureReliefValve();

	void Init(h_Pipe *p, h_Pipe *i, VESSEL *v, ThumbwheelSwitch *l, CircuitBrakerSwitch *plvlv, ThreePosSwitch *plv, e_object *plpower);
	void SystemTimestep(double simdt);
	void SetLeakSize(double s);
	void SetReliefPressurePSI(double p);
	void LoadState(char *line);
	void SaveState(int index, FILEHANDLE scn);

protected:
	h_Pipe *pipe;
	h_Pipe *inlet;
	VESSEL *saturn;
	ThumbwheelSwitch *lever;
	CircuitBrakerSwitch *postLandingValve;
	ThreePosSwitch *postLandingVent;
	e_object *postLandingPower;
	Sound &postLandingVentSound;

	double leakSize;
	double reliefPressure;
};

///
/// This class simulates the suit circuit return valve in the CSM.
/// \ingroup InternalSystems
/// \brief Suit circuit return valve.
///
class SuitCircuitReturnValve {

public:
	SuitCircuitReturnValve();
	virtual ~SuitCircuitReturnValve();

	void Init(h_Pipe *p, CircuitBrakerSwitch *l);
	void SystemTimestep(double simdt);
	bool IsOpen() { return (pipe->in->open != 0); };

protected:
	h_Pipe *pipe;
	CircuitBrakerSwitch *lever;
};

///
/// This class simulates the O2 SM supply valve, the surge tank and the repress package in the CSM.
/// \ingroup InternalSystems
/// \brief O2 SM supply.
///
class O2SMSupply {

public:
	O2SMSupply();
	virtual ~O2SMSupply();

	void Init(h_Tank *o2sm, h_Tank *o2mr, h_Tank *o2st, h_Tank *o2rp, 
		      RotationalSwitch *smv, RotationalSwitch *stv, RotationalSwitch *rpv);
	void SystemTimestep(double simdt);
	void Close();
	void LoadState(char *line);
	void SaveState(FILEHANDLE scn);
	
protected:
	h_Tank *o2SMSupply;
	h_Tank *o2MainRegulator;
	h_Tank *o2SurgeTank;
	h_Tank *o2RepressPackage;
	RotationalSwitch *smSupplyValve;
	RotationalSwitch *surgeTankValve;
	RotationalSwitch *repressPackageValve;

	bool closed;
};

///
/// This class simulates the crew status (dead or alive) in the CSM.
/// \ingroup InternalSystems
/// \brief crew status.
///

#define ECS_CREWSTATUS_OK			0
#define ECS_CREWSTATUS_CRITICAL		1
#define ECS_CREWSTATUS_DEAD			2


class CrewStatus {

public:
	CrewStatus(Sound &crewdeadsound);
	virtual ~CrewStatus();
	void Init(Saturn *s);
	void Timestep(double simdt);
	int GetStatus() { return status; };
	void LoadState(char *line);
	void SaveState(FILEHANDLE scn);

protected:
	int status;
	double suitPressureLowTime;
	double suitPressureHighTime;
	double suitTemperatureTime;
	double suitCO2Time;
	double accelerationTime;
	double lastVerticalVelocity;

	Saturn *saturn;
	Sound &crewDeadSound;
	bool firstTimestepDone;
};

#endif // _PA_ECS_H