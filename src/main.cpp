#include "main.h"

// FreeRTOS queue handles
static QueueHandle_t haptics_queue = NULL;
static QueueHandle_t laser_queue = NULL;
static QueueHandle_t hid_queue = NULL;

// Shared system state
static InputEvent* input_event = new InputEvent();
static SystemMode* current_system_mode = new SystemMode(FunctionMode::Presentation, InputMode::SimpleInput);

// Task objects
static DeviceManager device_manager;
static ButtonTask button_task;
static HapticsTask haptics_task;
static LaserTask laser_task;
static HIDTask hid_task;
static MPUTask mpu_task;
static OTATask ota_task;

void setup() {
  Serial.begin(115200);
  
  // Create queues for inter-task communication
  haptics_queue = xQueueCreate(3, sizeof(InputEvent));
  laser_queue = xQueueCreate(3, sizeof(InputEvent));
  hid_queue = xQueueCreate(3, sizeof(InputEvent));

  // Initialize device manager
  device_manager.begin();

  // Start input tasks
  button_task.start(haptics_queue, laser_queue, hid_queue, &device_manager, input_event, current_system_mode);
  mpu_task.start(haptics_queue, laser_queue, hid_queue, &device_manager, input_event, current_system_mode);
  
  // Start output tasks
  haptics_task.start(haptics_queue, &device_manager, current_system_mode);
  laser_task.start(laser_queue, &device_manager, current_system_mode);
  hid_task.start(hid_queue, &device_manager, current_system_mode);
  ota_task.start(&device_manager);
}

void loop() {
  vTaskDelay(pdMS_TO_TICKS(1000));
}