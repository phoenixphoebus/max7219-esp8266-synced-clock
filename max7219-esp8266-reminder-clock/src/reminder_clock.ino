#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <time.h> 

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4

#define CS_PIN    15

// HARDWARE SPI
MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
const uint16_t PAUSE_TIME = 1000;

const char TIME_PLACEHOLDER[] = "time placeholder - 070190f8-3ee9-4316-8af2-e53ee8cd0983";
const char MSG1_PLACEHOLDER[] = "msg1 placeholder - 070190f8-3ee9-4316-8af2-e53ee8cd0983";
const char MSG2_PLACEHOLDER[] = "msg2 placeholder - 070190f8-3ee9-4316-8af2-e53ee8cd0983";

// Sprite Definitions
const uint8_t F_PMAN1 = 6;
const uint8_t W_PMAN1 = 8;
const uint8_t PROGMEM pacman1[F_PMAN1 * W_PMAN1] =  // gobbling pacman animation
{
  0x00, 0x81, 0xc3, 0xe7, 0xff, 0x7e, 0x7e, 0x3c,
  0x00, 0x42, 0xe7, 0xe7, 0xff, 0xff, 0x7e, 0x3c,
  0x24, 0x66, 0xe7, 0xff, 0xff, 0xff, 0x7e, 0x3c,
  0x3c, 0x7e, 0xff, 0xff, 0xff, 0xff, 0x7e, 0x3c,
  0x24, 0x66, 0xe7, 0xff, 0xff, 0xff, 0x7e, 0x3c,
  0x00, 0x42, 0xe7, 0xe7, 0xff, 0xff, 0x7e, 0x3c,
};

const uint8_t F_PMAN2 = 6;
const uint8_t W_PMAN2 = 18;
const uint8_t PROGMEM pacman2[F_PMAN2 * W_PMAN2] =  // ghost pursued by a pacman
{
  0x00, 0x81, 0xc3, 0xe7, 0xff, 0x7e, 0x7e, 0x3c, 0x00, 0x00, 0x00, 0xfe, 0x7b, 0xf3, 0x7f, 0xfb, 0x73, 0xfe,
  0x00, 0x42, 0xe7, 0xe7, 0xff, 0xff, 0x7e, 0x3c, 0x00, 0x00, 0x00, 0xfe, 0x7b, 0xf3, 0x7f, 0xfb, 0x73, 0xfe,
  0x24, 0x66, 0xe7, 0xff, 0xff, 0xff, 0x7e, 0x3c, 0x00, 0x00, 0x00, 0xfe, 0x7b, 0xf3, 0x7f, 0xfb, 0x73, 0xfe,
  0x3c, 0x7e, 0xff, 0xff, 0xff, 0xff, 0x7e, 0x3c, 0x00, 0x00, 0x00, 0xfe, 0x7b, 0xf3, 0x7f, 0xfb, 0x73, 0xfe,
  0x24, 0x66, 0xe7, 0xff, 0xff, 0xff, 0x7e, 0x3c, 0x00, 0x00, 0x00, 0xfe, 0x7b, 0xf3, 0x7f, 0xfb, 0x73, 0xfe,
  0x00, 0x42, 0xe7, 0xe7, 0xff, 0xff, 0x7e, 0x3c, 0x00, 0x00, 0x00, 0xfe, 0x7b, 0xf3, 0x7f, 0xfb, 0x73, 0xfe,
};

const uint8_t F_WAVE = 14;
const uint8_t W_WAVE = 14;
const uint8_t PROGMEM wave[F_WAVE * W_WAVE] =  // triangular wave / worm
{
  0x08, 0x04, 0x02, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x40, 0x20, 0x10,
  0x10, 0x08, 0x04, 0x02, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x40, 0x20,
  0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x40,
  0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80,
  0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40,
  0x40, 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20,
  0x20, 0x40, 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0x02, 0x04, 0x08, 0x10,
  0x10, 0x20, 0x40, 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0x02, 0x04, 0x08,
  0x08, 0x10, 0x20, 0x40, 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0x02, 0x04,
  0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0x02,
  0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01,
  0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02,
  0x02, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x40, 0x20, 0x10, 0x08, 0x04,
  0x04, 0x02, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x40, 0x20, 0x10, 0x08,
};

const uint8_t F_ROLL1 = 4;
const uint8_t W_ROLL1 = 8;
const uint8_t PROGMEM roll1[F_ROLL1 * W_ROLL1] =  // rolling square
{
  0xff, 0x8f, 0x8f, 0x8f, 0x81, 0x81, 0x81, 0xff,
  0xff, 0xf1, 0xf1, 0xf1, 0x81, 0x81, 0x81, 0xff,
  0xff, 0x81, 0x81, 0x81, 0xf1, 0xf1, 0xf1, 0xff,
  0xff, 0x81, 0x81, 0x81, 0x8f, 0x8f, 0x8f, 0xff,
};

const uint8_t F_ROLL2 = 4;
const uint8_t W_ROLL2 = 8;
const uint8_t PROGMEM roll2[F_ROLL2 * W_ROLL2] =  // rolling octagon
{
  0x3c, 0x4e, 0x8f, 0x8f, 0x81, 0x81, 0x42, 0x3c,
  0x3c, 0x72, 0xf1, 0xf1, 0x81, 0x81, 0x42, 0x3c,
  0x3c, 0x42, 0x81, 0x81, 0xf1, 0xf1, 0x72, 0x3c,
  0x3c, 0x42, 0x81, 0x81, 0x8f, 0x8f, 0x4e, 0x3c,
};

const uint8_t F_LINES = 3;
const uint8_t W_LINES = 8;
const uint8_t PROGMEM lines[F_LINES * W_LINES] =  // spaced lines
{
  0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00,
  0xff, 0xff, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00,
  0xff, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff,
};

const uint8_t F_ARROW1 = 3;
const uint8_t W_ARROW1 = 10;
const uint8_t PROGMEM arrow1[F_ARROW1 * W_ARROW1] =  // arrow fading to center
{
  0x18, 0x3c, 0x7e, 0xff, 0x7e, 0x00, 0x00, 0x3c, 0x00, 0x00,
  0x18, 0x3c, 0x7e, 0xff, 0x00, 0x7e, 0x00, 0x00, 0x18, 0x00,
  0x18, 0x3c, 0x7e, 0xff, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x18,
};

const uint8_t F_ARROW2 = 3;
const uint8_t W_ARROW2 = 9;
const uint8_t PROGMEM arrow2[F_ARROW2 * W_ARROW2] =  // arrow fading to outside
{
  0x18, 0x3c, 0x7e, 0xe7, 0x00, 0x00, 0xc3, 0x00, 0x00,
  0x18, 0x3c, 0x7e, 0xe7, 0xe7, 0x00, 0x00, 0x81, 0x00,
  0x18, 0x3c, 0x7e, 0xe7, 0x00, 0xc3, 0x00, 0x00, 0x81,
};

const uint8_t F_SAILBOAT = 1;
const uint8_t W_SAILBOAT = 11;
const uint8_t PROGMEM sailboat[F_SAILBOAT * W_SAILBOAT] =  // sail boat
{
  0x10, 0x30, 0x58, 0x94, 0x92, 0x9f, 0x92, 0x94, 0x98, 0x50, 0x30,
};

const uint8_t F_STEAMBOAT = 2;
const uint8_t W_STEAMBOAT = 11;
const uint8_t PROGMEM steamboat[F_STEAMBOAT * W_STEAMBOAT] =  // steam boat
{
  0x10, 0x30, 0x50, 0x9c, 0x9e, 0x90, 0x91, 0x9c, 0x9d, 0x90, 0x71,
  0x10, 0x30, 0x50, 0x9c, 0x9c, 0x91, 0x90, 0x9d, 0x9e, 0x91, 0x70,
};

const uint8_t F_HEART = 5;
const uint8_t W_HEART = 9;
const uint8_t PROGMEM heart[F_HEART * W_HEART] =  // beating heart
{
  0x0e, 0x11, 0x21, 0x42, 0x84, 0x42, 0x21, 0x11, 0x0e,
  0x0e, 0x1f, 0x33, 0x66, 0xcc, 0x66, 0x33, 0x1f, 0x0e,
  0x0e, 0x1f, 0x3f, 0x7e, 0xfc, 0x7e, 0x3f, 0x1f, 0x0e,
  0x0e, 0x1f, 0x33, 0x66, 0xcc, 0x66, 0x33, 0x1f, 0x0e,
  0x0e, 0x11, 0x21, 0x42, 0x84, 0x42, 0x21, 0x11, 0x0e,
};

const uint8_t F_INVADER = 2;
const uint8_t W_INVADER = 10;
const uint8_t PROGMEM invader[F_INVADER * W_INVADER] =  // space invader
{
  0x0e, 0x98, 0x7d, 0x36, 0x3c, 0x3c, 0x36, 0x7d, 0x98, 0x0e,
  0x70, 0x18, 0x7d, 0xb6, 0x3c, 0x3c, 0xb6, 0x7d, 0x18, 0x70,
};

const uint8_t F_ROCKET = 2;
const uint8_t W_ROCKET = 11;
const uint8_t PROGMEM rocket[F_ROCKET * W_ROCKET] =  // rocket
{
  0x18, 0x24, 0x42, 0x81, 0x99, 0x18, 0x99, 0x18, 0xa5, 0x5a, 0x81,
  0x18, 0x24, 0x42, 0x81, 0x18, 0x99, 0x18, 0x99, 0x24, 0x42, 0x99,
};

const uint8_t F_FBALL = 2;
const uint8_t W_FBALL = 11;
const uint8_t PROGMEM fireball[F_FBALL * W_FBALL] =  // fireball
{
  0x7e, 0xab, 0x54, 0x28, 0x52, 0x24, 0x40, 0x18, 0x04, 0x10, 0x08,
  0x7e, 0xd5, 0x2a, 0x14, 0x24, 0x0a, 0x30, 0x04, 0x28, 0x08, 0x10,
};

const uint8_t F_CHEVRON = 1;
const uint8_t W_CHEVRON = 9;
const uint8_t PROGMEM chevron[F_CHEVRON * W_CHEVRON] =  // chevron
{
  0x18, 0x3c, 0x66, 0xc3, 0x99, 0x3c, 0x66, 0xc3, 0x81,
};

const uint8_t F_WALKER = 5;
const uint8_t W_WALKER = 7;
const uint8_t PROGMEM walker[F_WALKER * W_WALKER] =  // walking man
{
    0x00, 0x48, 0x77, 0x1f, 0x1c, 0x94, 0x68,
    0x00, 0x90, 0xee, 0x3e, 0x38, 0x28, 0xd0,
    0x00, 0x00, 0xae, 0xfe, 0x38, 0x28, 0x40,
    0x00, 0x00, 0x2e, 0xbe, 0xf8, 0x00, 0x00, 
    0x00, 0x10, 0x6e, 0x3e, 0xb8, 0xe8, 0x00,
};

const uint8_t F_BOUNCINGHEART = 7;
const uint8_t W_BOUNCINGHEART = 5;
const uint8_t PROGMEM bouncingHeart[F_BOUNCINGHEART * W_BOUNCINGHEART] = 
{
    0x30, 0x78, 0xf0, 0x78, 0x30,
    0x18, 0x3c, 0x78, 0x3c, 0x18,
    0x0c, 0x1e, 0x3e, 0x1e, 0x0c,
    0x06, 0x0f, 0x1e, 0x1f, 0x06,
    0x0c, 0x1e, 0x3e, 0x1e, 0x0c,
    0x18, 0x3c, 0x78, 0x3c, 0x18,
    0x30, 0x78, 0xf0, 0x78, 0x30,
};

const uint8_t F_TONGUEEMOJI = 7;
const uint8_t W_TONGUEEMOJI = 8;
const uint8_t PROGMEM tongueEmoji[F_TONGUEEMOJI * W_TONGUEEMOJI] = 
{
    0x10,0x16,0x76,0x90,0x90,0x76,0x16,0x10,
    0x10,0x16,0x36,0x50,0x50,0x36,0x16,0x10,
    0x10,0x16,0x16,0x30,0x30,0x16,0x16,0x10,
    0x10,0x16,0x16,0x10,0x10,0x16,0x16,0x10,
    0x10,0x16,0x16,0x30,0x30,0x16,0x16,0x10,
    0x10,0x16,0x36,0x50,0x50,0x36,0x16,0x10,
    0x10,0x16,0x76,0x90,0x90,0x76,0x16,0x10,
};

const uint8_t F_WINKINGEMOJI = 6;
const uint8_t W_WINKINGEMOJI = 8;
const uint8_t PROGMEM winkingEmoji[F_TONGUEEMOJI * W_TONGUEEMOJI] = 
{
    0x20,0x4e,0x4e,0x40,0x40,0x4e,0x4e,0x20,
    0x20,0x4e,0x4e,0x40,0x40,0x4e,0x4e,0x20,
    0x20,0x44,0x44,0x40,0x40,0x4e,0x4e,0x20,
    0x20,0x44,0x44,0x40,0x40,0x4e,0x4e,0x20,
    0x20,0x44,0x44,0x40,0x40,0x4e,0x4e,0x20,
    0x20,0x44,0x44,0x40,0x40,0x4e,0x4e,0x20,
};

const uint8_t F_LAUGHEMOJI = 7;
const uint8_t W_LAUGHEMOJI = 8;
const uint8_t PROGMEM laughEmoji[F_LAUGHEMOJI * W_LAUGHEMOJI] =
{
    0x30,0x56,0x96,0x90,0x90,0x96,0x56,0x30,
    0x10,0x36,0x56,0x50,0x50,0x56,0x36,0x10,
    0x10,0x36,0x36,0x30,0x30,0x36,0x36,0x10,
    0x10,0x16,0x16,0x10,0x10,0x16,0x16,0x10,
    0x10,0x36,0x36,0x30,0x30,0x36,0x36,0x10,
    0x10,0x36,0x56,0x50,0x50,0x56,0x36,0x10,
    0x30,0x56,0x96,0x90,0x90,0x96,0x56,0x30,
};

const uint8_t F_WHEEL = 6;
const uint8_t W_WHEEL = 8;
const uint8_t PROGMEM wheelreverse[F_WHEEL * W_WHEEL] =
{
    0x3c,0x5a,0x99,0xff,0xff,0x99,0x5a,0x3c,
    0x3c,0x4e,0xed,0xf9,0x9f,0xb7,0x72,0x3c,
    0x3c,0x66,0xef,0xb9,0x9d,0xf7,0x66,0x3c,
    0x3c,0x66,0xf7,0x9d,0xb9,0xef,0x66,0x3c,
    0x3c,0x72,0xb7,0x9f,0xf9,0xed,0x4e,0x3c,
    0x3c,0x7a,0x93,0xdf,0xfb,0xc9,0x5e,0x3c,
};

const uint8_t PROGMEM wheelforward[F_WHEEL * W_WHEEL] =  
{
    0x3c,0x5a,0x99,0xff,0xff,0x99,0x5a,0x3c,
    0x3c,0x7a,0x93,0xdf,0xfb,0xc9,0x5e,0x3c,
    0x3c,0x72,0xb7,0x9f,0xf9,0xed,0x4e,0x3c,
    0x3c,0x66,0xf7,0x9d,0xb9,0xef,0x66,0x3c,
    0x3c,0x66,0xef,0xb9,0x9d,0xf7,0x66,0x3c,
    0x3c,0x4e,0xed,0xf9,0x9f,0xb7,0x72,0x3c,
};

const uint8_t F_HOURGLASS = 19;
const uint8_t W_HOURGLASS = 8;
const uint8_t PROGMEM hourglass[F_HOURGLASS * W_HOURGLASS] = 
{
    0xc3,0xa7,0x9f,0x9f,0xa7,0xc3,0x00,0x00,
    0xc3,0xa7,0x9f,0x9f,0xa7,0xc3,0x00,0x00,
    //start dropping
    0xc3,0xa7,0x9f,0xbd,0xa7,0xc3,0x00,0x00,
    0xc3,0xa7,0x9f,0xdd,0xa7,0xc3,0x00,0x00,
    0xc3,0xa7,0xbd,0xdd,0xa7,0xc3,0x00,0x00,
    0xc3,0xa7,0xdd,0xdd,0xa7,0xc3,0x00,0x00,
    0xc3,0xa5,0xfd,0xdd,0xa7,0xc3,0x00,0x00,
    0xc3,0xe5,0xdd,0xdd,0xa7,0xc3,0x00,0x00,
    0xc3,0xe5,0xdd,0xfd,0xa5,0xc3,0x00,0x00,
    0xc3,0xe5,0xdd,0xdd,0xe5,0xc3,0x00,0x00,
    0xc3,0xe5,0xf9,0xdd,0xe5,0xc3,0x00,0x00,
    0xc3,0xe5,0xf9,0xf9,0xe5,0xc3,0x00,0x00,
    //start flipping
    0x42,0xe7,0xfb,0xfb,0xe7,0x42,0x00,0x00,
    0x24,0x76,0x7a,0x7a,0x76,0x24,0x00,0x00,
    0x14,0x3c,0x7e,0x7e,0x3c,0x14,0x00,0x00,
    0x18,0x3c,0x7e,0x7e,0x3c,0x18,0x00,0x00,
    0x2c,0x3e,0x7f,0x7f,0x3e,0x2c,0x00,0x00,
    0x46,0x6f,0x5f,0x5f,0x6f,0x46,0x00,0x00,
    0xc3,0xa7,0x9f,0x9f,0xa7,0xc3,0x00,0x00,
};


struct 
{
  const uint8_t *entryData;
  uint8_t entryWidth;
  uint8_t entryFrames;
  const uint8_t *exitData;
  uint8_t exitWidth;
  uint8_t exitFrames;
} 
sprite[] =
{
  { pacman1, W_PMAN1, F_PMAN1, pacman2, W_PMAN2, F_PMAN2 },
  { walker, W_WALKER, F_WALKER, walker, W_WALKER, F_WALKER },
  { invader, W_INVADER, F_INVADER, invader, W_INVADER, F_INVADER },
  { chevron, W_CHEVRON, F_CHEVRON, chevron, W_CHEVRON, F_CHEVRON },
  { heart, W_HEART, F_HEART, heart, W_HEART, F_HEART },
  { arrow1, W_ARROW1, F_ARROW1, arrow1, W_ARROW1, F_ARROW1 },
  { steamboat, W_STEAMBOAT, F_STEAMBOAT, steamboat, W_STEAMBOAT, F_STEAMBOAT },
  { fireball, W_FBALL, F_FBALL, fireball, W_FBALL, F_FBALL },
  { rocket, W_ROCKET, F_ROCKET, rocket, W_ROCKET, F_ROCKET },
  { roll2, W_ROLL2, F_ROLL2, roll2, W_ROLL2, F_ROLL2 },
  { pacman2, W_PMAN2, F_PMAN2, pacman2, W_PMAN2, F_PMAN2 },
  { lines, W_LINES, F_LINES, lines, W_LINES, F_LINES },
  { roll1, W_ROLL1, F_ROLL1, roll1, W_ROLL1, F_ROLL1 },
  { sailboat, W_SAILBOAT, F_SAILBOAT, sailboat, W_SAILBOAT, F_SAILBOAT },
  { arrow2, W_ARROW2, F_ARROW2, arrow2, W_ARROW2, F_ARROW2 },
  { wave, W_WAVE, F_WAVE, wave, W_WAVE, F_WAVE },
  { pacman1, W_PMAN1, F_PMAN1, pacman1, W_PMAN1, F_PMAN1 },
  { bouncingHeart , W_BOUNCINGHEART, F_BOUNCINGHEART, bouncingHeart , W_BOUNCINGHEART, F_BOUNCINGHEART },
  { tongueEmoji , W_TONGUEEMOJI, F_TONGUEEMOJI, tongueEmoji , W_TONGUEEMOJI, F_TONGUEEMOJI },
  { winkingEmoji , W_WINKINGEMOJI, F_WINKINGEMOJI, winkingEmoji , W_WINKINGEMOJI, F_WINKINGEMOJI },
  { laughEmoji , W_LAUGHEMOJI, F_LAUGHEMOJI, laughEmoji , W_LAUGHEMOJI, F_LAUGHEMOJI },
  { wheelreverse , W_WHEEL, F_WHEEL, wheelreverse , W_WHEEL, F_WHEEL },
  { wheelforward , W_WHEEL, F_WHEEL, wheelforward , W_WHEEL, F_WHEEL },
  { hourglass , W_HOURGLASS, F_HOURGLASS, hourglass , W_HOURGLASS, F_HOURGLASS }
};

const char *ssid     = "Wifi ssid";
const char *password = "Wifi password";

/* Configuration of NTP */
#define MY_NTP_SERVER "pool.ntp.org"           
#define MY_TZ "EST5EDT"  

char daysOfTheWeek[7][3] = {"Su", "Mo", "Tu", "We", "Th", "Fr", "Sa"};
char month[12][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

uint8_t	curString = 0;
const char *msg[] =
{
  TIME_PLACEHOLDER,
  MSG1_PLACEHOLDER,
  MSG2_PLACEHOLDER
};

char display_char_array[10];
const char *message;
/* Globals */
time_t now;
tm tm;  

void setup(void)
{
  Serial.begin(115200);
  P.begin();
  configTime(MY_TZ, MY_NTP_SERVER);

  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
  }

}

void loop(void)
{

  static uint8_t  curFX = 0;
  
  if (P.displayAnimate()) {
    time(&now);
    localtime_r(&now, &tm);
    if(strcmp(msg[curString], TIME_PLACEHOLDER) == 0){
      setMessagePointerForDisplay(convertToTwoDigitString(tm.tm_hour) + ":" + convertToTwoDigitString(tm.tm_min));
    }
    else if (strcmp(msg[curString], MSG1_PLACEHOLDER) == 0) {
      if(tm.tm_wday == 2 || tm.tm_wday == 4 || tm.tm_wday == 6){
        setMessagePointerForDisplay("Shampoo");
      }
      else{
        setMessagePointerForDisplay(String(month[tm.tm_mon])+" "+String(convertToTwoDigitString(tm.tm_mday)));
      }
      
    }
    else if (strcmp(msg[curString], MSG2_PLACEHOLDER) == 0) {
      if(tm.tm_wday == 2 || tm.tm_wday == 4 || tm.tm_wday == 6){
        setMessagePointerForDisplay("Scrub");
      }
      else{
        int current_year = tm.tm_year + 1900;
        setMessagePointerForDisplay("'"+String(current_year).substring(2,4)+" "+String(daysOfTheWeek[tm.tm_wday]));
      }
    }

    P.displayText(message, PA_CENTER, 100, PAUSE_TIME, PA_SPRITE, PA_SPRITE);
    P.setSpriteData(sprite[curFX].entryData, sprite[curFX].entryWidth, sprite[curFX].entryFrames,  // entry sprite
                    sprite[curFX].exitData, sprite[curFX].exitWidth, sprite[curFX].exitFrames); // exit sprite
    P.displayReset();
    curFX++;
    curString++;

    if(curFX == ARRAY_SIZE(sprite)){
      curFX = 0;
    }
    if(curString == ARRAY_SIZE(msg)){
      curString = 0;
    }
  }
}

String convertToTwoDigitString(int integer){
  return integer>9 ? String(integer) : "0"+String(integer);
}

void setMessagePointerForDisplay(String string){
  int string_len = string.length() + 1;
  string.toCharArray(display_char_array, string_len);
  message = display_char_array;
}