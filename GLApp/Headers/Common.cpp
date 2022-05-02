#include "Common.h"

const std::string Constants::AppName = "Rocket Engine App";

std::string Constants::TerrainHeightmapFilePath = "../GLApp\\Textures\\Terrain\\3.jpg";
std::string Constants::TerrainBaseTexFilePath = "../GLApp\\Textures\\Terrain\\grass.jpg";
std::string Constants::TerrainDetailTexFilePath = "../GLApp\\Textures\\Terrain\\grass.jpg";

std::string Constants::TerrainGrassTexFilePath = "../GLApp\\Textures\\Terrain\\slope_textures\\dry-green_CLR_url.jpg";
std::string Constants::TerrainGrassNormalTexFilePath = "../GLApp\\Textures\\Terrain\\slope_textures\\dry-green_NRM2x.jpg";

std::string Constants::TerrainSlopeTexFilePath = "../GLApp\\Textures\\Terrain\\slope_textures\\rocky-grass_CLR.jpg";
std::string Constants::TerrainSlopeNormalTexFilePath = "../GLApp\\Textures\\Terrain\\slope_textures\\rocky-grass_NRM.jpg";

std::string Constants::TerrainRockTexFilePath = "../GLApp\\Textures\\Terrain\\slope_textures\\rock_CLR.jpg";
std::string Constants::TerrainRockNormalTexFilePath = "../GLApp\\Textures\\Terrain\\slope_textures\\rock_NRM.jpg";

std::vector<std::string> Constants::Errors = {
												"GLFW initialization failed",
												"Unable to initialize GLFWindow"
												"ERROR: Could not set up OpenGL Context"
											};



std::string Constants::Cube = "../Models\\Wavefront\\Textured\\Cube\\Cube.obj";
std::string Constants::None = "../Models\\Wavefront\\Teapot.obj"; //to be updated
std::string Constants::Monkey = "../Models\\Wavefront\\Monkey.obj";
std::string Constants::Sphere = "../Models\\Wavefront\\Sphere.obj";
std::string Constants::Teapot = "../Models\\Wavefront\\Teapot.obj";

std::string Constants::PlainTexture = "../GLApp\\Textures\\plain.png";

std::string Constants::pbrScar		= "../Models\\Wavefront\\pbr\\scar\\scarL.obj";
std::string Constants::pbrBrownrock = "../Models\\Wavefront\\pbr\\Sphere\\brownrock.obj";
std::string Constants::pbrCarpet = "../Models\\Wavefront\\pbr\\Sphere\\carpet.obj";
std::string Constants::pbrDentedMetal = "../Models\\Wavefront\\pbr\\Sphere\\dented_metal.obj";
std::string Constants::pbrGold = "../Models\\Wavefront\\pbr\\Sphere\\gold.obj";
std::string Constants::pbrGranite = "../Models\\Wavefront\\pbr\\Sphere\\granite.obj";
std::string Constants::pbrLeather = "../Models\\Wavefront\\pbr\\Sphere\\leather.obj";
std::string Constants::pbrRustedIron = "../Models\\Wavefront\\pbr\\Sphere\\rusted iron.obj";
std::string Constants::pbrHelmet	= "../Models\\Wavefront\\pbr\\helmet\\helmet.obj";
std::string Constants::pbrShield	= "../Models\\Wavefront\\pbr\\shield\\Viking shield.obj";
std::string Constants::pbrKnife		= "../Models\\Wavefront\\pbr\\Knife\\Tactical Knife.obj";