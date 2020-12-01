#include "EnemyLoader.h"
#include "Rabite.h"
#include "..\Engine\AnimationClip.h"
#include "..\Engine\ResourceManager.h"

using namespace std;

EnemyLoader::EnemyLoader()
{
}

EnemyLoader::~EnemyLoader()
{
}

Enemy* EnemyLoader::LoadEnemy(const std::string& _fileName,ResourceManager* _res)
{
	ifstream file(_fileName);

	Enemy* enemy = nullptr;


	if (file.bad())
	{
		throw exception("Filepath bad");
	}

	while (!file.eof())
	{
		string word;
		file >> word;

		//Figure out type and based on it 
		if (word == "EnemyType")
		{
			file >> word;
			if (word == "Rabite")
			{
				enemy =LoadRabite(file,_res);
				enemy->m_FileName = _fileName;
			}

		}


	}
	file.close();


	return enemy;
}

Enemy* EnemyLoader::LoadRabite(std::ifstream& _fs, ResourceManager* _res)
{

	Rabite* rab = new Rabite();


	while (!_fs.eof())
	{
		string word;
		_fs >> word;
		if (word == "speed")
		{
			_fs>>rab->m_Stats.speed;
		}
		if (word == "health")
		{
			_fs >> rab->m_Stats.maxHealth;
			 rab->m_Stats.health = rab->m_Stats.maxHealth;;
		}
		if (word == "damage")
		{
			_fs >> rab->m_Stats.damage;
		}
		if (word == "Object")
		{
			Object obj = Object();
			_fs >> word>>word;
			std::string texname;
			_fs>> texname;
			_fs >> word;
			_fs>>obj.m_Size.x>>word>>obj.m_Size.y;

			obj.m_RenderInterface.texture = _res->LoadTexture(texname);
			rab->m_Object = obj;
		}
		if (word == "Animations")
		{
			while (word != "}"|| !_fs.eof())
			{
				_fs >> word;
				if (word == "IDLE")
				{
					std::vector<AnimationClip> clips =GetClips(_fs, _res);
					rab->SetIdleAnimations(clips[0], clips[2], clips[1]);
				}
				if (word == "MOVING")
				{
					std::vector<AnimationClip> clips = GetClips(_fs, _res);
					rab->SetMoveAnimations(clips[0], clips[2], clips[1]);

				}
				if (word == "ATTACKING")
				{
					std::vector<AnimationClip> clips = GetClips(_fs, _res);
					rab->SetAttackAnimations(clips[0], clips[2], clips[1]);
				}


			}
		}

	}




	return rab;
}

//Returns animation clip in the following order, North =0, south = 1 east = 2 west =3
std::vector<AnimationClip> EnemyLoader::GetClips(std::ifstream& _filestream, ResourceManager* _res)
{
	std::string word;
	AnimationClip n, s, e, w;
	while (word != "}")
	{
		_filestream >> word;
		if (word == "NORTH")
		{
			_filestream >> word;
			n.LoadClipFromFile(word, _res);
		}
		if (word == "SOUTH")
		{
			_filestream >> word;
			s.LoadClipFromFile(word, _res);
		}
		if (word == "EAST")
		{
			_filestream >> word;
			e.LoadClipFromFile(word, _res);
		}
		if (word == "WEST")
		{
			_filestream >> word;
			w.LoadClipFromFile(word, _res);
		}
	}
	std::vector<AnimationClip> clips;
	clips.push_back(n);
	clips.push_back(s);
	clips.push_back(e);
	clips.push_back(w);


	return clips;
}
