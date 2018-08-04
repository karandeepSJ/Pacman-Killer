#include "main.h"
#include "timer.h"
#include "ball.h"
#include "floor.h"
#include "magnet.h"
#include "trampoline.h"
#include "plank.h"
#include "porcupine.h"
#include "pond.h"
using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;


const int num_enemies=300;
/**************************
* Object declarations *
**************************/
Ball player,enemies[num_enemies];
Plank planks[num_enemies/3],mag_end;
Pond pond;
Porcupine porc[10];
Magnet magnet,magnet2;
Trampoline trampoline[2];
Floor lfloor,ufloor;

/**************************
* Game Parameters *
**************************/
float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0, screen_l=-8, screen_r=8;
int auto_pan=1, level=1,frame=0,game_over=0,pause=0;
int prev_up = GLFW_RELEASE,prev_left=GLFW_RELEASE,prev_right=GLFW_RELEASE;
Timer t60(1.0 / 60);
int level_time[4]={40,30,40,40}, level_score[4]={200,300,400,400};

/**************************
* Gravity and velocities*
**************************/
float g=0.005, ground_launch_vel=0.18,relaunch_vel=0.13;

/**************************
* Player parameters*
**************************/
float player_radius=0.2;
int score=0, f=0;

/**************************
* Enemy parameters*
**************************/
int num_porcs=6;
bool killed[num_enemies]={false},killed_porc[10]={false};
float porc_l[10],porc_n[10],porc_r[10];

/**************************
* Floor parameters*
**************************/
float lfloor_h=1.5, ufloor_h=0.5;

/**************************
* Pond parameters*
**************************/
float pond_rad=1, pond_x=-2, pond_y=-4+lfloor_h+ufloor_h, pond_l=-3.0, pond_r=-1.0, pond_jump=0;

/**************************
* Magnet parameters*
**************************/
float mag_acc=0.001;
int mag_appear,in_pond=0;


/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    // glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    // glm::vec3 target (0, 0, 0);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    // glm::vec3 up (0, 1, 0);

    // Compute Camera matrix (view)
    // Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    ufloor.draw(VP);
    lfloor.draw(VP);
    for(int i=0;i<2 &&level>=2;i++)
        trampoline[i].draw(VP);
    if(level>=3)
        pond.draw(VP);
    if(level>=4)
    {
        for(int i=0;i<num_porcs;i++)
        {
            if(!killed_porc[i])
               porc[i].draw(VP);
        }
    }
    if(mag_appear && level>=3)
    {
        magnet.set_position(screen_center_x-2.5,screen_center_y+3);
        magnet2.set_position(screen_center_x-2.2,screen_center_y+3);
        mag_end.position.x=-2.4+screen_center_x;
        mag_end.position.y=screen_center_y+3;
        magnet.draw(VP);
        mag_end.draw(VP);
        magnet2.draw(VP);
    }
    player.draw(VP);
    for(int i=0;i<num_enemies;i++)
        if(!killed[i])
            enemies[i].draw(VP);
    if(level>=2)
    {
        for(int i=0;i<num_enemies/3;i++)
        {
            if(!killed[3*i])
                planks[i].draw(VP);
        }
    }
}

void tick_input(GLFWwindow *window) {
    int left  = glfwGetKey(window, GLFW_KEY_A);
    int right = glfwGetKey(window, GLFW_KEY_D);
    int up = glfwGetKey(window, GLFW_KEY_SPACE);

    if(left==GLFW_RELEASE && f==0)
        player.speed=0.0f;
    if(right==GLFW_RELEASE && f==0)
        player.speed=0.0f;
    if (left==GLFW_PRESS) {
        player.speed=(in_pond==0)?-0.03f:-0.02f;
        f=0;
    }
    if(right==GLFW_PRESS)
    {
        f=0;
        player.speed=(in_pond==0)?0.03f:0.02f;
    }

    //Ensuring ball does not go out of boundaries
    if(player.position.x+player_radius>screen_r)
        player.position.x=screen_r-player_radius;
    if(player.position.x-player_radius<screen_l)
    {
        player.position.x=screen_l+player_radius;
    }
    if(prev_up == GLFW_PRESS && up==GLFW_RELEASE)
    {
        prev_up = GLFW_RELEASE;
    }
    if(up==GLFW_PRESS)
    {
        if(prev_up==GLFW_RELEASE)
        {
            if(!in_pond)
                player.jump(ground_launch_vel,(-4+lfloor_h+ufloor_h+player_radius),0);
            else
            {
                pond_jump=1;
                player.jump(relaunch_vel,player.position.y,1);
            }
        }
            prev_up=GLFW_PRESS;
    }
}

void tick_elements() {
   char title[200];
   sprintf(title,"Level: %d\t\tScore: %d\tObjective: %d\tTime Left: %d\tAuto-pan camera: %s",level,score,level_score[level-1],level_time[level-1]-(frame/60),auto_pan?"On":"Off");
   if(level_time[level-1]-(frame/60)==0)
       game_over=1;
   glfwSetWindowTitle(window, title);
   for(int i=0;i<num_porcs && level>=4; i++)
   {
       porc[i].tick();
   }
    if(player.position.x>pond_l+0.01 && player.position.x<pond_r && player.position.y-player_radius<=pond_y && level>=3)
        in_pond=1;
    else
    {
        in_pond=0;
        pond_jump=0;
    }
//    f=0;
    if(mag_appear && level>=3)
    {
        player.speed-=mag_acc;
        f=1;
    }
    for(int i=0;i<num_enemies;i++)
        enemies[i].tick();
    if(level>=2)
    {
        for(int i=0;i<num_enemies/3;i++)
            planks[i].tick();
    }
    player.tick();
    if(in_pond)
    {

        if(pond_jump)
        {
            player.v-=(g);
            player.position.y=(((player.u*player.u)-(player.v*player.v))/(2*g))+player.init_pos;
        }
        if(!detect_collision_ball_pond(player.bounding_box()))
            player.position.y-=0.02;
        else
        {
            pond_jump=0;
            if(fabs(player.position.x-pond_x)<0.01)
                return;
            else if(player.position.x<pond_x)
                player.position.x+=0.01;
            else if(player.position.x>pond_x)
                player.position.x-=0.01;
            if(fabs(player.position.x-pond_l)<=0.001 || player.position.x>pond_r)
                in_pond=0;
            else
            {
                float exp_y;
                if((pond_rad*pond_rad) - ((player.position.x-pond_x)*(player.position.x-pond_x))<0)
                    exp_y=-1.8;
                else
                    exp_y = -sqrt((pond_rad*pond_rad) - ((player.position.x-pond_x)*(player.position.x-pond_x)))+pond_y+player_radius;
                player.position.y=exp_y;

            }
        }
    }
    else
    {
        g=0.005;
        if(player.u!=0)
        {
            player.v-=(g);
            player.position.y=(((player.u*player.u)-(player.v*player.v))/(2*g))+player.init_pos;
        }
        for(int i=0;i<num_enemies;i++)
        {
            if(level>=2)
            {
                if (i%3 && !killed[i] && detect_collision_ball_ball(enemies[i].bounding_box(), player.bounding_box()) && player.v<0) {
                    player.jump(relaunch_vel,enemies[i].position.y+enemies[i].rad,1);
                    killed[i]=true;
                    inc_score(enemies[i].color);
                }
                else if(i%3==0 && !killed[i] && detect_collision_ball_ball(player.bounding_box(),enemies[i].bounding_box()) && player.v<0){
                    player.speed = 1.2*((player.speed*cos(2*planks[i/3].angle)) + (player.v*sin(2*planks[i/3].angle)));
                    player.jump(((player.speed*sin(2*planks[i/3].angle)) - (player.v*cos(2*planks[i/3].angle))),player.position.y,1);
                    f=1;
                    killed[i]=true;
                    inc_score(enemies[i].color);
                }
            }
            else
            {
                if (!killed[i] && detect_collision_ball_ball(enemies[i].bounding_box(), player.bounding_box()) && player.v<0) {
                    player.jump(relaunch_vel,enemies[i].position.y+enemies[i].rad,1);
                    killed[i]=true;
                    inc_score(enemies[i].color);
                }
            }
        }
        if (detect_collision_ball_floor(player.bounding_box(), ufloor.bounding_box(0.5)))
        {
            if((level<3) || (level>=3 &&(player.position.x<pond_l || player.position.x>pond_r))) {
                player.v = 0;
                player.u = 0;
                f=0;
            }
        }
        for(int i=0;i<2 && level>=2;i++)
        {
            if (detect_collision_ball_tramp(player.bounding_box(), trampoline[i].bounding_box()) && player.v<0) {
                player.jump(ground_launch_vel+0.03,player.position.y,1);
            }
        }
        for(int i=0;i<num_porcs && level>=4;i++)
        {
            if(detect_collision_ball_porc(player.bounding_box(), porc[i].bounding_box()) && !killed_porc[i])
            {
                player.jump(ground_launch_vel,player.position.y,1);
                killed_porc[i]=true;
                score-=20;
            }
        }
        if(player.position.y<(-4+lfloor_h+ufloor_h+player_radius))
            player.position.y=(-4+lfloor_h+ufloor_h+player_radius);
    }
    for(int i=0;i<num_porcs;i++)
    {
        if(porc[i].position.x+porc[0].width>=porc_r[i] || porc[i].position.x<=porc_l[i])
            porc[i].speed=-porc[i].speed;
    }
    if(auto_pan)
    {
        screen_center_x=player.position.x;
        if(player.position.x>4)
            screen_center_x=4;
        else if(player.position.x<-4)
            screen_center_x=-4;
        screen_center_y=(player.position.y>0)?player.position.y:0;
        reset_screen();
    }
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models
    color_t colors[4] = {COLOR_BLUE,COLOR_GREEN,COLOR_PURPLE,COLOR_YELLOW};
    player       = Ball(0, -4+lfloor_h+ufloor_h+player_radius, player_radius, 0, COLOR_RED);
    for(int i=0;i<num_enemies;i++)
    {
        float vel = 0.01 + (0.04*(((float)rand())/((float)RAND_MAX)));
        int col = (rand())%4;
        float rad = 0.1 + (0.2*(((float)rand())/((float)RAND_MAX)));
        float pos = 3 * (((float)rand())/((float)RAND_MAX));
        enemies[i] = Ball(-6*((i/10)+1),pos,rad,vel,colors[col]);
        if(i%3==0)
        {
            float angle = (rand()%90)-45;
            planks[i/3]=Plank(-6*((i/10)+1)-(rad*sin(angle*M_PI/180.0f)),pos+(rad*cos(angle*M_PI/180.0f)),3*rad,rad/2,angle,COLOR_BROWN);
        }
    }
    lfloor      = Floor(0,-4+(lfloor_h/2),lfloor_h,COLOR_BROWN);
    ufloor      = Floor(0,-4+lfloor_h+(ufloor_h/2),ufloor_h,COLOR_GREEN);
    trampoline[0]  =Trampoline(3,-1.2,0.4,COLOR_RED);
    trampoline[1]  =Trampoline(-6,-1.2,0.4,COLOR_RED);
    pond        =Pond(pond_x,pond_y,pond_rad,COLOR_LIGHT_BLUE);
    magnet      =Magnet(-2.5,3,1.3,1,COLOR_RED);
    mag_end     =Plank(-2.4,3,0.2,2,0,COLOR_SILVER);
    magnet2      =Magnet(-2.2,3,1.3,0.8,COLOR_BACKGROUND);
    porc_l[0]=-4, porc_l[1]=pond_r, porc_l[2]=1, porc_l[3]=-8, porc_l[4]=4,porc_l[5]=6;
    porc_r[0]=pond_l, porc_r[1]=1, porc_r[2]=4, porc_r[3]=-4, porc_r[4]=6, porc_r[5]=8;
    porc_n[0]=2, porc_n[1]=1, porc_n[2]=3, porc_n[3]=5,porc_n[4]=4,porc_n[5]=2;
    for(int i=0;i<num_porcs;i++)
    {
        porc[i]     =Porcupine(porc_l[i],-2,0.2,0.4,porc_n[i],COLOR_WHITE);
    }
    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {
    while(level<5){
        srand(time(0));
        frame=0;
        screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
        prev_up = GLFW_RELEASE,prev_left=GLFW_RELEASE,prev_right=GLFW_RELEASE,f=0;
        for(int i=0;i<300;i++) killed[i]=false;
        for(int i=0;i<10;i++) killed_porc[i]=false;
        score=0;
        int width  = 800;
        int height = 800;

        window = initGLFW(width, height);

        initGL (window, width, height);

        /* Draw in loop */
        while (!glfwWindowShouldClose(window)) {
            // Process timers
                if (t60.processTick()) {
                    if(pause)
                    {
                        continue;
                    }
                    frame++;
                    if(frame%420>=300 && frame%420<=420)
                        mag_appear=1;
                    else
                        mag_appear=0;
                    // 60 fps
                    // OpenGL Draw commands
                    draw();
                    // Swap Frame Buffer in double buffering
                    glfwSwapBuffers(window);

                    tick_elements();
                    tick_input(window);
                    if(game_over)
                        break;
                    if(score>=level_score[level-1])
                    {
                        level++;
                        break;
                    }
                }

            // Poll for Keyboard and mouse events
            glfwPollEvents();
        }
        char mesg[200];
        if(game_over)
        {
            sprintf(mesg,"Final Score: %d. Game Over :(. You could not achieve the target in the given time",score);
            glfwSetWindowTitle(window,mesg);
            for(int i=0;i<1000000000;i++);
            break;
        }
        if(level==5)
            sprintf(mesg,"Final Score: %d. Congrats!!! You completed the game",score);
        else
            sprintf(mesg,"Final Score: %d. You proceed to the next level",score);
        glfwSetWindowTitle(window,mesg);
        for(int i=0;i<1000000000;i++);

    }
    quit(window);
}

void inc_score(color_t color)
{
//    const color_t color=col;
    if(color.r==COLOR_BLUE.r && color.g==COLOR_BLUE.g && color.b==COLOR_BLUE.b)
        score+=5;
    if(color.r==COLOR_GREEN.r && color.g==COLOR_GREEN.g && color.b==COLOR_GREEN.b)
        score+=10;
    if(color.r==COLOR_PURPLE.r && color.g==COLOR_PURPLE.g && color.b==COLOR_PURPLE.b)
        score+=15;
    if(color.r==COLOR_YELLOW.r && color.g==COLOR_YELLOW.g && color.b==COLOR_YELLOW.b)
        score+=20;
}

bool detect_collision_ball_plank(bounding_box_circle_t a, bounding_box_plank_t b)
{
    float c = b.y-(b.m*b.x);
    float num=(b.m*a.x) - a.y + c, denom = sqrt((b.m*b.m)+1), dist=(fabs(num))/denom;
    float xp=((-b.m*num)/denom) + a.x, yp=(num/denom) + a.y;
    float xcheck=fabs((xp-b.x)/cos(b.angle)),ycheck=fabs((yp-b.y)/sin(b.angle));
    return dist<=a.r && xcheck<=b.width/2 && ycheck<=b.width/2;
}

bool detect_collision_ball_ball(bounding_box_circle_t a, bounding_box_circle_t b) {
    return (sqrt(((a.x - b.x) * (a.x - b.x)) + ((a.y - b.y) * (a.y - b.y)))<=a.r+b.r);
}

bool detect_collision_ball_tramp(bounding_box_circle_t p, bounding_box_circle_t t) {
    return p.x>=t.x-t.r && p.x<=t.x+t.r && p.y-t.y<=p.r && p.y>t.y;
}
bool detect_collision_ball_pond(bounding_box_circle_t a) {
    return (sqrt(((a.x - pond_x) * (a.x - pond_x)) + ((a.y - pond_y) * (a.y - pond_y)))>=pond_rad-a.r);
}
bool detect_collision_ball_floor(bounding_box_circle_t a, bounding_box_rectangle_t b) {
    return ((a.y-b.y)<=(a.r+(b.height/2)));
}

bool detect_collision_ball_porc(bounding_box_circle_t a, bounding_box_rectangle_t b) {
    return ((a.x>=b.x) && (a.x<=b.x+b.width) && (a.y-b.y<=b.height));
}
void reset_screen() {
    float top    = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left   = screen_center_x - 4 / screen_zoom;
    float right  = screen_center_x + 4 / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}
