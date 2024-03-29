#pragma once

#include "stdafx.h"
#include "DataFrames.h"

class Digitizer
{
public:
	Digitizer() {;}
	~Digitizer();
    void Open(CAEN_DGTZ_ConnectionType LinkType, int LinkNum, int ConetNode, uint32_t VMEBaseAddress);

    // CAEN_DGTZ_ErrorCode GetRet() {return ret;}
    CAEN_DGTZ_ErrorCode Execute(CAEN_DGTZ_ErrorCode ret);
	void Program(Dconfig dconfig);//
	DigiData ReadEvent();

	// uint16_t GetEnabledMask() {return EnabledMask;}
	// void SetEnabledMask(uint16_t enmask) {EnabledMask = enmask;}
	
	// void SetDconfig(Dconfig dconfig) {dconfig_ = dconfig;}
	// void SetRunParameters(RunParameters &runparameters, Dconfig dconfig) {runparameters_ = runparameters; dconfig_ = dconfig;}
	// RunParameters GetRunParameters() {return runparameters_;}
	
	CAEN_DGTZ_BoardInfo_t GetBoardInfo() {return BoardInfo;}
	
	void AllocateEvents();
	void StartAquisition();
	void StopAquisition();

	// int GetRunStatus() {return runparameters_.nevent;}
	int GetHandle() {return handle;}
	// friend long GetCurrentTime();
protected:
	void AndTriggger(Dconfig dconfig);//
	void OrTriggger(Dconfig dconfig);//
	void WriteRegisterBitmask(uint32_t address, uint32_t data, uint32_t mask);
	
	// void SetVerbose(bool verbose) {Verbose = verbose;}
private:
	// Dconfig dconfig_;
	// DigiData digidata_;
    // CAEN_DGTZ_ErrorCode ret;
	char *buffer = nullptr;
	char *EventPtr = nullptr;
	uint32_t BufferSize;
	uint32_t AllocatedSize;
    CAEN_DGTZ_BoardInfo_t BoardInfo;
    // int MajorNumber;
	int handle;
	// long CurrentTime;
	uint16_t EnabledMask = 0;
	uint32_t NTrigChannels = 0;
	std::vector<CAEN_DGTZ_TriggerMode_t> ChannelTriggerMode;
	std::string errors[34] = {"Operation completed successfully", //0
				  "Communication error",  //1
				  "Unspecified error", //2
				  "Invalid parameter", //3
				  "Invalid Link Type", //4
				  "Invalid device handler", //5
				  "Maximum number of devices exceeded", //6
				  "Operation not allowed on this type of board", //7
				  "The interrupt level is not allowed", //8
				  "The event number is bad", //9
				  "Unable to read the registry", //10
				  "Unable to write into the registry", //11
				  "", //12
				  "The Channel is busy", //13
				  "The channel number is invalid", //14
				  "Invalid FPIO Mode", //15
				  "Wrong acquisition mode",//16
				  "This function is not allowed for this module",//17
				  "Communication Timeout",//18
				  "The buffer is invalid", //19
				  "The event is not found", //20
				  "The event is invalid", //21
				  "Out of memory", //22
				  "Unable to calibrate the board", //23
				  "Unable to open the digitizer", //24
				  "The Digitizer is already open", //25
				  "The Digitizer is not ready to operate", //26 
				  "The Digitizer has not the IRQ configured",//27
				  "The digitizer flash memory is corrupted",//28
				  "The digitizer DPP firmware is not supported in this lib version", //29
				  "Invalid Firmware License",//30
				  "The digitizer is found in a corrupted status",//31
				  "The given trace is not supported by the digitizer", //32
				  "The given probe is not supported for the given digitizer's trace"}; //33
};