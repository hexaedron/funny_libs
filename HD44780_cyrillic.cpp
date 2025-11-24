#include "HD44780_i2c.h"

void HD44780_i2c::printCyrillicChar(uint32_t cyrChar)
{
    switch (cyrChar)
    {
        case 0xd0b0: this->printChar('a');                                          break;
        case 0xd0b1: this->printChar(this->pushCyrChar(cyrillicGlyphs::be));        break;
        case 0xd0b2: this->printChar(this->pushCyrChar(cyrillicGlyphs::ve));        break;
        case 0xd0b3: this->printChar(this->pushCyrChar(cyrillicGlyphs::ghe));       break;
        case 0xd0b4: this->printChar(this->pushCyrChar(cyrillicGlyphs::de));        break;
        case 0xd0b5: this->printChar('e');                                          break;
        case 0xd191: this->printChar(this->pushCyrChar(cyrillicGlyphs::yo));        break;
        case 0xd0b6: this->printChar(this->pushCyrChar(cyrillicGlyphs::zhe));       break;
        case 0xd0b7: this->printChar(this->pushCyrChar(cyrillicGlyphs::ze));        break;
        case 0xd0b8: this->printChar(this->pushCyrChar(cyrillicGlyphs::i));         break;
        case 0xd0b9: this->printChar(this->pushCyrChar(cyrillicGlyphs::ikr));       break;
        case 0xd0ba: this->printChar('k');                                          break;
        case 0xd0bb: this->printChar(this->pushCyrChar(cyrillicGlyphs::l));         break;
        case 0xd0bc: this->printChar(this->pushCyrChar(cyrillicGlyphs::m));         break;
        case 0xd0bd: this->printChar(this->pushCyrChar(cyrillicGlyphs::n));         break;
        case 0xd0be: this->printChar('o');                                          break;
        case 0xd0bf: this->printChar(this->pushCyrChar(cyrillicGlyphs::p));         break;
        case 0xd180: this->printChar('p');                                          break;
        case 0xd181: this->printChar('c');                                          break;
        case 0xd182: this->printChar(this->pushCyrChar(cyrillicGlyphs::t));         break;
        case 0xd183: this->printChar('y');                                          break;
        case 0xd184: this->printChar(this->pushCyrChar(cyrillicGlyphs::f));         break;
        case 0xd185: this->printChar('x');                                          break;
        case 0xd186: this->printChar(this->pushCyrChar(cyrillicGlyphs::tse));       break;
        case 0xd187: this->printChar(this->pushCyrChar(cyrillicGlyphs::tsche));     break;
        case 0xd188: this->printChar(this->pushCyrChar(cyrillicGlyphs::sha));       break;
        case 0xd189: this->printChar(this->pushCyrChar(cyrillicGlyphs::scha));      break;
        case 0xd18a: this->printChar(this->pushCyrChar(cyrillicGlyphs::tv_znak));   break;
        case 0xd18b: this->printChar(this->pushCyrChar(cyrillicGlyphs::oui));       break;
        case 0xd18c: this->printChar(this->pushCyrChar(cyrillicGlyphs::mg_znak));   break;
        case 0xd18d: this->printChar(this->pushCyrChar(cyrillicGlyphs::e));         break;
        case 0xd18e: this->printChar(this->pushCyrChar(cyrillicGlyphs::you));       break;
        case 0xd18f: this->printChar(this->pushCyrChar(cyrillicGlyphs::ya));        break;


        case 0xd090: this->printChar('A');                                          break; 
        case 0xd091: this->printChar(this->pushCyrChar(cyrillicGlyphs::Be));        break; 
        case 0xd092: this->printChar('B');                                          break; 
        case 0xd093: this->printChar(this->pushCyrChar(cyrillicGlyphs::Ghe));       break; 
        case 0xd094: this->printChar(this->pushCyrChar(cyrillicGlyphs::De));        break;
        case 0xd095: this->printChar('E');                                          break; 
        case 0xd081: this->printChar(this->pushCyrChar(cyrillicGlyphs::Yo));        break; 
        case 0xd096: this->printChar(this->pushCyrChar(cyrillicGlyphs::Zhe));       break; 
        case 0xd097: this->printChar(this->pushCyrChar(cyrillicGlyphs::Ze));        break; 
        case 0xd098: this->printChar(this->pushCyrChar(cyrillicGlyphs::I));         break;
        case 0xd099: this->printChar(this->pushCyrChar(cyrillicGlyphs::Ikr));       break; 
        case 0xd09a: this->printChar('K');                                          break; 
        case 0xd09b: this->printChar(this->pushCyrChar(cyrillicGlyphs::L));         break; 
        case 0xd09c: this->printChar('M');                                          break; 
        case 0xd09d: this->printChar('H');                                          break; 
        case 0xd09e: this->printChar('O');                                          break; 
        case 0xd09f: this->printChar(this->pushCyrChar(cyrillicGlyphs::P));         break; 
        case 0xd0a0: this->printChar('P');                                          break; 
        case 0xd0a1: this->printChar('C');                                          break; 
        case 0xd0a2: this->printChar('T');                                          break; 
        case 0xd0a3: this->printChar(this->pushCyrChar(cyrillicGlyphs::Y));         break;
        case 0xd0a4: this->printChar(this->pushCyrChar(cyrillicGlyphs::F));         break; 
        case 0xd0a5: this->printChar('X');                                          break; 
        case 0xd0a6: this->printChar(this->pushCyrChar(cyrillicGlyphs::Tse));       break;
        case 0xd0a7: this->printChar(this->pushCyrChar(cyrillicGlyphs::Tsche));     break;
        case 0xd0a8: this->printChar(this->pushCyrChar(cyrillicGlyphs::Sha));       break;
        case 0xd0a9: this->printChar(this->pushCyrChar(cyrillicGlyphs::Scha));      break; 
        case 0xd0aa: this->printChar(this->pushCyrChar(cyrillicGlyphs::Tv_znak));   break; 
        case 0xd0ab: this->printChar(this->pushCyrChar(cyrillicGlyphs::Oui));       break; 
        case 0xd0ac: this->printChar(this->pushCyrChar(cyrillicGlyphs::Mg_znak));   break; 
        case 0xd0ad: this->printChar(this->pushCyrChar(cyrillicGlyphs::E));         break; 
        case 0xd0ae: this->printChar(this->pushCyrChar(cyrillicGlyphs::You));       break; 
        case 0xd0af: this->printChar(this->pushCyrChar(cyrillicGlyphs::Ya));        break; 

        default: break;
    }
}

uint8_t HD44780_i2c::pushCyrChar(const uint8_t* cyrChar)
{
    int8_t ret = this->searchCharmap(cyrChar);
    if(ret == -1)
    {
        this->createChar(this->currentCyrChar, cyrChar);
        this->setCursor(this->_col, this->_row);
        this->charmap[this->currentCyrChar] = cyrChar;
        this->currentCyrChar++;
        if(this->currentCyrChar >= 8) this->currentCyrChar = 0;
        return this->currentCyrChar - 1;
    }
    else
    {
        return (uint8_t)ret;
    }
}

int8_t HD44780_i2c::searchCharmap(const uint8_t* cyrChar)
{
    for(int8_t i = 0; i < 8; i++)
        if(this->charmap[i] == cyrChar)
            return i;

    return -1;
}

void HD44780_i2c::flushCharmap()
{
    this->currentCyrChar = 0;
    memset(this->charmap, 0, sizeof(this->charmap));
}