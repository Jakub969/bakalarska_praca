#include "FileManager.h"

FileManager::FileManager() {

}

std::map<std::string, std::string> FileManager::parseSMFile(const std::string& filename) {
    std::map<std::string, std::string> data;
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Nepodarilo sa otvorit subor.");
    }

    std::string line;
    bool isNotesSection = false;
    std::string notes;
    std::string notes_type;
    while (std::getline(file, line)) {
        // Ked sa zacnu #NOTES zacneme do notes nacitat riadky, kym nenarazime na ‘;’
        if (line.find("#NOTES:") != std::string::npos) {
            isNotesSection = true;
            continue;
        }
        if (isNotesSection) {
            if (line.find(";") != std::string::npos) {
                isNotesSection = false;
                data[notes_type] = notes;
                notes_type.clear();
                notes.clear();
            }
            else {
                notes += line + "\n";
                if (trim(line) == "Beginner:") {
                    notes_type = "beginner_notes";
                }
                else if (trim(line) == "Easy:") {
                    notes_type = "easy_notes";
                }
                else if (trim(line) == "Medium:") {
                    notes_type = "medium_notes";
                }
                else if (trim(line) == "Hard:") {
                    notes_type = "hard_notes";
                }
                else if (trim(line) == "Challenge:") {
                    notes_type = "challenge_notes";
                }
                continue;
            }
        }
        std::istringstream is_line(line);
        std::string key;
        if (std::getline(is_line, key, ':')) {
            //zmenenie pismen na male
            std::transform(key.begin(), key.end(), key.begin(), ::tolower);
            //odstranenie '#'
            key.erase(std::remove(key.begin(), key.end(), '#'), key.end());

            std::string value;
            if (std::getline(is_line, value, ';')) {
                std::string escaped_value = value;
                size_t pos = 0;
                while ((pos = escaped_value.find('\'', pos)) != std::string::npos) {
                    escaped_value.replace(pos, 1, "''");
                    pos += 2; //Presunut za vlozeny znak ''
                }
                data[key] = escaped_value;
            }
        }
    }

    return data;
}

void FileManager::insertDataToDB(const std::map<std::string, std::string>& data) {

    sql::mysql::MySQL_Driver* driver;
    sql::Connection* con;

    driver = sql::mysql::get_mysql_driver_instance();
    con = driver->connect("sql.endora.cz:3311", "jakubhrubizna", "Jakub1");

    con->setSchema("jakubhrubizna");

    sql::Statement* stmt = con->createStatement();

    std::string columns;
    std::string values;
    std::string where;

    for (const auto& kv : data) {
        if (!kv.second.empty()) {
            columns += kv.first + ",";
            values += "'" + kv.second + "',";
            where += kv.first + " = '" + kv.second + "' AND ";
        }
    }

    //Odstranenie "," na konci
    columns.pop_back();
    values.pop_back();
    where = where.substr(0, where.length() - 5); //Odstranenie posledneho 'AND'

    std::string query = "INSERT INTO songs (" + columns + ") "
        "SELECT * FROM (SELECT " + values + ") AS tmp "
        "WHERE NOT EXISTS ("
        "SELECT 1 FROM songs "
        "WHERE " + where +
        ") LIMIT 1;";

    try {
        stmt->execute(query);
    }
    catch (sql::SQLException& e) {
        std::cerr << "# ERR: SQLException in " << __FILE__;
        std::cerr << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
        std::cerr << "# ERR: " << e.what();
        std::cerr << " (MySQL error code: " << e.getErrorCode();
        std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
    }


    delete stmt;
    delete con;
}


std::string FileManager::trim(const std::string& str) {
    size_t first = str.find_first_not_of(' ');
    if (std::string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

void FileManager::processFilesInDirectory(const std::filesystem::path& path)
{
    if (!std::filesystem::exists(path) || !std::filesystem::is_directory(path)) {
        std::cout << "Cesta neexistuje alebo nie je adresar.\n";
        return;
    }
    for (const auto& entry : std::filesystem::recursive_directory_iterator(path)) {
        if (entry.is_regular_file() && entry.path().extension() == ".sm") {
            std::map<std::string, std::string> data = parseSMFile(entry.path().string());
            insertDataToDB(data);
        }
    }
}
