#include "MixingEngineService.h"
#include <iostream>
#include <memory>


/**
 * TODO: Implement MixingEngineService constructor
 */
MixingEngineService::MixingEngineService()
    : decks(), active_deck(1), auto_sync(false), bpm_tolerance(0)
{
    decks[0]=nullptr;
    decks[1]=nullptr;
    std::cout << "[MixingEngineService] Initialized with 2 empty decks.\n";
    // Your implementation here
}

/**
 * TODO: Implement MixingEngineService destructor
 */
MixingEngineService::~MixingEngineService() {
    std::cout << "[MixingEngineService] Cleaning up decks...\n";
    delete decks[0];
    delete decks[1];
    decks[0]=nullptr;
    decks[1]=nullptr;
    // Your implementation here
}


/**
 * TODO: Implement loadTrackToDeck method
 * @param track: Reference to the track to be loaded
 * @return: Index of the deck where track was loaded, or -1 on failure
 */
int MixingEngineService::loadTrackToDeck(const AudioTrack& track) {
    std::cout<< "\n=== Loading Track to Deck ===\n";
    PointerWrapper<AudioTrack> to_load=track.clone();
    if(!to_load){
        std::cout << "[ERROR] Track: \"" << track.get_title() << "\" failed to clone\n";
        return -1;
    }
    int target=1-active_deck;
    if(decks[0]==nullptr && decks[1]==nullptr){
        target=0;
    }
    if(static_cast<size_t>(target)!=active_deck){
        std::cout << "[Deck Switch] Target deck: " << target << "\n";
    }
    if(decks[target]){
        delete decks[target];
        decks[target]=nullptr;
    }
    to_load->load();
    to_load->analyze_beatgrid();
    if(decks[active_deck] && auto_sync){
        if(!can_mix_tracks(to_load)){
            sync_bpm(to_load);
        }
    }
    AudioTrack* p=to_load.release();
    decks[target]=p;
    std::cout << "[Load Complete] '" << track.get_title() << "' is now loaded on deck " << target << "\n";
    
    active_deck=target;
    std::cout <<"[Active Deck] Switched to deck "<<target<<"\n";


    // Your implementation here
    return target; // Placeholder
}

/**
 * @brief Display current deck status
 */
void MixingEngineService::displayDeckStatus() const {
    std::cout << "\n=== Deck Status ===\n";
    for (size_t i = 0; i < 2; ++i) {
        if (decks[i])
            std::cout << "Deck " << i << ": " << decks[i]->get_title() << "\n";
        else
            std::cout << "Deck " << i << ": [EMPTY]\n";
    }
    std::cout << "Active Deck: " << active_deck << "\n";
    std::cout << "===================\n";
}

/**
 * TODO: Implement can_mix_tracks method
 * 
 * Check if two tracks can be mixed based on BPM difference.
 * 
 * @param track: Track to check for mixing compatibility
 * @return: true if BPM difference <= tolerance, false otherwise
 */
bool MixingEngineService::can_mix_tracks(const PointerWrapper<AudioTrack>& track) const {
    if(!decks[active_deck]||!track){
        return false;
    }
    int bpm1=decks[active_deck]->get_bpm();
    int bpm2=track->get_bpm();
    int difference=std::abs(bpm1-bpm2);
        if(difference<=bpm_tolerance){
            return true;
        }

    // Your implementation here
    return false; // Placeholder
}

/**
 * TODO: Implement sync_bpm method
 * @param track: Track to synchronize with active deck
 */
void MixingEngineService::sync_bpm(const PointerWrapper<AudioTrack>& track) const {
    if(decks[active_deck]&&track){
        int bpm1=track->get_bpm();
        int bpm2=decks[active_deck]->get_bpm();
        int average=(bpm1+bpm2)/2;
        track->set_bpm(average);
        std::cout << "[Sync BPM] Syncing BPM from " << bpm1 << " to " << average << "\n";
    }
    // Your implementation here
}
