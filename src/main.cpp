#include "main.h" 

// Global variables for FreeRTOS objects
static EventGroupHandle_t device_mode_event_group = NULL;
static QueueHandle_t haptics_queue = NULL;
static QueueHandle_t laser_queue = NULL;
static QueueHandle_t hid_queue = NULL;
static InputEvent* input_event = new InputEvent(ButtonState(false, ButtonEvent::None), MotionEvent::None);
static SystemMode* currentSystemMode = new SystemMode{FunctionMode::Presentation, InputMode::SimpleInput};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  // Setup Tasks
  device_mode_event_group = xEventGroupCreate();
  xEventGroupSetBits(device_mode_event_group, USING_HAPTICS | USING_LASER | USING_HID); // Enable Haptics, Laser, HID by default

  haptics_queue = xQueueCreate(2, sizeof(InputEvent));
  laser_queue = xQueueCreate(2, sizeof(InputEvent));
  hid_queue = xQueueCreate(2, sizeof(InputEvent));

  ButtonTask::button_task_start(
    haptics_queue, 
    laser_queue, 
    hid_queue, 
    device_mode_event_group, 
    input_event, 
    currentSystemMode, 
    BUTTON_PIN
  ); // Pin 10 for button
  
  HapticsTask::haptics_task_start(
    haptics_queue, 
    device_mode_event_group, 
    currentSystemMode, 
    HAPTICS_PIN
  ); // Pin A0 for haptics
  LaserTask::laser_task_start(
    laser_queue, 
    device_mode_event_group, 
    currentSystemMode, 
    LASER_PIN
  ); // Pin A1 for laser
  HIDTask::hid_task_start(
    hid_queue, 
    device_mode_event_group, 
    currentSystemMode
  );
  MPUTask::mpu_task_start(
    haptics_queue, 
    laser_queue, 
    hid_queue, 
    device_mode_event_group, 
    input_event, 
    currentSystemMode
  );
  OTATask::ota_task_start(
    device_mode_event_group
  );

}

void loop() {
  // put your main code here, to run repeatedly:
  vTaskDelay(pdMS_TO_TICKS(1000));
}