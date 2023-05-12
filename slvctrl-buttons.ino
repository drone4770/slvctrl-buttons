#include <Bounce2.h>  // Bounce2 library makes button change detection easy
#include <SerialCommands.h>

const char* DEVICE_TYPE = "buttons";
const int FW_VERSION = 10000; // 1.00.00
const int PROTOCOL_VERSION = 10000; // 1.00.00


char serial_command_buffer[32];
SerialCommands serialCommands(&Serial, serial_command_buffer, sizeof(serial_command_buffer), "\n", " ");

Bounce button1 = Bounce();  
Bounce button2 = Bounce();  

void setup() {
    Serial.begin(9600);
  
    // wait for serial port to open on native usb devices
    /*while (!Serial) {
      delay(1);
    }*/

    pinMode(2, INPUT_PULLUP);
    button1.attach(2);
    button1.interval(10); 
    pinMode(1, INPUT_PULLUP); 
    button2.attach(1);
    button2.interval(10);    

    // Add commands
    serialCommands.SetDefaultHandler(commandUnrecognized);
    serialCommands.AddCommand(new SerialCommand("introduce", commandIntroduce));
    serialCommands.AddCommand(new SerialCommand("attributes", commandAttributes));
    serialCommands.AddCommand(new SerialCommand("status", commandStatus));
    serialCommands.AddCommand(new SerialCommand("get-button1", commandGetButton1));
    serialCommands.AddCommand(new SerialCommand("get-button2", commandGetButton2));

    serialCommands.GetSerial()->write(0x07);
}

void loop() {
    button1.update();
    button2.update();  

    serialCommands.ReadSerial();
}

void commandUnrecognized(SerialCommands* sender, const char* cmd)
{
    serial_printf(sender->GetSerial(), "Unrecognized command [%s]\n", cmd);
}

void commandIntroduce(SerialCommands* sender) {
    serial_printf(sender->GetSerial(), "introduce;%s,%d,%d\n", DEVICE_TYPE, FW_VERSION, PROTOCOL_VERSION);
}

void commandAttributes(SerialCommands* sender)
{
    serial_printf(sender->GetSerial(), "attributes;button1:ro[bool],button2:ro[bool]\n");
}

void commandStatus(SerialCommands* sender) {
    serial_printf(sender->GetSerial(), "status;button1:%d,button2:%d\n", button1.read() == LOW, button2.read() == LOW);
}

void commandGetButton1(SerialCommands* sender) {
    serial_printf(sender->GetSerial(), "get-button1;%d;status:successful\n", button1.read() == LOW);
}

void commandGetButton2(SerialCommands* sender) {
    serial_printf(sender->GetSerial(), "get-button2;%d;status:successful\n", button2.read() == LOW);
}


