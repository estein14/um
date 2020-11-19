#include <stdio.h>

typedef enum Um_opcode {
        CMOV = 0, SLOAD, YES, NO
} Um_opcode;


int main()
{
    
    Um_opcode code = YES;
    
    
    switch (code) 
    {
        
        case 0:
            printf("%s\n", "Hello\n");
            break;
            
        case 1:
            printf("%s\n", "Goodbye\n");
            break;
            
        case 2:
            printf("%d\n", 5);
            break;
            
        case 3:
            printf("%d\n", 7);
            break;
            
            
    }
    
    
    
    
    
    
    return 0;
}