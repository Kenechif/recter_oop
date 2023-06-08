/*
 * printer.h
 *
 *  Created on: Oct 12, 2022
 *      Author: tunjo
 */

#ifndef PRINTER_PRINTER_H_
#define PRINTER_PRINTER_H_

#include "stdbool.h"
#include "stdint.h"

// Internal character sets used with ESC R n
#define CHARSET_USA 0           //!< American character set
#define CHARSET_FRANCE 1        //!< French character set
#define CHARSET_GERMANY 2       //!< German character set
#define CHARSET_UK 3            //!< UK character set
#define CHARSET_DENMARK1 4      //!< Danish character set 1
#define CHARSET_SWEDEN 5        //!< Swedish character set
#define CHARSET_ITALY 6         //!< Italian character set
#define CHARSET_SPAIN1 7        //!< Spanish character set 1
#define CHARSET_JAPAN 8         //!< Japanese character set
#define CHARSET_NORWAY 9        //!< Norwegian character set
#define CHARSET_DENMARK2 10     //!< Danish character set 2
#define CHARSET_SPAIN2 11       //!< Spanish character set 2
#define CHARSET_LATINAMERICA 12 //!< Latin American character set
#define CHARSET_KOREA 13        //!< Korean character set
#define CHARSET_SLOVENIA 14     //!< Slovenian character set
#define CHARSET_CROATIA 14      //!< Croatian character set
#define CHARSET_CHINA 15        //!< Chinese character set

// Character code tables used with ESC t n
#define CODEPAGE_CP437 0    //!< USA, Standard Europe character code table
#define CODEPAGE_KATAKANA 1 //!< Katakana (Japanese) character code table
#define CODEPAGE_CP850 2    //!< Multilingual character code table
#define CODEPAGE_CP860 3    //!< Portuguese character code table
#define CODEPAGE_CP863 4    //!< Canadian-French character code table
#define CODEPAGE_CP865 5    //!< Nordic character code table
#define CODEPAGE_WCP1251 6  //!< Cyrillic character code table
#define CODEPAGE_CP866 7    //!< Cyrillic #2 character code table
#define CODEPAGE_MIK 8      //!< Cyrillic/Bulgarian character code table
#define CODEPAGE_CP755 9    //!< East Europe, Latvian 2 character code table
#define CODEPAGE_IRAN 10    //!< Iran 1 character code table
#define CODEPAGE_CP862 15   //!< Hebrew character code table
#define CODEPAGE_WCP1252 16 //!< Latin 1 character code table
#define CODEPAGE_WCP1253 17 //!< Greek character code table
#define CODEPAGE_CP852 18   //!< Latin 2 character code table
#define CODEPAGE_CP858 19 //!< Multilingual Latin 1 + Euro character code table
#define CODEPAGE_IRAN2 20 //!< Iran 2 character code table
#define CODEPAGE_LATVIAN 21     //!< Latvian character code table
#define CODEPAGE_CP864 22       //!< Arabic character code table
#define CODEPAGE_ISO_8859_1 23  //!< West Europe character code table
#define CODEPAGE_CP737 24       //!< Greek character code table
#define CODEPAGE_WCP1257 25     //!< Baltic character code table
#define CODEPAGE_THAI 26        //!< Thai character code table
#define CODEPAGE_CP720 27       //!< Arabic character code table
#define CODEPAGE_CP855 28       //!< Cyrillic character code table
#define CODEPAGE_CP857 29       //!< Turkish character code table
#define CODEPAGE_WCP1250 30     //!< Central Europe character code table
#define CODEPAGE_CP775 31       //!< Baltic character code table
#define CODEPAGE_WCP1254 32     //!< Turkish character code table
#define CODEPAGE_WCP1255 33     //!< Hebrew character code table
#define CODEPAGE_WCP1256 34     //!< Arabic character code table
#define CODEPAGE_WCP1258 35     //!< Vietnam character code table
#define CODEPAGE_ISO_8859_2 36  //!< Latin 2 character code table
#define CODEPAGE_ISO_8859_3 37  //!< Latin 3 character code table
#define CODEPAGE_ISO_8859_4 38  //!< Baltic character code table
#define CODEPAGE_ISO_8859_5 39  //!< Cyrillic character code table
#define CODEPAGE_ISO_8859_6 40  //!< Arabic character code table
#define CODEPAGE_ISO_8859_7 41  //!< Greek character code table
#define CODEPAGE_ISO_8859_8 42  //!< Hebrew character code table
#define CODEPAGE_ISO_8859_9 43  //!< Turkish character code table
#define CODEPAGE_ISO_8859_15 44 //!< Latin 3 character code table
#define CODEPAGE_THAI2 45       //!< Thai 2 character code page
#define CODEPAGE_CP856 46       //!< Hebrew character code page
#define CODEPAGE_CP874 47       //!< Thai character code page

/*!
 * Barcode types used with GS k m
 */
enum barcodes {
  UPC_A,   /**< UPC-A barcode system. 11-12 char */
  UPC_E,   /**< UPC-E barcode system. 11-12 char */
  EAN13,   /**< EAN13 (JAN13) barcode system. 12-13 char */
  EAN8,    /**< EAN8 (JAN8) barcode system. 7-8 char */
  CODE39,  /**< CODE39 barcode system. 1<=num of chars */
  ITF,     /**< ITF barcode system. 1<=num of chars, must be an even number */
  CODABAR, /**< CODABAR barcode system. 1<=num<=255 */
  CODE93,  /**< CODE93 barcode system. 1<=num<=255 */
  CODE128, /**< CODE128 barcode system. 2<=num<=255 */
};


typedef struct
{
	char* name;
	char* adr;
	uint8_t* logo;
	char transaction_id[18];
	char date_[20];
	char time_[20];
	char transaction_type[17];
	char voucher_[16];
	char product_[4];
	char pn[5];
	char pl[10];
	char ta[12];  //amount
	char tv[12];  // volume
}print_struct;


#ifdef __cplusplus
extern "C" {
#endif


void

	start_prn(uint16_t version),

	boldOff(),

	boldOn(),

	feed(uint8_t x),

	justify(char value),

	setFont(char font),  //font='A'

	setCharset(uint8_t val),  //val=0

	setDefault(),

		   /*!
		     * @brief Sets print and feed speed
		     * @param p print speed
		     * @param f feed speed
		     */
	setTimes(unsigned long p, unsigned long f),
		    /*!
		     * @brief Sets print head heating configuration
		     * @param dots max printing dots, 8 dots per increment
		     * @param time heating time, 10us per increment
		     * @param interval heating interval, 10 us per increment
		     */
	setHeatConfig(uint8_t dots, uint8_t time, uint8_t interval),  //11,120,4

	setSize(char value);

bool timeoutWait();
void timeoutSet();

void send_printer();

#ifdef __cplusplus
}
#endif


#endif /* PRINTER_PRINTER_H_ */
