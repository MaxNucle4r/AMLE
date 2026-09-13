#pragma once

#include <windows.h>




/// COLOR CODES:
/*

0	Black	                          setColor(0);
1	Blue	                          setColor(1);
2	Green	                          setColor(2);
3	Aqua / Cyan	                      setColor(3);
4	Red	                              setColor(4);
5	Purple / Magenta	              setColor(5);
6	Yellow / Dark Yellow	          setColor(6);
7	White / Light Gray	              setColor(7);
8	Gray / Dark Gray	              setColor(8);
9	Light Blue	                      setColor(9);
10	Light Green	                      setColor(10);
11	Light Aqua / Bright Cyan	      setColor(11);
12	Light Red	                      setColor(12);
13	Light Purple / Bright Magenta  	  setColor(13);
14	Light Yellow	                  setColor(14);
15	Bright White	                  setColor(15);

*/

inline void setTextColor(int textColor)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), static_cast<WORD>(textColor));
}



class ScopedColor
{
public:
    explicit ScopedColor(int color)
    {
        setTextColor(color);
    }

    ~ScopedColor()
    {
        setTextColor(7);
    }
};