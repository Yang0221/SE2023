#ifdef CHANGED

#ifndef PAGEPROVIDER_H
#define PAGEPROVIDER_H

#include "bitmap.h"


class PageProvider{
    public :
        PageProvider(int n );
        ~PageProvider();
        int GetEmptyPage();
        void ReleasePage(int n);
        int NumAvailPage() ;
    private :
        BitMap * pages ;
        
};
#endif// CHANGED

#endif