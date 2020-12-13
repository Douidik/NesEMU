#ifndef PPU_HPP
#define PPU_HPP

class Ram;

class Ppu
{
public:
    Ppu(Ram &ram);

private:
    Ram &m_Ram;
};    

#endif
