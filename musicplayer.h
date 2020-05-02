#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QMediaPlayer>
#include <QMediaPlaylist>

class MusicPlayer : public QObject {
    Q_OBJECT
public:
    //creates a music player that contains music relevant to project
    //@param parent - pointer to parent object
    MusicPlayer(QObject* parent = nullptr);

public slots:
    //plays title theme one time
    void playTitleTheme();

    //loops theme song for main game screen
    void playGameTheme();

    //pauses player
    void pause();

    //resumes playing the current song
    void resume();

private:
    QMediaPlayer player;
    QMediaPlaylist playlist;
};

#endif // MUSICPLAYER_H
