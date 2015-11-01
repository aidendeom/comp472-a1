#include "Animation.h"

#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>

using std::string;
using std::ofstream;
using std::ifstream;
using std::istringstream;
using std::runtime_error;
using std::getline;

auto Animation::saveToFile(string filename) -> void
{
	ofstream file{ filename, std::ios::trunc };
	
	for (size_t i = 0; i < keyframes.size(); i++)
	{
		file << i << ' ';
		file << keyframes[i] << '\n';
	}
}

auto Animation::loadFromFile(string filename) -> Animation
{
	Animation anim;

	ifstream file{ filename };

	string line;
	while (getline(file, line))
	{
		istringstream ss{ line };

		// First number is the index in the file
		int idx;
		ss >> idx;

		AnimationKeyFrame f;
		// Push back a identity rot first
		f.orientations.push_back(Quatf::identity);

		Quatf orientation;
		while (ss >> orientation)
		{
			f.orientations.push_back(orientation);
		}

		anim.keyframes.push_back(f);
	}

	return anim;
}