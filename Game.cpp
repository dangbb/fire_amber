#include "Game.h"

Game::Game()
{
    running = true;
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("Unable to initialize SDL: %s\n", SDL_GetError());
        running = false;
    }
    else
    {
        Global_Window = SDL_CreateWindow(game_name,
                                         SDL_WINDOWPOS_UNDEFINED,
                                         SDL_WINDOWPOS_UNDEFINED,
                                         SCREEN_WIDTH,
                                         SCREEN_HEIGHT,
                                         SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        if(Global_Window == NULL)
        {
            printf("Unable to create window: %s\n", SDL_GetError());
            running = false;
        }
        else
        {
            Global_Renderer = SDL_CreateRenderer(Global_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if(Global_Renderer == NULL)
            {
                printf("Unable to cerate renderer: %s\n", SDL_GetError());
                running = false;
            }
        }

        int flags = IMG_INIT_PNG;
        if((flags & IMG_Init(flags)) != flags)
        {
            printf("Unable to initialize IMG: $s\n", IMG_GetError());
            running = false;
        }
    }
    BackGround = NULL;
    Charactors_id = 0;

    screen_pos_x = 0;
    screen_pos_y = 0;
}

Game::~Game()
{
    SDL_DestroyWindow(Global_Window);
    delete BackGround;

    for(int i=0;i<Charactors.size();i++)
    {
        delete Charactors[i];
    }
    Charactors.clear();
}

void Game::Load()
{
    ifstream read(GAME_PATH);
    string BackGroundPath;
    read >> BackGroundPath;

    BackGround = new GameSprites(Global_Renderer, BackGroundPath);

    int player_num;
    read >> player_num;
    while(player_num--)
    {
        string PlayerPath;
        int PlayerPos_x;
        int PlayerPos_y;
        read >> PlayerPath >> PlayerPos_x >> PlayerPos_y;
        GamePlayer* Charactor = new GamePlayer(Global_Renderer, PlayerPath, PlayerPos_x, PlayerPos_y);

        Charactors.push_back(Charactor);
    }

    read >> Charactors_id;
    MainCharactor = Charactors[Charactors_id];
    read.close();
}

void Game::Render()
{
    SDL_SetRenderDrawColor(Global_Renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(Global_Renderer);

    screen_pos_x = MainCharactor->Position_x() - SCREEN_WIDTH / 2;
    screen_pos_y = MainCharactor->Position_y() - SCREEN_HEIGHT / 2;

    BackGround->Render(Global_Renderer, screen_pos_x, screen_pos_y);

    sort(Charactors.begin(), Charactors.end(), GamePlayer::cmp);

    for(int i=0;i<Charactors.size();i++)
    {
        Charactors[i]->Render(Global_Renderer, screen_pos_x, screen_pos_y);
    }

    SDL_SetRenderDrawColor(Global_Renderer, 0xFF, 0, 0, 0);

    SDL_Rect *rect = new SDL_Rect;
    rect->x = SCREEN_WIDTH / 2 - 5;
    rect->y = SCREEN_HEIGHT / 2 - 5;
    rect->h = 10;
    rect->w = 10;

    SDL_RenderFillRect(Global_Renderer, rect);

    delete rect;

    SDL_RenderPresent(Global_Renderer);
}

void Game::Update(Uint32 GameTime)
{
    BackGround->Update(GameTime);
    for(int i=0;i<Charactors.size();i++)
    {
        Charactors[i]->Update(GameTime);
    }
}

void Game::Event(SDL_Event e)
{
    if(e.type == SDL_QUIT)
    {
        running = false;
    }
    else if(e.type == SDL_KEYDOWN)
    {
        if(e.key.keysym.sym == SDLK_ESCAPE)
        {
            running = false;
        }
    }
    MainCharactor->Event(e);
}

void Game::Loop()
{

}

void Game::Run()
{
    Load();

    SDL_Event e;

    while(running)
    {
        while(SDL_PollEvent(&e))
        {
            Event(e);
        }
        Update(SDL_GetTicks());
        Loop();
        Render();
    }
}
