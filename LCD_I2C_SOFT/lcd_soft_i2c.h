#ifndef LCD_SOFT_I2C_H
#define LCD_SOFT_I2C_H

#include "i2c.h"

typedef struct {
    Soft_I2C_TypeDef *i2c;
    uint8_t address;   // I2C address c?a module LCD
} I2C_LCD_HandleTypedef;

// Kh?i t?o LCD
void lcd_clear(I2C_LCD_HandleTypedef* lcd);
void lcd_gotoxy(I2C_LCD_HandleTypedef* lcd, uint8_t col, uint8_t row);
void lcd_init(I2C_LCD_HandleTypedef* lcd, Soft_I2C_TypeDef* _hi2c, uint16_t _address);
void lcd_send_string(I2C_LCD_HandleTypedef* lcd, char* str);
void lcd_send_char(I2C_LCD_HandleTypedef* lcd, char c);
void lcd_create_char(I2C_LCD_HandleTypedef* lcd, uint8_t location, uint8_t charmap[]);

#endif
