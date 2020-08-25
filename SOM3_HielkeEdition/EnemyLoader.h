#pragma once
#include "Enemy.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

//This class will load in enemies and initialize them based on their tyep
//And return the enemy
class ResourceManager;
class EnemyLoader
{
public:
	EnemyLoader();
	~EnemyLoader();

	Enemy* LoadEnemy(const std::string& _fileName,ResourceManager* _res);

private:
	Enemy* LoadRabite(std::ifstream& _filestream, ResourceManager* _res);

	std::vector<AnimationClip> GetClips(std::ifstream& _filestream, ResourceManager* _res);

};

