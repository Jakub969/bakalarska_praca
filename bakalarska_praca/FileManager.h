#pragma once
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include <algorithm>
#include "mysql_connection.h"
#include "mysql_driver.h"
#include "cppconn/driver.h"
#include "cppconn/statement.h"

class FileManager
{
public:
    FileManager();
    std::map<std::string, std::string> parseSMFile(const std::string& filename);
    void insertDataToDB(const std::map<std::string, std::string>& data);
    std::string trim(const std::string& str);
    void processFilesInDirectory(const std::filesystem::path& path);
};

