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
#define DARK_GREEN_RED_MIN 100
#define DARK_GREEN_RED_MAX 120
#define DARK_GREEN_GREEN_MIN 160
#define DARK_GREEN_GREEN_MAX 180
#define DARK_GREEN_BLUE_MIN 120
#define DARK_GREEN_BLUE_MAX 140
#define WATER_RED_MIN 40
#define WATER_RED_MAX 90
#define WATER_GREEN_MIN 150
#define WATER_GREEN_MAX 170
#define WATER_BLUE_MIN 160
#define WATER_BLUE_MAX 200

#define MAX_THIRST 7000
#define MAX_HUNGER 8000

typedef enum {
    FREE_ROAM,
    SEARCH_WATER,
    SEARCH_FOOD,
    DEAD
} RobotState;

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
RobotState current_state = FREE_ROAM;  // Initial state

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
int Green_val = 0;
int water = 0;
int thirst = 7000;
int health = 1000;
int hunger = 8000;
int water_move = 0;
int timer = 0;
int randomx = 3;
int randomy = 3;
int redL = 0;
int blueL = 0;
int greenL = 0;
int redM = 0;  
int blueM = 0;
int greenM = 0;
int redR = 0;  
int blueR = 0;
int greenR = 0;
int darkGreenL = 0;
int darkGreenM = 0;
int darkGreenR = 0;

// Move function 
static void move(int l,int r){
    wb_motor_set_velocity(left_motor, l);
    wb_motor_set_velocity(right_motor, r);
}



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
       move(15,15);
       }
     wb_receiver_next_packet(communication);            
 }}

static void camera_view(){
int x;
int y;

// Left camera colour detection
wb_camera_enable(cameraL,time_step);
const unsigned char *cameraDataL = wb_camera_get_image(cameraL);

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

printf("red=%d,green=%d,blue=%d\n",redL,greenL,blueL);
printf("red=%d,green=%d,blue=%d\n",redM,greenM,blueM);
printf("red=%d,green=%d,blue=%d\n",redR,greenR,blueR);
}


// Green check for dark green areas as food

static int GreenCheck() {
    // Reset dark green counts
    darkGreenL = 0;
    darkGreenM = 0;
    darkGreenR = 0;

    // Check if Left camera values are within dark green thresholds
    if (redL >= DARK_GREEN_RED_MIN && redL <= DARK_GREEN_RED_MAX &&
        greenL >= DARK_GREEN_GREEN_MIN && greenL <= DARK_GREEN_GREEN_MAX &&
        blueL >= DARK_GREEN_BLUE_MIN && blueL <= DARK_GREEN_BLUE_MAX) {
        darkGreenL++;
        printf("Dark green detected on Left camera.\n");
    }

    // Check if Middle camera values are within dark green thresholds
    if (redM >= DARK_GREEN_RED_MIN && redM <= DARK_GREEN_RED_MAX &&
        greenM >= DARK_GREEN_GREEN_MIN && greenM <= DARK_GREEN_GREEN_MAX &&
        blueM >= DARK_GREEN_BLUE_MIN && blueM <= DARK_GREEN_BLUE_MAX) {
        darkGreenM++;
        printf("Dark green detected on Middle camera.\n");
    }

    // Check if Right camera values are within dark green thresholds
    if (redR >= DARK_GREEN_RED_MIN && redR <= DARK_GREEN_RED_MAX &&
        greenR >= DARK_GREEN_GREEN_MIN && greenR <= DARK_GREEN_GREEN_MAX &&
        blueR >= DARK_GREEN_BLUE_MIN && blueR <= DARK_GREEN_BLUE_MAX) {
        darkGreenR++;
        printf("Dark green detected on Right camera.\n");
    }

    // Debugging output
    printf("Dark green pixels detected: Left=%d, Middle=%d, Right=%d\n", darkGreenL, darkGreenM, darkGreenR);

    // printf("Dark Green: %d\n", darkGreenL);
// if (darkGreenL > darkGreenM && darkGreenL > darkGreenR){
  // Green_val = darkGreenL;
  // printf("Dark green is on the left.\n");
  // move(3, 5); // Turn left  
// }
// else if (darkGreenM > darkGreenL && darkGreenM > darkGreenR){
  // Green_val = darkGreenM;
  // printf("Dark green is on the middle.\n");
  // move(5, 5); // Turn left  
// }
// else if (darkGreenR > darkGreenL && darkGreenR > darkGreenM){
  // Green_val = darkGreenR;
  // printf("Dark green is on the right.\n");
  // move(5, 3); // Turn left  
// }
    // Return total dark green count
    return darkGreenL + darkGreenM + darkGreenR;
}





// checking blue values from each camera to determiune which way to move
static int BlueCheck() {
  int water_detected = 0;
  int direction = -1;  // -1 = no water, 0 = middle, 1 = left, 2 = right
  // Check if Left camera values match water thresholds
  if (redL >= WATER_RED_MIN && redL <= WATER_RED_MAX && greenL >= WATER_GREEN_MIN && greenL <= WATER_GREEN_MAX && blueL >= WATER_BLUE_MIN && blueL <= WATER_BLUE_MAX) {
    printf("Water detected on Left camera.\n");
    water_detected++;
    direction = 1;}  // Prioritize left if detected
    

   // Check if Middle camera values match water thresholds
  if (redM >= WATER_RED_MIN && redM <= WATER_RED_MAX && greenM >= WATER_GREEN_MIN && greenM <= WATER_GREEN_MAX && blueM >= WATER_BLUE_MIN && blueM <= WATER_BLUE_MAX) {
    printf("Water detected on Middle camera.\n");
    water_detected++;
    direction = 0;} // Middle takes priority over right
    

    // Check if Right camera values match water thresholds
  if (redR >= WATER_RED_MIN && redR <= WATER_RED_MAX && greenR >= WATER_GREEN_MIN && greenR <= WATER_GREEN_MAX && blueR >= WATER_BLUE_MIN && blueR <= WATER_BLUE_MAX) {
    printf("Water detected on Right camera.\n");
    water_detected++;
        // Only prioritize right if no middle or left detected
    direction = 2;}
        
    

    // Debugging output
    printf("Water detected cameras: %d, Direction: %d\n", water_detected, direction);

    return direction;  // Return direction where water is detected
}


      
  
//static void metaRate(){
//thirst--;
//}

// Eat function for food
static void eat() {
    if (hunger < MAX_HUNGER) {
        hunger += 10;
        printf("Ate food. Hunger: %d\n", hunger);
    } else {
        printf("Hunger is at maximum level (%d).\n", MAX_HUNGER);
        current_state = FREE_ROAM;  // Transition back to FREE_ROAM
    }
}

// Drink funciton for water
static void drink() {
    if (thirst < MAX_THIRST) {
        thirst += 10;
        printf("Drank water. Thirst: %d\n", thirst);
    } else {
        printf("Thirst is at maximum level (%d).\n", MAX_THIRST);
        current_state = FREE_ROAM;  // Transition back to FREE_ROAM
    }
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

static void SearchWater() {
    printf("State: SEARCH_WATER\n");
    int direction = BlueCheck();  // Check for water and get its direction

    if (blueL > blueM && blueL > blueR) {  // Water in the middle
        move(3, 5);  // Move left
    } else if (blueM > blueL && blueM > blueR) {  // Water on the left
        move(5, 5);  // Turn forward
    } else if (blueR > blueM && blueR > blueL) {  // Water on the right
        move(5, 3);  // Turn right
    } else {
        FreeRoam();  // Continue searching if no water detected
    }

    if (direction != -1) {  // If water is detected in any direction
        drink();  // Attempt to drink
    }
}



static void SearchFood() {
    printf("State: SEARCH_FOOD\n");
    if (GreenCheck() > 0) {
        printf("Food source found! Now eating...\n");
        eat();  // Eat and check if maximum hunger level is reached
    } else {
        move(5, 5);  // Continue searching
    }
}

static void Dead() {
    printf("State: DEAD\n");
    move(0, 0);  // Stop all movement
    health = 0;
    thirst = 0;
    hunger = 0;    
    printf("Robot has died. Exiting simulation.\n");

}
//main loop
int main() {
    initialize();

    while (wb_robot_step(time_step) != -1) {
        camera_view();  // Update camera data

        // Check homeostasis and transition to DEAD state if needed
        if (!homeostasis()) {
            current_state = DEAD;
        }

        // State machine
        switch (current_state) {
            case FREE_ROAM:
                FreeRoam();
                if (thirst < 6000) {
                    current_state = SEARCH_WATER;
                } else if (hunger < 4000) {
                    current_state = SEARCH_FOOD;
                }
                break;

            case SEARCH_WATER:
                SearchWater();
                break;

            case SEARCH_FOOD:
                SearchFood();
                break;

            case DEAD:
                Dead();
                break;
        }

        // Handle communication and other tasks
        message();
        printf("Health = %d, Hunger = %d, Thirst = %d\n", health, hunger, thirst);
    }

    return 0;
}
