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

static WbDeviceTag sensors[MAX_SENSOR_NUMBER], camera, left_motor, right_motor;
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
  if (communication != 0){
    wb_receiver_enable(communication, time_step);
  }
  else {
    printf("Error:");
    }
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
    camera = wb_robot_get_device("camera");
    wb_camera_enable(camera, time_step);
    width = wb_camera_get_width(camera); // all camera use same 
    height = wb_camera_get_height(camera);
  }}

    /*  Your code under here. Don't touch the above, unless you are happy to fix it yourself! it. */


int Blue_val = 0;
int water = 0;
int thirst = 1000;
int health = 1000;


static int readIR(sensor_value){
/* we have 8 IR sensors, code to read one sensor, can you read all 8 without repetition */
int IR_value = wb_distance_sensor_get_value(sensors[sensor_value]);

return IR_value;
}

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
       printf("Red is nearby! \n");
       }
     wb_receiver_next_packet(communication);            
 }}

static void camera_view(){
int x;
int y;

wb_camera_enable(camera,time_step);
const unsigned char *cameraData = wb_camera_get_image(camera);
int red1 = 0;
int green1 = 0;
int blue1 = 0;
int red2 = 0;
int green2 = 0;
int blue2 = 0;
int red3 = 0;
int green3 = 0;
int blue3 = 0;

for (x = 0; x < width; x++){
  for(y = 0; y < height; y++){
    if (x < width/3 ){
      red1 += wb_camera_image_get_red(cameraData, width, x,y);
      green1 += wb_camera_image_get_green(cameraData, width, x,y);
      blue1 += wb_camera_image_get_blue(cameraData, width, x,y);}
   else if (x < width/2) {
      red2 += wb_camera_image_get_red(cameraData, width, x,y);
      green2 += wb_camera_image_get_green(cameraData, width, x,y);
      blue2 += wb_camera_image_get_blue(cameraData, width, x,y);}
    else{
      red3 += wb_camera_image_get_red(cameraData, width, x,y);
      green3 += wb_camera_image_get_green(cameraData, width, x,y);
      blue3 += wb_camera_image_get_blue(cameraData, width, x,y);}  
  }
}

red1 = red1/1363;  
blue1 = blue1/1363;
green1 = green1/1363;
red2 = red2/1363;  
blue2 = blue2/1363;
green2 = green2/1363;
red3 = red3/1363;  
blue3 = blue3/1363;
green3 = green3/1363;

printf("red=%d,green=%d,blue=%d\n",red1,green1,blue1);
printf("red=%d,green=%d,blue=%d\n",red2,green2,blue2);
printf("red=%d,green=%d,blue=%d\n",red3,green3,blue3);
Blue_val = blue1;
}


static void move(int l,int r){
    wb_motor_set_velocity(left_motor, l);
    wb_motor_set_velocity(right_motor, r);}
      
  
static void metaRate(){
thirst--;
}

static void drink(){
thirst = thirst + 100;
}

int main() {
  initialize();
  while (wb_robot_step(time_step) != -1) {
    move(1,1);
    printf("Initial thirst level: %d\n" , thirst);
    metaRate();
    message();
    
  int j = 0;
  for (j = 0; j < 8; j++){
    printf("readIR: %d\n ", readIR(j));
  }
  if (thirst > 10000){
    thirst = 5000;
    }
  if (Blue_val >= 150){
    drink();
  }
   printf("Thirst =  %d\n", thirst);
 camera_view();
}

  return 0;
}