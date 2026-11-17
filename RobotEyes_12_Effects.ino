/*
  TECHWITHMANOJ
  12 ROBOT EYES ANIMATION EFFECTS

  Board: Arduino UNO
  Display: 2.4 inch 320x240 TFT LCD Shield

  Libraries:
  1. MCUFRIEND_kbv
  2. Adafruit_GFX
*/

#include <MCUFRIEND_kbv.h>
#include <Adafruit_GFX.h>

MCUFRIEND_kbv tft;

// COLORS
const uint16_t BG      = 0x0841;
const uint16_t WHITE_C = 0xFFFF;
const uint16_t CYAN_C  = 0x07FF;
const uint16_t BLUE_C  = 0x04DF;
const uint16_t DARK_C  = 0x0000;
const uint16_t RED_C   = 0xF800;
const uint16_t PINK_C  = 0xF81F;

// EYE DIMENSIONS
const int L = 39;
const int R = 181;
const int TOP = 84;
const int EW = 100;
const int EH = 66;
const int CY = TOP + EH / 2;

// ANIMATION SPEED
const unsigned long EFFECT_MS = 4000UL;
const unsigned long FRAME_MS = 70UL;

// 12 EFFECTS
enum Effect {
  LOOK,
  BLINK,
  HAPPY,
  ANGRY,
  SURPRISED,
  SLEEPY,
  WINK,
  SAD,
  CONFUSED,
  HEARTS,
  DIZZY,
  EXCITED
};

uint8_t effect = LOOK;

unsigned long effectAt = 0;
unsigned long frameAt = 0;
unsigned long targetAt = 0;

float lookX = 0;
float lookY = 0;

int targetX = 0;
int targetY = 0;

int8_t lastRenderedEffect = -1;
int lastA = -999;
int lastB = -999;

// DRAW THICK LINE
void thickLine(
  int x1, int y1,
  int x2, int y2,
  uint16_t color
) {
  for (int d = 0; d < 3; d++) {
    tft.drawLine(
      x1, y1 + d,
      x2, y2 + d,
      color
    );
  }
}

// DARK PUPILS
void drawPupil(int x, int y) {

  // Blue iris
  tft.fillCircle(x, y, 14, BLUE_C);

  // Black pupil
  tft.fillCircle(x, y, 10, DARK_C);

  // Reflection
  tft.fillCircle(
    x - 4,
    y - 4,
    3,
    WHITE_C
  );
}

// OPEN EYE
void openEye(
  int left,
  int dx,
  int dy,
  int height = EH
) {

  int y = TOP + (EH - height) / 2;

  int radius =
    (height < 24) ? height / 2 : 13;

  tft.fillRoundRect(
    left,
    y,
    EW,
    height,
    radius,
    WHITE_C
  );

  if (height >= 38) {
    drawPupil(
      left + EW / 2 + dx,
      CY + dy
    );
  }
}

// CLOSED EYE
void closedEye(int cx) {

  thickLine(
    cx - 28, CY + 2,
    cx, CY - 3,
    CYAN_C
  );

  thickLine(
    cx, CY - 3,
    cx + 28, CY + 2,
    CYAN_C
  );
}

// HAPPY EYE
void happyEye(int cx) {

  thickLine(
    cx - 31, CY + 10,
    cx, CY - 9,
    CYAN_C
  );

  thickLine(
    cx, CY - 9,
    cx + 31, CY + 10,
    CYAN_C
  );
}

// HEART EYES
void drawHeart(int cx, int cy, int r) {

  tft.fillCircle(
    cx - r / 2,
    cy - 5,
    r / 2 + 2,
    PINK_C
  );

  tft.fillCircle(
    cx + r / 2,
    cy - 5,
    r / 2 + 2,
    PINK_C
  );

  tft.fillTriangle(
    cx - r, cy - 3,
    cx + r, cy - 3,
    cx, cy + r + 7,
    PINK_C
  );
}

// SMILING MOUTH
void drawSmile() {

  thickLine(138, 179, 151, 191, CYAN_C);
  thickLine(151, 191, 169, 191, CYAN_C);
  thickLine(169, 191, 182, 179, CYAN_C);
}

// EYEBROWS + MOUTH + LABELS
void drawDecorations() {

  switch (effect) {

    case ANGRY:
      thickLine(48, 61, 129, 76, RED_C);
      thickLine(191, 76, 272, 61, RED_C);
      break;

    case SAD:
      thickLine(48, 75, 127, 61, CYAN_C);
      thickLine(193, 61, 272, 75, CYAN_C);
      break;

    case CONFUSED:
      thickLine(48, 57, 128, 67, CYAN_C);
      thickLine(193, 73, 272, 73, CYAN_C);
      break;

    default:
      thickLine(48, 72, 127, 68, CYAN_C);
      thickLine(193, 68, 272, 72, CYAN_C);
      break;
  }

  // MOUTH
  switch (effect) {

    case HAPPY:
    case HEARTS:
    case EXCITED:
      drawSmile();
      break;

    case ANGRY:
      thickLine(148, 187, 172, 187, RED_C);
      break;

    case SURPRISED:
      tft.drawCircle(160, 184, 12, CYAN_C);
      tft.drawCircle(160, 184, 11, CYAN_C);
      break;

    case SAD:
      thickLine(138, 193, 160, 180, CYAN_C);
      thickLine(160, 180, 182, 193, CYAN_C);
      break;

    case SLEEPY:
      thickLine(151, 184, 169, 184, CYAN_C);
      break;

    case CONFUSED:
      thickLine(145, 188, 160, 181, CYAN_C);
      thickLine(160, 181, 176, 185, CYAN_C);
      break;

    default:
      tft.drawCircle(160, 184, 7, CYAN_C);
      break;
  }

  // EFFECT NAME
  tft.setTextSize(2);
  tft.setTextColor(CYAN_C, BG);
  tft.setCursor(10, 215);

  switch (effect) {

    case LOOK:
      tft.print(F("01 LOOK"));
      break;

    case BLINK:
      tft.print(F("02 BLINK"));
      break;

    case HAPPY:
      tft.print(F("03 HAPPY"));
      break;

    case ANGRY:
      tft.print(F("04 ANGRY"));
      break;

    case SURPRISED:
      tft.print(F("05 SURPRISE"));
      break;

    case SLEEPY:
      tft.print(F("06 SLEEPY"));
      break;

    case WINK:
      tft.print(F("07 WINK"));
      break;

    case SAD:
      tft.print(F("08 SAD"));
      break;

    case CONFUSED:
      tft.print(F("09 CONFUSED"));
      break;

    case HEARTS:
      tft.print(F("10 HEARTS"));
      break;

    case DIZZY:
      tft.print(F("11 DIZZY"));
      break;

    case EXCITED:
      tft.print(F("12 EXCITED"));
      break;
  }
}

// START NEW EFFECT
void beginEffect() {

  tft.fillScreen(BG);

  drawDecorations();

  lastRenderedEffect = -1;
  lastA = -999;
  lastB = -999;

  lookX = 0;
  lookY = 0;

  targetX = 0;
  targetY = 0;

  targetAt = millis();
}

// MAIN EYE ANIMATION
void renderEyes(unsigned long elapsed) {

  int a = 0;
  int b = 0;

  switch (effect) {

    // EFFECT 1: SMOOTH MOVEMENT
    case LOOK:
      a = (int)(
        lookX +
        (lookX >= 0 ? 0.5f : -0.5f)
      );

      b = (int)(
        lookY +
        (lookY >= 0 ? 0.5f : -0.5f)
      );
      break;

    // EFFECT 2: BLINK
    case BLINK: {
      unsigned long p = elapsed % 1200UL;

      a = 66;

      if (p < 130UL) {
        a = 66 - (int)(p * 61UL / 130UL);
      }
      else if (p < 260UL) {
        a = 5 +
          (int)((p - 130UL) * 61UL / 130UL);
      }
      break;
    }

    // EFFECT 6: SLEEPY
    case SLEEPY:
      a = (elapsed < 3000UL)
        ? 66 - (int)(elapsed * 61UL / 3000UL)
        : 5;
      break;

    // EFFECT 7: WINK
    case WINK:
      a = (elapsed / 650UL) % 2;
      break;

    // EFFECT 10: HEARTS
    case HEARTS:
      a = (elapsed / 320UL) % 2;
      break;

    // EFFECT 11: DIZZY
    case DIZZY:
      a = (elapsed / 140UL) % 8;
      break;

    // EFFECT 12: EXCITED
    case EXCITED:
      a = (elapsed / 140UL) % 6;
      break;

    default:
      break;
  }

  // SKIP UNCHANGED FRAME
  if (
    lastRenderedEffect == (int8_t)effect &&
    a == lastA &&
    b == lastB
  ) {
    return;
  }

  lastRenderedEffect = effect;
  lastA = a;
  lastB = b;

  // CLEAR ONLY EYE AREAS
  tft.fillRect(34, 80, 110, 76, BG);
  tft.fillRect(176, 80, 110, 76, BG);

  switch (effect) {

    // 1. MOVING EYES
    case LOOK:
      openEye(L, a, b);
      openEye(R, a, b);
      break;

    // 2. BLINKING EYES
    case BLINK:
      openEye(L, 0, 0, a);
      openEye(R, 0, 0, a);
      break;

    // 3. HAPPY EYES
    case HAPPY:
      happyEye(L + EW / 2);
      happyEye(R + EW / 2);
      break;

    // 4. ANGRY EYES
    case ANGRY:
      openEye(L, 8, 1, 44);
      openEye(R, -8, 1, 44);
      break;

    // 5. SURPRISED EYES
    case SURPRISED:
      tft.fillCircle(
        L + EW / 2, CY, 34, WHITE_C
      );

      tft.fillCircle(
        R + EW / 2, CY, 34, WHITE_C
      );

      drawPupil(L + EW / 2, CY);
      drawPupil(R + EW / 2, CY);
      break;

    // 6. SLEEPY EYES
    case SLEEPY:
      openEye(L, 0, 0, a);
      openEye(R, 0, 0, a);
      break;

    // 7. WINKING EYES
    case WINK:
      if (a == 0) {
        closedEye(L + EW / 2);
        openEye(R, 0, 0);
      }
      else {
        openEye(L, 0, 0);
        closedEye(R + EW / 2);
      }
      break;

    // 8. SAD EYES
    case SAD:
      openEye(L, 0, 8, 62);
      openEye(R, 0, 8, 62);
      break;

    // 9. CONFUSED EYES
    case CONFUSED:
      openEye(L, -11, 2, 51);
      openEye(R, 11, -3, 66);
      break;

    // 10. HEART EYES
    case HEARTS:
      tft.fillRoundRect(
        L, TOP, EW, EH, 13, WHITE_C
      );

      tft.fillRoundRect(
        R, TOP, EW, EH, 13, WHITE_C
      );

      drawHeart(
        L + EW / 2, CY, a ? 15 : 12
      );

      drawHeart(
        R + EW / 2, CY, a ? 15 : 12
      );
      break;

    // 11. DIZZY EYES
    case DIZZY: {
      const int8_t ox[8] = {
        0, 7, 10, 7, 0, -7, -10, -7
      };

      const int8_t oy[8] = {
        -10, -7, 0, 7, 10, 7, 0, -7
      };

      openEye(L, ox[a], oy[a]);
      openEye(R, -ox[a], -oy[a]);
      break;
    }

    // 12. EXCITED EYES
    case EXCITED: {
      int bounce = a < 3
        ? -a * 3
        : -(6 - a) * 3;

      openEye(L, 0, bounce);
      openEye(R, 0, bounce);
      break;
    }
  }
}

// SETUP
void setup() {

  Serial.begin(9600);

  uint16_t id = tft.readID();

  Serial.print(F("TFT ID: 0x"));
  Serial.println(id, HEX);

  // COMMON ILI9341 FALLBACK
  if (
    id == 0xD3D3 ||
    id == 0xFFFF ||
    id == 0x0000
  ) {
    id = 0x9341;
  }

  tft.begin(id);

  // LANDSCAPE MODE
  tft.setRotation(1);

  randomSeed(analogRead(A5));

  effectAt = millis();
  frameAt = effectAt;

  beginEffect();
  renderEyes(0);
}

// LOOP
void loop() {

  unsigned long now = millis();

  // CHANGE EFFECT EVERY 4 SECONDS
  if (now - effectAt >= EFFECT_MS) {

    effect = (effect + 1) % 12;

    effectAt = now;

    beginEffect();
    renderEyes(0);
  }

  // FRAME UPDATE
  if (now - frameAt >= FRAME_MS) {

    frameAt = now;

    // RANDOM SMOOTH EYE MOVEMENT
    if (effect == LOOK) {

      if (now - targetAt >= 700UL) {

        targetX = random(-17, 18);
        targetY = random(-9, 10);

        targetAt = now;
      }

      lookX +=
        (targetX - lookX) * 0.22f;

      lookY +=
        (targetY - lookY) * 0.22f;
    }

    renderEyes(now - effectAt);
  }
}
