
#include<stdio.h>

#include<SDL/SDL.h>

#include<SDL/SDL_image.h>

#include<SDL/SDL_mixer.h>

#include<math.h>


// True False Definition
#define TRUE 1
#define FALSE 0

// Constant Paramenters
#define SCREENHEIGHT 640
#define SCREENWIDTH 960
#define BUTTONHEIGHT 60
#define BUTTONWIDTH 260

// GamePlay Constants
#define TIME2DEATH 1200
#define ZOMBIENUMBER 18
#define BULLET 50
#define DIFFICULTY 1
#define ZOMBIELIFE 1
#define LEVELTIME 200
#define LEVEL 5
#define STREAK 5

// Global SDL parameters required in all functions
SDL_Surface * screen = NULL, * background = NULL, * opaque = NULL;
SDL_Rect buttonoffset;
SDL_Event event;

// For loop manipulations
int i, j;

// Functions
void initialize();
void showoff();
void main_menu();
void level_initialization();
int level(int time2death, int bullet, int zombienumber, int difficulty, int zombielife, int leveltime, int timer, int achievement[], int level_number);
int pause(int now, int time2death);
int mouse_over_rect(SDL_Rect Target);
void number_display(int x, int y, int number, int digit);
void best_display();
void bg_display(SDL_Surface * image);
void clean_up();

int main(int argc, char ** argv) {
    initialize();
    showoff();
    main_menu();
    clean_up();
}

void initialize() {
    // Start SDL
    SDL_Init(SDL_INIT_EVERYTHING);

    // Set Game Screen
    screen = SDL_SetVideoMode(SCREENWIDTH, SCREENHEIGHT, 32, SDL_SWSURFACE);
    SDL_WM_SetCaption("Zombie Invasion!!", NULL);

    // Load Global Backgrounds
    background = IMG_Load("Image/background.jpg");
    opaque = IMG_Load("Image/opaque.png");

}

void showoff()
{

    // Declare,  Load and Set Images
    SDL_Surface * intro = NULL, * logo = NULL;

    SDL_Rect logooffset;
    logooffset.h = SCREENHEIGHT;
    logooffset.w = SCREENWIDTH;
    logooffset.y = 0;

    intro = IMG_Load("Image/Game_Intro/intro_bg.jpg");
    logo = IMG_Load("Image/Game_Intro/logo.png");

    // Marquee Logo
    for (i = -SCREENWIDTH; i < SCREENWIDTH; i += 40) {
        SDL_BlitSurface(intro, NULL, screen, NULL);
        logooffset.x = i;
        SDL_BlitSurface(logo, NULL, screen, & logooffset);
        logooffset.x = -i;
        SDL_BlitSurface(logo, NULL, screen, & logooffset);
        SDL_Flip(screen);
        if (i == 0)
            bg_display(NULL);
    }

    // Free things
    SDL_FreeSurface(intro);
    SDL_FreeSurface(logo);

}

void main_menu()
{
    int flow = TRUE, buttonnumber = 1;

    // Background Music
    Mix_Music * music = NULL;
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
    music = Mix_LoadMUS("Audio/intro.ogg");
    Mix_PlayMusic(music, -1);

    // Background Images and Button loads

    SDL_Surface * menu_background = NULL, * instruction_background = NULL, * credit_background = NULL;
    SDL_Surface * button[2][5] = {
        {
            NULL,
            NULL,
            NULL,
            NULL,
            NULL
        },
        {
            NULL,
            NULL,
            NULL,
            NULL,
            NULL
        }
    };

    menu_background = IMG_Load("Image/menu.png");
    instruction_background = IMG_Load("Image/instruction_bg.jpg");
    credit_background = IMG_Load("Image/credit_bg.jpg");

    button[0][0] = IMG_Load("Image/Buttons/Menu Screen/play1.jpg");
    button[0][1] = IMG_Load("Image/Buttons/Menu Screen/highscore1.jpg");
    button[0][2] = IMG_Load("Image/Buttons/Menu Screen/instruction1.jpg");
    button[0][3] = IMG_Load("Image/Buttons/Menu Screen/credit1.jpg");
    button[0][4] = IMG_Load("Image/Buttons/Menu Screen/exit1.jpg");
    button[1][0] = IMG_Load("Image/Buttons/Menu Screen/play2.jpg");
    button[1][1] = IMG_Load("Image/Buttons/Menu Screen/highscore2.jpg");
    button[1][2] = IMG_Load("Image/Buttons/Menu Screen/instruction2.jpg");
    button[1][3] = IMG_Load("Image/Buttons/Menu Screen/credit2.jpg");
    button[1][4] = IMG_Load("Image/Buttons/Menu Screen/exit2.jpg");

    // Setting Button Offset
    buttonoffset.h = BUTTONHEIGHT;
    buttonoffset.w = BUTTONWIDTH;

    // Loop for Menu
    while (flow) {
        int press = FALSE;

        while (SDL_PollEvent( & event)) {

            if (event.key.keysym.sym == SDLK_DOWN)
                buttonnumber = buttonnumber++ % 5;
            if (event.key.keysym.sym == SDLK_UP) {
                if (buttonnumber == 1)
                    buttonnumber = 5;
                else
                    buttonnumber = buttonnumber--;
            }

            for (i = 0; i < 5; i++) {
                buttonoffset.x = 350;
                buttonoffset.y = 250 + i * 75;
                if (mouse_over_rect(buttonoffset))
                    buttonnumber = i + 1;
                if ((event.type == SDL_MOUSEBUTTONDOWN && mouse_over_rect(buttonoffset)) || event.key.keysym.sym == SDLK_RETURN)
                    press = TRUE;
            }
        }

        // Blit Background and Buttons
        SDL_BlitSurface(menu_background, NULL, screen, NULL);
        for (i = 0; i < 5; i++) {
            buttonoffset.x = 350;
            buttonoffset.y = 250 + i * 75;
            if (i == buttonnumber - 1)
                SDL_BlitSurface(button[1][i], NULL, screen, & buttonoffset);
            else
                SDL_BlitSurface(button[0][i], NULL, screen, & buttonoffset);
        }

        //Menu options

        if (press)
            if (buttonnumber == 1) {
                level_initialization();
                Mix_PlayMusic(music, -1);
            }
        else if (buttonnumber == 2)
            best_display();
        else if (buttonnumber == 3)
            bg_display(instruction_background);
        else if (buttonnumber == 4)
            bg_display(credit_background);
        else if (buttonnumber == 5)
            flow = FALSE;

        SDL_Flip(screen);

    }

    // Free things
    SDL_FreeSurface(menu_background);
    SDL_FreeSurface(instruction_background);
    SDL_FreeSurface(credit_background);
    SDL_FreeSurface( ** button);

}

void level_initialization()
{
    // Set an Intro

    Mix_Music * beep = NULL;
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
    beep = Mix_LoadMUS("Audio/beep.wav");
    Mix_PlayMusic(beep, 1);

    SDL_Surface * button[8];
    button[0] = IMG_Load("Image/Buttons/Level/ready.png");
    button[1] = IMG_Load("Image/Buttons/Level/set.png");
    button[2] = IMG_Load("Image/Buttons/Level/go.png");
    buttonoffset.x = 350;
    buttonoffset.y = 250;

    for (i = 0; i < 3; i++) {
        SDL_BlitSurface(background, NULL, screen, NULL);
        SDL_BlitSurface(opaque, NULL, screen, NULL);
        SDL_BlitSurface(button[i], NULL, screen, & buttonoffset);

        SDL_Flip(screen);
        SDL_Delay(1000);
    }

    // Run Recursive sub_level with constant parameters

    int time2death = TIME2DEATH, zombienumber = ZOMBIENUMBER, difficulty = DIFFICULTY, zombielife = ZOMBIELIFE, leveltime = LEVELTIME, bullet = BULLET * zombielife, timer = SDL_GetTicks(), score = 0, streak = 0, headshot = 0, achievement[3] = {
        score,
        streak,
        headshot
    };
    int win = level(time2death, bullet, zombienumber, difficulty, zombielife, leveltime, timer, achievement, 0);

    // Load to show Achievements

    score = achievement[0];
    streak = achievement[1];
    headshot = achievement[2];

    button[0] = IMG_Load("Image/Buttons/Achievement/game_over.png");
    button[1] = IMG_Load("Image/Buttons/Achievement/win.png");
    button[2] = IMG_Load("Image/Buttons/Achievement/score.png");
    button[3] = IMG_Load("Image/Buttons/Achievement/streak.png");
    button[4] = IMG_Load("Image/Buttons/Achievement/headshot.png");
    button[5] = IMG_Load("Image/Buttons/Achievement/new_highscore.png");
    button[6] = IMG_Load("Image/Buttons/Achievement/new_streak.png");
    button[7] = IMG_Load("Image/Buttons/Achievement/most_headshot.png");

    // Set and Blit Backgrounds and Buttons
    beep = Mix_LoadMUS("Audio/energy_off.wav");
    Mix_PlayMusic(beep, 1);
    SDL_BlitSurface(background, NULL, screen, NULL);
    SDL_BlitSurface(opaque, NULL, screen, NULL);

    buttonoffset.x = 350;
    buttonoffset.y = 100;
    SDL_BlitSurface(button[win], NULL, screen, & buttonoffset);
    buttonoffset.x = 350;
    buttonoffset.y = 250;
    SDL_BlitSurface(button[2], NULL, screen, & buttonoffset);
    number_display(350, 300, score, 6);
    buttonoffset.y = 350;
    SDL_BlitSurface(button[3], NULL, screen, & buttonoffset);
    number_display(440, 400, streak, 2);
    buttonoffset.y = 450;
    SDL_BlitSurface(button[4], NULL, screen, & buttonoffset);
    number_display(440, 500, headshot, 2);

    // Save Achievements if Record Breaking

    int highscore = 0, max_streak = 0, max_headshot = 0;

    char main_file[] = "highscore.txt";
    FILE * file = NULL;
    if ((file = fopen(main_file, "r")) == NULL)
        file = fopen(main_file, "a");

    char temp_file[] = "temp.txt";
    FILE * temp = NULL;
    temp = fopen(temp_file, "w");

    fscanf(file, "%d %d %d", & highscore, & max_streak, & max_headshot);

    if (score > highscore) {
        highscore = score;
        buttonoffset.y = 250;
        buttonoffset.x = 50;
        SDL_BlitSurface(button[5], NULL, screen, & buttonoffset);
        buttonoffset.x = SCREENWIDTH - BUTTONWIDTH - 50;
        SDL_BlitSurface(button[5], NULL, screen, & buttonoffset);
    }
    if (streak > max_streak) {
        max_streak = streak;
        buttonoffset.y = 350;
        buttonoffset.x = 50;
        SDL_BlitSurface(button[6], NULL, screen, & buttonoffset);
        buttonoffset.x = SCREENWIDTH - BUTTONWIDTH - 50;
        SDL_BlitSurface(button[6], NULL, screen, & buttonoffset);
    }
    if (headshot > max_headshot) {
        max_headshot = headshot;
        buttonoffset.y = 450;
        buttonoffset.x = 50;
        SDL_BlitSurface(button[7], NULL, screen, & buttonoffset);
        buttonoffset.x = SCREENWIDTH - BUTTONWIDTH - 50;
        SDL_BlitSurface(button[7], NULL, screen, & buttonoffset);
    }

    fprintf(temp, "%d %d %d", highscore, max_streak, max_headshot);

    remove(main_file);
    rename(temp_file, main_file);

    fclose(file);
    fclose(temp);

    SDL_Flip(screen);
    bg_display(NULL);

    // Free Things

    SDL_FreeSurface( * button);

}

int level(int time2death, int bullet, int zombienumber, int difficulty, int zombielife, int leveltime, int timer, int achievement[], int level_number)
{
    SDL_Delay(500);

    // Declare Requirements

    int quit = FALSE, win = FALSE, xposition, yposition, randomzombie[6][3][2], now;
    int score = achievement[0], streak = achievement[1], streak_event = FALSE, current_streak = 0, headshot = achievement[2], headshot_event = FALSE;

    SDL_ShowCursor(0);

    // Create and Load Images and Music

    SDL_Surface * evlgnius[3] = {
        NULL,
        NULL,
        NULL
    };
    SDL_Surface * button[2] = {
        NULL,
        NULL
    };
    SDL_Surface * target = NULL, * blast = NULL, * blood = NULL;

    evlgnius[0] = IMG_Load("Image/Level/evlgnius1.png");
    evlgnius[1] = IMG_Load("Image/Level/evlgnius2.png");
    evlgnius[2] = IMG_Load("Image/Level/evlgnius3.png");
    button[0] = IMG_Load("Image/Buttons/Achievement/headshot.png");
    button[1] = IMG_Load("Image/Buttons/Achievement/streak.png");
    target = IMG_Load("Image/Level/target.png");
    blast = IMG_Load("Image/Level/blast.png");
    blood = IMG_Load("Image/Level/blood.png");

    Mix_Music * shot[4] = {
        NULL,
        NULL,
        NULL,
        NULL
    };
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
    shot[0] = Mix_LoadMUS("Audio/shotgun.wav");
    shot[1] = Mix_LoadMUS("Audio/shot_1.wav");
    shot[2] = Mix_LoadMUS("Audio/shot_2.wav");
    shot[3] = Mix_LoadMUS("Audio/shot_3.wav");

    // Set Image Rects
    SDL_Rect evloff;

    SDL_Rect targetoff;
    targetoff.h = 50;
    targetoff.w = 50;

    buttonoffset.y = 550;

    // Give Life 2 Zombies
    for (i = 0; i < 6; i++)
        for (j = 0; j < 3; j++)
            randomzombie[i][j][1] = zombielife;

    // Main Game Loop
    while (!quit) {

        // For Time
        now = (SDL_GetTicks() - timer) / 100;

        // To determine Quit n Win case
        if (now == time2death || zombienumber == 0 || zombienumber * zombielife > bullet)
            quit = TRUE;
        if (zombienumber == 0)
            win = TRUE;

        // To Randomize Zombie Array

        for (i = 0; i < 6; i++)
            for (j = 0; j < 3; j++)
                if (now % 10 == 0) {
                    randomzombie[i][j][0] = rand() % difficulty;
                    int k = rand() % 5;
                    int l = rand() % 3;
                    int t = randomzombie[k][l][1];
                    randomzombie[k][l][1] = randomzombie[i][j][1];
                    randomzombie[i][j][1] = t;
                }

        // Blit Background,  Zombies and Numbers

        SDL_BlitSurface(background, NULL, screen, NULL);

        for (i = 0; i < 6; i++)
            for (j = 0; j < 3; j++)
                if (randomzombie[i][j][0] == 0 && randomzombie[i][j][1]) {
                    evloff.x = (2 * i + 1) * 80 - 20 * j;
                    evloff.y = 210 + j * 76;
                    SDL_BlitSurface(evlgnius[randomzombie[i][j][1] - 1], NULL, screen, & evloff);
                }

        number_display(200, 170, (time2death - now) / 10, 3);
        number_display(400, 170, zombienumber, 2);
        number_display(600, 170, bullet, 3);
        number_display(700, 600, score, 6);

        // To Detect Shots

        buttonoffset.y = 550;
        while (SDL_PollEvent( & event)) {

            if (event.type == SDL_MOUSEMOTION) {
                xposition = event.button.x;
                yposition = event.button.y;
                targetoff.x = event.button.x - 25;
                targetoff.y = event.button.y - 25;
            }

            if (event.type == SDL_MOUSEBUTTONDOWN) {
                Mix_PlayMusic(shot[0], 0);

                for (i = 0; i < 6; i++)
                    for (j = 0; j < 3; j++) {
                        evloff.h = 80;
                        evloff.w = 80;
                        evloff.x = (2 * i + 1) * 80 - 20 * j;
                        evloff.y = 210 + j * 76;
                        if (mouse_over_rect(evloff) && randomzombie[i][j][0] == 0 && randomzombie[i][j][1]) {
                            Mix_PlayMusic(shot[randomzombie[i][j][1]], 0);
                            randomzombie[i][j][1]--;
                            SDL_BlitSurface(blood, NULL, screen, & targetoff);
                            if (randomzombie[i][j][1] == 0)
                                zombienumber--;
                            score += 25;

                            evloff.h = 40;
                            evloff.w = 30;
                            evloff.x += 25;
                            evloff.y += 10;
                            if (mouse_over_rect(evloff)) {
                                headshot++;
                                headshot_event = TRUE;
                            }
                            current_streak++;
                            streak_event = TRUE;

                        }
                    }

                if (!streak_event)
                    current_streak = 0;

                SDL_BlitSurface(blast, NULL, screen, & targetoff);
                SDL_Delay(10);
                bullet--;
            }

            if (event.key.keysym.sym == SDLK_ESCAPE) {
                timer = pause(now, time2death);
            }
        }

        // Check Streak

        if (current_streak > STREAK && streak_event) {
            score += 3 * current_streak;
            buttonoffset.x = 360;
            SDL_BlitSurface(button[1], NULL, screen, & buttonoffset);
            number_display(310, 565, current_streak, 2);
            if (current_streak > streak)
                streak = current_streak;

        }

        // Check Headshots

        if (headshot_event) {
            score += 25;
            buttonoffset.x = 50;
            SDL_BlitSurface(button[0], NULL, screen, & buttonoffset);

        }

        if ((current_streak > STREAK && streak_event) || headshot_event) {
            SDL_Flip(screen);
            SDL_Delay(300);
            streak_event = FALSE;
            headshot_event = FALSE;
        }

        SDL_BlitSurface(target, NULL, screen, & targetoff);
        SDL_Flip(screen);

    }

    if (win)
        score += bullet * 10 + time2death / 100;

    achievement[0] = score;
    achievement[1] = streak;
    achievement[2] = headshot;

    // Set values N apply Recursion

    level_number++;
    if (level_number % 2 == 0)
        difficulty++;
    else
        zombielife++;

    if (win && level_number < LEVEL)
        win = level(time2death + leveltime, bullet + 18 * zombielife + 10, 18, difficulty, zombielife, leveltime, timer, achievement, level_number);

    SDL_ShowCursor(1);

    // Free Things

    SDL_FreeSurface( * evlgnius);
    SDL_FreeSurface( * button);
    SDL_FreeSurface(target);
    SDL_FreeSurface(blast);
    SDL_FreeSurface(blood);

    return win;

}

int pause(int now, int time2death)
{
    SDL_ShowCursor(1);

    // Load and Blit Images

    SDL_Surface * button[3] = {
        NULL,
        NULL,
        NULL
    };
    button[0] = IMG_Load("Image/Buttons/Paused/pause.png");
    button[1] = IMG_Load("Image/Buttons/Paused/resume.jpg");
    button[2] = IMG_Load("Image/Buttons/Paused/exit.jpg");

    SDL_BlitSurface(background, NULL, screen, NULL);
    SDL_BlitSurface(opaque, NULL, screen, NULL);

    buttonoffset.x = 350;
    buttonoffset.y = 200;
    SDL_BlitSurface(button[0], NULL, screen, & buttonoffset);

    buttonoffset.y = 350;
    buttonoffset.x = 140;
    SDL_BlitSurface(button[1], NULL, screen, & buttonoffset);
    buttonoffset.x = 540;
    SDL_BlitSurface(button[2], NULL, screen, & buttonoffset);
    SDL_Flip(screen);

    // Pause Loop

    while (TRUE) {
        while (SDL_PollEvent( & event)) {

            if (event.type == SDL_MOUSEBUTTONDOWN) {
                buttonoffset.x = 140;
                if (mouse_over_rect(buttonoffset)) {
                    SDL_ShowCursor(0);
                    return SDL_GetTicks() - now * 100;
                }
                buttonoffset.x = 540;
                if (mouse_over_rect(buttonoffset)) {
                    SDL_ShowCursor(0);
                    return SDL_GetTicks() - time2death * 100;
                }
            }
        }
    }

    // Free Thing

    SDL_FreeSurface( * button);

}

int mouse_over_rect(SDL_Rect Target)
{
    int x = event.button.x, y = event.button.y;
    if (x > Target.x && x < Target.x + Target.w && y > Target.y && y < Target.y + Target.h)
        return TRUE;
    return FALSE;
}

void number_display(int x, int y, int number, int digit)
{

    SDL_Surface * numberimage = NULL;
    numberimage = IMG_Load("Image/Level/num.png");
    SDL_Rect numberoff1;
    numberoff1.h = 40;
    numberoff1.w = 40;
    numberoff1.x = 0;

    SDL_Rect numberoff2;
    numberoff2.y = y;
    for (i = 0; i < digit; i++) {
        numberoff2.h = 40;
        numberoff2.w = 40;
        numberoff2.x = x + i * 40;

        numberoff1.y = (int(number / pow(10, digit - i - 1)) % 10) * 40;
        SDL_BlitSurface(numberimage, & numberoff1, screen, & numberoff2);

    }

    SDL_FreeSurface(numberimage);
}

void best_display()
{
    // Reading Best Records

    int highscore = 0, max_streak = 0, max_headshot = 0;

    char main_file[] = "highscore.txt";
    FILE * file = NULL;
    if ((file = fopen(main_file, "r")) == NULL)
        file = fopen(main_file, "a");
    fscanf(file, "%d %d %d", & highscore, & max_streak, & max_headshot);
    fclose(file);

    // Display Best Records

    SDL_Surface * button[3] = {
        NULL,
        NULL,
        NULL
    };
    button[0] = IMG_Load("Image/Buttons/Achievement/highscore.png");
    button[1] = IMG_Load("Image/Buttons/Achievement/streak.png");
    button[2] = IMG_Load("Image/Buttons/Achievement/headshot.png");

    SDL_BlitSurface(background, NULL, screen, NULL);
    SDL_BlitSurface(opaque, NULL, screen, NULL);

    buttonoffset.x = 350;
    buttonoffset.y = 200;
    SDL_BlitSurface(button[0], NULL, screen, & buttonoffset);
    number_display(350, 250, highscore, 6);
    buttonoffset.y = 350;
    SDL_BlitSurface(button[1], NULL, screen, & buttonoffset);
    number_display(440, 400, max_streak, 2);
    buttonoffset.y = 500;
    SDL_BlitSurface(button[2], NULL, screen, & buttonoffset);
    number_display(440, 550, max_headshot, 2);
    SDL_Flip(screen);

    bg_display(NULL);

    SDL_FreeSurface( * button);

}

void bg_display(SDL_Surface * image)
{

    // Displays an Image

    SDL_BlitSurface(image, NULL, screen, NULL);
    SDL_Flip(screen);

    // Creates a Loop until Space or Esc

    int flow = TRUE;
    while (flow)
        while (SDL_PollEvent( & event))
            if (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_SPACE)
                flow = FALSE;

}

void clean_up()
{
    // Free Things

    SDL_FreeSurface(screen);
    SDL_FreeSurface(background);
    SDL_FreeSurface(opaque);

    // Quit SDL

    SDL_Quit();
}