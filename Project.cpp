//The headers
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib> // for rand() and srand()
#include <ctime>// for time()
#include <fstream>
using namespace std;
//Screen attributes
const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 584;
const int SCREEN_BPP = 32;
const int SQUARE_WIDTH = 57;//ferari
const int SQUARE_HEIGHT = 99;
bool singleplay = false;
bool twoplay = false;
bool againstcomp = false;
bool start = false;
bool instruct = false;
bool high_score = false;
bool goback = false;
bool quit = false;
int score = 0;
float speed;
std::stringstream SCORE;
std::stringstream HIGHSCORE;
//offset
SDL_Rect clip;
SDL_Rect clips[13];

//The frames per second
const int FRAMES_PER_SECOND = 200;

//The surfaces
SDL_Surface *buttonPage=NULL;
SDL_Surface *restart=NULL;
SDL_Surface *startscreen=NULL;
SDL_Surface *instruction=NULL;
SDL_Surface *singleplayer=NULL;
SDL_Surface *twoplayer=NULL;
SDL_Surface *againstcomputer=NULL;
SDL_Surface *newgame=NULL;
SDL_Surface *message = NULL;
SDL_Surface *road = NULL;
SDL_Surface *screen = NULL;
SDL_Surface *square = NULL;
SDL_Surface *obst=NULL;
SDL_Surface *Score=NULL;
SDL_Surface *HScore=NULL;
//The event structure
SDL_Event event;

//The font
TTF_Font *font = NULL;
TTF_Font *score_font=NULL;
TTF_Font *headfont=NULL;
//The color of the font
SDL_Color textColor = { 0xEE, 0, 0 };
SDL_Color titleColor = {0xFF, 0xFF, 0xFF };
//The button
class Button
{
    private:
    //The attributes of the button
    SDL_Rect box;

    public:
    //Initialize the variables
    Button( int x, int y, int w, int h );

    //Handles events
    void handle_events(bool &var);

    void press_button(SDL_Surface* button , SDL_Rect* clip = NULL, SDL_Rect* clip1 = NULL );
};
//The square
class BOX
{
    private:
    //The collision box of the square
    SDL_Rect box;

    //The velocity of the square
    int xVel, yVel;

    public:
    //Initializes the variables
    BOX();
    BOX(int w,int h);

    //Takes key presses and adjusts the square's velocity
    void handle_input();

    //Moves the square
    void move();

    //Shows the square on the screen
    void show(SDL_Surface *sq);
    //auto updates poition of square
    void update_obstacle();
    //checks collision between two objects of the class
    bool check_collision(BOX B);
};
//The timer
class Timer
{
    private:
    //The clock time when the timer started
    int startTicks;

    //The ticks stored when the timer was paused
    int pausedTicks;

    //The timer status
    bool paused;
    bool started;

    public:
    //Initializes variables
    Timer();

    //The various clock actions
    void start();
    void stop();
    void pause();
    void unpause();

    //Gets the timer's time
    int get_ticks();

    //Checks the status of the timer
    bool is_started();
    bool is_paused();
};

SDL_Surface *load_image( std::string filename )
{
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;

    //The optimized surface that will be used
    SDL_Surface* optimizedImage = NULL;

    //Load the image
    loadedImage = IMG_Load( filename.c_str() );

    //If the image loaded
    if( loadedImage != NULL )
    {
        //Create an optimized surface
        optimizedImage = SDL_DisplayFormat( loadedImage );

        //Free the old surface
        SDL_FreeSurface( loadedImage );

        //If the surface was optimized
        if( optimizedImage != NULL )
        {
            //Color key surface
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0xFF, 0xFF, 0xFF ) );
        }
    }

    //Return the optimized surface
    return optimizedImage;
}
SDL_Surface *opaqueload_image( std::string filename )
{
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;

    //The optimized surface that will be used
    SDL_Surface* optimizedImage = NULL;

    //Load the image
    loadedImage = IMG_Load( filename.c_str() );

    //If the image loaded
    if( loadedImage != NULL )
    {
        //Create an optimized surface
        optimizedImage = SDL_DisplayFormat( loadedImage );

        //Free the old surface
        SDL_FreeSurface( loadedImage );

    }

    //Return the optimized surface
    return optimizedImage;
}
void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
{
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}

bool init()
{
    clip.x=136;
    clip.y=292;
    clip.h=584;
    clip.w=1000;
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }

    //Set up the screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    //If there was an error in setting up the screen
    if( screen == NULL )
    {
        return false;
    }

    //Initialize SDL_ttf
    if( TTF_Init() == -1 )
    {
        return false;
    }

    //Set the window caption
    SDL_WM_SetCaption( "HIGHWAY RACER", NULL );

    //If everything initialized fine
    return true;
}
void set_clips()
{
    //Clip the button sheet
    clips[0].x=0;
    clips[0].y=0;
    clips[0].w=300;
    clips[0].h=75;

    clips[1].x=300;
    clips[1].y=0;
    clips[1].w=300;
    clips[1].h=75;

    clips[2].x=0;
    clips[2].y=75;
    clips[2].w=300;
    clips[2].h=75;

    clips[3].x=300;
    clips[3].y=75;
    clips[3].w=300;
    clips[3].h=75;

    clips[4].x=600;
    clips[4].y=0;
    clips[4].w=300;
    clips[4].h=75;

    clips[5].x=600;
    clips[5].y=75;
    clips[5].w=300;
    clips[5].h=75;

    clips[6].x=0;
    clips[6].y=150;
    clips[6].w=300;
    clips[6].h=75;

    clips[7].x=300;
    clips[7].y=150;
    clips[7].w=300;
    clips[7].h=75;

    clips[8].x=600;
    clips[8].y=150;
    clips[8].w=300;
    clips[8].h=75;

    clips[9].x=0;
    clips[9].y=225;
    clips[9].w=300;
    clips[9].h=75;

    clips[10].x=300;
    clips[10].y=225;
    clips[10].w=300;
    clips[10].h=75;

    clips[11].x=600;
    clips[11].y=225;
    clips[11].w=300;
    clips[11].h=75;

    clips[12].x=0;
    clips[12].y=300;
    clips[12].w=300;
    clips[12].h=75;

}
Button::Button( int x, int y, int w, int h )
{
    //Set the button's attributes
    box.x = x;
    box.y = y;
    box.w = w;
    box.h = h;
}

void Button::handle_events(bool &var)
{
    //The mouse offsets
    int x = 0, y = 0;
    //If a mouse button was pressed
    if( event.type == SDL_MOUSEBUTTONDOWN )
    {
        //If the left mouse button was pressed
        if( event.button.button == SDL_BUTTON_LEFT )
        {
            //Get the mouse offsets
            x = event.button.x;
            y = event.button.y;

            //If the mouse is over the button
            if( ( x > box.x ) && ( x < box.x + box.w ) && ( y > box.y ) && ( y < box.y + box.h ) )
            {
                var=true;
            }
        }
    }
}

void Button::press_button(SDL_Surface* button , SDL_Rect* clip, SDL_Rect* clip1)
{
     //The mouse offsets
    int x = 0, y = 0;

    //If the mouse moved
    if( event.type == SDL_MOUSEMOTION )
    {
        //Get the mouse offsets
        x = event.motion.x;
        y = event.motion.y;

        //If the mouse is over the button
        if( ( x > box.x ) && ( x < box.x + box.w ) && ( y > box.y ) && ( y < box.y + box.h ) )
        {
            //Show press button
            apply_surface( box.x, box.y, buttonPage, screen, clip);

        }
         //If not
        else
        {
            apply_surface( box.x, box.y, button, screen, clip1);
        }
    }
}
bool load_files()
{
    //Load buttonPage
    buttonPage = opaqueload_image("buttonPage.png");
    //Load singleplayer
    singleplayer = opaqueload_image("singleplayer.png");
    //Load instruction
    instruction = opaqueload_image("instruction.png") ;
    //Load newgame
    newgame = opaqueload_image("newgame.png") ;
    //Load restart
    restart = opaqueload_image("restart.png") ;
    //Load startscreen
    startscreen = load_image("startscreen.png") ;
    //load road
    road = load_image("road.png") ;
    //Load the square image
    square = load_image( "carfin1.png" );
    //Load the obstacle
    obst=load_image("block.png");
     //If there was a problem in loading
    if( square == NULL|| obst==NULL|| startscreen==NULL|| road==NULL  )
    {
        return false;
    }


    //Open the font
    font = TTF_OpenFont( "Capture_it.ttf", 100 );
    score_font=TTF_OpenFont("score.ttf",45);
    headfont= TTF_OpenFont( "score.ttf", 80 );
    //If there was an error in loading the font
    if( font == NULL|| score_font==NULL || headfont==NULL )
    {
        return false;
    }

    //If everything loaded fine
    return true;
}
bool BOX::check_collision(BOX B )
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = box.x;
    rightA = box.x + box.w;
    topA = box.y;
    bottomA =box.y + box.h;

    //Calculate the sides of rect B
    leftB = B.box.x;
    rightB = B.box.x + B.box.w;
    topB = B.box.y;
    bottomB = B.box.y + B.box.h;

    //If any of the sides from A are outside of B
    if( bottomA <= topB )
    {
        return false;
    }

    if( topA >= bottomB )
    {
        return false;
    }

    if( rightA <= leftB )
    {
        return false;
    }

    if( leftA >= rightB )
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}

void clean_up()
{
    //Free the surfaces
    SDL_FreeSurface( buttonPage );
    SDL_FreeSurface( singleplayer );
    SDL_FreeSurface( instruction );
    SDL_FreeSurface( newgame );
    SDL_FreeSurface( restart );
    SDL_FreeSurface( startscreen );
    SDL_FreeSurface( road );
    SDL_FreeSurface( square );
    SDL_FreeSurface(obst);
    SDL_FreeSurface(message);
    SDL_FreeSurface(Score);
    //Close the font
    TTF_CloseFont( font );
    TTF_CloseFont(score_font);
    TTF_CloseFont(headfont);
    //Quit SDL_ttf
    TTF_Quit();

    //Quit SDL
    SDL_Quit();
}
BOX::BOX()
{
    //Initialize the offsets
    box.x = 439;
    box.y = 584-SQUARE_HEIGHT;

    //Set the square's dimentions
    box.w = SQUARE_WIDTH;
    box.h = SQUARE_HEIGHT;

    //Initialize the velocity
    xVel = 0;
    yVel = 0;
}
BOX::BOX(int w, int h)
{
    box.x=(int)((((float)(rand())/(float)(RAND_MAX)))*(400.0))+272 ;
    box.y= 0;
    box.w=w;
    box.h=h;
    xVel=0;
    yVel=speed;
}

void BOX::handle_input()
{
    //If a key was pressed
    if( event.type == SDL_KEYDOWN )
    {
        //Adjust the velocity
        switch( event.key.keysym.sym )
        {

            case SDLK_LEFT: xVel -= SQUARE_WIDTH / 7; break;
            case SDLK_RIGHT: xVel += SQUARE_WIDTH / 7; break;
        }
    }
    //If a key was released
    else if( event.type == SDL_KEYUP )
    {
        //Adjust the velocity
        switch( event.key.keysym.sym )
        {


            case SDLK_LEFT: xVel += SQUARE_WIDTH / 7; break;
            case SDLK_RIGHT: xVel -= SQUARE_WIDTH / 7; break;
        }
    }
}

void BOX::move()
{//move the square up or down
    box.y+=yVel;
    //Move the square left or right
    box.x += xVel;

    //If the square went too far to the left or right or has collided with the wall
    if( ( box.x<272)||(box.x>275+450-57))
    {
        //Move back
        box.x -= xVel;
    }
}

void BOX::show(SDL_Surface *sq)
{
    //Show the square
    apply_surface( box.x, box.y, sq, screen );
}

void BOX::update_obstacle()
{
    yVel=speed;
    move();
    if(box.y>584)
    {
        box.x=(int)((((float)(rand())/(float)(RAND_MAX)))*(400.0))+ 272 ;
        box.y=0;
        score+=10;
    }
}
Timer::Timer()
{
    //Initialize the variables
    startTicks = 0;
    pausedTicks = 0;
    paused = false;
    started = false;
}

void Timer::start()
{
    //Start the timer
    started = true;

    //Unpause the timer
    paused = false;

    //Get the current clock time
    startTicks = SDL_GetTicks();
}

void Timer::stop()
{
    //Stop the timer
    started = false;

    //Unpause the timer
    paused = false;
}

void Timer::pause()
{
    //If the timer is running and isn't already paused
    if( ( started == true ) && ( paused == false ) )
    {
        //Pause the timer
        paused = true;

        //Calculate the paused ticks
        pausedTicks = SDL_GetTicks() - startTicks;
    }
}

void Timer::unpause()
{
    //If the timer is paused
    if( paused == true )
    {
        //Unpause the timer
        paused = false;

        //Reset the starting ticks
        startTicks = SDL_GetTicks() - pausedTicks;

        //Reset the paused ticks
        pausedTicks = 0;
    }
}

int Timer::get_ticks()
{
    //If the timer is running
    if( started == true )
    {
        //If the timer is paused
        if( paused == true )
        {
            //Return the number of ticks when the timer was paused
            return pausedTicks;
        }
        else
        {
            //Return the current time minus the start time
            return SDL_GetTicks() - startTicks;
        }
    }

    //If the timer isn't running
    return 0;
}

bool Timer::is_started()
{
    return started;
}

bool Timer::is_paused()
{
    return paused;
}

int main( int argc, char* args[] )
{
    ofstream ofile("Singleplayer.txt",ios::app);
    do
    {
        score=0;
        srand(time(0)*(time(0))); // set initial seed value to system clock
        int i=0; speed=3;
        //Keep track of the current frame
        int frame = 0;
        //The square
        BOX mySquare;
        //obstacle
        BOX myobstacle(49,27);

        //Whether or not to cap the frame rate
        bool cap = true;

        //The frame rate regulator
        Timer fps;
        //Initialize
        if( init() == false )
        {
            return 1;
        }

        //Load the files
        if( load_files() == false )
        {
            return 1;
        }
        set_clips();
        Button new_game( 10,150, 300, 75 );
        Button restart_game(350,325,300,75);
        Button instruc(10,275,300,75);
        Button highscr(10,400,300,75);
        Button single_player(10,150,300,75);
        Button two_player(10,275,300,75);
        Button against_comp(10,400,300,75);
        Button Back(348,359,300,75);
        do
        {
            start = false;
            instruct=false;
            high_score=false;
            goback=false;
            quit = false;
            singleplay = false;
            twoplay = false;
            againstcomp = false;
            apply_surface(0,0,startscreen,screen,NULL);
            message=TTF_RenderText_Solid(headfont,"HIGHWAY MOTOR RACING",titleColor);
            apply_surface((SCREEN_WIDTH-message->w)/2,10,message,screen);
            apply_surface(10,150,newgame,screen);
            apply_surface(10,275,instruction,screen);
            apply_surface(10,400,buttonPage,screen,&clips[6]);
            //Update the screen
            if( SDL_Flip( screen ) == -1 )
            {
                return 1;
            }
            while(start == false && quit==false && instruct==false && high_score==false)
            {
                if( SDL_PollEvent( &event ) )
                {
                    //Handle button events
                    new_game.press_button(newgame,&clips[0]);

                    instruc.press_button(instruction,&clips[2]);

                    highscr.press_button(buttonPage,&clips[5],&clips[6]);

                    new_game.handle_events(start);

                    instruc.handle_events(instruct);

                    highscr.handle_events(high_score);

                    if( event.type == SDL_QUIT )
                    {
                        //Quit the program
                        quit = true;
                    }
                }
                if( SDL_Flip( screen ) == -1 )
                {
                    return 1;
                }
            }
            if(start==true||instruct==true)
            {
                apply_surface(0,0,startscreen,screen);
                apply_surface((SCREEN_WIDTH-message->w)/2,10,message,screen);
                apply_surface(10,150,buttonPage,screen,&clips[7]);
                apply_surface(10,275,buttonPage,screen,&clips[8]);
                apply_surface(10,400,buttonPage,screen,&clips[11]);
                if( SDL_Flip( screen ) == -1 )
                {
                    return 1;
                }
                while(singleplay==false&&twoplay==false&&againstcomp==false&&quit== false)
                {
                    if( SDL_PollEvent( &event ) )
                    {
                        single_player.press_button(buttonPage,&clips[9],&clips[7]);
                        two_player.press_button(buttonPage,&clips[10],&clips[8]);
                        against_comp.press_button(buttonPage,&clips[12],&clips[11]);
                        single_player.handle_events(singleplay);
                        if( event.type == SDL_QUIT )
                        {
                        //Quit the program
                        quit = true;
                        }
                    }
                    if( SDL_Flip( screen ) == -1 )
                    {
                        return 1;
                    }
                }
            }
            if(high_score==true)
            {
                apply_surface(0,0,startscreen,screen);
                apply_surface((SCREEN_WIDTH-message->w)/2,10,message,screen);
                apply_surface(10,150,buttonPage,screen,&clips[7]);
                if( SDL_Flip( screen ) == -1 )
                {
                    return 1;
                }
                while(singleplay==false&&quit== false)
                {
                    if( SDL_PollEvent( &event ) )
                    {
                        single_player.press_button(buttonPage,&clips[9],&clips[7]);
                        single_player.handle_events(singleplay);
                        if( event.type == SDL_QUIT )
                        {
                        //Quit the program
                        quit = true;
                        }
                    }
                    if( SDL_Flip( screen ) == -1 )
                    {
                        return 1;
                    }
                }
            }
            if(instruct==true&&singleplay==true)
            {
                apply_surface(0,0,singleplayer,screen);
                if( SDL_Flip( screen ) == -1 )
                {
                    return 1;
                }
            }
            if(high_score==true&&singleplay==true)
            {
                apply_surface(0,0,startscreen,screen);
                message=TTF_RenderText_Solid(headfont,"HIGHSCORE:",titleColor);
                apply_surface((SCREEN_WIDTH-message->w)/2,10,message,screen);
                apply_surface(348,359,buttonPage,screen,&clips[4]);
                ifstream ifile("Singleplayer.txt");
                int input,highscore;
                highscore=0;
                while(ifile>>input)
                {
                    if(highscore<input)
                    {
                        highscore=input;
                    }
                }
                HIGHSCORE.str("");
                HIGHSCORE.clear();
                HIGHSCORE<<highscore;
                HScore=TTF_RenderText_Solid(headfont,HIGHSCORE.str().c_str(),titleColor);
                apply_surface((SCREEN_WIDTH-HScore->w)/2,100,HScore,screen);
                ifile.close();
            }
            while(instruct==true&&goback==false&&quit==false)
            {
                if( SDL_PollEvent( &event ) )
                {
                    Back.press_button(buttonPage,&clips[3],&clips[4]);
                    Back.handle_events(goback);
                    if( event.type == SDL_QUIT )
                    {
                        //Quit the program
                        quit = true;
                    }
                }
                if( SDL_Flip( screen ) == -1 )
                {
                    return 1;
                }
            }
            while(high_score==true&&goback==false&&quit==false)
            {
                if( SDL_PollEvent( &event ) )
                {
                    Back.press_button(buttonPage,&clips[3],&clips[4]);
                    Back.handle_events(goback);
                    if( event.type == SDL_QUIT )
                    {
                        //Quit the program
                        quit = true;
                    }
                }
                if( SDL_Flip( screen ) == -1 )
                {
                    return 1;
                }
            }
        }
        while(goback==true&&quit==false);
        //While the user hasn't quit
        quit=false;
        while( quit == false && start == true && singleplay == true)
        {
            //Start the frame timer
            fps.start();
            //update obstacle
            myobstacle.update_obstacle();
            //While there's events to handle
            if ( SDL_PollEvent( &event ) )
            {
                //Handle events for the square
                mySquare.handle_input();

                //If a key was pressed
                if( event.type == SDL_KEYDOWN )
                {
                    //If enter was pressed
                    if( event.key.keysym.sym == SDLK_RETURN )
                    {
                        //Turn frame rate regulator on/off
                        cap = ( !cap );
                    }
                }

                //If the user has Xed out the window
                else if( event.type == SDL_QUIT )
                {
                    //Quit the program
                    quit = true;
                }
            }

            //modify offset
            clip.y=876-i;
            i=((int)(i+speed)%876);
            //modify speed
            if(speed<13)
            speed = speed + 0.0023-0.00011*speed;

            //Move the square
            mySquare.move();
            //update score
            SCORE.str("");
            SCORE.clear();
            SCORE<<"SCORE: "<<score;
            Score=TTF_RenderText_Solid(score_font,SCORE.str().c_str(),textColor);
            //Apply the road
            apply_surface( 0, 0, road, screen,&clip );
            //Show the car on the screen
            mySquare.show(square);
            //show obstacle
            myobstacle.show(obst);
            //show score
            apply_surface(0,0,Score,screen);
            //check collision between car and obstacle
            if( mySquare.check_collision(myobstacle))
            {
                quit= true;
            }

            //Update the screen
            if( SDL_Flip( screen ) == -1 )
            {
                return 1;
            }

            //Increment the frame counter
            frame++;

            //If we want to cap the frame rate
            if( ( cap == true ) && ( fps.get_ticks() < 1000 / FRAMES_PER_SECOND ) )
            {
                //Sleep the remaining frame time
                SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks() );
            }
        }
        //after game gets over
        if(quit==true)
        {
            message=TTF_RenderText_Solid(font,"GAME OVER",textColor);
            apply_surface((SCREEN_WIDTH-message->w)/2,SCREEN_HEIGHT/4-message->h/2,message,screen);
            apply_surface(350,325,restart,screen);
            //Update the screen
            if( SDL_Flip( screen ) == -1 )
            {
                return 1;
            }

            quit = false;
            while (quit==false&&goback==false)
            {
                if ( SDL_PollEvent( &event ) )
                {
                    restart_game.press_button(restart,&clips[1]);
                    restart_game.handle_events(goback);

                    if( event.type == SDL_QUIT)
                    {
                        quit=true;
                    }
                }
                if( SDL_Flip( screen ) == -1 )
                {
                    return 1;
                }
            }
        }
        ofile<<score<<endl;
    }
    while(goback==true&&quit==false);
    //Clean up
    clean_up();
    ofile.close();
    return 0;
}

