#include "page.h"
#include "stdio.h"
#include "stdlib.h"

int PDBR=0xd80;

void translate(unsigned int vaddr) {
    unsigned char pde=memory[PDBR+((vaddr>>10)&0x1F)];
    printf("  --> pde index: 0x%x  pde contents:(valid %d, pfn 0x%x)\n",(vaddr>>10)&0x1F,pde>>7,pde&0x7F);
    if (!(pde>>7))
	{
        printf("    --> Fault (page directory entry not valid)\n");
        return;
    }
    unsigned char pte=memory[(((unsigned int)(pde & 0x7F))<<5)+((vaddr>>5)&0x1F)];
    printf("    --> pte index:0x%x  pte contents:(valid %d, pfn 0x%x)\n",(vaddr>>5)&0x1F,pte>>7,pte&0x7F);
    unsigned int res=(((unsigned)(pte&0x7F))<<5)+(vaddr&0x1F);
    if (!(pte>>7))
	{
        if ((pte&0x7F)==0x7F)
		{
            printf("      --> Fault (the page does not exist)\n");
            return;
        } 
		else
		{
            int address=((pte&0x7F)<<5)+(vaddr&0x1F);
            printf("      --> Translates to Disk Sector Address 0x%x --> Value: 0x%x\n",address,disk[address]);
            return;
        }
    }
    printf("      --> Translates to Physical Address 0x%x --> Value: 0x%x\n",res,memory[res]);
    return ;
}

int main()
{
    printf("please input your virtual address:\n>");
    int input;
    scanf("%x",&input);
    printf("Virtual Address %x:\n",input);
    translate(input&0x7FFF);
}
