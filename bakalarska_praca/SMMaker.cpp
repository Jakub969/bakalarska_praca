#include "SMMaker.h"
#include <sstream>
#include <fstream>
#include "mysql_connection.h"
#include "mysql_driver.h"
#include "cppconn/driver.h"
#include "cppconn/statement.h"
#include <filesystem>

Song SMMaker::bestFit(std::tuple<double, double, double> result)
{
    sql::mysql::MySQL_Driver* driver;
    sql::Connection* con;

    driver = sql::mysql::get_mysql_driver_instance();
    con = driver->connect("sql.endora.cz:3311", "jakubhrubizna", "Jakub1");

    con->setSchema("jakubhrubizna");
    double analyzedBpm = std::get<0>(result);
    double analyzedOffset = std::get<1>(result);
    double analyzedSampleLength = std::get<2>(result);
    // TODO 
    // Ak sa nenajde v danom rozptyle BPM tak zmen toleranciu a vyhladaj znova
    //double analyzedBpm = 77.4969;
    //double analyzedOffset = 0;
    //double analyzedSampleLength = 221.356;

    double bpmTolerance = 1;
    double offsetTolerance = 0.5;

    std::stringstream query;
    query << "SELECT * FROM songs WHERE "
        << "ABS(SUBSTRING_INDEX(bpms, '=', -1) - " << analyzedBpm << ") < " << bpmTolerance << " AND "
        << "ABS(offset - " << analyzedOffset << ") < " << offsetTolerance;


    //BPM: 163.651 offset: 0 sampleLength: 215.528
    try {
        sql::Statement* stmt = con->createStatement();
        sql::ResultSet* res = stmt->executeQuery(query.str());

        std::vector<Song> songs;

        while (res->next()) {
            Song song(
                res->isNull("title") ? "null" : res->getString("title"),
                res->isNull("subtitle") ? "null" : res->getString("subtitle"),
                res->isNull("artist") ? "null" : res->getString("artist"),
                res->isNull("titletranslit") ? "null" : res->getString("titletranslit"),
                res->isNull("subtitletranslit") ? "null" : res->getString("subtitletranslit"),
                res->isNull("artisttranslit") ? "null" : res->getString("artisttranslit"),
                res->isNull("genre") ? "null" : res->getString("genre"),
                res->isNull("credit") ? "null" : res->getString("credit"),
                res->isNull("banner") ? "null" : res->getString("banner"),
                res->isNull("background") ? "null" : res->getString("background"),
                res->isNull("lyricspath") ? "null" : res->getString("lyricspath"),
                res->isNull("cdtitle") ? "null" : res->getString("cdtitle"),
                res->isNull("music") ? "null" : res->getString("music"),
                res->isNull("offset") ? "null" : res->getString("offset"),
                res->isNull("samplestart") ? "null" : res->getString("samplestart"),
                res->isNull("samplelength") ? "null" : res->getString("samplelength"),
                res->isNull("selectable") ? "null" : res->getString("selectable"),
                res->isNull("bpms") ? "null" : res->getString("bpms"),
                res->isNull("stops") ? "null" : res->getString("stops"),
                res->isNull("bgchanges") ? "null" : res->getString("bgchanges"),
                res->isNull("keysounds") ? "null" : res->getString("keysounds"),
                res->isNull("attacks") ? "null" : res->getString("attacks"),
                res->isNull("beginner_notes") ? "null" : res->getString("beginner_notes"),
                res->isNull("easy_notes") ? "null" : res->getString("easy_notes"),
                res->isNull("medium_notes") ? "null" : res->getString("medium_notes"),
                res->isNull("hard_notes") ? "null" : res->getString("hard_notes"),
                res->isNull("challenge_notes") ? "null" : res->getString("challenge_notes")
            );
            songs.push_back(song);
            std::cout << "Song: " << res->getString("title") << " bpm: " << res->getString("bpms") << " offset: " << res->getString("offset") << " samplelength: " << res->getString("samplelength") << "\n";
        }


        Song bestMatch;
        double smallestDifference = 9999.99;

        for (const auto& song : songs) {
            double bpmDifference = std::abs(std::stod(song.getBpms().substr(song.getBpms().find('=') + 1)) - analyzedBpm);
            double offsetDifference = std::abs(std::stod(song.getOffset()) - analyzedOffset);

            // Vypoèítaný celkový rozdiel na základe všetkých kritérií
            double totalDifference = bpmDifference + offsetDifference;

            if (totalDifference < smallestDifference) {
                smallestDifference = totalDifference;
                bestMatch = song;
            }
        }

        std::cout << "The best match is: " << bestMatch.getBpms() << "\n";

        delete res;
        delete stmt;
        return bestMatch;
    }
    catch (sql::SQLException& e) {
        std::cerr << "# ERR: SQLException in " << __FILE__;
        std::cerr << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
        std::cerr << "# ERR: " << e.what();
        std::cerr << " (MySQL error code: " << e.getErrorCode();
        std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
    }
    return Song();
}

void SMMaker::createSMfile(Song song, std::string path)
{
    size_t start = path.find_last_of("/") + 1;
    size_t end = path.find_last_of(".");
    std::string filename = path.substr(start, end - start);
    std::string directoryPath = path;
    if (end != 0) {
        std::string directoryPath = path.substr(0, start);
    }
    std::filesystem::create_directory(directoryPath + "/" + filename);
    std::ofstream file(directoryPath + "/" + filename + "/" + filename + ".sm");
    std::filesystem::copy(path, directoryPath + "/" + filename + "/" + filename + ".mp3");

    file << "#TITLE:" << filename << ";\n";
    file << "#SUBTITLE:;\n";
    song.getArtist() == "null" ? file << "#ARTIST:Jakub;\n" : file << "#ARTIST:" << song.getArtist() << ";\n";
    song.getTitleTranslit() == "null" ? file << "#TITLETRANSLIT:;\n" : file << "#TITLETRANSLIT:" << song.getTitleTranslit() << ";\n";
    song.getSubtitleTranslit() == "null" ? file << "#SUBTITLETRANSLIT:;\n" : file << "#SUBTITLETRANSLIT:" << song.getSubtitleTranslit() << ";\n";
    song.getArtistTranslit() == "null" ? file << "#ARTISTTRANSLIT:;\n" : file << "#ARTISTTRANSLIT:" << song.getArtistTranslit() << ";\n";
    song.getGenre() == "null" ? file << "#GENRE:;\n" : file << "#GENRE:" << song.getGenre() << ";\n";
    song.getCredit() == "null" ? file << "#CREDIT:;\n" : file << "#CREDIT:" << song.getCredit() << ";\n";
    song.getBanner() == "null" ? file << "#BANNER:;\n" : file << "#BANNER:" << song.getBanner() << ";\n";
    song.getBackground() == "null" ? file << "#BACKGROUND:;\n" : file << "#BACKGROUND:" << song.getBackground() << ";\n";
    song.getLyricsPath() == "null" ? file << "#LYRICSPATH:;\n" : file << "#LYRICSPATH:" << song.getLyricsPath() << ";\n";
    song.getCdTitle() == "null" ? file << "#CDTITLE:;\n" : file << "#CDTITLE:" << song.getCdTitle() << ";\n";
    song.getMusic() == "null" ? file << "#MUSIC:;\n" : file << "#MUSIC:" << song.getMusic() << ";\n";
    song.getOffset() == "null" ? file << "#OFFSET:;\n" : file << "#OFFSET:" << song.getOffset() << ";\n";
    song.getSampleStart() == "null" ? file << "#SAMPLESTART:;\n" : file << "#SAMPLESTART:" << song.getSampleStart() << ";\n";
    song.getSampleLength() == "null" ? file << "#SAMPLELENGTH:;\n" : file << "#SAMPLELENGTH:" << song.getSampleLength() << ";\n";
    song.getSelectable() == "null" ? file << "#SELECTABLE:;\n" : file << "#SELECTABLE:" << song.getSelectable() << ";\n";
    song.getBpms() == "null" ? file << "#BPMS:;\n" : file << "#BPMS:" << song.getBpms() << ";\n";
    song.getStops() == "null" ? file << "#STOPS:;\n" : file << "#STOPS:" << song.getStops() << ";\n";
    song.getBgChanges() == "null" ? file << "#BGCHANGES:;\n" : file << "#BGCHANGES:" << song.getBgChanges() << ";\n";
    song.getKeySounds() == "null" ? file << "#KEYSOUNDS:;\n" : file << "#KEYSOUNDS:" << song.getKeySounds() << ";\n";
    song.getAttacks() == "null" ? file << "#ATTACKS:;\n" : file << "#ATTACKS:" << song.getAttacks() << ";\n";
    if (song.getBeginnerNotes() != "null") file << "#NOTES:\n" << song.getBeginnerNotes() << ";\n";
    if (song.getEasyNotes() != "null") file << "#NOTES:\n" << song.getEasyNotes() << ";\n";
    if (song.getMediumNotes() != "null") file << "#NOTES:\n" << song.getMediumNotes() << ";\n";
    if (song.getHardNotes() != "null") file << "#NOTES:\n" << song.getHardNotes() << ";\n";
    if(song.getChallengeNotes() != "null") file << "#NOTES:\n" << song.getChallengeNotes() << ";\n";

    file.close();
}
