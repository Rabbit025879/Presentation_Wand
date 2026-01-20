#include "main.h" 

// Global variables for FreeRTOS objects
static EventGroupHandle_t device_mode_event_group = NULL;
static QueueHandle_t haptics_queue = NULL;
static QueueHandle_t laser_queue = NULL;
static QueueHandle_t hid_queue = NULL;
static InputEvent* input_event = new InputEvent(ButtonState(), ButtonState(), ButtonState(), MotionState());
static SystemMode* currentSystemMode = new SystemMode{FunctionMode::Presentation, InputMode::MotionControl};
// Instantiate task objects
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
  device_mode_event_group = xEventGroupCreate();
  xEventGroupSetBits(device_mode_event_group, USING_HAPTICS | USING_LASER | USING_HID); // Enable Haptics, Laser, HID by default

  haptics_queue = xQueueCreate(3, sizeof(InputEvent));
  laser_queue = xQueueCreate(3, sizeof(InputEvent));
  hid_queue = xQueueCreate(3, sizeof(InputEvent));

  button_task.start(
    haptics_queue, 
    laser_queue, 
    hid_queue, 
    device_mode_event_group, 
    input_event, 
    currentSystemMode
  );
  
  haptics_task.start(
    haptics_queue, 
    device_mode_event_group, 
    currentSystemMode
  ); // Pin A0 for haptics
  laser_task.start(
    laser_queue, 
    device_mode_event_group, 
    currentSystemMode
  ); // Pin A1 for laser
  hid_task.start(
    hid_queue, 
    device_mode_event_group, 
    currentSystemMode
  );
  mpu_task.start(
    haptics_queue, 
    laser_queue, 
    hid_queue, 
    device_mode_event_group, 
    input_event, 
    currentSystemMode
  );
  ota_task.start(
    device_mode_event_group
  );

}

void loop() {
  // put your main code here, to run repeatedly:
  vTaskDelay(pdMS_TO_TICKS(1000));
}