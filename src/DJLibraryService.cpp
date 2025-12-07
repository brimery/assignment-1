#include "DJLibraryService.h"
#include "SessionFileParser.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>
#include <filesystem>


DJLibraryService::DJLibraryService(const Playlist& playlist) 
    : playlist(playlist), library() {}
/**
 * @brief Load a playlist from track indices referencing the library
 * @param library_tracks Vector of track info from config
 */
DJLibraryService::~DJLibraryService(){
    for(AudioTrack* track:library){
        delete track;
    }
}
void DJLibraryService::buildLibrary(const std::vector<SessionConfig::TrackInfo>& library_tracks) {
    library.clear();

    for (const auto& t : library_tracks) {
        if (t.type == "MP3") {
            bool has_tags = (t.extra_param2 != 0);
            MP3Track* mp = new MP3Track(
                t.title,
                t.artists,
                t.duration_seconds,
                t.bpm,
                t.extra_param1,
                has_tags
            );
            library.push_back(mp);
            //std::cout << "MP3Track created: " << mp->get_bitrate() << " kbps\n";
        }
        else if (t.type == "WAV") {
            WAVTrack* wav = new WAVTrack(
                t.title,
                t.artists,
                t.duration_seconds,
                t.bpm,
                t.extra_param1,
                t.extra_param2
            );
            library.push_back(wav);
            //std::cout << "WAVTrack created: "<< wav->get_sample_rate() << "Hz/" << wav->get_bit_depth() << "bit\n";
        }
    }

    std::cout << "[INFO] Track library built: "<< library.size() << " tracks loaded\n";
}


/**
 * @brief Display the current state of the DJ library playlist
 * 
 */
void DJLibraryService::displayLibrary() const {
    std::cout << "=== DJ Library Playlist: " 
              << playlist.get_name() << " ===" << std::endl;

    if (playlist.is_empty()) {
        std::cout << "[INFO] Playlist is empty.\n";
        return;
    }

    // Let Playlist handle printing all track info
    playlist.display();

    std::cout << "Total duration: " << playlist.get_total_duration() << " seconds" << std::endl;
}

/**
 * @brief Get a reference to the current playlist
 * 
 * @return Playlist& 
 */
Playlist& DJLibraryService::getPlaylist() {
    // Your implementation here
    return playlist;
}

/**
 * TODO: Implement findTrack method
 * 
 * HINT: Leverage Playlist's find_track method
 */
AudioTrack* DJLibraryService::findTrack(const std::string& track_title) {
    // Your implementation here
    return playlist.find_track(track_title); // Placeholder
}

void DJLibraryService::loadPlaylistFromIndices(const std::string& playlist_name, 
                                               const std::vector<int>& track_indices) {
    // Your implementation here
    std:: cout<< "[INFO] Loading playlist: " << playlist_name << std::endl;
    playlist=Playlist(playlist_name);
    for(size_t index :track_indices){
        if(index<=0 || index > library.size()){
            std::cout << "[WARNING] Invalid track index: " << index << std::endl;
        }else{
            PointerWrapper<AudioTrack> copy= library[index-1]->clone();
            AudioTrack* raw = copy.release();
            if(!raw){
                std::cout << "[ERROR] Failed to clone track at index: " << index << std::endl;
                continue;
            }
            raw->load();
            raw->analyze_beatgrid();
            playlist.add_track(raw);
        }

    }
    std::cout << "[INFO] Playlist loaded: " 
          << playlist.get_name()
          << " (" << playlist.get_track_count() << " tracks)" 
          << std::endl;


}
/**
 * TODO: Implement getTrackTitles method
 * @return Vector of track titles in the playlist
 */
std::vector<std::string> DJLibraryService::getTrackTitles() const {
    std::vector<std::string> titles;
    for (AudioTrack* track : playlist.getTracks()) {
        if (track) {
            titles.insert(titles.begin(), track->get_title());
        }
    }
    // Your implementation here
    return titles; // Placeholder
}
