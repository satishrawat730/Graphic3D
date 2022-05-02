#pragma once

#include <string>
#include <vector>

class Constants
{
public:
	static const std::string AppName;

	static std::string TerrainHeightmapFilePath;
	static std::string TerrainBaseTexFilePath;
	static std::string TerrainDetailTexFilePath;

	static std::string TerrainGrassTexFilePath;
	static std::string TerrainGrassNormalTexFilePath;
	static std::string TerrainSlopeTexFilePath;
	static std::string TerrainSlopeNormalTexFilePath;
	static std::string TerrainRockTexFilePath;
	static std::string TerrainRockNormalTexFilePath;
	static std::vector<std::string> Errors;
	static std::string None;
	static std::string Cube;
	static std::string Monkey;
	static std::string Teapot;
	static std::string Sphere;
	static std::string PlainTexture;
	static std::string pbrBrownrock;
	static std::string pbrCarpet;
	static std::string pbrDentedMetal;
	static std::string pbrGold;
	static std::string pbrGranite;
	static std::string pbrLeather;
	static std::string pbrRustedIron;
	static std::string pbrKnife;
	static std::string pbrHelmet;	
	static std::string pbrShield;
	static std::string pbrScar;
};