#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <webots/camera.h>
#include <webots/distance_sensor.h>
#include <webots/motor.h>
#include <webots/receiver.h>
#include <webots/robot.h>
#define COMMUNICATION_CHANNEL 1 

#define MAX_SENSOR_NUMBER 16
#define RANGE (1024 / 2)
#define BOUND(x, a, b) (((x) < (a)) ? (a) : ((x) > (b)) ? (b) : (x))

static WbDeviceTag sensors[MAX_SENSOR_NUMBER], cameraL, cameraM, cameraR, left_motor, right_motor;
static double matrix[MAX_SENSOR_NUMBER][2];
static int num_sensors;
static double range;
static int time_step = 0;
static double max_speed = 0.0;
static double speed_unit = 1.0;
static int camera_enabled;
static int width,height;
static WbDeviceTag communication;

static void initialize() {
  wb_robot_init();
  time_step = wb_robot_get_basic_time_step();
  communication = wb_robot_get_device("receiver");
    wb_receiver_enable(communication, time_step);
  
  const char khepera_name[] = "ds0";
  char sensors_name[5];
  const double(*temp_matrix)[2];
  camera_enabled = 1;
  range = RANGE;
  const double khepera3_matrix[9][2] = {{-5000, -5000},  {-20000, 40000}, {-30000, 50000}, {-70000, 70000}, {70000, -60000},
                                        {50000, -40000}, {40000, -20000}, {-5000, -5000},  {-10000, -10000}};
  const double khepera3_max_speed = 19.1;
  const double khepera3_speed_unit = 0.00053429;
  num_sensors = 9;
  sprintf(sensors_name, "%s", khepera_name);
  temp_matrix = khepera3_matrix;
  range = 2000;
  max_speed = khepera3_max_speed;
  speed_unit = khepera3_speed_unit;
  int i;
  for (i = 0; i < num_sensors; i++) {
    sensors[i] = wb_robot_get_device(sensors_name);
    wb_distance_sensor_enable(sensors[i], time_step);
    if ((i + 1) >= 10) {
      sensors_name[2] = '1';
      sensors_name[3]++;
      if ((i + 1) == 10) {
        sensors_name[3] = '0';
        sensors_name[4] = '\0';
      }
    } else
      sensors_name[2]++;
      
    int j;
    for (j = 0; j < 2; j++)
      matrix[i][j] = temp_matrix[i][j];
  }

  left_motor = wb_robot_get_device("left wheel motor");
  right_motor = wb_robot_get_device("right wheel motor");
  wb_motor_set_position(left_motor, INFINITY);
  wb_motor_set_position(right_motor, INFINITY);
  wb_motor_set_velocity(left_motor, 0.0);
  wb_motor_set_velocity(right_motor, 0.0);
  camera_enabled =1;
  if (camera_enabled == 1) {
    cameraL = wb_robot_get_device("CameraLeft");
    wb_camera_enable(cameraL, time_step);
    width = wb_camera_get_width(cameraL); // all camera use same 
    height = wb_camera_get_height(cameraL);
    cameraM = wb_robot_get_device("CameraMiddle");
    wb_camera_enable(cameraM, time_step);
    width = wb_camera_get_width(cameraM); // all camera use same 
    height = wb_camera_get_height(cameraM);
    cameraR = wb_robot_get_device("CameraRight");
    wb_camera_enable(cameraR, time_step);
    width = wb_camera_get_width(cameraR); // all camera use same 
    height = wb_camera_get_height(cameraR);
  }}

    /*  Your code under here. Don't touch the above, unless you are happy to fix it yourself! it. */
    
// My global variables 
int Blue_val = 0;
int water = 0;
int thirst = 10000;
int health = 1000;
int hunger = 8000;
int water_move = 0;
int timer = 0;
int randomx = 3;
int randomy = 3;


// Add a random function to move in random directions

static int Randx(int minX, int maxX, int count) {
 // for (int i = 0; i < count; i++) {
    int movex = rand() % (maxX - minX + 1) + minX;

return movex;
//}
}

// homeostasis function - if any level reaches 0 the robot stops    
static int homeostasis(){
thirst-=2;
hunger--;
if (thirst <= 0 || hunger <0 || health <= 0){
  return false;}
else {
  return true;}
}

static int readIR(int sensor_value){
/* we have 8 IR sensors, code to read one sensor, can you read all 8 without repetition */
int IR_value = wb_distance_sensor_get_value(sensors[sensor_value]);

return IR_value;
}





// Pred Check
static void message(){
  int message_printed = 0; 
  if (wb_receiver_get_queue_length(communication) > 0) {
    const char *buffer = wb_receiver_get_data(communication);
    if (message_printed != 1) {
          message_printed = 1;
        }
    int redCheck =1;
    redCheck = strcmp(buffer,"Red");
    if (redCheck == 0){
      health--;
       printf("Red is nearby! \n");
       }
     wb_receiver_next_packet(communication);            
 }}

static int camera_view(){
int x;
int y;

// Left camera colour detection
wb_camera_enable(cameraL,time_step);
const unsigned char *cameraDataL = wb_camera_get_image(cameraL);
int redL = 0;
int greenL = 0;
int blueL = 0;

for (x = 0; x < width; x++){
  for(y = 0; y < height; y++){
    if (x < width ){
      redL += wb_camera_image_get_red(cameraDataL, width, x,y);
      greenL += wb_camera_image_get_green(cameraDataL, width, x,y);
      blueL += wb_camera_image_get_blue(cameraDataL, width, x,y);}
  }
}

// Middle camera colour detection
wb_camera_enable(cameraM,time_step);
const unsigned char *cameraDataM = wb_camera_get_image(cameraM);
int redM = 0;
int greenM = 0;
int blueM = 0;

for (x = 0; x < width; x++){
  for(y = 0; y < height; y++){
    if (x < width ){
      redM += wb_camera_image_get_red(cameraDataM, width, x,y);
      greenM += wb_camera_image_get_green(cameraDataM, width, x,y);
      blueM += wb_camera_image_get_blue(cameraDataM, width, x,y);}
  }
}

// Right camera colour detection
wb_camera_enable(cameraR,time_step);
const unsigned char *cameraDataR = wb_camera_get_image(cameraR);
int redR = 0;
int greenR = 0;
int blueR = 0;

for (x = 0; x < width; x++){
  for(y = 0; y < height; y++){
    if (x < width ){
      redR += wb_camera_image_get_red(cameraDataR, width, x,y);
      greenR += wb_camera_image_get_green(cameraDataR, width, x,y);
      blueR += wb_camera_image_get_blue(cameraDataR, width, x,y);}
  }
}

// Put the RGB values in a normal range and print them
redL = redL/4096;  
blueL = blueL/4096;
greenL = greenL/4096;
redM = redM/4096;  
blueM = blueM/4096;
greenM = greenM/4096;
redR = redR/4096;  
blueR = blueR/4096;
greenR = greenR/4096;

// checking blue values from each camera to determiune which way to move
if (blueL > blueM && blueL > blueR){
  Blue_val = blueL;
  water_move = 1;
  printf("Blue is left\n");
}

else if (blueM > blueL && blueM > blueR){
  Blue_val = blueM;
  water_move = 0;
  printf("Blue is Middle\n");
}

else if (blueR > blueM && blueR > blueL){
  Blue_val = blueR;
  water_move = 2;
  printf("Blue is Right\n");
}


printf("red=%d,green=%d,blue=%d\n",redL,greenL,blueL);
printf("red=%d,green=%d,blue=%d\n",redM,greenM,blueM);
printf("red=%d,green=%d,blue=%d\n",redR,greenR,blueR);


return Blue_val;

}
// Move function 
static void move(int l,int r){
    wb_motor_set_velocity(left_motor, l);
    wb_motor_set_velocity(right_motor, r);
    }
      
  
//static void metaRate(){
//thirst--;
//}

// Drink funciton for water
static void drink(){
thirst = thirst + 100;
}

static void FreeRoam(){

  timer ++;
  if (timer >= 160){
    timer = 0;
    randomx = Randx(2, 7, 10);
    randomy = Randx(2, 7, 10);
  }
  move(randomx,randomy);
  printf("Freeroam\n");
  printf("timer = %d\n", timer);
}

//main loop

int main() {
  initialize();
  if (homeostasis() == false){
    move(0,0);
    printf("Robot has died :( \n");
   }
  while (wb_robot_step(time_step) != -1 && homeostasis() == true) {
    FreeRoam();
    if (water_move == 0){
      move(5,5);
  }
    if (water_move == 1){
      move(3,5);
  }
    if (water_move == 2){
      move(5,3);
  }
    
    //printf("health = %d \n", health);
    homeostasis();
    message();
    
 // int j = 0;
 // for (j = 0; j < 8; j++){
   // printf("readIR: %d\n ", readIR(j));
  //}
  if (thirst > 10000){
    thirst = 5000;
    }
  if (Blue_val <= 150){
    drink();
  }
  printf("blue val =  %d\n", Blue_val);
  printf("hunger =  %d\n", hunger);
  printf("Thirst = %d\n", thirst);
 camera_view();
}

  return 0;
}
