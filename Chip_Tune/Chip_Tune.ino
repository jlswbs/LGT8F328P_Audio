// Chiptune music ported from Instructables //

#define LED_PIN     13
#define LED_PORT    PORTB
#define LED_BIT     5
#define SAMPLE_RATE 16000

#define TRACKLEN	32
#define MAXTRACK	0x92
#define SONGLEN		0x37

int8_t fuzz(int8_t x, uint8_t clip, uint8_t gain) {
  
  float y = x * gain;            
  if(y > clip) y = clip;
  else if(y < -clip) y = -clip;
  return y;

}

const byte songdata[] PROGMEM = {
  	0x77, 0x00, 0x2a, 0x84, 0x05, 0xbb, 0x90, 0x18, 0x40, 0xc3, 0x6c, 0x50,
    0x0e, 0xd1, 0x41, 0x3e, 0x4c, 0x08, 0x1a, 0xb1, 0x24, 0xa4, 0x44, 0x99,
    0x30, 0x13, 0x67, 0x82, 0x50, 0x60, 0x0a, 0x53, 0x11, 0x2c, 0x9e, 0x45,
    0xbe, 0xe8, 0x18, 0x2b, 0xe3, 0x68, 0x9c, 0x8d, 0xc2, 0xd1, 0x39, 0x60,
    0xc7, 0xf5, 0xe0, 0x1f, 0x1c, 0x64, 0x88, 0x84, 0x11, 0x3c, 0xb2, 0x48,
    0x4e, 0x49, 0x2d, 0x89, 0x26, 0xe8, 0x84, 0x9f, 0x38, 0x94, 0x8e, 0x92,
    0x53, 0x9e, 0x4a, 0x57, 0xb1, 0x2b, 0x8d, 0x45, 0xb5, 0x14, 0x97, 0xe9,
    0x32, 0x5e, 0xe8, 0x0b, 0x83, 0x29, 0x31, 0x40, 0xc6, 0xcc, 0x28, 0x9a,
    0x51, 0x63, 0x6c, 0xaa, 0xcd, 0xb8, 0xe1, 0x37, 0x10, 0x27, 0xe5, 0x54,
    0x1d, 0xb9, 0x73, 0x7a, 0x72, 0x4f, 0xf2, 0xc9, 0x3e, 0x03, 0x08, 0x00,
    0x00, 0x1b, 0x00, 0x00, 0xa8, 0xb5, 0x00, 0x80, 0x9b, 0x2b, 0x00, 0x00,
    0x00, 0x02, 0x00, 0x00, 0x20, 0x90, 0x18, 0x00, 0x02, 0x09, 0x00, 0x20,
    0x90, 0x00, 0x4c, 0x02, 0x09, 0x00, 0x25, 0x90, 0x00, 0x54, 0x0e, 0x09,
    0xc0, 0x24, 0x90, 0x00, 0x4c, 0x02, 0x09, 0x00, 0x25, 0x90, 0x00, 0x54,
    0x02, 0x09, 0xc2, 0x24, 0x90, 0x50, 0x4c, 0x02, 0x89, 0x05, 0x25, 0x90,
    0x60, 0x54, 0x02, 0x89, 0xc6, 0x24, 0x90, 0x70, 0x4c, 0x02, 0x89, 0x07,
    0x25, 0x90, 0x90, 0x5c, 0x0e, 0x89, 0xc6, 0x44, 0x60, 0xc2, 0x7c, 0x04,
    0xa7, 0x0c, 0x48, 0x80, 0xd2, 0x84, 0x04, 0xa9, 0x8d, 0x48, 0x88, 0x3e,
    0xce, 0x48, 0xa0, 0xea, 0x90, 0x04, 0x2c, 0x4f, 0xe9, 0xb0, 0x1a, 0x10,
    0x02, 0x89, 0xd7, 0x24, 0x90, 0x80, 0x51, 0x02, 0x89, 0x58, 0x25, 0x90,
    0x90, 0x4d, 0x02, 0x89, 0xd9, 0x24, 0x90, 0xa0, 0x51, 0x02, 0x89, 0xd8,
    0xe5, 0x90, 0x90, 0x4d, 0x82, 0xef, 0x74, 0xe6, 0x34, 0x82, 0xf0, 0x7c,
    0x26, 0x35, 0x84, 0xf1, 0x64, 0x66, 0x35, 0x82, 0xf2, 0x6c, 0xe6, 0x34,
    0x82, 0xc9, 0x84, 0xe6, 0x34, 0x82, 0xc9, 0x84, 0x26, 0x35, 0x84, 0xc9,
    0x8c, 0x66, 0x35, 0x8e, 0xc9, 0x8c, 0xe6, 0x34, 0x8c, 0xf8, 0xcc, 0xe7,
    0x34, 0x8c, 0xf8, 0xcc, 0x27, 0x35, 0x8c, 0xf8, 0xcc, 0x67, 0x35, 0x8c,
    0xf8, 0xcc, 0xe7, 0x34, 0x8c, 0xf8, 0xcc, 0xe7, 0x34, 0x8c, 0xf8, 0xcc,
    0x27, 0x35, 0x88, 0xed, 0xcc, 0xe7, 0x35, 0x90, 0xee, 0x28, 0x5a, 0x03,
    0x09, 0x03, 0x08, 0xff, 0x07, 0x01, 0x09, 0x02, 0x01, 0x90, 0x0b, 0x31,
    0x05, 0xa0, 0x02, 0xf0, 0x00, 0x08, 0xff, 0x09, 0x03, 0x07, 0x02, 0x09,
    0x02, 0x0b, 0x31, 0x01, 0x70, 0x05, 0xd0, 0x07, 0x02, 0x02, 0xf8, 0x04,
    0x01, 0x00, 0x09, 0x02, 0x06, 0x05, 0x0b, 0x31, 0x08, 0xff, 0x02, 0xf0,
    0x07, 0x06, 0x02, 0x00, 0x07, 0x16, 0x0a, 0x25, 0x00, 0x09, 0x03, 0x08,
    0xff, 0x07, 0x01, 0x09, 0x00, 0x06, 0x05, 0x0b, 0x3d, 0x02, 0xf0, 0x07,
    0x06, 0x02, 0x00, 0x07, 0x20, 0x02, 0xf0, 0x00, 0x09, 0x03, 0x08, 0xff,
    0x07, 0x01, 0x09, 0x02, 0x01, 0x50, 0x06, 0x01, 0x0b, 0x31, 0x07, 0x05,
    0x02, 0xfe, 0x00, 0x09, 0x02, 0x01, 0x80, 0x0b, 0x3d, 0x08, 0xc0, 0x02,
    0x08, 0x07, 0x02, 0x02, 0xf0, 0x07, 0x02, 0x02, 0x00, 0x07, 0x16, 0x0a,
    0x34, 0x00, 0x09, 0x03, 0x08, 0xff, 0x02, 0xfc, 0x00, 0x03, 0x00, 0x09,
    0x03, 0x08, 0xff, 0x07, 0x01, 0x09, 0x02, 0x06, 0x05, 0x02, 0xff, 0x0b,
    0x3d, 0x07, 0x03, 0x0b, 0x38, 0x07, 0x03, 0x0b, 0x34, 0x07, 0x03, 0x0b,
    0x31, 0x07, 0x03, 0x04, 0x07, 0x00, 0x03, 0x00, 0x09, 0x03, 0x08, 0xff,
    0x07, 0x01, 0x09, 0x02, 0x06, 0x05, 0x02, 0xff, 0x0b, 0x3d, 0x07, 0x03,
    0x0b, 0x38, 0x07, 0x03, 0x0b, 0x35, 0x07, 0x03, 0x0b, 0x31, 0x07, 0x03,
    0x04, 0x07, 0x00, 0x03, 0x00, 0x09, 0x03, 0x08, 0xff, 0x07, 0x01, 0x09,
    0x02, 0x06, 0x05, 0x02, 0xff, 0x0b, 0x3d, 0x07, 0x03, 0x0b, 0x38, 0x07,
    0x03, 0x0b, 0x36, 0x07, 0x03, 0x0b, 0x31, 0x07, 0x03, 0x04, 0x07, 0x00,
    0x09, 0x03, 0x08, 0xff, 0x07, 0x01, 0x09, 0x00, 0x06, 0x05, 0x0b, 0x3d,
    0x02, 0xf0, 0x07, 0x06, 0x02, 0x00, 0x07, 0x06, 0x0a, 0x25, 0x00, 0x09,
    0x03, 0x08, 0xff, 0x02, 0xf0, 0x00, 0x08, 0xc4, 0x09, 0x00, 0x06, 0x05,
    0x0b, 0x3d, 0x02, 0xf0, 0x07, 0x06, 0x02, 0x00, 0x07, 0x06, 0x0a, 0x25,
    0x00, 0x00, 0x00, 0x6b, 0x04, 0x00, 0x20, 0x0d, 0x2c, 0x23, 0x58, 0x23,
    0x00, 0xb4, 0x81, 0x80, 0x44, 0xc0, 0x34, 0x90, 0x06, 0xd2, 0xc0, 0x32,
    0x02, 0x60, 0x8d, 0x40, 0x1a, 0x00, 0x11, 0x00, 0x6b, 0x04, 0xd2, 0x00,
    0x00, 0x2c, 0x23, 0x00, 0x80, 0x35, 0x02, 0x00, 0x90, 0x06, 0x80, 0x65,
    0x04, 0x00, 0x00, 0x00, 0x94, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x8b, 0x45, 0x62,
    0x89, 0x25, 0x96, 0x18, 0x12, 0x03, 0x90, 0x18, 0xc2, 0x42, 0x58, 0x00,
    0xc2, 0x42, 0x4a, 0x48, 0x09, 0x01, 0x21, 0x20, 0x00, 0x2b, 0x1d, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xcb,
    0x08, 0x11, 0x6a, 0x8d, 0x48, 0x23, 0x0d, 0xa4, 0x81, 0x65, 0x84, 0x08,
    0xd0, 0x0c, 0x42, 0x11, 0x01, 0xd6, 0x08, 0xa4, 0x81, 0x65, 0x84, 0x08,
    0xb5, 0x46, 0x20, 0x0d, 0xa4, 0x81, 0x65, 0x84, 0x08, 0xb5, 0x46, 0x20,
    0x8d, 0x34, 0xd2, 0x48, 0x03, 0x6b, 0x04, 0x00, 0x20, 0x0d, 0x2c, 0x23,
    0x44, 0x80, 0x35, 0x02, 0x40, 0x1b, 0x08, 0x98, 0xc6, 0x32, 0x42, 0x04,
    0x58, 0x23, 0x90, 0x06, 0xd2, 0xc0, 0x32, 0x82, 0x35, 0x02, 0xcb, 0x08,
    0x11, 0x20, 0x19, 0x22, 0x00, 0x6b, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa7, 0xd1, 0x00, 0x20, 0x25,
    0x84, 0x86, 0xcc, 0x90, 0x12, 0x12, 0x43, 0x6c, 0xc8, 0x0c, 0xa1, 0x21,
    0x25, 0x84, 0x86, 0xcc, 0x90, 0x12, 0x12, 0x43, 0x66, 0xc8, 0x0b, 0xc7,
    0xcd, 0x00, 0x00, 0x44, 0x60, 0x00, 0x48, 0x0c, 0x20, 0x02, 0x03, 0x40,
    0x70, 0x44, 0x20, 0x22, 0x00, 0x82, 0x23, 0x02, 0x24, 0x07, 0x10, 0x01,
    0x62, 0x23, 0x02, 0xa4, 0x87, 0xdc, 0x00, 0xc3, 0x0d, 0x40, 0x04, 0x06,
    0x80, 0xc4, 0x00, 0x22, 0x30, 0x00, 0x04, 0x87, 0xd8, 0x10, 0x1a, 0x11,
    0x20, 0x31, 0x80, 0x08, 0x10, 0x1a, 0x11, 0x20, 0xb8, 0xe4, 0xb2, 0x4b,
    0x0f, 0x03, 0x0e, 0x80, 0x00, 0x91, 0x1e, 0x80, 0xe4, 0x10, 0x21, 0x02,
    0x44, 0x7e, 0x08, 0x10, 0x11, 0x22, 0x3b, 0x00, 0xc1, 0x21, 0x33, 0xc4,
    0x06, 0x03, 0x0e, 0xf2, 0x03, 0x11, 0x00, 0xe9, 0x01, 0x00, 0x00, 0x00,
    0x00, 0x28, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8b, 0x0d, 0x11, 0x20,
    0x33, 0x22, 0x30, 0x10, 0x1a, 0x88, 0x00, 0x48, 0x8c, 0x08, 0x1c, 0x1b,
    0x11, 0x20, 0x31, 0x22, 0x40, 0x70, 0xb1, 0x85, 0x86, 0xe0, 0x88, 0x00,
    0xb1, 0x21, 0x34, 0x22, 0x40, 0x66, 0x44, 0x80, 0xc4, 0x00, 0x00, 0x63,
    0x0d, 0xe7, 0x3f, 0x25, 0x24, 0x86, 0x94, 0x10, 0x16, 0x52, 0x42, 0x62,
    0x1c, 0x80, 0x94, 0x10, 0x5a, 0x66, 0x89, 0x85, 0x15, 0x55, 0x50, 0x29,
    0x05, 0x14, 0x4e, 0x3c, 0xf9, 0x04, 0x94, 0x52, 0x5a, 0x89, 0x05, 0x07,
    0x2b, 0x0d, 0xe7, 0x3f, 0x19, 0x24, 0x83, 0xa0, 0x90, 0x0c, 0x52, 0x42,
    0x58, 0xc8, 0x08, 0x29, 0x21, 0x19, 0x24, 0x83, 0xa0, 0x90, 0x17, 0xc2,
    0xe2, 0x00, 0xa4, 0x94, 0x53, 0x50, 0x61, 0x01, 0x63, 0x0d, 0xe7, 0x3f,
    0x31, 0x24, 0x83, 0xb0, 0x90, 0x12, 0x92, 0x41, 0x58, 0x44, 0x80, 0xc6,
    0x06, 0x80, 0x08, 0x90, 0x0c, 0xe7, 0x3f, 0x2c, 0xa4, 0x84, 0x64, 0x90,
    0x12, 0x92, 0xe1, 0x00, 0x84, 0x45, 0x04, 0xc8, 0x8b, 0x08, 0x00, 0xa3,
    0x0d, 0x11, 0x20, 0x36, 0x00, 0xc1, 0x11, 0x01, 0x81, 0xd0, 0x90, 0x5d,
    0x72, 0xc1, 0x21, 0x3b, 0xa4, 0x87, 0xfc, 0x88, 0x00, 0x11, 0x02, 0x08,
    0x0e, 0x99, 0x21, 0x36, 0x00, 0x6b, 0x14, 0x12, 0x00, 0x00, 0x80, 0x08,
    0x00, 0xc4, 0x10, 0x44, 0x16, 0x48, 0x03, 0x09, 0x20, 0x01, 0x00, 0x20,
    0x02, 0x00, 0x00, 0x00, 0x00, 0xb0, 0x46, 0x01, 0x44, 0x40, 0x20, 0x0d,
    0x10, 0x01, 0x90, 0x06, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4b,
    0x14, 0x00, 0x20, 0x15, 0x10, 0x01, 0x10, 0x08, 0x92, 0x40, 0x0c, 0xc8,
    0x02, 0x59, 0x00, 0x44, 0x80, 0x20, 0x88, 0xc0, 0x40, 0x06, 0x44, 0x60,
    0x20, 0x08, 0x00, 0x0b, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x4b, 0x14, 0x00, 0x20, 0x15, 0x10, 0x01,
    0x10, 0x08, 0x92, 0x40, 0x0c, 0x08, 0x02, 0x41, 0x00, 0x44, 0x80, 0x20,
    0x88, 0xc0, 0x40, 0x06, 0x44, 0x60, 0x20, 0x08, 0x00, 0x43, 0x19, 0x00,
    0x00, 0x00, 0x80, 0x8c, 0x40, 0x04, 0x40, 0x56, 0x00, 0x40, 0x04, 0x88,
    0x0a, 0x00, 0x88, 0x00, 0x41, 0x01, 0x44, 0x00, 0x3b, 0x19, 0x00, 0x10,
    0x01, 0x82, 0x22, 0x02, 0x44, 0x05, 0x00, 0x00, 0x00, 0x4a, 0x3f, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x23, 0x19, 0x00, 0x00, 0x00, 0x80, 0x98, 0x00,
    0x04, 0x05, 0x00, 0x00, 0x42, 0x02, 0x58, 0xf8, 0x00, 0x00, 0x1b, 0x19,
    0x00, 0x00, 0xa0, 0xfc, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x3b, 0x19, 0x80, 0x9c, 0x10, 0x14, 0x80, 0xa0, 0x10, 0x15, 0x00,
    0x0a, 0x3e, 0x80, 0xb4, 0x00, 0xa4, 0x85, 0xa8, 0x88, 0x00, 0x21, 0x11,
    0x01, 0x32, 0x22, 0x02, 0x00, 0x43, 0x19, 0x00, 0x0a, 0x3f, 0x80, 0xac,
    0x00, 0x50, 0xf8, 0x01, 0x44, 0x05, 0x80, 0xc2, 0x0f, 0x20, 0x26, 0xb0,
    0x06, 0x00, 0x00, 0x43, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x23, 0x14, 0x80, 0x10, 0x10, 0x08, 0x80,
    0x2c, 0x10, 0x07, 0xf2, 0x40, 0x08, 0x00, 0x42, 0x40, 0x20, 0x00, 0xb2,
    0x40, 0x20, 0x81, 0x64, 0x11, 0x02, 0x5b, 0x14, 0x80, 0x2c, 0x90, 0x0b,
    0x80, 0x18, 0x90, 0x04, 0xa2, 0x40, 0x16, 0x00, 0xd2, 0x00, 0x10, 0x07,
    0x80, 0x3c, 0x58, 0xe0, 0x00, 0x63, 0x14, 0xc2, 0x00, 0x11, 0x00, 0x39,
    0x20, 0x0a, 0x84, 0x01, 0x22, 0x00, 0xa2, 0x40, 0x18, 0x08, 0x03, 0x44,
    0x00, 0xe4, 0x80, 0x3c, 0x10, 0x07, 0xc2, 0x40, 0x0a, 0x00, 0x3b, 0x14,
    0x72, 0x00, 0x90, 0x09, 0x80, 0x40, 0x10, 0x07, 0xb2, 0x40, 0x12, 0x39,
    0xe4, 0x92, 0x4a, 0x26, 0x81, 0xc4, 0x91, 0x44, 0x0e, 0x00, 0x92, 0x00,
    0x00, 0x5b, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x63, 0x14, 0x00, 0x00, 0x00, 0x80, 0x30, 0x00, 0xc4,
    0x01, 0x00, 0x00, 0x00, 0x20, 0x0e, 0x00, 0x83, 0x14, 0x00, 0x20, 0x10,
    0x10, 0x01, 0x00, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x2b, 0x1d, 0x00, 0x00, 0x00, 0x86, 0x42, 0x00, 0x41, 0x11, 0x01, 0x82,
    0x02, 0x00, 0x84, 0x46, 0x04, 0x04, 0x42, 0x23, 0x02, 0x04, 0x05, 0x00,
    0x1b, 0x25, 0x00, 0x10, 0x01, 0xf2, 0x22, 0x02, 0x02, 0x19, 0x01, 0xc8,
    0x88, 0x08, 0x90, 0x17, 0x80, 0x8c, 0x00, 0x64, 0x84, 0xbc, 0x90, 0x11,
    0x00, 0x23, 0x25, 0x00, 0x10, 0x01, 0x02, 0x23, 0x02, 0x02, 0x21, 0x01,
    0x08, 0x89, 0x08, 0x10, 0x12, 0x00, 0x20, 0x30, 0x22, 0x20, 0x10, 0x18,
    0x11, 0x20, 0x24, 0x00, 0x5b, 0x25, 0x11, 0x20, 0x1f, 0x80, 0x08, 0x90,
    0x15, 0x11, 0x20, 0x2b, 0x22, 0x40, 0x3e, 0x00, 0x11, 0x20, 0x2b, 0x22,
    0x40, 0x56, 0x44, 0x80, 0x7c, 0x00, 0x22, 0x40, 0x3e, 0x00, 0xb2, 0x22,
    0x02, 0x02, 0xf9, 0x10, 0x01, 0x00, 0x23, 0x25, 0x80, 0x90, 0x88, 0x00,
    0x81, 0x81, 0x08, 0x80, 0xc0, 0x88, 0x00, 0x21, 0x01, 0x88, 0x09, 0x44,
    0x00, 0xc4, 0x44, 0x04, 0x88, 0x0c, 0x20, 0x02, 0x44, 0x46, 0x04, 0x88,
    0x89, 0x08, 0x10, 0x19, 0x11, 0x00, 0xa3, 0x25, 0x42, 0x0b, 0x2d, 0x34,
    0x22, 0x40, 0x50, 0x20, 0x02, 0x20, 0x28, 0x22, 0x40, 0x50, 0x08, 0x0d,
    0x41, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa3, 0x29, 0x42, 0x0b, 0x2a,
    0x34, 0x22, 0x40, 0x50, 0x20, 0x02, 0x20, 0x28, 0x22, 0x40, 0x50, 0x00,
    0x42, 0x43, 0x68, 0xa1, 0x85, 0x46, 0x04, 0x08, 0x0a, 0x44, 0x00, 0x04,
    0x45, 0x04, 0x08, 0x0a, 0xa1, 0x01, 0x0b, 0x25, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xc8, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2b, 0x21, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2b,
    0x1d, 0x00, 0x00, 0x18, 0x6b, 0x91, 0x12, 0x80, 0xb0, 0x90, 0x18, 0xc2,
    0x42, 0x4a, 0x08, 0x8d, 0x02, 0x90, 0x19, 0xc2, 0x42, 0x4a, 0x88, 0x0d,
    0xc3, 0x2d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x48, 0x0c,
    0xa1, 0xc5, 0x96, 0x5b, 0x70, 0xd9, 0xa5, 0x07, 0x03, 0x2e, 0x00, 0x00,
    0x48, 0x0f, 0x40, 0x80, 0x00, 0xf2, 0x23, 0x02, 0x64, 0x07, 0x22, 0x00,
    0x82, 0x03, 0x10, 0x1c, 0x11, 0x20, 0x3f, 0x64, 0x07, 0x03, 0x6e, 0x7e,
    0xe9, 0x05, 0x17, 0x5a, 0x66, 0xb1, 0x65, 0x16, 0x5a, 0x66, 0x89, 0x85,
    0x95, 0x57, 0x54, 0x99, 0xe5, 0x95, 0x18, 0x11, 0x20, 0x31, 0x22, 0x40,
    0x62, 0x44, 0x80, 0xc4, 0x00, 0x22, 0x40, 0x62, 0x44, 0x80, 0xf4, 0x88,
    0x00, 0x89, 0x11, 0x01, 0x00, 0x03, 0x2e, 0x12, 0x43, 0x62, 0x48, 0x0f,
    0x89, 0x21, 0x3d, 0x44, 0x18, 0x60, 0x7e, 0x08, 0x10, 0x89, 0x21, 0x31,
    0xa4, 0x87, 0xc4, 0x90, 0x1e, 0x22, 0x0c, 0x30, 0x3f, 0x00, 0xfb, 0x2d,
    0x02, 0x24, 0x02, 0x84, 0x48, 0x04, 0x48, 0x6f, 0xc4, 0x9e, 0x70, 0xd7,
    0xeb, 0xc5, 0x76, 0xc2, 0x1e, 0x70, 0x17, 0xeb, 0xe1, 0x76, 0xc0, 0xde,
    0x6f, 0xd7, 0xeb, 0xc5, 0x76, 0xbf, 0xde, 0x6e, 0x17, 0xeb, 0xe1, 0x76,
    0xbb, 0x1e, 0x6e, 0x87, 0xeb, 0xc5, 0x76, 0xc4, 0xde, 0x6e, 0x17, 0xeb,
    0xf5, 0x36, 0x3f, 0xa3, 0x0d, 0x00, 0x00, 0x48, 0x0c, 0x40, 0x68, 0x00,
    0x32, 0x03, 0x00, 0x00, 0x89, 0x01, 0xc8, 0x0b, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xbd, 0x26, 0xb1, 0xcc,
    0x00, 0x7f, 0x4d, 0x41, 0x00, 0x48, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x2b, 0x21, 0x11, 0x20, 0x25, 0x80, 0x08,
    0x90, 0x58, 0x62, 0x29, 0x11, 0x01, 0x52, 0x22, 0x02, 0x00, 0x29, 0x11,
    0x01, 0x56, 0x52, 0x22, 0x00, 0x90, 0x12, 0x11, 0x60, 0x25, 0x24, 0x02,
    0x00, 0x29, 0x11, 0x01, 0x52, 0x22, 0x02, 0xa4, 0x44, 0x04, 0x00, 0xcf,
    0xcc, 0xc0, 0x11, 0x01, 0x52, 0x22, 0x02, 0xa4, 0x44, 0x04, 0x48, 0x86,
    0x08, 0x90, 0x12, 0x11, 0x20, 0x19, 0x22, 0x40, 0x4a, 0x44, 0x80, 0x94,
    0x88, 0x00, 0xc9, 0x10, 0x01, 0x52, 0x22, 0x02, 0xa4, 0x44, 0x04, 0x48,
    0x86, 0x08, 0x90, 0x12, 0x11, 0xa0, 0x99, 0x01, 0x68, 0x46, 0x80, 0x53,
    0x22, 0x02, 0xa4, 0x44, 0x04
};

volatile unsigned char callbackwait;
volatile unsigned char lastsample;
volatile unsigned char timetoplay;

unsigned char trackwait;
unsigned char trackpos;
unsigned char playsong;
unsigned char songpos;
unsigned long noiseseed = 1;

const unsigned short freqtable[] = {
	0x0085, 0x008d, 0x0096, 0x009f, 0x00a8, 0x00b2, 0x00bd, 0x00c8, 0x00d4,
	0x00e1, 0x00ee, 0x00fc, 0x010b, 0x011b, 0x012c, 0x013e, 0x0151, 0x0165,
	0x017a, 0x0191, 0x01a9, 0x01c2, 0x01dd, 0x01f9, 0x0217, 0x0237, 0x0259,
	0x027d, 0x02a3, 0x02cb, 0x02f5, 0x0322, 0x0352, 0x0385, 0x03ba, 0x03f3,
	0x042f, 0x046f, 0x04b2, 0x04fa, 0x0546, 0x0596, 0x05eb, 0x0645, 0x06a5,
	0x070a, 0x0775, 0x07e6, 0x085f, 0x08de, 0x0965, 0x09f4, 0x0a8c, 0x0b2c,
	0x0bd6, 0x0c8b, 0x0d4a, 0x0e14, 0x0eea, 0x0fcd, 0x10be, 0x11bd, 0x12cb,
	0x13e9, 0x1518, 0x1659, 0x17ad, 0x1916, 0x1a94, 0x1c28, 0x1dd5, 0x1f9b,
	0x217c, 0x237a, 0x2596, 0x27d3, 0x2a31, 0x2cb3, 0x2f5b, 0x322c, 0x3528,
	0x3851, 0x3bab, 0x3f37
};

const char sinetable[] = {
	0, 12, 25, 37, 49, 60, 71, 81, 90, 98, 106, 112, 117, 122, 125, 126,
	127, 126, 125, 122, 117, 112, 106, 98, 90, 81, 71, 60, 49, 37, 25, 12,
	0, -12, -25, -37, -49, -60, -71, -81, -90, -98, -106, -112, -117, -122,
	-125, -126, -127, -126, -125, -122, -117, -112, -106, -98, -90, -81,
	-71, -60, -49, -37, -25, -12
};

const unsigned char validcmds[] = "0dfijlmtvw~+=";

enum {
	WF_TRI,
	WF_SAW,
	WF_PUL,
	WF_NOI
};

volatile struct oscillator {
	unsigned short	freq;
	unsigned short	phase;
	unsigned short	duty;
	unsigned char	waveform;
	unsigned char	volume;	// 0-255
} osc[4];

struct trackline {
	unsigned char	note;
	unsigned char	instr;
	unsigned char	cmd[2];
	unsigned char	param[2];
	};

struct track {
	struct trackline	line[TRACKLEN];
};

struct unpacker {
	unsigned short	nextbyte;
	unsigned char	buffer;
	unsigned char	bits;
};

struct channel {
	struct unpacker		trackup;
	unsigned char			tnum;
	char			transp;
	unsigned char			tnote;
	unsigned char			lastinstr;
	unsigned char			inum;
	unsigned short			iptr;
	unsigned char			iwait;
	unsigned char			inote;
	char			bendd;
	short			bend;
	char			volumed;
	short			dutyd;
	unsigned char			vdepth;
	unsigned char			vrate;
	unsigned char			vpos;
	short			inertia;
	unsigned short			slur;
} channel[4];

unsigned short resources[16 + MAXTRACK];

struct unpacker songup;

byte readsongbyte(unsigned short offset)
{
  
   return pgm_read_byte_near(&songdata[0] + offset); 
}

void initup(struct unpacker *up, unsigned short offset) {
	up->nextbyte = offset;
	up->bits = 0;
}

unsigned char readbit(struct unpacker *up) {
	unsigned char val;

	if(!up->bits) {
		up->buffer = readsongbyte(up->nextbyte++);
		up->bits = 8;
	}

	up->bits--;
	val = up->buffer & 1;
	up->buffer >>= 1;

	return val;
}

unsigned short readchunk(struct unpacker *up, unsigned char n) {
	unsigned short val = 0;
	unsigned char i;

	for(i = 0; i < n; i++) {
		if(readbit(up)) {
			val |= (1 << i);
		}
	}

	return val;
}

void readinstr(byte num, byte pos, byte *dest) {
	dest[0] = readsongbyte(resources[num] + 2 * pos + 0);
	dest[1] = readsongbyte(resources[num] + 2 * pos + 1);
}

void runcmd(unsigned char ch, unsigned char cmd, unsigned char param) {
	switch(validcmds[cmd]) {
		case '0':
			channel[ch].inum = 0;
			break;
		case 'd':
			osc[ch].duty = param << 8;
			break;
		case 'f':
			channel[ch].volumed = param;
			break;
		case 'i':
			channel[ch].inertia = param << 1;
			break;
		case 'j':
			channel[ch].iptr = param;
			break;
		case 'l':
			channel[ch].bendd = param;
			break;
		case 'm':
			channel[ch].dutyd = param << 6;
			break;
		case 't':
			channel[ch].iwait = param;
			break;
		case 'v':
			osc[ch].volume = param;
			break;
		case 'w':
			osc[ch].waveform = param;
			break;
		case '+':
			channel[ch].inote = param + channel[ch].tnote - 12 * 4;
			break;
		case '=':
			channel[ch].inote = param;
			break;
		case '~':
			if(channel[ch].vdepth != (param >> 4)) {
				channel[ch].vpos = 0;
			}
			channel[ch].vdepth = param >> 4;
			channel[ch].vrate = param & 15;
			break;
	}
}

void initresources() {
	unsigned char i;
	struct unpacker up;

	initup(&up, 0);
	for(i = 0; i < 16 + MAXTRACK; i++) {
		resources[i] = readchunk(&up, 13);
	}

	initup(&songup, resources[0]);
}

void playroutine() {			// called at 50 Hz
	unsigned char ch;
	unsigned char lights;

	if(playsong) {
		if(trackwait) {
			trackwait--;
		} else {
			trackwait = 4;

			if(!trackpos) {
				if(playsong) {
					if(songpos >= SONGLEN) {
						//playsong = 0;
                                                songpos=0;
                                                trackpos=0;
                                                initresources();
					} else {
						for(ch = 0; ch < 4; ch++) {
							unsigned char gottransp;
							unsigned char transp;

							gottransp = readchunk(&songup, 1);
							channel[ch].tnum = readchunk(&songup, 6);
							if(gottransp) {
								transp = readchunk(&songup, 4);
								if(transp & 0x8) transp |= 0xf0;
							} else {
								transp = 0;
							}
							channel[ch].transp = (char) transp;
							if(channel[ch].tnum) {
								initup(&channel[ch].trackup, resources[16 + channel[ch].tnum - 1]);
							}
						}
						songpos++;
					}
				}
			}

			if(playsong) {
				for(ch = 0; ch < 4; ch++) {
					if(channel[ch].tnum) {
						unsigned char note, instr, cmd, param;
						unsigned char fields;

						fields = readchunk(&channel[ch].trackup, 3);
						note = 0;
						instr = 0;
						cmd = 0;
						param = 0;
						if(fields & 1) note = readchunk(&channel[ch].trackup, 7);
						if(fields & 2) instr = readchunk(&channel[ch].trackup, 4);
						if(fields & 4) {
							cmd = readchunk(&channel[ch].trackup, 4);
							param = readchunk(&channel[ch].trackup, 8);
						}
						if(note) {
							channel[ch].tnote = note + channel[ch].transp;
							if(!instr) instr = channel[ch].lastinstr;
						}
						if(instr) {
							LED_PORT ^= 1 << LED_BIT;
							channel[ch].lastinstr = instr;
							channel[ch].inum = instr;
							channel[ch].iptr = 0;
							channel[ch].iwait = 0;
							channel[ch].bend = 0;
							channel[ch].bendd = 0;
							channel[ch].volumed = 0;
							channel[ch].dutyd = 0;
							channel[ch].vdepth = 0;
						}
						if(cmd) runcmd(ch, cmd, param);
					}
				}

				trackpos++;
				trackpos &= 31;
			}
		}
	}

	for(ch = 0; ch < 4; ch++) {
		short vol;
		unsigned short duty;
		unsigned short slur;

		while(channel[ch].inum && !channel[ch].iwait) {
			unsigned char il[2];

			readinstr(channel[ch].inum, channel[ch].iptr, il);
			channel[ch].iptr++;

			runcmd(ch, il[0], il[1]);
		}
		if(channel[ch].iwait) channel[ch].iwait--;

		if(channel[ch].inertia) {
			short diff;

			slur = channel[ch].slur;
			diff = freqtable[channel[ch].inote] - slur;
			//diff >>= channel[ch].inertia;
			if(diff > 0) {
				if(diff > channel[ch].inertia) diff = channel[ch].inertia;
			} else if(diff < 0) {
				if(diff < -channel[ch].inertia) diff = -channel[ch].inertia;
			}
			slur += diff;
			channel[ch].slur = slur;
		} else {
			slur = freqtable[channel[ch].inote];
		}
		osc[ch].freq =
			slur +
			channel[ch].bend +
			((channel[ch].vdepth * sinetable[channel[ch].vpos & 63]) >> 2);
		channel[ch].bend += channel[ch].bendd;
		vol = osc[ch].volume + channel[ch].volumed;
		if(vol < 0) vol = 0;
		if(vol > 255) vol = 255;
		osc[ch].volume = vol;

		duty = osc[ch].duty + channel[ch].dutyd;
		if(duty > 0xe000) duty = 0x2000;
		if(duty < 0x2000) duty = 0xe000;
		osc[ch].duty = duty;

		channel[ch].vpos += channel[ch].vrate;
	}

}


void setup() {

	pinMode(LED_PIN, OUTPUT);

  analogReference(DEFAULT);
  pinMode(DAC0, ANALOG);

  cli();

  TCCR1B = (TCCR1B & ~_BV(WGM13)) | _BV(WGM12);
  TCCR1A = TCCR1A & ~(_BV(WGM11) | _BV(WGM10));
  TCCR1B = (TCCR1B & ~(_BV(CS12) | _BV(CS11))) | _BV(CS10);
  OCR1A = F_CPU / SAMPLE_RATE;
  TIMSK1 |= _BV(OCIE1A);

  sei();

	timetoplay = 0;
	trackwait = 0;
	trackpos = 0;
	playsong = 1;
	songpos = 0;   

	osc[0].volume = 0;
	channel[0].inum = 0;
	osc[1].volume = 0;
	channel[1].inum = 0;
	osc[2].volume = 0;
	channel[2].inum = 0;
	osc[3].volume = 0;
	channel[3].inum = 0;

	initresources();
	
}


ISR(TIMER1_COMPA_vect) {

	char i;
	short acc;
	char newbit;

	DALR = fuzz(lastsample, 80, 4);

	newbit = 0;
	if(noiseseed & 0x80000000L) newbit ^= 1;
	if(noiseseed & 0x01000000L) newbit ^= 1;
	if(noiseseed & 0x00000040L) newbit ^= 1;
	if(noiseseed & 0x00000200L) newbit ^= 1;
	noiseseed = (noiseseed << 1) | newbit;

	if(callbackwait) {
		callbackwait--;
	} else {
		timetoplay++;
		callbackwait = 180 - 1;
	}

	acc = 0;
	for(i = 0; i < 4; i++) {
		char value; // [-32,31]

		switch(osc[i].waveform) {
			case WF_TRI:
				if(osc[i].phase < 0x8000) {
					value = -32 + (osc[i].phase >> 9);
				} else {
					value = 31 - ((osc[i].phase - 0x8000) >> 9);
				}
				break;
			case WF_SAW:
				value = -32 + (osc[i].phase >> 10);
				break;
			case WF_PUL:
				value = (osc[i].phase > osc[i].duty)? -32 : 31;
				break;
			case WF_NOI:
				value = (noiseseed & 63) - 32;
				break;
			default:
				value = 0;
				break;
		}
		osc[i].phase += osc[i].freq;

		acc += value * osc[i].volume; // rhs = [-8160,7905]
	}

	lastsample = 128 + (acc >> 8);

}

void loop() {
	
	while(!timetoplay);
	
	timetoplay--;
	playroutine();

}