#ifndef GAME_H
#define GAME_H

class QKeyEvent;

class Game
{
public:

    enum KEY_ACTIONS
    {
        ACTION_MOVE_FASTER = 0,
        ACTION_MOVE_SLOWER,
        ACTION_MOVE_UP,
        ACTION_MOVE_DOWN,
        ACTION_MOVE_LEFT,
        ACTION_MOVE_RIGHT,
        ACTION_FIRE_PRIMARY,
        ACTION_FIRE_SECONDARY,
        ACTION_COUNT
    };

    Game();

    void init();

    void run();

    void processKeyEvents( QKeyEvent *evt, bool pressed );

protected:
    bool mActionTriggered[ACTION_COUNT];
    int mActionMap[ACTION_COUNT];
};

#endif // GAME_H
