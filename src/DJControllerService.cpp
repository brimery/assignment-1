#include "DJControllerService.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>

DJControllerService::DJControllerService(size_t cache_size)
    : cache(cache_size) {}
/**
 * TODO: Implement loadTrackToCache method
 */
int DJControllerService::loadTrackToCache(AudioTrack& track) {
    if(cache.contains(track.get_title())){
        cache.get(track.get_title());
        return 1;
    }else{
        PointerWrapper<AudioTrack> wrapped = track.clone();
        if(!wrapped){
            std::cout << "[DJControllerService] Error: Track cloning failed for " 
              << track.get_title() << "\n";
            return 0; 
        }
        wrapped->load();
        wrapped->analyze_beatgrid();
        bool evicted=cache.put(std::move(wrapped));
        if (evicted){
            return -1;
        }else{
            return 0;
        }
    }
    // Placeholder
}

void DJControllerService::set_cache_size(size_t new_size) {
    cache.set_capacity(new_size);
}
//implemented
void DJControllerService::displayCacheStatus() const {
    std::cout << "\n=== Cache Status ===\n";
    cache.displayStatus();
    std::cout << "====================\n";
}

/**
 * TODO: Implement getTrackFromCache method
 */
AudioTrack* DJControllerService::getTrackFromCache(const std::string& track_title) {
    // Your implementation here
    return cache.get(track_title);

    // Placeholder
}
