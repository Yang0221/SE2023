#ifdef CHANGED

#include "pageprovider.h"
#include "bitmap.h"
#include "system.h"

PageProvider::PageProvider(int n)
{
    pages = new BitMap(n);
}
PageProvider::~PageProvider()
{
    delete pages;
}
int
PageProvider::GetEmptyPage()
{
    int emptyPage = pages->Find();
    ASSERT(emptyPage != -1);
    bzero(machine->mainMemory + emptyPage*PageSize,PageSize);
    return emptyPage ;
}
void
PageProvider::ReleasePage(int n){
    pages->Clear(n);
}
int
PageProvider::NumAvailPage(){
    int numAvailPage = pages->NumClear();
    return numAvailPage ; 
} 

#endif // CHANGED