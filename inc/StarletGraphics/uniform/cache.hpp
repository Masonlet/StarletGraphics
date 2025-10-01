#pragma once

class Cache {
public :
	void setProgram(unsigned int programID) { program = programID; }	
	virtual bool cacheLocations() = 0;
	virtual ~Cache() = default;

protected:
	unsigned int program{ 0 };
	bool getUniformLocation(int& location, const char* name) const;
};