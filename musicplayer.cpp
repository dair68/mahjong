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
