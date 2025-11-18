#include "lcd_soft_i2c.h"

void lcd_send_cmd(I2C_LCD_HandleTypedef* lcd, char cmd){
    char upper_nibble, lower_nibble;
    uint8_t data_t[4];
    upper_nibble = (cmd & 0xF0); // get 4 bit upper
    lower_nibble = (cmd << 4) & 0xF0; // get 4 bit lower

    data_t[0] = upper_nibble | 0x04; // en=1, rw=0, rs=0
    data_t[1] = upper_nibble | 0x00; // en=0, rw=0, rs=0
    data_t[2] = lower_nibble | 0x04; // en=1, rw=0, rs=0
    data_t[3] = lower_nibble | 0x00; // en=0, rw=0, rs=0

    i2c_soft_write_addr8_bytes(lcd->i2c, lcd->address, 0x00, (uint8_t*)data_t, 4);
}

void lcd_send_data(I2C_LCD_HandleTypedef* lcd, char data){
    char upper_nibble, lower_nibble;
    uint8_t data_t[4];
    upper_nibble = (data & 0xF0);
    lower_nibble = (data << 4) & 0xF0;

    data_t[0] = upper_nibble | 0x05; // en=1, rw=0, rs=1
    data_t[1] = upper_nibble | 0x01; // en=0, rw=0, rs=1
    data_t[2] = lower_nibble | 0x05; // en=1, rw=0, rs=1
    data_t[3] = lower_nibble | 0x01; // en=0, rw=0, rs=1

    i2c_soft_write_addr8_bytes(lcd->i2c, lcd->address, 0x00, (uint8_t*)data_t, 4);
}

void lcd_clear(I2C_LCD_HandleTypedef* lcd){
    lcd_send_cmd(lcd, 0x01);
    HAL_Delay(2);
}

void lcd_gotoxy(I2C_LCD_HandleTypedef* lcd, uint8_t col, uint8_t row){
    uint8_t cmd_address;
    switch(row){
        case 0: cmd_address = 0x80 + col; break;
        case 1: cmd_address = 0xC0 + col; break;
        case 2: cmd_address = 0x94 + col; break;
        case 3: cmd_address = 0xD4 + col; break;
        default: return;
    }
    lcd_send_cmd(lcd, cmd_address);
}

void lcd_init(I2C_LCD_HandleTypedef* lcd, Soft_I2C_TypeDef* i2c, uint16_t _address){
    lcd->i2c = i2c;
    lcd->address = _address;

    lcd_send_cmd(lcd, 0x33);
    lcd_send_cmd(lcd, 0x32);
    lcd_send_cmd(lcd, 0x28);
    lcd_send_cmd(lcd, 0x0C);
    lcd_send_cmd(lcd, 0x06);
    lcd_send_cmd(lcd, 0x01);
    HAL_Delay(2);
}

void lcd_send_string(I2C_LCD_HandleTypedef* lcd, char* str){
    while(*str) lcd_send_data(lcd, *str++);
}

void lcd_send_char(I2C_LCD_HandleTypedef* lcd, char c){
    lcd_send_data(lcd, c);
}

void lcd_create_char(I2C_LCD_HandleTypedef* lcd, uint8_t location, uint8_t charmap[]){
    location &= 0x7;
    lcd_send_cmd(lcd, 0x40 | (location << 3));
    for(uint8_t i=0; i<8; i++){
        lcd_send_data(lcd, charmap[i]);
    }
}
