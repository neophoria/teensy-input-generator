#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SerialFlash.h>
#include <Bounce.h>

#define BEEP_LED A14
#define MIC_LED A15
#define BUTTON A16

//MODES
#define MIC 0
#define BEEP 1

int mode = BEEP;
int buttonPressedAt = 0;

// GUItool: begin automatically generated code
AudioInputI2S            i2s2;           //xy=74,496
AudioSynthWaveform       waveform1;      //xy=110,705
AudioMixer4              mixer1;         //xy=381,437
AudioMixer4              mixer2;         //xy=413,629
AudioOutputI2S           i2s1;           //xy=648,614
AudioConnection          patchCord1(i2s2, 0, mixer1, 0);
AudioConnection          patchCord2(i2s2, 1, mixer2, 0);
AudioConnection          patchCord3(waveform1, 0, mixer1, 1);
AudioConnection          patchCord4(waveform1, 0, mixer2, 1);
AudioConnection          patchCord5(mixer1, 0, i2s1, 0);
AudioConnection          patchCord6(mixer2, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=262,190
// GUItool: end automatically generated code

Bounce bouncer = Bounce(BUTTON, 5);

void setup() {
  AudioMemory(10);
  // put your setup code here, to run once:
  Serial.begin(115200);

  pinMode(BEEP_LED, OUTPUT);
  pinMode(MIC_LED, OUTPUT);
  
  pinMode(BUTTON, INPUT_PULLDOWN);

  sgtl5000_1.enable();
  sgtl5000_1.volume(0.8);
  sgtl5000_1.adcHighPassFilterDisable();
  disableMic();
  disableWaveform();
  waveform1.frequency(440);
  waveform1.amplitude(0.1);
  waveform1.begin(WAVEFORM_SINE);

  delay(1000);
}

void loop() {
  bouncer.update();

  int cur = millis();
  if (bouncer.read() == HIGH && (cur - buttonPressedAt) > 500) {
    buttonPressedAt = cur;
    
    Serial.println("Button pressed");
    if (mode == BEEP) {
      mode = MIC;
      enableMic();
    } else {
      mode = BEEP;  
      disableMic();
    }
  }

  if (mode == BEEP) {
    beep();
  }
}

void beep() {
  enableWaveform();
  delay(300);
  disableWaveform();
  delay(1000);
}

void enableWaveform() {
  digitalWrite(BEEP_LED, HIGH);
  mixer1.gain(1, 0.1);
  mixer2.gain(1, 0.1);
}

void disableWaveform() {
  digitalWrite(BEEP_LED, LOW);
  mixer1.gain(1, 0.0);
  mixer2.gain(1, 0.0);
}

void enableMic() {
  digitalWrite(MIC_LED, HIGH);
  mixer1.gain(0, 1.0);
  mixer2.gain(0, 1.0);
}

void disableMic() {
  digitalWrite(MIC_LED, LOW);
  mixer1.gain(0, 0.0);
  mixer2.gain(0, 0.0);
}
