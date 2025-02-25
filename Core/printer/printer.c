/*
 * printer.c
 *
 *  Created on: Oct 12, 2022
 *      Author: tunjo
 */

#include "myLibraries.h"
#include "printer.h"

#include "stdbool.h"
#include "stdint.h"
//#include "stm32f4xx_hal.c"
//#include "stm32f4xx_hal_uart.h"
#include "main.h"

#define ASCII_TAB '\t' //!< Horizontal tab
#define ASCII_LF '\n'  //!< Line feed
#define ASCII_FF '\f'  //!< Form feed
#define ASCII_CR '\r'  //!< Carriage return
#define ASCII_DC2 18   //!< Device control 2
#define ASCII_ESC 27   //!< Escape
#define ASCII_FS 28    //!< Field separator
#define ASCII_GS 29    //!< Group separator

#define true 1

#define BAUDRATE   9600
#define BYTE_TIME (((11L * 1000000L) + (BAUDRATE / 2)) / BAUDRATE)

// === Character commands ===
#define FONT_MASK (1 << 0) //!< Select character font A or B
#define INVERSE_MASK  (1 << 1) //!< Turn on/off white/black reverse printing mode. Not in 2.6.8
           //!< firmware (see inverseOn())
#define UPDOWN_MASK (1 << 2)        //!< Turn on/off upside-down printing mode
#define BOLD_MASK (1 << 3)          //!< Turn on/off bold printing mode
#define DOUBLE_HEIGHT_MASK (1 << 4) //!< Turn on/off double-height printing mode
#define DOUBLE_WIDTH_MASK (1 << 5)  //!< Turn on/off double-width printing mode
#define STRIKE_MASK (1 << 6)        //!< Turn on/off deleteline mode



  uint8_t printMode,
      prevByte,      // Last character issued to printer
      column,        // Last horizontal column printed
      maxColumn,     // Page width (output 'wraps' at this point)
      charHeight,    // Height of characters, in 'dots'
      lineSpacing,   // Inter-line spacing (not line height), in dots
      barcodeHeight, // Barcode height in dots, not including text
      maxChunkHeight,
      dtrPin;         // DTR handshaking pin (experimental)
  uint16_t firmware;  // Firmware version
  bool dtrEnabled = 1; // True if DTR pin set & printer initialized
  unsigned long
      resumeTime,   // Wait until micros() exceeds this before sending byte
      dotPrintTime, // Time to print a single dot line, in microseconds
      dotFeedTime;  // Time to feed a single dot line, in microseconds

  char prn[40] = {0};
//============================================================================

 void writeBytes(uint8_t a),
	  writeBytes2(uint8_t a, uint8_t b),
      writeBytes3(uint8_t a, uint8_t b, uint8_t c),
      writeBytes4(uint8_t a, uint8_t b, uint8_t c, uint8_t d),

	  setPrintMode(uint8_t mask),
	  unsetPrintMode(uint8_t mask),
      writePrintMode(),
	  adjustCharValues(uint8_t printMode);

 void doubleWidthOff() ;
 void doubleWidthOn();
 void doubleHeightOff();
 void doubleHeightOn();
 void MANUAL();
 void AUTO();
//-----------------------------------------------------
 void flush() ;
 void reset();
 void setCodePage(uint8_t val);
 void wake();

//=============================================================================

 bool dtr = 0;

//---------------------------------------------------
bool timeoutWait()
{
//  if (dtrEnabled == 1 )
//  {
//    //if (digitalRead(dtrPin) == HIGH) return
//  }
  return 0;
}
//---------------------------------------------------
void timeoutSet(unsigned long x)
{
 // if (!dtrEnabled)
    resumeTime = x ; //+ micros() ;

}


void setTimes(unsigned long p, unsigned long f)
{
  dotPrintTime = p;
  dotFeedTime = f;
}

void write(uint8_t c)
{
	  if (c != 13) { // Strip carriage returns
	    timeoutWait();
	    //HAL_UART_Transmit_IT(huart, pData, Size); // stream->write(c);
	    unsigned long d = BYTE_TIME;
	    if ((c == '\n') || (column == maxColumn)) { // If newline or wrap
	      d += (prevByte == '\n') ? ((charHeight + lineSpacing) * dotFeedTime)
	                              : // Feed line
	               ((charHeight * dotPrintTime) +
	                (lineSpacing * dotFeedTime)); // Text line
	      column = 0;
	      c = '\n'; // Treat wrap as newline on next pass
	    } else {
	      column++;
	    }
	    timeoutSet(d);
	    prevByte = c;
	  }

}

void writeBytes(uint8_t a)
{
  timeoutWait();
  write(a);
  timeoutSet(BYTE_TIME);
}

void writeBytes2(uint8_t a, uint8_t b)
{
  timeoutWait();
  write(a);
  write(b);
 // timeoutSet(2 * BYTE_TIME);
}

void writeBytes3(uint8_t a, uint8_t b, uint8_t c)
{
	  timeoutWait();
	  write(a);
	  write(b);
	  write(c);
	  //timeoutSet(3 * BYTE_TIME);
}

void writeBytes4(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
{
  timeoutWait();
  write(a);
  write(b);
  write(c);
  write(d);
  //timeoutSet(4 * BYTE_TIME);
}

void start_prn(uint16_t version)
{

	  firmware = version;

	  // The printer can't start receiving data immediately upon power up --
	  // it needs a moment to cold boot and initialize.  Allow at least 1/2
	  // sec of uptime before printer can receive data.
	  timeoutSet(500000L);

	  wake();
	  reset();

	  setHeatConfig(11,120,4);

	  // Enable DTR pin if requested
	  if (dtrPin < 255)
	  {
	   // pinMode(dtrPin, INPUT_PULLUP);
	    writeBytes3(ASCII_GS, 'a', (1 << 5));
	    dtrEnabled = true;
	  }

	  dotPrintTime = 30000; // See comments near top of file for
	  dotFeedTime = 2100;   // an explanation of these values.
	  maxChunkHeight = 255;
	}
//------------------------------------------------------
void reset()
{
  writeBytes2(ASCII_ESC, '@'); // Init command
  prevByte = '\n';            // Treat as if prior line is blank
  column = 0;
  maxColumn = 32;
  charHeight = 24;
  lineSpacing = 6;
  barcodeHeight = 50;

  if (firmware >= 264)
  {
    // Configure tab stops on recent printers
    writeBytes2(ASCII_ESC, 'D'); // Set tab stops...
    writeBytes4(4, 8, 12, 16);   // ...every 4 columns,
    writeBytes4(20, 24, 28, 0);  // 0 marks end-of-list.
  }
}

void setDefault()
{
	  AUTO();
	  justify('L');
	//  inverseOff();
	//  doubleHeightOff();
	//  setLineHeight(30);
	  boldOff();
	//  underlineOff();
	//  setBarcodeHeight(50);
	  setSize('s');
	  setCharset(0);
	  setCodePage(0);
}


void setFont(char font)
{
  switch ((font)) {
  case 'B':
    setPrintMode(FONT_MASK);
    break;
  case 'A':
  default:
    unsetPrintMode(FONT_MASK);
  }
}

void setPrintMode(uint8_t mask)
{
  printMode |= mask;
  writePrintMode();
  adjustCharValues(printMode);
  // charHeight = (printMode & DOUBLE_HEIGHT_MASK) ? 48 : 24;
  // maxColumn = (printMode & DOUBLE_WIDTH_MASK) ? 16 : 32;
}

void unsetPrintMode(uint8_t mask)
{
  printMode &= ~mask;
  writePrintMode();
  adjustCharValues(printMode);
  // charHeight = (printMode & DOUBLE_HEIGHT_MASK) ? 48 : 24;
  // maxColumn = (printMode & DOUBLE_WIDTH_MASK) ? 16 : 32;
}

void writePrintMode() {
	  writeBytes3(ASCII_ESC, '!', printMode);
	}


void adjustCharValues(uint8_t printMode) {
  uint8_t charWidth;
  if (printMode & FONT_MASK) {
    // FontB
    charHeight = 17;
    charWidth = 9;
  } else {
    // FontA
    charHeight = 24;
    charWidth = 12;
  }
  // Double Width Mode
  if (printMode & DOUBLE_WIDTH_MASK) {
    maxColumn /= 2;
    charWidth *= 2;
  }
  // Double Height Mode
  if (printMode & DOUBLE_HEIGHT_MASK) {
    charHeight *= 2;
  }
  maxColumn = (384 / charWidth);
}



void setSize(char value) {
  uint8_t size;

  switch ((value)) {
  default: // Small: standard width and height
    // size = 0x00;
    // charHeight = 24;
    // maxColumn = 32;
    doubleWidthOff();
    doubleHeightOff();
    break;
  case 'M': // Medium: double height
    // size = 0x01;
    // charHeight = 48;
    // maxColumn = 32;
    doubleHeightOn();
    doubleWidthOff();
    break;
  case 'L': // Large: double width and height
    // size = 0x11;
    // charHeight = 48;
    // maxColumn = 16;
    doubleHeightOn();
    doubleWidthOn();
    break;
  }
}

void setCharset(uint8_t val)
{
	  if (val > 15)
	    val = 15;
	  writeBytes3(ASCII_ESC, 'R', val);
	}


void setCodePage(uint8_t val)
{
	  if (val > 47)
	    val = 47;
	  writeBytes3(ASCII_ESC, 't', val);
	}

void boldOn() { setPrintMode(BOLD_MASK); }
void boldOff() { unsetPrintMode(BOLD_MASK); }

  void doubleHeightOn() { setPrintMode(DOUBLE_HEIGHT_MASK); }
  void doubleHeightOff() { unsetPrintMode(DOUBLE_HEIGHT_MASK); }

  void doubleWidthOff() { unsetPrintMode(DOUBLE_WIDTH_MASK); }
  void doubleWidthOn() { setPrintMode(DOUBLE_WIDTH_MASK); }
  void MANUAL() { writeBytes3(ASCII_ESC, '=', 0); }

 void AUTO() { writeBytes3(ASCII_ESC, '=', 1); }
//-----------------------------------------------------
 void flush() { writeBytes(ASCII_FF); }


 //----------------------------------

 void wake()
 {
   timeoutSet(0);   // Reset timeout counter
   writeBytes(255); // Wake
   if (firmware >= 264)
   {
     HAL_Delay(50);
     writeBytes4(ASCII_ESC, '8', 0, 0); // Sleep off (important!)
   } else {
     // Datasheet recommends a 50 mS delay before issuing further commands,
     // but in practice this alone isn't sufficient (e.g. text size/style
     // commands may still be misinterpreted on wake).  A slightly longer
     // delay, interspersed with NUL chars (no-ops) seems to help.
     for (uint8_t i = 0; i < 10; i++) {
       writeBytes(0);
       timeoutSet(10000L);
     }
   }
 }

void justify(char value)
{
  uint8_t pos = 0;

  switch ((value))
  {
  case 'L':
    pos = 0;
    break;
  case 'C':
    pos = 1;
    break;
  case 'R':
    pos = 2;
    break;
  }
  writeBytes3(ASCII_ESC, 'a', pos);
}

// Feeds by the specified number of lines
void feed(uint8_t x)
{
  if (firmware >= 264)
  {
    writeBytes3(ASCII_ESC, 'd', x);
    timeoutSet(dotFeedTime * charHeight);
    prevByte = '\n';
    column = 0;
  }
    else
  {
    while (x--)
      write('\n'); // Feed manually; old firmware feeds excess lines
  }
}
//---------------------------------------------------------------------
void setHeatConfig(uint8_t dots, uint8_t time, uint8_t interval)
{
  writeBytes2(ASCII_ESC, '7');       // Esc 7 (print settings)
  writeBytes3(dots, time, interval); // Heating dots, heat time, heat interval
}
//---------------------------------------------------------------------
void printBitmap(int w, int h, const uint8_t *bitmap, bool fromProgMem)
{
  int rowBytes, rowBytesClipped, rowStart, chunkHeight, chunkHeightLimit, x, y,
      i;

  rowBytes = (w + 7) / 8; // Round up to next byte boundary
  rowBytesClipped = (rowBytes >= 48) ? 48 : rowBytes; // 384 pixels max width

  // Est. max rows to write at once, assuming 256 byte printer buffer.
  if (dtrEnabled) {
    chunkHeightLimit = 255; // Buffer doesn't matter, handshake!
  } else {
    chunkHeightLimit = 256 / rowBytesClipped;
    if (chunkHeightLimit > maxChunkHeight)
      chunkHeightLimit = maxChunkHeight;
    else if (chunkHeightLimit < 1)
      chunkHeightLimit = 1;
  }

  for (i = rowStart = 0; rowStart < h; rowStart += chunkHeightLimit) {
    // Issue up to chunkHeightLimit rows at a time:
    chunkHeight = h - rowStart;
    if (chunkHeight > chunkHeightLimit)
      chunkHeight = chunkHeightLimit;

    writeBytes4(ASCII_DC2, '*', chunkHeight, rowBytesClipped);

    for (y = 0; y < chunkHeight; y++) {
      for (x = 0; x < rowBytesClipped; x++, i++) {
        timeoutWait();
        //stream->write(fromProgMem ? pgm_read_byte(bitmap + i) : *(bitmap + i));
      }
      i += rowBytes - rowBytesClipped;
    }
    timeoutSet(chunkHeight * dotPrintTime);
  }
  prevByte = '\n';
}

//===========================================================

void send_printer(char* ss)
{
	//char prn[40] = {0};
	//strncpy((char*)prn,ss,40);
	extern UART_HandleTypeDef huart3;
    HAL_UART_Transmit(&huart3,(char*)ss, strlen(ss),1000  );
}
