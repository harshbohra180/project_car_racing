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
bool quit1 = false;
int score = 0;
float speed;
bool collision1=false;
bool collision2=false;
bool collision3=false;
bool win=true,fire=false;
std::stringstream SCORE;
std::stringstream HIGHSCORE;
std::stringstream BULLETCOUNT;
int health1=168;
int health2=168;
int ammo=2;
//offset
SDL_Rect clip;
SDL_Rect clip2;//for healthbar 1
SDL_Rect clip3;//for health bar 2
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
SDL_Surface *square1 = NULL;
SDL_Surface *obst=NULL;
SDL_Surface *Score=NULL;
SDL_Surface *HScore=NULL;
SDL_Surface *HealthBar=NULL;
SDL_Surface *Finishline=NULL;
SDL_Surface *bullet=NULL;
SDL_Surface *bulletcount=NULL;
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
    public:
    //The collision box of the square
    SDL_Rect box;

    //The velocity of the square
    int xVel, yVel;

    public:
    //Initializes the variables
    BOX(int x);
    BOX(int w,int h,int x,int y);
    //Takes key presses and adjusts the square's velocity
    void handle_input();
    void handle_input1();

    //Moves the square
    void move();

    //Shows the square on the screen
    void show(SDL_Surface *sq);
    //auto updates poition of square
    void update_obstacle();
    //checks collision between two objects of the class
    void update_car(BOX other_car);
    void update_car1();
    void update_car1(BOX &other_car );
    bool check_collision(BOX B);
    void handle_AI(BOX,BOX);
    void handle_finishline();
    bool handle_ammo(BOX);
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
    clip2.x=0;
    clip2.y=0;
    clip2.h=24;
    clip2.w=health1;//rectangle for healthbar1 car1
    clip3.x=0;
    clip3.y=0;
    clip3.h=24;
    clip3.w=health2;
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
    //Load twoplayer
    twoplayer = opaqueload_image("twoplayer.png");
    //Load againstcomp
    againstcomputer = opaqueload_image("againstcomp.png");
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
    square1 = load_image( "carfin2.png" );
    //Load the obstacle
    obst=load_image("block.png");
    //Load the Health bar
    HealthBar=load_image("healthbar.png");
    //Load the FINISHLINE
    Finishline=load_image("FINISHLINE.png");
    //Load the Bullet
    bullet=load_image("bullet.png");
     //If there was a problem in loading
    if( square == NULL|| square1==NULL|| obst==NULL|| startscreen==NULL|| road==NULL || bullet==NULL )
    {
        return false;
    }


    //Open the font
    font = TTF_OpenFont( "Capture_it.ttf", 80 );
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
    SDL_FreeSurface( twoplayer );
    SDL_FreeSurface( againstcomputer );
    SDL_FreeSurface( instruction );
    SDL_FreeSurface( newgame );
    SDL_FreeSurface( restart );
    SDL_FreeSurface( startscreen );
    SDL_FreeSurface( road );
    SDL_FreeSurface( square );
    SDL_FreeSurface( square1 );
    SDL_FreeSurface( obst);
    SDL_FreeSurface( message);
    SDL_FreeSurface( Score);
    SDL_FreeSurface( HScore);
    SDL_FreeSurface( HealthBar);
    SDL_FreeSurface( bullet);
    SDL_FreeSurface( Finishline);
    SDL_FreeSurface( bulletcount);
    //Close the font
    TTF_CloseFont( font );
    TTF_CloseFont(score_font);
    TTF_CloseFont(headfont);
    //Quit SDL_ttf
    TTF_Quit();

    //Quit SDL
    SDL_Quit();
}
BOX::BOX(int x)
{
    //Initialize the offsets
    box.x = x;
    box.y = 584-SQUARE_HEIGHT;

    //Set the square's dimentions
    box.w = SQUARE_WIDTH;
    box.h = SQUARE_HEIGHT;

    //Initialize the velocity
    xVel = 0;
    yVel = 0;
}
BOX::BOX(int w, int h, int x=(int)((((float)(rand())/(float)(RAND_MAX)))*(400.0))+272 , int y=0)
{
    box.x=x ;
    box.y=y;
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
            case SDLK_UP : yVel-= SQUARE_HEIGHT/30; break;
            case SDLK_DOWN : yVel += SQUARE_HEIGHT/30; break;
            if(againstcomp==true)
            case SDLK_SPACE:fire=true;break;
        }
    }
    //If a key was released
    else if( event.type == SDL_KEYUP && (xVel !=0 || yVel!=0) )
    {
        //Adjust the velocity
        switch( event.key.keysym.sym )
        {

            case SDLK_LEFT: xVel += SQUARE_WIDTH / 7; break;
            case SDLK_RIGHT: xVel -= SQUARE_WIDTH / 7; break;
            case SDLK_UP : yVel+= SQUARE_HEIGHT/30; break;
            case SDLK_DOWN : yVel -= SQUARE_HEIGHT/30; break;
            if(againstcomp==true)
            case SDLK_SPACE:fire=true;break;
        }
    }
}
void BOX::handle_AI(BOX obstacle,BOX car)

{
    //constant to and fro motion of computer driven car
    if(box.x>275+450-66)
    {
        xVel=-1;
    }
    if(box.x<280)
    {
        xVel=1;
    }

    if(box.y>584-127)
    {
        yVel=-1;
    }
    if(box.y<250)
    {
        yVel=1;
    }
    //moving in opposite direction to obstacle
    if (box.x>= obstacle.box.x && ((box.x-obstacle.box.x)<60) )
    {
        xVel = 4;
    }
    else
    if(box.x<obstacle.box.x && ((obstacle.box.x-box.x)<60) )
    {
        xVel = -4;
    }

    //avoid getting stuck at end of road
    if( obstacle.box.x-272<58 && box.x<obstacle.box.x+3)
    {
        xVel=4;
    }
    if( 275+450-obstacle.box.x<50+58 && box.x>obstacle.box.x-3)
    {
        xVel=-4;
    }
}
void BOX::handle_input1()
{
    //If a key was pressed
    if( event.type == SDL_KEYDOWN )
    {
        //Adjust the velocity
        switch( event.key.keysym.sym )
        {

            case SDLK_a: xVel -= SQUARE_WIDTH / 7; break;
            case SDLK_d: xVel += SQUARE_WIDTH / 7; break;
            case SDLK_w : yVel-= SQUARE_HEIGHT/30; break;
            case SDLK_s : yVel += SQUARE_HEIGHT/30; break;
        }
    }
    //If a key was released
    else if( event.type == SDL_KEYUP && (xVel !=0 || yVel!=0) )
    {
        //Adjust the velocity
        switch( event.key.keysym.sym )
        {


            case SDLK_a: xVel += SQUARE_WIDTH / 7; break;
            case SDLK_d: xVel -= SQUARE_WIDTH / 7; break;
            case SDLK_w : yVel+= SQUARE_HEIGHT/30; break;
            case SDLK_s : yVel -= SQUARE_HEIGHT/30; break;
        }
    }
}
bool BOX::handle_ammo(BOX mySquare)
{
    yVel=-8;
    move();
    return check_collision(mySquare);
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

void BOX::update_car(BOX other_car)
{   //move the square up or down
    box.y+=yVel;
    //Move the square left or right
    box.x += xVel;

    //If the square went too far to the left or right or has collided with the wall
    if( ( box.x<272)||(box.x>275+450-57) )
    {
        //Move back
        box.x -= xVel;
    }
    if( box.y<0||box.y >584-SQUARE_HEIGHT)
    {
        box.y -=yVel;
    }

    if((collision3))
    {
        if(box.x<other_car.box.x)
            box.x-=11;
        else
            box.x+=11;
        if(box.y<other_car.box.y)
            box.y-=8;
        else
            box.y+=8;
        if (box.x<272)
            box.x=272;
        if (box.x>275+450-57)
            box.x=275+450-57;
        if( box.y<0)
            box.y = 0;
        if (box.y >584-SQUARE_HEIGHT)
            box.y =485;

        yVel=0;
        xVel=0;
    }
}
void BOX::update_car1(BOX &other_car)
{   //move the square up or down
    box.y+=yVel;
    //Move the square left or right
    box.x += xVel;

    //If the square went too far to the left or right or has collided with the wall
    if( ( box.x<272)||(box.x>275+450-57) )
    {
        //Move back
        box.x -= xVel;
    }
    if( box.y<0||box.y >584-SQUARE_HEIGHT)
    {
        box.y -=yVel;
    }

    if((collision3) )
    {
        if(box.x<other_car.box.x )
           {

                box.x-=2;
                other_car.box.x+=10;
           }
        else
        {
            box.x+=10;
            other_car.box.x-=13;
        }
        if(box.y<other_car.box.y)
        {
            box.y-=10;
            other_car.box.y+=11;
        }
        else
         {
            box.y+=10;
            other_car.box.y-=11;
         }
        if (box.x<272)
            box.x=272;
        if (other_car.box.x<272)
            other_car.box.x=272;
        if (box.x>275+450-57)
            box.x=275+450-57;
        if (other_car.box.x>275+450-57)
            other_car.box.x=275+450-57;
        if( box.y<0)
            box.y = 0;
        if (other_car.box.y<0)
            other_car.box.y=0;
        if (box.y >584-SQUARE_HEIGHT)
            box.y =485;
        if (other_car.box.y>584-SQUARE_HEIGHT)
            other_car.box.y=485;

        yVel=0;
        xVel=0;

    }
}
void BOX::update_car1()
{
    //move the square up or down
    box.y+=yVel;
    //Move the square left or right
    box.x += xVel;

    //If the square went too far to the left or right or has collided with the wall
    if( ( box.x<272)||(box.x>275+450-57) )
    {
        //Move back
        box.x -= xVel;
    }
    if( box.y<0||box.y >584-SQUARE_HEIGHT)
    {
        box.y -=yVel;
    }
}
void BOX::update_obstacle()
{
    yVel=speed;
    move();
    if(box.y>584||collision1==true||collision2==true)
    {
        box.x=(int)((((float)(rand())/(float)(RAND_MAX)))*(400.0))+ 272 ;
        box.y=0;
        score+=10;
        if(collision1==true)
        score-=10;
        collision1=false;
        collision2=false;
    }
}
void BOX::handle_finishline()
{
    box.x=320;
    yVel=speed;
    move();
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
        health1=168;
        health2=168;
        ammo=2;
        collision1=false;
        collision2=false;
        collision3=false;
        win=true;
        fire=false;
        srand(time(0)*(time(0))); // set initial seed value to system clock
        int i=0; speed=3;int ctr=0;
        //Keep track of the current frame
        int frame = 0;
        //The square
        BOX mysquare(439);
        BOX mySquare(272);
        BOX mySquareAI(272);
        BOX mySquare1(275+450-57);

        //obstacle
        BOX myobstacle(45,55);
        BOX myfinishline(354,90,450+275,0);
        mySquareAI.xVel=1;
        BOX mybullet(5,10);

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
        Button Back(348,489,300,75);
        do
        {
            start = false;
            instruct=false;
            high_score=false;
            goback=false;
            quit = false;
            quit1 = false;
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
                        two_player.handle_events(twoplay);
                        against_comp.handle_events(againstcomp);
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
                apply_surface(348,489,buttonPage,screen,&clips[4]);
                if( SDL_Flip( screen ) == -1 )
                {
                    return 1;
                }
            }
            else if(instruct==true&&twoplay==true)
            {
                apply_surface(0,0,twoplayer,screen);
                apply_surface(348,489,buttonPage,screen,&clips[4]);
                if( SDL_Flip( screen ) == -1 )
                {
                    return 1;
                }
            }
            else if(instruct==true&&againstcomp==true)
            {
                apply_surface(0,0,againstcomputer,screen);
                apply_surface(348,489,buttonPage,screen,&clips[4]);
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
                apply_surface(348,489,buttonPage,screen,&clips[4]);
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
                mysquare.handle_input();

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
            mysquare.update_car(mysquare);
            //update score
            SCORE.str("");
            SCORE.clear();
            SCORE<<"SCORE: "<<score;
            Score=TTF_RenderText_Solid(score_font,SCORE.str().c_str(),textColor);
            //Apply the road
            apply_surface( 0, 0, road, screen,&clip );
            //Show the car on the screen
            mysquare.show(square);
            //show obstacle
            myobstacle.show(obst);
            //show score
            apply_surface(0,0,Score,screen);
            //check collision between car and obstacle
            if( mysquare.check_collision(myobstacle))
            {
                health1 -= 21;
                clip2.y +=24;
                clip2.w=health1;
                if(health1==0)
                quit=true;
                collision1=true;
            }

            apply_surface(5,100,HealthBar,screen,&clip2);
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
        ofile<<score<<endl;
        while( quit == false && start == true && twoplay == true)
        {
            //Start the frame timer
            fps.start();
            //update obstacle
            myobstacle.update_obstacle();

            //While there's events to handle
            if ( SDL_PollEvent( &event ) )
            {
                //Handle events for the square
                mySquare.handle_input1();
                mySquare1.handle_input();
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
            //for collision between two cars
            collision3=mySquare1.check_collision(mySquare);
            //update car
            mySquare.update_car(mySquare1);
            mySquare1.update_car(mySquare);
            apply_surface( 0, 0, road, screen,&clip );
            //Show the car on the screen
            mySquare.show(square);
            mySquare1.show(square1);
            //show obstacle
            myobstacle.show(obst);
            //check collision between car and obstacle
            if( mySquare.check_collision(myobstacle))
            {
                health1 -= 21;
                clip2.y +=24;
                clip2.w=health1;
                if(health1==0)
                quit=true;
                collision1=true;
            }
            if( mySquare1.check_collision(myobstacle))
            {
                health2 -= 21;
                clip3.y +=24;
                clip3.w=health2;
                if(health2==0)
                quit=true;
                collision2=true;
            }
            message=TTF_RenderText_Solid(score_font,"PLAYER 1:",textColor);
            apply_surface(0,0,message,screen);
            message=TTF_RenderText_Solid(score_font,"PLAYER 2:",textColor);
            apply_surface(800,0,message,screen);
            apply_surface(5,100,HealthBar,screen,&clip2);
            apply_surface(800,100,HealthBar,screen,&clip3);

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
        while( quit == false && start == true && againstcomp == true && quit1 == false)
        {
            //Start the frame timer
            fps.start();
            //update obstacle
            if(score<1000)
            myobstacle.update_obstacle();
            if(score>=1000)
            myfinishline.handle_finishline();
            if((score/100) > ctr)
            {
                ammo ++;ctr++;
            }
            //While there's events to handle
            if ( SDL_PollEvent( &event ) )
            {

                //Handle events for the square
                mySquare1.handle_input();
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
                    quit1 = true;
                }
            }

            //modify offset
            clip.y=876-i;
            i=((int)(i+speed)%876);
            //modify speed
            if(speed<13)
            speed = speed + 0.0023-0.00011*speed;
            //for collision between two cars
            collision3=mySquare1.check_collision(mySquareAI);
            //update car
            mySquare1.update_car1(mySquareAI);
            mySquareAI.update_car1();
            if (fire==false)
            {
                mybullet.box.x=(mySquare1.box.x)+(SQUARE_WIDTH-5)/2;mybullet.box.y=(mySquare1.box.y+10);
            }
            //update score
            SCORE.str("");
            SCORE.clear();
            SCORE<<"DISTANCE LEFT:"<<(1000-score)<<"m";
            Score=TTF_RenderText_Solid(score_font,SCORE.str().c_str(),textColor);
            //Apply the road
            apply_surface( 0, 0, road, screen,&clip );
            //Show the car on the screen
            mySquareAI.show(square);
            mySquare1.show(square1);
            //show obstacle
            if(score<1000)
            myobstacle.show(obst);
            if(score>=1000)
            myfinishline.show(Finishline);
            //show score
            apply_surface(0,0,Score,screen);

            //check collision between car and obstacle
            if( mySquareAI.check_collision(myobstacle))
            {
                health1 -= 21;
                clip2.y +=24;
                clip2.w=health1;
                if(health1==0)
                quit=true;
                collision1=true;
            }
            mySquareAI.handle_AI(myobstacle,mySquare1);
            if( mySquare1.check_collision(myobstacle))
            {
                health2 -= 21;
                clip3.y +=24;
                clip3.w=health2;
                if(health2==0)
                quit=true;
                collision2=true;
            }
            mySquare1.check_collision(mySquareAI);
            if (mySquareAI.check_collision(myfinishline))
            {
                quit=true;
                win=false;
            }
            if (mySquare1.check_collision(myfinishline))
            {
                quit=true;
                win=true;
            }
            if(mySquareAI.check_collision(myfinishline)&&mySquare1.check_collision(myfinishline))
            {
                quit=true;
                if(health1>health2)
                win=false;
                else
                win=true;
            }
            if(fire)
            {
                if(ammo>0)
                {
                    if( mybullet.handle_ammo(mySquareAI))
                    {
                        health1 -= 21;
                        clip2.y +=24;
                        clip2.w=health1;
                        if(health1==0)
                        quit=true;
                        fire=false;
                        ammo--;
                    }
                    if(mybullet.box.y<0)
                    {   ammo--;
                        fire=false;
                    }
                    mybullet.show(bullet);
                }
                else
                fire=false;
            }
            BULLETCOUNT.str("");
            BULLETCOUNT.clear();
            BULLETCOUNT<<"BULLET COUNT:"<<ammo;
            bulletcount=TTF_RenderText_Solid(score_font,BULLETCOUNT.str().c_str(),textColor);
            apply_surface(750,0,bulletcount,screen);
            message=TTF_RenderText_Solid(score_font,"COMPUTER:",textColor);
            apply_surface(0,250,message,screen);
            message=TTF_RenderText_Solid(score_font,"YOU:",textColor);
            apply_surface(800,250,message,screen);
            apply_surface(5,300,HealthBar,screen,&clip2);
            apply_surface(800,300,HealthBar,screen,&clip3);

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
        if(quit1==true)
        {
            message=TTF_RenderText_Solid(font,"GAME OVER",textColor);
            apply_surface((SCREEN_WIDTH-message->w)/2,SCREEN_HEIGHT/4-message->h/2,message,screen);
            apply_surface(350,325,restart,screen);
            //Update the screen
            if( SDL_Flip( screen ) == -1 )
            {
                return 1;
            }
            quit1=false;
            while (quit1==false&&goback==false)
            {
                if ( SDL_PollEvent( &event ) )
                {
                    restart_game.press_button(restart,&clips[1]);
                    restart_game.handle_events(goback);

                    if( event.type == SDL_QUIT)
                    {
                        quit1=true;
                    }
                }
                if( SDL_Flip( screen ) == -1 )
                {
                    return 1;
                }
            }
        }
        if(quit==true)
        {
            if(singleplay==true)
            {
                message=TTF_RenderText_Solid(font,"GAME OVER",textColor);
                apply_surface((SCREEN_WIDTH-message->w)/2,SCREEN_HEIGHT/4-message->h/2,message,screen);
            }
            if(twoplay==true)
            {
                if(health1>health2)
                message=TTF_RenderText_Solid(font,"PLAYER 1 WINS",textColor);
                else if(health1<health2)
                message=TTF_RenderText_Solid(font,"PLAYER 2 WINS",textColor);
                else
                message=TTF_RenderText_Solid(font,"TIE",textColor);
                apply_surface((SCREEN_WIDTH-message->w)/2,SCREEN_HEIGHT/4-message->h/2,message,screen);
            }
            if(againstcomp==true)
            {
                if(health2==0||!(win))
                message=TTF_RenderText_Solid(font,"COMPUTER WINS",textColor);
                else
                message=TTF_RenderText_Solid(font,"YOU WIN",textColor);
                apply_surface((SCREEN_WIDTH-message->w)/2,SCREEN_HEIGHT/4-message->h/2,message,screen);
            }
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

    }
    while(goback==true&&quit==false);
    //Clean up
    clean_up();
    ofile.close();
    return 0;
}


