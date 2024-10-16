
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <webots/camera.h>
#include <webots/distance_sensor.h>
#include <webots/motor.h>
#include <webots/robot.h>

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

static void initialize() {
  wb_robot_init();
  time_step = wb_robot_get_basic_time_step();
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





static int readIR(sensor_value){
/* we have 8 IR sensors, code to read one sensor, can you read all 8 without repetition */
int IR_value = wb_distance_sensor_get_value(sensors[sensor_value]);

return IR_value;

}




static void move(int l,int r){
    wb_motor_set_velocity(left_motor, l);
    wb_motor_set_velocity(right_motor, r);}

int main() {
  initialize();
  while (wb_robot_step(time_step) != -1) {
    move(10,1);
  
  int j = 0;
  for (j = 0; j < 8; j++){
    readIR(j);
    printf("readIR: %d\n ", readIR(j));
  }

}
  return 0;
}