#include "Animation.h"

#include <string>
#include <fstream>
#include <stdexcept>

using std::string;
using std::ofstream;
using std::runtime_error;

auto Animation::saveToFile(string filename) -> void
{
	ofstream file{ filename, std::ios::trunc };
	
	for (size_t i = 0; i < keyframes.size(); i++)
	{
		file << i << ' ';
		file << keyframes[i] << '\n';
	}
}