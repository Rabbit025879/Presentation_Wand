#include "main.h"

// Global variables for FreeRTOS objects
static QueueHandle_t haptics_queue = NULL;
static QueueHandle_t laser_queue = NULL;
static QueueHandle_t hid_queue = NULL;
static InputEvent* input_event = new InputEvent(ButtonState(), ButtonState(), ButtonState(), MotionState());
static SystemMode* currentSystemMode = new SystemMode{FunctionMode::Presentation, InputMode::MotionControl};

// Instantiate task objects
static DeviceManager device_manager;
static ButtonTask button_task;
static HapticsTask haptics_task;
static LaserTask laser_task;
static HIDTask hid_task;
static MPUTask mpu_task;
static OTATask ota_task;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  // Setup Tasks
  haptics_queue = xQueueCreate(3, sizeof(InputEvent));
  laser_queue = xQueueCreate(3, sizeof(InputEvent));
  hid_queue = xQueueCreate(3, sizeof(InputEvent));

  // Initialize DeviceManager
  device_manager.begin();

  button_task.start(
    haptics_queue, 
    laser_queue, 
    hid_queue, 
    &device_manager,
    input_event, 
    currentSystemMode
  );
  
  haptics_task.start(
    haptics_queue, 
    &device_manager,
    currentSystemMode
  );
  
  laser_task.start(
    laser_queue, 
    &device_manager,
    currentSystemMode
  );
  
  hid_task.start(
    hid_queue, 
    &device_manager,
    currentSystemMode
  );
  
  mpu_task.start(
    haptics_queue, 
    laser_queue, 
    hid_queue, 
    &device_manager,
    input_event, 
    currentSystemMode
  );
  
  ota_task.start(
    &device_manager
  );
}

void loop() {
  // put your main code here, to run repeatedly:
  vTaskDelay(pdMS_TO_TICKS(1000));
}