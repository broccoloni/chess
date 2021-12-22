#include "IOManager.h"
#include <fstream>

bool IOManager::readFileToBuffer(std::string filepath, std::vector<unsigned char>& buffer){
	std::ifstream file(filepath, std::ios::binary);
	if (file.fail()){
		perror(filepath.c_str());
		return false;
	}
	
	//we need to determine the size of the file
	//seek to end of the file
	file.seekg(0, std::ios::end);

	//get the file size
	int fileSize = file.tellg();

	//return to start of file
	file.seekg(0, std::ios::beg);

	//remove header of file size
	fileSize -= file.tellg();

	buffer.resize(fileSize);
	file.read((char *)&(buffer[0]), fileSize);
	file.close();

	return true;
}
