#include <SPI.h>
#include <LiquidCrystal.h>

#define CLR(x,y) (x&=(~(1<<y)))
#define SET(x,y) (x|=(1<<y))

// LCD
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);
char* program_name;
bool lcd_update_required = false;

// PINS (port numbers)
int pe_vccs_latch = 9;
int pe_gnds_latch = 5;

// The leds matrix that will be shown
unsigned char LEDS [] =
{
  0x00,
  0x00,
  0x00,
  0x0,
  0x0,
  0x0,
  0x0,
  0
};
bool lcd_direction_mode = false; // The direction, false= left bottom to right up, true= right bottom to left up
bool leds_on = true;
// We need these for program updates
int counter = 0;
int i = 0;
int scroll_speed = 11;
int program_index = 0;
// Inputs program
int input_mode = 0;
int input_counter = 0;
int input_sensivity = 30;

// Program - Scan
int p_s_index = 0;
// Program - Wave
int p_w_col0 = 0;
// Program - River
int p_r_i = 0;
// Program - Cycle
int p_c_counter = 0;
int p_c_timer = 70;
int p_c_program_index = 0;
// Program - Alphabet
int p_a_counter = 0;
int p_a_char_index = 0;
int p_a_wait = 15;
// Program - Circle
int p_c_index = 0;
bool p_c_flp;

void setup() {
  //PORTE on Arduino MEGA 2560 is for PWM 0 to 7, you may need to change the direction port on this code.
  // Here, we are setting all PWM pins we need to OUTPUT.
  SET(DDRE, pe_vccs_latch);
  SET(DDRE, pe_gnds_latch);

  // Setup button pins
  pinMode(4, INPUT_PULLUP);
  pinMode(0, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);

  SPI.begin();
  SPI.beginTransaction(SPISettings(SPI_CLOCK_DIV2, MSBFIRST, SPI_MODE0));

  // Setup values
  leds_on = true;
  program_index = 22;
  program_name = "Random Prog";

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("WELCOME !!");
  lcd_update_required = true;
}

// Rendering methods
void Render()
{
  if (!leds_on)
    return;
  if (!lcd_direction_mode)
  {
    // 1st row
    PutRowGND(0xFE);
    for (int col = 0; col < 8; col ++)
    {
      int reg = 1 << col;
      PutColumnVCC((unsigned char)(LEDS[0] & reg));
    }
    PutColumnVCC(0);
    // 2nd row
    PutRowGND(0xFD);
    for (int col = 0; col < 8; col ++)
    {
      int reg = 1 << col;
      PutColumnVCC((unsigned char)(LEDS[1] & reg));
    }
    PutColumnVCC(0);
    // 3rd row
    PutRowGND(0xFB);
    for (int col = 0; col < 8; col ++)
    {
      int reg = 1 << col;
      PutColumnVCC((unsigned char)(LEDS[2] & reg));
    }
    PutColumnVCC(0);
    // 4th row
    PutRowGND(0xF7);
    for (int col = 0; col < 8; col ++)
    {
      int  reg = 1 << col;
      PutColumnVCC((unsigned char)(LEDS[3] & reg));
    }
    PutColumnVCC(0);
    // 5th row
    PutRowGND(0xEF);
    for (int col = 0; col < 8; col ++)
    {
      int reg = 1 << col;
      PutColumnVCC((unsigned char)(LEDS[4] & reg));
    }
    PutColumnVCC(0);
    // 6th row
    PutRowGND(0xDF);
    for (int col = 0; col < 8; col ++)
    {
      int reg = 1 << col;
      PutColumnVCC((unsigned char)(LEDS[5] & reg));
    }
    PutColumnVCC(0);
    // 7th row
    PutRowGND(0xBF);
    for (int col = 0; col < 8; col ++)
    {
      int reg = 1 << col;
      PutColumnVCC((unsigned char)(LEDS[6] & reg));
    }
    PutColumnVCC(0);
    // 8th row
    PutRowGND(0x7F);
    for (int col = 0; col < 8; col ++)
    {
      int reg = 1 << col;
      PutColumnVCC((unsigned char)(LEDS[7] & reg));
    }
    PutColumnVCC(0);
  }
  else // Render method 2. Optional
  {
    // 8th row
    PutRowGND(0x7F);
    for (int col = 0; col < 8; col ++)
    {
      int  reg = 0x80 >> col;
      PutColumnVCC(LEDS[0] & reg);
    }
    PutColumnVCC(0);
    // 7th row
    PutRowGND(0xBF);
    for (int col = 0; col < 8; col ++)
    {
      int   reg = 0x80 >> col;
      PutColumnVCC(LEDS[1] & reg);
    }
    PutColumnVCC(0);
    // 6th row
    PutRowGND(0xDF);
    for (int col = 0; col < 8; col ++)
    {
      int  reg = 0x80 >> col;
      PutColumnVCC(LEDS[2] & reg);
    }
    PutColumnVCC(0);
    // 5th row
    PutRowGND(0xEF);
    for (int col = 0; col < 8; col ++)
    {
      int   reg = 0x80 >> col;
      PutColumnVCC(LEDS[3] & reg);
    }
    PutColumnVCC(0);
    // 4th row
    PutRowGND(0xF7);
    for (int col = 0; col < 8; col ++)
    {
      int  reg = 0x80 >> col;
      PutColumnVCC(LEDS[4] & reg);
    }
    PutColumnVCC(0);
    // 3rd row
    PutRowGND(0xFB);
    for (int col = 0; col < 8; col ++)
    {
      int reg = 0x80 >> col;
      PutColumnVCC(LEDS[5] & reg);
    }
    PutColumnVCC(0);
    // 2nd row
    PutRowGND(0xFD);
    for (int col = 0; col < 8; col ++)
    {
      int  reg = 0x80 >> col;
      PutColumnVCC(LEDS[6] & reg);
    }
    PutColumnVCC(0);
    // 1st row
    PutRowGND(0xFE);
    for (int col = 0; col < 8; col ++)
    {
      int reg = 1 << col;
      if (lcd_direction_mode)
        reg = 0x80 >> col;
      PutColumnVCC(LEDS[7] & reg);
    }
    PutColumnVCC(0);
  }
}
void UpdateLCD()
{
  if (!lcd_update_required)
    return;

  if (!leds_on)
  {
    lcd.setCursor(0, 0);
    lcd.print("LEDS OFF     ");
    lcd.print("             ");
    lcd.setCursor(0, 1);
    lcd.print("PRESS ON BUTTON ");
    return;

  }
  // See what mode we are at
  switch (input_mode)
  {
    case 0: // Programs switching
      {
        lcd.setCursor(0, 0);
        lcd.print("PAGE1: PRORGRAM  ");
        lcd.setCursor(0, 1);
        lcd.print("");
        lcd.print(program_index + 1);
        lcd.print(" ");
        lcd.print(program_name);
        lcd.print("             ");
        break;
      }
    case 1: // SPEED SET
      {
        lcd.setCursor(0, 0);
        lcd.print("PAGE2: SPEED SET");
        lcd.setCursor(0, 1);
        lcd.print("Update Speed: ");
        lcd.print(scroll_speed);
        lcd.print("             ");
        break;
      }
    case 2: // LEDS DIRECT
      {
        lcd.setCursor(0, 0);
        lcd.print("PAGE3: DIRECTION");
        lcd.setCursor(0, 1);
        if (lcd_direction_mode)
        {
          lcd.print("REVERSE");
        }
        else
        {
          lcd.print("NORMAL");
        }

        lcd.print("             ");
        break;
      }
  }
  lcd_update_required = false;
}
void UpdateProgram()
{
  if (!leds_on)
    return;
  counter ++;
  if (counter >= scroll_speed)
  {
    counter = 0;
    ChooseProgram(program_index);
  }
}
void HandleInputs()
{
  if (input_counter > 0)
  {
    input_counter--;
    return;
  }

  // ON / OFF
  if (digitalRead(A3) == LOW)
  {
    leds_on = !leds_on;
    input_counter = input_sensivity;

    SetProgramName();
    lcd_update_required = true;

    if (!leds_on)
    {
      PutColumnVCC(0);
      PutRowGND(0);
    }
  }

  // Page select
  if (digitalRead(A2) == LOW)
  {
    input_mode++;
    if (input_mode > 2)
    {
      input_mode = 0;
    }
    input_counter = input_sensivity;

    SetProgramName();
    lcd_update_required = true;
  }
  // See what mode we are at
  switch (input_mode)
  {
    case 0: // Prorams switching
      {
        if (digitalRead(A0) == LOW)
        {
          input_counter = input_sensivity;
          // Previous program
          program_index--;
          if (program_index < 0)
            program_index = 22;
          SetProgramName();
          lcd_update_required = true;
        }
        else if (digitalRead(A1) == LOW)
        {
          input_counter = input_sensivity;
          // Next program
          program_index++;
          if (program_index > 22)
            program_index = 0;
          SetProgramName();
          lcd_update_required = true;
        }
        break;
      }
    case 1: // Speed Set
      {
        if (digitalRead(A0) == LOW)
        {
          input_counter = input_sensivity;

          scroll_speed--;
          if (scroll_speed < 1)
            scroll_speed = 25;

          lcd_update_required = true;
        }
        else if (digitalRead(A1) == LOW)
        {
          input_counter = input_sensivity;
          // Next program
          scroll_speed++;
          if (scroll_speed > 25)
            scroll_speed = 1;

          lcd_update_required = true;
        }
        break;
      }
    case 2: // LEDS mode select
      {
        if (digitalRead(A0) == LOW || digitalRead(A1) == LOW)
        {
          input_counter = input_sensivity;

          lcd_direction_mode = !lcd_direction_mode;
          lcd_update_required = true;
        }
        break;
      }
  }
}
void SetProgramName()
{
  switch (program_index)
  {
    case 0: program_name = "Scan"; break;
    case 1:  program_name = "Fill"; break;
    case 2: program_name = "All";  break;
    case 3: program_name = "Bars"; break;
    case 4: program_name = "Wave 1"; break;
    case 5: program_name = "Wave 2"; break;
    case 6: program_name = "Fill"; break;
    case 7: program_name = "Random"; break;
    case 8: program_name = "River"; break;
    case 9: program_name = "Yellows"; break;
    case 10: program_name = "Greens"; break;
    case 11: program_name = "Reds"; break;
    case 12: program_name = "Alphabet"; break;
    case 13: program_name = "Numbers"; break;
    case 14: program_name = "Love You"; break;
    case 15: program_name = "Circle"; break;
    case 16: program_name = "Tunnle"; break;

    case 17: program_name = "Heart 1"; break;
    case 18: program_name = "Heart 2"; break;
    case 19: program_name = "Heart 3"; break;
    case 20: program_name = "Heart 4"; break;

    case 21: program_name = "Cycle Programs"; break;
    case 22: program_name = "Random Prog"; break;
  }
}
void ChooseProgram(int index)
{
  switch (index)
  {
    case 0: UpdateProgram_SCAN(); program_name = "Scan"; break;
    case 1: UpdateProgram_Count(); program_name = "Fill"; break;
    case 2: UpdateProgram_Full(); program_name = "All";  break;
    case 3: UpdateProgram_Analiser(); program_name = "Bars"; break;
    case 4: UpdateProgram_Wave(); program_name = "Wave 1"; break;
    case 5: UpdateProgram_MiddWave(); program_name = "Wave 2"; break;
    case 6: UpdateProgram_FILL(); program_name = "Fill"; break;
    case 7: UpdateProgram_Random(); program_name = "Random"; break;
    case 8: UpdateProgram_River(); program_name = "River"; break;
    case 9: UpdateProgram_Yellows(); program_name = "Yellows"; break;
    case 10: UpdateProgram_Greens(); program_name = "Greens"; break;
    case 11: UpdateProgram_Reds(); program_name = "Reds"; break;
    case 12: UpdateProgram_AlphaBet(); program_name = "Alphabet"; break;
    case 13: UpdateProgram_CountNumbers(); program_name = "Numbers"; break;
    case 14: UpdateProgram_ILoveYou(); program_name = "Love You"; break;
    case 15: UpdateProgram_Circle(); program_name = "Circle"; break;
    case 16: UpdateProgram_Tunnle(); program_name = "Tunnle"; break;

    case 17: UpdateProgram_Heart1(); program_name = "Heart 1"; break;
    case 18: UpdateProgram_Heart2(); program_name = "Heart 2"; break;
    case 19: UpdateProgram_Heart3(); program_name = "Heart 3"; break;
    case 20: UpdateProgram_Heart4(); program_name = "Heart 4"; break;

    case 21: UpdateProgram_CyclePrograms(); program_name = "Cycle Programs"; break;
    case 22: UpdateProgram_CycleProgramsRandom(); program_name = "Random Prog"; break;
  }
}
// These methods are required to shift values into the shift registers
void PutColumnVCC(unsigned char col)
{
  CLR(PORTE, pe_vccs_latch);// Clear PWM pin 2 (PE4)

  SPI.transfer(col);

  SET(PORTE, pe_vccs_latch);// Set PWM pin 2 (PE4)
}
void PutRowGND(unsigned char row)
{
  CLR(PORTE, pe_gnds_latch);// Clear PWM pin 3 (PE5)

  SPI.transfer(row);

  SET(PORTE, pe_gnds_latch);// Set PWM pin 3 (PE5)
}
// Optional methods, plays around with leds.
void SetChar(char* c)
{
  // The scaning process starts from row 0 to row 7. Since the leds are aranged from bottom to top, we need to setup them from top to bottom.

  if (c == "-")
  {
    LEDS [7] = B00000000;
    LEDS [6] = B00000000;
    LEDS [5] = B00000000;
    LEDS [4] = B01111110;
    LEDS [3] = B01111110;
    LEDS [2] = B00000000;
    LEDS [1] = B00000000;
    LEDS [0] = B00000000;
  }
  else if (c == "heart")
  {
    LEDS [7] = B00000000;
    LEDS [6] = B11100111;
    LEDS [5] = B10011001;
    LEDS [4] = B10000001;
    LEDS [3] = B10000001;
    LEDS [2] = B01000010;
    LEDS [1] = B00100100;
    LEDS [0] = B00011000;
  }
  else if (c == "heartfull")
  {
    LEDS [7] = B00000000;
    LEDS [6] = B11100111;
    LEDS [5] = B11111111;
    LEDS [4] = B11111111;
    LEDS [3] = B11111111;
    LEDS [2] = B01111110;
    LEDS [1] = B00111100;
    LEDS [0] = B00011000;
  }
  else if (c == "1")
  {
    LEDS [7] = B00000000;
    LEDS [6] = B00001110;
    LEDS [5] = B00010110;
    LEDS [4] = B00100110;
    LEDS [3] = B01000110;
    LEDS [2] = B00000110;
    LEDS [1] = B00000110;
    LEDS [0] = B00000000;
  }
  else if (c == "2")
  {
    LEDS [7] = B00000000;
    LEDS [6] = B01111110;
    LEDS [5] = B00000010;
    LEDS [4] = B01111110;
    LEDS [3] = B01111110;
    LEDS [2] = B01000000;
    LEDS [1] = B01111110;
    LEDS [0] = B00000000;
  }
  else if (c == "3")
  {
    LEDS [7] = B00000000;
    LEDS [6] = B01111110;
    LEDS [5] = B00000010;
    LEDS [4] = B01111110;
    LEDS [3] = B01111110;
    LEDS [2] = B00000010;
    LEDS [1] = B01111110;
    LEDS [0] = B00000000;
  }
  else if (c == "4")
  {
    LEDS [7] = B00000000;
    LEDS [6] = B01000010;
    LEDS [5] = B01000010;
    LEDS [4] = B01111110;
    LEDS [3] = B01111110;
    LEDS [2] = B00000010;
    LEDS [1] = B00000010;
    LEDS [0] = B00000000;
  }
  else if (c == "5")
  {
    LEDS [7] = B00000000;
    LEDS [6] = B01111110;
    LEDS [5] = B01000000;
    LEDS [4] = B01111110;
    LEDS [3] = B01111110;
    LEDS [2] = B00000010;
    LEDS [1] = B01111110;
    LEDS [0] = B00000000;
  }
  else if (c == "6")
  {
    LEDS [7] = B00000000;
    LEDS [6] = B01111110;
    LEDS [5] = B01000000;
    LEDS [4] = B01111110;
    LEDS [3] = B01111110;
    LEDS [2] = B01000010;
    LEDS [1] = B01111110;
    LEDS [0] = B00000000;
  }
  else if (c == "7")
  {
    LEDS [7] = B00000000;
    LEDS [6] = B01111110;
    LEDS [5] = B00000110;
    LEDS [4] = B00001100;
    LEDS [3] = B00011000;
    LEDS [2] = B00110000;
    LEDS [1] = B01100000;
    LEDS [0] = B00000000;
  }
  else if (c == "8")
  {
    LEDS [7] = B00000000;
    LEDS [6] = B00111100;
    LEDS [5] = B01000010;
    LEDS [4] = B00111100;
    LEDS [3] = B00111100;
    LEDS [2] = B01000010;
    LEDS [1] = B00111100;
    LEDS [0] = B00000000;
  }
  else if (c == "9")
  {
    LEDS [7] = B00000000;
    LEDS [6] = B01111110;
    LEDS [5] = B01000010;
    LEDS [4] = B01111110;
    LEDS [3] = B01111110;
    LEDS [2] = B00000010;
    LEDS [1] = B01111110;
    LEDS [0] = B00000000;
  }
  else if (c == "0")
  {
    LEDS [7] = B00000000;
    LEDS [6] = B00111100;
    LEDS [5] = B01000110;
    LEDS [4] = B01001010;
    LEDS [3] = B01010010;
    LEDS [2] = B01100010;
    LEDS [1] = B00111100;
    LEDS [0] = B00000000;
  }
  else if (c == "a" || c == "A")
  {
    LEDS [7] = B00000000;
    LEDS [6] = B01111110;
    LEDS [5] = B01000010;
    LEDS [4] = B01111110;
    LEDS [3] = B01000010;
    LEDS [2] = B01000010;
    LEDS [1] = B01000010;
    LEDS [0] = B00000000;
  }
  else if (c == "b" || c == "B")
  {
    LEDS [7] = B00000000;
    LEDS [6] = B01111100;
    LEDS [5] = B01000010;
    LEDS [4] = B01111100;
    LEDS [3] = B01000010;
    LEDS [2] = B01000010;
    LEDS [1] = B01111100;
    LEDS [0] = B00000000;
  }
  else if (c == "c" || c == "C")
  {
    LEDS [7] = B00000000;
    LEDS [6] = B01111110;
    LEDS [5] = B01000000;
    LEDS [4] = B01000000;
    LEDS [3] = B01000000;
    LEDS [2] = B01000000;
    LEDS [1] = B01111110;
    LEDS [0] = B00000000;
  }
  else if (c == "d" || c == "D")
  {
    LEDS [7] = B00000000;
    LEDS [6] = B01111100;
    LEDS [5] = B01000010;
    LEDS [4] = B01000010;
    LEDS [3] = B01000010;
    LEDS [2] = B01000010;
    LEDS [1] = B01111100;
    LEDS [0] = B00000000;
  }
  else if (c == "e" || c == "E")
  {
    LEDS [7] = B00000000;
    LEDS [6] = B01111110;
    LEDS [5] = B01000000;
    LEDS [4] = B01111110;
    LEDS [3] = B01111110;
    LEDS [2] = B01000000;
    LEDS [1] = B01111110;
    LEDS [0] = B00000000;
  }
  else if (c == "f" || c == "F")
  {
    LEDS [7] = B00000000;
    LEDS [6] = B01111110;
    LEDS [5] = B01000000;
    LEDS [4] = B01111110;
    LEDS [3] = B01111110;
    LEDS [2] = B01000000;
    LEDS [1] = B01000000;
    LEDS [0] = B00000000;
  }
  else if (c == "g" || c == "G")
  {
    LEDS [7] = B00000000;
    LEDS [6] = B01111110;
    LEDS [5] = B01000000;
    LEDS [4] = B01011110;
    LEDS [3] = B01010010;
    LEDS [2] = B01000010;
    LEDS [1] = B01111110;
    LEDS [0] = B00000000;
  }
  else if (c == "h" || c == "H")
  {
    LEDS [7] = B00000000;
    LEDS [6] = B01000010;
    LEDS [5] = B01000010;
    LEDS [4] = B01111110;
    LEDS [3] = B01000010;
    LEDS [2] = B01000010;
    LEDS [1] = B01000010;
    LEDS [0] = B00000000;
  }
  else if (c == "i" || c == "I")
  {
    LEDS [7] = B00000000;
    LEDS [6] = B00111100;
    LEDS [5] = B00011000;
    LEDS [4] = B00011000;
    LEDS [3] = B00011000;
    LEDS [2] = B00011000;
    LEDS [1] = B00111100;
    LEDS [0] = B00000000;
  }
  else if (c == "j" || c == "J")
  {
    LEDS [7] = B00000000;
    LEDS [6] = B00011110;
    LEDS [5] = B00001100;
    LEDS [4] = B00001100;
    LEDS [3] = B00001100;
    LEDS [2] = B00101100;
    LEDS [1] = B00111100;
    LEDS [0] = B00000000;
  }
  else if (c == "k" || c == "K")
  {
    LEDS [7] = B00000000;
    LEDS [6] = B01001000;
    LEDS [5] = B01010000;
    LEDS [4] = B01100000;
    LEDS [3] = B01100000;
    LEDS [2] = B01011000;
    LEDS [1] = B01000100;
    LEDS [0] = B00000000;
  }
  else if (c == "l" || c == "L")
  {
    LEDS [7] = B00000000;
    LEDS [6] = B01000000;
    LEDS [5] = B01000000;
    LEDS [4] = B01000000;
    LEDS [3] = B01000000;
    LEDS [2] = B01000000;
    LEDS [1] = B01111110;
    LEDS [0] = B00000000;
  }
  else if (c == "m" || c == "M")
  {
    LEDS [7] = B00000000;
    LEDS [6] = B01111110;
    LEDS [5] = B01011010;
    LEDS [4] = B01011010;
    LEDS [3] = B01011010;
    LEDS [2] = B01011010;
    LEDS [1] = B01011010;
    LEDS [0] = B00000000;
  }
  else if (c == "n" || c == "N")
  {
    LEDS [7] = B00000000;
    LEDS [6] = B01000010;
    LEDS [5] = B01100010;
    LEDS [4] = B01010010;
    LEDS [3] = B01001010;
    LEDS [2] = B01000110;
    LEDS [1] = B01000010;
    LEDS [0] = B00000000;
  }
  else if (c == "o" || c == "O")
  {
    LEDS [7] = B00000000;
    LEDS [6] = B00111100;
    LEDS [5] = B01000010;
    LEDS [4] = B01000010;
    LEDS [3] = B01000010;
    LEDS [2] = B01000010;
    LEDS [1] = B00111100;
    LEDS [0] = B00000000;
  }
  else if (c == "p" || c == "P")
  {
    LEDS [7] = B00000000;
    LEDS [6] = B01111110;
    LEDS [5] = B01000010;
    LEDS [4] = B01111110;
    LEDS [3] = B01111110;
    LEDS [2] = B01000000;
    LEDS [1] = B01000000;
    LEDS [0] = B00000000;
  }
  else if (c == "q" || c == "Q")
  {
    LEDS [7] = B00000000;
    LEDS [6] = B01111110;
    LEDS [5] = B01000010;
    LEDS [4] = B01000010;
    LEDS [3] = B01001010;
    LEDS [2] = B01000110;
    LEDS [1] = B01111110;
    LEDS [0] = B00000001;
  }
  else if (c == "r" || c == "R")
  {
    LEDS [7] = B00000000;
    LEDS [6] = B01111110;
    LEDS [5] = B01000010;
    LEDS [4] = B01111110;
    LEDS [3] = B01010000;
    LEDS [2] = B01001000;
    LEDS [1] = B01000100;
    LEDS [0] = B00000000;
  }
  else if (c == "s" || c == "S")
  {
    LEDS [7] = B00000000;
    LEDS [6] = B01111110;
    LEDS [5] = B01000000;
    LEDS [4] = B01111110;
    LEDS [3] = B00000010;
    LEDS [2] = B00000010;
    LEDS [1] = B01111110;
    LEDS [0] = B00000000;
  }
  else if (c == "t" || c == "T")
  {
    LEDS [7] = B00000000;
    LEDS [6] = B01111110;
    LEDS [5] = B00011000;
    LEDS [4] = B00011000;
    LEDS [3] = B00011000;
    LEDS [2] = B00011000;
    LEDS [1] = B00011000;
    LEDS [0] = B00000000;
  }
  else if (c == "u" || c == "U")
  {
    LEDS [7] = B00000000;
    LEDS [6] = B01000010;
    LEDS [5] = B01000010;
    LEDS [4] = B01000010;
    LEDS [3] = B01000010;
    LEDS [2] = B01000010;
    LEDS [1] = B00111100;
    LEDS [0] = B00000000;
  }
  else if (c == "v" || c == "V")
  {
    LEDS [7] = B00000000;
    LEDS [6] = B01000010;
    LEDS [5] = B01000010;
    LEDS [4] = B01000010;
    LEDS [3] = B01000010;
    LEDS [2] = B00100100;
    LEDS [1] = B00011000;
    LEDS [0] = B00000000;
  }
  else if (c == "w" || c == "W")
  {
    LEDS [7] = B00000000;
    LEDS [6] = B01011010;
    LEDS [5] = B01011010;
    LEDS [4] = B01011010;
    LEDS [3] = B01011010;
    LEDS [2] = B01011010;
    LEDS [1] = B01111110;
    LEDS [0] = B00000000;
  }
  else if (c == "x" || c == "X")
  {
    LEDS [7] = B00000000;
    LEDS [6] = B01000010;
    LEDS [5] = B00100100;
    LEDS [4] = B00011000;
    LEDS [3] = B00100100;
    LEDS [2] = B01000010;
    LEDS [1] = B00000000;
    LEDS [0] = B00000000;
  }
  else if (c == "y" || c == "Y")
  {
    LEDS [7] = B00000000;
    LEDS [6] = B01000010;
    LEDS [5] = B00100100;
    LEDS [4] = B00011000;
    LEDS [3] = B00011000;
    LEDS [2] = B00011000;
    LEDS [1] = B00011000;
    LEDS [0] = B00000000;
  }
  else if (c == "z" || c == "Z")
  {
    LEDS [7] = B00000000;
    LEDS [6] = B01111110;
    LEDS [5] = B00000100;
    LEDS [4] = B00001000;
    LEDS [3] = B00010000;
    LEDS [2] = B00100000;
    LEDS [1] = B01111110;
    LEDS [0] = B00000000;
  }

  for (int i = 0; i < 8; i ++)
  {
    byte reg = (LEDS[i] & 0x80) >> 7;
    reg |= (LEDS[i] & 0x40) >> 5;
    reg |= (LEDS[i] & 0x20) >> 3;
    reg |= (LEDS[i] & 0x10) >> 1;
    reg |= (LEDS[i] & 0x08) << 1;
    reg |= (LEDS[i] & 0x04) << 3;
    reg |= (LEDS[i] & 0x02) << 5;
    reg |= (LEDS[i] & 0x01) << 7;

    LEDS[i] = reg;
  }
}

// Programs
void UpdateProgram_Count()
{
  p_s_index++;
  if (p_s_index >= 64)
  {
    p_s_index = 0;

    LEDS[0] = 0;
    LEDS[1] = 0;
    LEDS[2] = 0;
    LEDS[3] = 0;
    LEDS[4] = 0;
    LEDS[5] = 0;
    LEDS[6] = 0;
    LEDS[7] = 0;
  }

  int row = p_s_index / 8;
  int col = p_s_index % 8;

  byte reg = 0;

  for (int i = 0; i <= col; i++)
  {
    reg |= 0x01 << i;
  }
  LEDS[row] =  reg;
}
void UpdateProgram_Full()
{
  for (int i = 0; i < 8; i++)
  {
    LEDS[i] = 0xFF;
  }
}
void UpdateProgram_SCAN()
{
  p_s_index++;
  if (p_s_index >= 64)
  {
    p_s_index = 0;
  }

  int row = p_s_index / 8;
  int col = p_s_index % 8;

  LEDS[row] = (unsigned char)(1 << col);
  if (row > 0)
    LEDS[row - 1] = 0;
  else
    LEDS[7] = 0;
}
void UpdateProgram_Analiser()
{
  int col_0 = random(0, 8);
  int col_1 = random(0, 8);
  int col_2 = random(0, 8);
  int col_3 = random(0, 8);
  int col_4 = random(0, 8);
  int col_5 = random(0, 8);
  int col_6 = random(0, 8);
  int col_7 = random(0, 8);
  // Clear all leds
  for (int i = 0; i < 8; i++)
  {
    LEDS[i] = 0;
  }
  // Set first column
  for (int c = 0; c <= col_0; c++)
  {
    LEDS[c] = 1;
  }
  // Set second column
  for (int c = 0; c <= col_1; c++)
  {
    LEDS[c] |= 0x2;
  }
  // Set third column
  for (int c = 0; c <= col_2; c++)
  {
    LEDS[c] |= 0x4;
  }
  // Set fourth column
  for (int c = 0; c <= col_3; c++)
  {
    LEDS[c] |= 0x8;
  }
  // Set fifth column
  for (int c = 0; c <= col_4; c++)
  {
    LEDS[c] |= 0x10;
  }
  // Set sixth column
  for (int c = 0; c <= col_5; c++)
  {
    LEDS[c] |= 0x20;
  }
  // Set seventh column
  for (int c = 0; c <= col_6; c++)
  {
    LEDS[c] |= 0x40;
  }
  // Set eighth column
  for (int c = 0; c <= col_7; c++)
  {
    LEDS[c] |= 0x80;
  }
}
void UpdateProgram_Wave()
{
  int col_0 = 0;
  int col_1 = 0;
  int col_2 = 0;
  int col_3 = 0;
  int col_4 = 0;
  int col_5 = 0;
  int col_6 = 0;
  int col_7 = 0;

  p_w_col0++;
  if (p_w_col0 > 14)
  {
    p_w_col0 = 0;
  }

  if (p_w_col0 > 7)
  {
    col_0 = 14 - p_w_col0;

    col_1 = col_0 - 1;
    if (col_1 < 0)
    {
      col_1 *= -1;
    }

    col_2 = col_0 - 2;
    if (col_2 < 0)
    {
      col_2 *= -1;
    }

    col_3 = col_0 - 3;
    if (col_3 < 0)
    {
      col_3 *= -1;
    }

    col_4 = col_0 - 4;
    if (col_4 < 0)
    {
      col_4 *= -1;
    }

    col_5 = col_0 - 5;
    if (col_5 < 0)
    {
      col_5 *= -1;
    }
    col_6 = col_0 - 6;
    if (col_6 < 0)
    {
      col_6 *= -1;
    }
    col_7 = col_0 - 7;
    if (col_7 < 0)
    {
      col_7 *= -1;
    }
  }
  else
  {
    col_0 = p_w_col0;

    col_1 = col_0 + 1;
    if (col_1 > 7)
    {
      col_1 = 14 - col_1;
    }

    col_2 = col_0 + 2;
    if (col_2 > 7)
    {
      col_2 = 14 - col_2;
    }

    col_3 = col_0 + 3;
    if (col_3 > 7)
    {
      col_3 = 14 - col_3;
    }

    col_4 = col_0 + 4;
    if (col_4 > 7)
    {
      col_4 = 14 - col_4;
    }

    col_5 = col_0 + 5;
    if (col_5 > 7)
    {
      col_5 = 14 - col_5;
    }

    col_6 = col_0 + 6;
    if (col_6 > 7)
    {
      col_6 = 14 - col_6;
    }

    col_7 = col_0 + 7;
    if (col_7 > 7)
    {
      col_7 = 14 - col_7;
    }
  }

  // Clear all leds
  for (int i = 0; i < 8; i++)
  {
    LEDS[i] = 0;
  }
  // Set first column
  for (int c = 0; c <= col_0; c++)
  {
    LEDS[c] = 1;
  }
  // Set second column
  for (int c = 0; c <= col_1; c++)
  {
    LEDS[c] |= 0x2;
  }
  // Set third column
  for (int c = 0; c <= col_2; c++)
  {
    LEDS[c] |= 0x4;
  }
  // Set fourth column
  for (int c = 0; c <= col_3; c++)
  {
    LEDS[c] |= 0x8;
  }
  // Set fifth column
  for (int c = 0; c <= col_4; c++)
  {
    LEDS[c] |= 0x10;
  }
  // Set sixth column
  for (int c = 0; c <= col_5; c++)
  {
    LEDS[c] |= 0x20;
  }
  // Set seventh column
  for (int c = 0; c <= col_6; c++)
  {
    LEDS[c] |= 0x40;
  }
  // Set eighth column
  for (int c = 0; c <= col_7; c++)
  {
    LEDS[c] |= 0x80;
  }
}
void UpdateProgram_MiddWave()
{
  int col_0 = 0;
  int col_1 = 0;
  int col_2 = 0;
  int col_3 = 0;
  int col_4 = 0;
  int col_5 = 0;
  int col_6 = 0;
  int col_7 = 0;

  p_w_col0++;
  if (p_w_col0 > 14)
  {
    p_w_col0 = 0;
  }

  if (p_w_col0 > 7)
  {
    col_0 = 14 - p_w_col0;

    col_1 = col_0 - 2;
    if (col_1 < 0)
    {
      col_1 *= -1;
    }

    col_2 = col_0 - 4;
    if (col_2 < 0)
    {
      col_2 *= -1;
    }

    col_3 = col_0 - 6;
    if (col_3 < 0)
    {
      col_3 *= -1;
    }

    col_4 = col_0 - 6;
    if (col_4 < 0)
    {
      col_4 *= -1;
    }

    col_5 = col_0 - 6;
    if (col_5 < 0)
    {
      col_5 *= -1;
    }
    col_6 = col_0 - 4;
    if (col_6 < 0)
    {
      col_6 *= -1;
    }
    col_7 = col_0 - 2;
    if (col_7 < 0)
    {
      col_7 *= -1;
    }
  }
  else
  {
    col_0 = p_w_col0;

    col_1 = col_0 + 2;
    if (col_1 > 7)
    {
      col_1 = 14 - col_1;
    }

    col_2 = col_0 + 4;
    if (col_2 > 7)
    {
      col_2 = 14 - col_2;
    }

    col_3 = col_0 + 6;
    if (col_3 > 7)
    {
      col_3 = 14 - col_3;
    }

    col_4 = col_0 + 6;
    if (col_4 > 7)
    {
      col_4 = 14 - col_4;
    }

    col_5 = col_0 + 6;
    if (col_5 > 7)
    {
      col_5 = 14 - col_5;
    }

    col_6 = col_0 + 4;
    if (col_6 > 7)
    {
      col_6 = 14 - col_6;
    }

    col_7 = col_0 + 2;
    if (col_7 > 7)
    {
      col_7 = 14 - col_7;
    }
  }

  // Clear all leds
  for (int i = 0; i < 8; i++)
  {
    LEDS[i] = 0;
  }
  // Set first column
  for (int c = 0; c <= col_0; c++)
  {
    LEDS[c] = 1;
  }
  // Set second column
  for (int c = 0; c <= col_1; c++)
  {
    LEDS[c] |= 0x2;
  }
  // Set third column
  for (int c = 0; c <= col_2; c++)
  {
    LEDS[c] |= 0x4;
  }
  // Set fourth column
  for (int c = 0; c <= col_3; c++)
  {
    LEDS[c] |= 0x8;
  }
  // Set fifth column
  for (int c = 0; c <= col_4; c++)
  {
    LEDS[c] |= 0x10;
  }
  // Set sixth column
  for (int c = 0; c <= col_5; c++)
  {
    LEDS[c] |= 0x20;
  }
  // Set seventh column
  for (int c = 0; c <= col_6; c++)
  {
    LEDS[c] |= 0x40;
  }
  // Set eighth column
  for (int c = 0; c <= col_7; c++)
  {
    LEDS[c] |= 0x80;
  }
}
void UpdateProgram_FILL()
{
  int col_0 = 0;
  int col_1 = 0;
  int col_2 = 0;
  int col_3 = 0;
  int col_4 = 0;
  int col_5 = 0;
  int col_6 = 0;
  int col_7 = 0;

  p_w_col0++;
  if (p_w_col0 >= 64)
  {
    p_w_col0 = 0;
  }

  col_0 = p_w_col0;
  if (col_0 > 7)
    col_0 = 7;

  col_1 = p_w_col0 - 8;
  if (col_1 < 0)
  {
    col_1 = 0;
  }
  else if (col_1 > 7)
  {
    col_1 = 7;
  }

  col_2 = p_w_col0 - 16;
  if (col_2 < 0)
  {
    col_2 = 0;
  }
  else if (col_2 > 7)
  {
    col_2 = 7;
  }

  col_3 = p_w_col0 - 24;
  if (col_3 < 0)
  {
    col_3 = 0;
  }
  else if (col_3 > 7)
  {
    col_3 = 7;
  }

  col_4 = p_w_col0 - 32;
  if (col_4 < 0)
  {
    col_4 = 0;
  }
  else if (col_4 > 7)
  {
    col_4 = 7;
  }

  col_5 = p_w_col0 - 40;
  if (col_5 < 0)
  {
    col_5 = 0;
  }
  else if (col_5 > 7)
  {
    col_5 = 7;
  }

  col_6 = p_w_col0 - 48;
  if (col_6 < 0)
  {
    col_6 = 0;
  }
  else if (col_6 > 7)
  {
    col_6 = 7;
  }

  col_7 = p_w_col0 - 56;
  if (col_7 < 0)
  {
    col_7 = 0;
  }
  else if (col_7 > 7)
  {
    col_7 = 7;
  }

  // Clear all leds
  for (int i = 0; i < 8; i++)
  {
    LEDS[i] = 0;
  }
  // Set first column
  for (int c = 0; c <= col_0; c++)
  {
    LEDS[c] = 1;
  }
  // Set second column
  for (int c = 0; c <= col_1; c++)
  {
    LEDS[c] |= 0x2;
  }
  // Set third column
  for (int c = 0; c <= col_2; c++)
  {
    LEDS[c] |= 0x4;
  }
  // Set fourth column
  for (int c = 0; c <= col_3; c++)
  {
    LEDS[c] |= 0x8;
  }
  // Set fifth column
  for (int c = 0; c <= col_4; c++)
  {
    LEDS[c] |= 0x10;
  }
  // Set sixth column
  for (int c = 0; c <= col_5; c++)
  {
    LEDS[c] |= 0x20;
  }
  // Set seventh column
  for (int c = 0; c <= col_6; c++)
  {
    LEDS[c] |= 0x40;
  }
  // Set eighth column
  for (int c = 0; c <= col_7; c++)
  {
    LEDS[c] |= 0x80;
  }
}
void UpdateProgram_Random()
{
  for (int i = 0; i < 8; i++)
  {
    LEDS[i] = random(0, 0xFF);
  }
}
void UpdateProgram_River()
{
  p_r_i++;
  if (p_r_i >= 8)
    p_r_i = 0;

  int sh1 = (p_r_i + 1);
  int sh2 = (p_r_i + 2);
  int sh3 = (p_r_i + 3);
  int sh4 = (p_r_i + 4);
  int sh5 = (p_r_i + 5);
  int sh6 = (p_r_i + 6);
  int sh7 = (p_r_i + 7);

  if (sh1 > 7)
    sh1 -= 7;
  if (sh2 > 7)
    sh2 -= 7;
  if (sh3 > 7)
    sh3 -= 7;
  if (sh4 > 7)
    sh4 -= 7;
  if (sh5 > 7)
    sh5 -= 7;
  if (sh6 > 7)
    sh6 -= 7;
  if (sh7 > 7)
    sh7 -= 7;
  LEDS[0] = 1 << p_r_i;
  LEDS[1] = 1 << sh6;
  LEDS[2] = 1 << sh4;
  LEDS[3] = 1 << sh3;
  LEDS[4] = 1 << sh6;
  LEDS[5] = 1 << sh2;
  LEDS[6] = 1 << sh1;
  LEDS[7] = 1 << sh7;
}
void UpdateProgram_Greens()
{
  LEDS[0] = 0;
  LEDS[1] = 0;
  LEDS[2] = 0;
  LEDS[3] = random(0, 0xFF);
  LEDS[4] = random(0, 0xFF);
  LEDS[5] = random(0, 0xFF);
  LEDS[6] = 0;
  LEDS[7] = 0;
}
void UpdateProgram_Yellows()
{
  LEDS[0] = 0;
  LEDS[1] = random(0, 0xFF);
  LEDS[2] = random(0, 0xFF);
  LEDS[3] = 0;
  LEDS[4] = 0;
  LEDS[5] = 0;
  LEDS[6] = 0;
  LEDS[7] = 0;
}
void UpdateProgram_Reds()
{
  LEDS[0] = 0;
  LEDS[1] = 0;
  LEDS[2] = 0;
  LEDS[3] = 0;
  LEDS[4] = 0;
  LEDS[5] = 0;
  LEDS[6] = random(0, 0xFF);
  LEDS[7] = random(0, 0xFF);
}
void UpdateProgram_AlphaBet()
{
  p_a_counter--;
  if (p_a_counter <= 0)
  {
    p_a_counter = p_a_wait;
    switch (p_a_char_index)
    {
      case 0 : SetChar("a"); break;
      case 1 : SetChar("b"); break;
      case 2 : SetChar("c"); break;
      case 3 : SetChar("d"); break;
      case 4 : SetChar("e"); break;
      case 5 : SetChar("f"); break;
      case 6 : SetChar("g"); break;
      case 7 : SetChar("h"); break;
      case 8 : SetChar("i"); break;
      case 9 : SetChar("j"); break;
      case 10 : SetChar("k"); break;
      case 11 : SetChar("l"); break;
      case 12: SetChar("m"); break;
      case 13 : SetChar("n"); break;
      case 14: SetChar("o"); break;
      case 15 : SetChar("p"); break;
      case 16 : SetChar("q"); break;
      case 17 : SetChar("r"); break;
      case 18 : SetChar("s"); break;
      case 19 : SetChar("t"); break;
      case 20 : SetChar("u"); break;
      case 21 : SetChar("v"); break;
      case 22 : SetChar("w"); break;
      case 23 : SetChar("x"); break;
      case 24 : SetChar("y"); break;
      case 25 : SetChar("z"); break;
      case 26 : SetChar("1"); break;
      case 27 : SetChar("2"); break;
      case 28 : SetChar("3"); break;
      case 29 : SetChar("4"); break;
      case 30 : SetChar("5"); break;
      case 31 : SetChar("6"); break;
      case 32 : SetChar("7"); break;
      case 33 : SetChar("8"); break;
      case 34 : SetChar("9"); break;
      case 35 : SetChar("0"); break;
    }
    p_a_char_index++;
    if (p_a_char_index > 35)
      p_a_char_index = 0;
  }
}
void UpdateProgram_CountNumbers()
{
  p_a_counter--;
  if (p_a_counter <= 0)
  {
    p_a_counter = p_a_wait;
    switch (p_a_char_index)
    {
      case 0 : SetChar("0"); break;
      case 1 : SetChar("1"); break;
      case 2 : SetChar("2"); break;
      case 3 : SetChar("3"); break;
      case 4 : SetChar("4"); break;
      case 5 : SetChar("5"); break;
      case 6 : SetChar("6"); break;
      case 7 : SetChar("7"); break;
      case 8 : SetChar("8"); break;
      case 9 : SetChar("9"); break;
    }
    p_a_char_index++;
    if (p_a_char_index > 9)
      p_a_char_index = 0;
  }
}
void UpdateProgram_ILoveYou()
{
  p_a_counter--;
  if (p_a_counter <= 0)
  {
    p_a_counter = p_a_wait;
    switch (p_a_char_index)
    {

      case 0 : SetChar("i"); break;
      case 1 : SetChar("l"); break;
      case 2: SetChar("o"); break;
      case 3 : SetChar("v"); break;
      case 4 : SetChar("e"); break;
      case 5 : SetChar("-"); break;
      case 6 : SetChar("y"); break;
      case 7 : SetChar("o"); break;
      case 8 : SetChar("u"); break;
      case 9 : SetChar("heart"); break;
      case 10 : SetChar("heart"); break;
      case 11 : SetChar("heartfull"); break;
      case 12 : SetChar("heartfull"); break;
    }
    p_a_char_index++;
    if (p_a_char_index > 12)
      p_a_char_index = 0;
  }
}
void UpdateProgram_Circle()
{
  switch (p_c_index)
  {
    case 0 :
      {
        LEDS [7] = B00000000;
        LEDS [6] = B00000000;
        LEDS [5] = B00000000;
        LEDS [4] = B00011000;
        LEDS [3] = B00011000;
        LEDS [2] = B00000000;
        LEDS [1] = B00000000;
        LEDS [0] = B00000000;
        break;
      }
    case 1 :
      {
        LEDS [7] = B00000000;
        LEDS [6] = B00000000;
        LEDS [5] = B00111100;
        LEDS [4] = B00100100;
        LEDS [3] = B00100100;
        LEDS [2] = B00111100;
        LEDS [1] = B00000000;
        LEDS [0] = B00000000;
        break;
      }
    case 2 :
      {
        LEDS [7] = B00000000;
        LEDS [6] = B01111110;
        LEDS [5] = B01000010;
        LEDS [4] = B01000010;
        LEDS [3] = B01000010;
        LEDS [2] = B01000010;
        LEDS [1] = B01111110;
        LEDS [0] = B00000000;
        break;
      }
    case 3 :
      {
        LEDS [7] = B11111111;
        LEDS [6] = B10000001;
        LEDS [5] = B10000001;
        LEDS [4] = B10000001;
        LEDS [3] = B10000001;
        LEDS [2] = B10000001;
        LEDS [1] = B10000001;
        LEDS [0] = B11111111;
        break;
      }
  }
  if (!p_c_flp)
  {
    p_c_index ++;
    if (p_c_index > 3)
    {
      p_c_index = 3;
      p_c_flp = !p_c_flp;
    }
  }
  else
  {
    p_c_index --;
    if (p_c_index < 0)
    {
      p_c_index = 0;
      p_c_flp = !p_c_flp;
    }
  }
}
void UpdateProgram_Tunnle()
{
  switch (p_c_index)
  {
    case 0 :
      {
        LEDS [7] = B00000000;
        LEDS [6] = B00000000;
        LEDS [5] = B00000000;
        LEDS [4] = B00011000;
        LEDS [3] = B00011000;
        LEDS [2] = B00000000;
        LEDS [1] = B00000000;
        LEDS [0] = B00000000;
        break;
      }
    case 1 :
      {
        LEDS [7] = B00000000;
        LEDS [6] = B00000000;
        LEDS [5] = B00111100;
        LEDS [4] = B00100100;
        LEDS [3] = B00100100;
        LEDS [2] = B00111100;
        LEDS [1] = B00000000;
        LEDS [0] = B00000000;
        break;
      }
    case 2 :
      {
        LEDS [7] = B00000000;
        LEDS [6] = B01111110;
        LEDS [5] = B01000010;
        LEDS [4] = B01000010;
        LEDS [3] = B01000010;
        LEDS [2] = B01000010;
        LEDS [1] = B01111110;
        LEDS [0] = B00000000;
        break;
      }
    case 3 :
      {
        LEDS [7] = B11111111;
        LEDS [6] = B10000001;
        LEDS [5] = B10000001;
        LEDS [4] = B10000001;
        LEDS [3] = B10000001;
        LEDS [2] = B10000001;
        LEDS [1] = B10000001;
        LEDS [0] = B11111111;
        break;
      }
  }

  p_c_index ++;
  if (p_c_index > 3)
  {
    p_c_index = 0;
  }
}
void UpdateProgram_Heart1()
{
  switch (p_c_index)
  {
    case 0 :
      {
        LEDS [7] = B00000000;
        LEDS [6] = B00000000;
        LEDS [5] = B00000000;
        LEDS [4] = B00111100;
        LEDS [3] = B00011000;
        LEDS [2] = B00000000;
        LEDS [1] = B00000000;
        LEDS [0] = B00000000;
        break;
      }
    case 1 :
      {
        LEDS [7] = B00000000;
        LEDS [6] = B00000000;
        LEDS [5] = B00100100;
        LEDS [4] = B00111100;
        LEDS [3] = B00100100;
        LEDS [2] = B00011000;
        LEDS [1] = B00000000;
        LEDS [0] = B00000000;
        break;
      }
    case 2 :
      {
        LEDS [7] = B00000000;
        LEDS [6] = B00000000;
        LEDS [5] = B01100110;
        LEDS [4] = B01011010;
        LEDS [3] = B01000010;
        LEDS [2] = B00100100;
        LEDS [1] = B00011000;
        LEDS [0] = B00000000;
        break;
      }
    case 3 :
      {
        LEDS [7] = B00000000;
        LEDS [6] = B11100111;
        LEDS [5] = B10011001;
        LEDS [4] = B10000001;
        LEDS [3] = B10000001;
        LEDS [2] = B01000010;
        LEDS [1] = B00100100;
        LEDS [0] = B00011000;
        break;
      }
  }
  if (!p_c_flp)
  {
    p_c_index ++;
    if (p_c_index > 3)
    {
      p_c_index = 3;
      p_c_flp = !p_c_flp;
    }
  }
  else
  {
    p_c_index --;
    if (p_c_index < 0)
    {
      p_c_index = 0;
      p_c_flp = !p_c_flp;
    }
  }
}
void UpdateProgram_Heart2()
{
  switch (p_c_index)
  {
    case 0 :
      {
        LEDS [7] = B00000000;
        LEDS [6] = B00000000;
        LEDS [5] = B00000000;
        LEDS [4] = B00111100;
        LEDS [3] = B00011000;
        LEDS [2] = B00000000;
        LEDS [1] = B00000000;
        LEDS [0] = B00000000;
        break;
      }
    case 1 :
      {
        LEDS [7] = B00000000;
        LEDS [6] = B00000000;
        LEDS [5] = B00100100;
        LEDS [4] = B00111100;
        LEDS [3] = B00100100;
        LEDS [2] = B00011000;
        LEDS [1] = B00000000;
        LEDS [0] = B00000000;
        break;
      }
    case 2 :
      {
        LEDS [7] = B00000000;
        LEDS [6] = B00000000;
        LEDS [5] = B01100110;
        LEDS [4] = B01011010;
        LEDS [3] = B01000010;
        LEDS [2] = B00100100;
        LEDS [1] = B00011000;
        LEDS [0] = B00000000;
        break;
      }
    case 3 :
      {
        LEDS [7] = B00000000;
        LEDS [6] = B11100111;
        LEDS [5] = B10011001;
        LEDS [4] = B10000001;
        LEDS [3] = B10000001;
        LEDS [2] = B01000010;
        LEDS [1] = B00100100;
        LEDS [0] = B00011000;
        break;
      }
  }

  p_c_index ++;
  if (p_c_index > 3)
  {
    p_c_index = 0;
  }

}
void UpdateProgram_Heart3()
{
  switch (p_c_index)
  {
    case 0 :
      {
        LEDS [7] = B00000000;
        LEDS [6] = B00000000;
        LEDS [5] = B00000000;
        LEDS [4] = B00111100;
        LEDS [3] = B00011000;
        LEDS [2] = B00000000;
        LEDS [1] = B00000000;
        LEDS [0] = B00000000;
        break;
      }
    case 1 :
      {
        LEDS [7] = B00000000;
        LEDS [6] = B00000000;
        LEDS [5] = B00100100;
        LEDS [4] = B00111100;
        LEDS [3] = B00111100;
        LEDS [2] = B00011000;
        LEDS [1] = B00000000;
        LEDS [0] = B00000000;
        break;
      }
    case 2 :
      {
        LEDS [7] = B00000000;
        LEDS [6] = B00000000;
        LEDS [5] = B01100110;
        LEDS [4] = B01111110;
        LEDS [3] = B01111110;
        LEDS [2] = B00111100;
        LEDS [1] = B00011000;
        LEDS [0] = B00000000;
        break;
      }
    case 3 :
      {
        LEDS [7] = B00000000;
        LEDS [6] = B11100111;
        LEDS [5] = B11111111;
        LEDS [4] = B11111111;
        LEDS [3] = B11111111;
        LEDS [2] = B01111110;
        LEDS [1] = B00111100;
        LEDS [0] = B00011000;
        break;
      }
  }
  if (!p_c_flp)
  {
    p_c_index ++;
    if (p_c_index > 3)
    {
      p_c_index = 3;
      p_c_flp = !p_c_flp;
    }
  }
  else
  {
    p_c_index --;
    if (p_c_index < 0)
    {
      p_c_index = 0;
      p_c_flp = !p_c_flp;
    }
  }
}
void UpdateProgram_Heart4()
{
  switch (p_c_index)
  {
    case 0 :
      {
        LEDS [7] = B00000000;
        LEDS [6] = B00000000;
        LEDS [5] = B00000000;
        LEDS [4] = B00111100;
        LEDS [3] = B00011000;
        LEDS [2] = B00000000;
        LEDS [1] = B00000000;
        LEDS [0] = B00000000;
        break;
      }
    case 1 :
      {
        LEDS [7] = B00000000;
        LEDS [6] = B00000000;
        LEDS [5] = B00100100;
        LEDS [4] = B00111100;
        LEDS [3] = B00111100;
        LEDS [2] = B00011000;
        LEDS [1] = B00000000;
        LEDS [0] = B00000000;
        break;
      }
    case 2 :
      {
        LEDS [7] = B00000000;
        LEDS [6] = B00000000;
        LEDS [5] = B01100110;
        LEDS [4] = B01111110;
        LEDS [3] = B01111110;
        LEDS [2] = B00111100;
        LEDS [1] = B00011000;
        LEDS [0] = B00000000;
        break;
      }
    case 3 :
      {
        LEDS [7] = B00000000;
        LEDS [6] = B11100111;
        LEDS [5] = B11111111;
        LEDS [4] = B11111111;
        LEDS [3] = B11111111;
        LEDS [2] = B01111110;
        LEDS [1] = B00111100;
        LEDS [0] = B00011000;
        break;
      }
  }

  p_c_index ++;
  if (p_c_index > 3)
  {
    p_c_index = 0;
  }

}

void UpdateProgram_CyclePrograms()
{
  p_c_counter++;
  if (p_c_counter >= p_c_timer)
  {
    p_c_counter = 0;
    p_c_program_index++;
    if (p_c_program_index > 20)
      p_c_program_index = 0;
  }
  ChooseProgram( p_c_program_index);
}
void UpdateProgram_CycleProgramsRandom()
{
  p_c_counter++;
  if (p_c_counter >= p_c_timer)
  {
    p_c_counter = 0;
    p_c_program_index = random(0, 20);
    if (p_c_program_index > 20)
      p_c_program_index = 0;
  }
  ChooseProgram( p_c_program_index);
}

// DO LOOP
void loop() {
  Render();
  UpdateLCD();
  HandleInputs();
  UpdateProgram();
}
