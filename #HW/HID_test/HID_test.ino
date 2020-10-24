#include "HIDremote.h";

void setup() {
  // put your setup code here, to run once:
  HIDRemote.begin();
  
}

void loop() {
  // put your main code here, to run repeatedly:
  
  HIDRemote.press(HID_REMOTE_MUTE);
  HIDRemote.release(HID_REMOTE_MUTE);
  // HIDRemote.pressAndRelease(HID_REMOTE_MUTE);
  
  delay(2000);
}

/*
 * 
    HID_REMOTE_PLAY Play button; if media is already playing, should have no effect.
    HID_REMOTE_PAUSE Pause button; if no media is playing, should have no effect.
    HID_REMOTE_RECORD Record key, to start recording
    HID_REMOTE_FAST_FORWARD Fast-forward
    HID_REMOTE_REWIND Rewind
    HID_REMOTE_NEXT Skip to next track
    HID_REMOTE_PREVIOUS Skip to previous track
    HID_REMOTE_STOP Stop media playback
    HID_REMOTE_EJECT Eject media
    HID_REMOTE_RANDOM Select randomized track playback order
    HID_REMOTE_VOLUME_UP Increment volumne
    HID_REMOTE_VOLUME_DOWN Decrement volumne
    HID_REMOTE_SLEEP Enter sleep mode
    HID_REMOTE_REPEAT Activate track or playlist repeating
    HID_REMOTE_PLAY_PAUSE Play/pause button; if media playing, pause playback; if media is paused, resume playback
    HID_REMOTE_MUTE Mute/unmute volumne

 */
