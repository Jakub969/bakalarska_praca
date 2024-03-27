#pragma once
#include <iostream>
#include <string>

class Song {
private:
    std::string title;
    std::string subtitle;
    std::string artist;
    std::string titleTranslit;
    std::string subtitleTranslit;
    std::string artistTranslit;
    std::string genre;
    std::string credit;
    std::string banner;
    std::string background;
    std::string lyricsPath;
    std::string cdTitle;
    std::string music;
    std::string offset;
    std::string sampleStart;
    std::string sampleLength;
    std::string selectable;
    std::string bpms;
    std::string stops;
    std::string bgChanges;
    std::string keySounds;
    std::string attacks;
    std::string beginnerNotes;
    std::string easyNotes;
    std::string mediumNotes;
    std::string hardNotes;
    std::string challengeNotes;

public:
    Song()=default;
    Song(std::string _title, std::string _subtitle, std::string _artist,
        std::string _titleTranslit, std::string _subtitleTranslit, std::string _artistTranslit,
        std::string _genre, std::string _credit, std::string _banner, std::string _background,
        std::string _lyricsPath, std::string _cdTitle, std::string _music,
        std::string _offset, std::string _sampleStart, std::string _sampleLength,
        std::string _selectable, std::string _bpms, std::string _stops, std::string _bgChanges, std::string _keySounds,
        std::string _attacks, std::string _beginnerNotes, std::string _easyNotes, std::string _mediumNotes,
        std::string _hardNotes, std::string _challengeNotes)
        : title(_title), subtitle(_subtitle), artist(_artist),
        titleTranslit(_titleTranslit), subtitleTranslit(_subtitleTranslit), artistTranslit(_artistTranslit),
        genre(_genre), credit(_credit), banner(_banner), background(_background),
        lyricsPath(_lyricsPath), cdTitle(_cdTitle), music(_music),
        offset(_offset), sampleStart(_sampleStart), sampleLength(_sampleLength),
        selectable(_selectable), bpms(_bpms), stops(_stops), bgChanges(_bgChanges),
        keySounds(_keySounds), attacks(_attacks), beginnerNotes(_beginnerNotes),
        easyNotes(_easyNotes), mediumNotes(_mediumNotes), hardNotes(_hardNotes),
        challengeNotes(_challengeNotes) {}

    std::string getTitle() const { return title; }
    std::string getSubtitle() const { return subtitle; }
    std::string getArtist() const { return artist; }
    std::string getTitleTranslit() const { return titleTranslit; }
    std::string getSubtitleTranslit() const { return subtitleTranslit; }
    std::string getArtistTranslit() const { return artistTranslit; }
    std::string getGenre() const { return genre; }
    std::string getCredit() const { return credit; }
    std::string getBanner() const { return banner; }
    std::string getBackground() const { return background; }
    std::string getLyricsPath() const { return lyricsPath; }
    std::string getCdTitle() const { return cdTitle; }
    std::string getMusic() const { return music; }
    std::string getOffset() const { return offset; }
    std::string getSampleStart() const { return sampleStart; }
    std::string getSampleLength() const { return sampleLength; }
    std::string getSelectable() const { return selectable; }
    std::string getBpms() const { return bpms; }
    std::string getStops() const { return stops; }
    std::string getBgChanges() const { return bgChanges; }
    std::string getKeySounds() const { return keySounds; }
    std::string getAttacks() const { return attacks; }
    std::string getBeginnerNotes() const { return beginnerNotes; }
    std::string getEasyNotes() const { return easyNotes; }
    std::string getMediumNotes() const { return mediumNotes; }
    std::string getHardNotes() const { return hardNotes; }
    std::string getChallengeNotes() const { return challengeNotes; }
};
