#include "musicplayer.h"

MusicPlayer::MusicPlayer(QObject* parent) : QObject(parent), player(), playlist() {

}

void MusicPlayer::playTitleTheme() {
    playlist.clear();
    playlist.addMedia(QUrl("qrc:/songs/David_Szesztay_-_Sweet_Water.mp3"));
    //playlist.setPlaybackMode(QMediaPlaylist::Loop);
    playlist.setCurrentIndex(1);

    player.setVolume(50);
    player.setPlaylist(&playlist);
    player.play();
}

void MusicPlayer::playGameTheme() {
    playlist.clear();

    //adding 60 copies of song to replicate looping for an hour
    for(int i=0; i<60; i++) {
        playlist.addMedia(QUrl("qrc:/songs/Podington_Bear_-_Frogs.mp3"));
        playlist.addMedia(QUrl("qrc:/songs/Podington_Bear_-_Frogs - Copy.mp3"));
    }

    //playlist.setPlaybackMode(QMediaPlaylist::Loop);
    playlist.setCurrentIndex(1);

    player.setVolume(50);
    player.setPlaylist(&playlist);
    player.play();
}

void MusicPlayer::pause() {
    player.pause();
}

void MusicPlayer::resume() {
    player.play();
}
