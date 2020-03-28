#ifndef _GAME_H_
#define _GAME_H_

#include "header/GameOptions.h"
#include "header/GameAnimate.h"
#include "header/GameSprite.h"
#include "header/GamePlayer.h"
#include "header/GameEntity.h"
#include "header/GameEntity.h"

class Game
{
    public:
        Game();
        ~Game();

        void Run();
    private:
        void Load();
        void Render();
        void Update(Uint32 GameTime);
        void Event(SDL_Event e);
        void Loop();

        bool running;

        SDL_Renderer* Global_Renderer;
        SDL_Window* Global_Window;

        GameSprites* BackGround;
        vector< GamePlayer* > Charactors;

        GamePlayer* MainCharactor;
        int Charactors_id;
        int screen_pos_x;
        int screen_pos_y;
};

#endif // _GAME_H_


