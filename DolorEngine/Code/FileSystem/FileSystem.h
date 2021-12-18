#pragma once

#include <string>
#include <filesystem>
#include <shared_mutex>
#include <iostream>
#include <map>

#include <windows.h>
#include "crc32.h"
//#include "../EntityManager.h"

typedef std::shared_mutex Lock;
typedef std::unique_lock<Lock>  WriteLock;
typedef std::shared_lock<Lock>  ReadLock;

class EntityManager;

class FileSystem
{
public:
#if defined(_WINDOWS)
	enum : char { e_cNativeSlash = '\\', e_cNonNativeSlash = '/' };
#else
	enum : char { e_cNativeSlash = '/', e_cNonNativeSlash = '\\' };
#endif

	FileSystem();
	~FileSystem();

	const std::string& GetMediaRoot();
	const std::string& GetScriptsRoot();
	const std::string& GetSavesRoot();
	void  CreateInitMapOfFiles();
	void Update(EntityManager* entityManager);
	std::map<std::filesystem::path, std::filesystem::file_time_type> m_mapFileReport;

private:
	std::string m_strMediaRoot;
	std::string m_strScriptsRoot;
	std::string m_strSavesRoot;
	
	Lock m_RWLock;
};