// Type your code here, or load an example.
#include <stdio.h>
#include <stdint.h>
#include <assert.h>


void extended_print(const uint32_t * const __restrict__ a,
                        const uint32_t size)
{
        register uint32_t i;
        for(i=0;i<size;++i){
            if(i){
                printf(" + ");
            }
            printf("%u * 2^%d",a[i],(i<<5)/*32*i*/);
        }
        printf("\n");
}


void extended_add(const uint32_t * const __restrict__ a,
                    const uint32_t * const __restrict__ b,
                    uint32_t * const __restrict__ result,
                    const uint32_t size)
{
        #define BIT_ADD(a,b,res,next_carriage)\
            next_carriage=(a && b);\
            res= a ^ b; //exclusive or
            
        uint32_t bit=0;
        uint8_t carriage=0;
        uint32_t n_bits=(size<<5);//32*size
        for(bit=0;bit<size;++bit){
            result[bit]=0;
        }
        for(bit=0;bit<n_bits;++bit){
            register uint32_t index=bit>>5;//bit/32;
            register uint8_t shift=bit&0x1F;//bit%32;
            
            register uint8_t to_add_a=((a[index]&(1<<shift))>>shift);
            register uint8_t to_add_b=((b[index]&(1<<shift))>>shift);
            register uint8_t add_res=0,next_carriage1=0,next_carriage2=0;
            BIT_ADD(to_add_a,to_add_b,add_res,next_carriage1);
            BIT_ADD(add_res,carriage,add_res,next_carriage2);
           // printf("%d ",add_res);
            result[index]|=(add_res<<shift);
            carriage=next_carriage1 || next_carriage2;
            assert(!(next_carriage1 && next_carriage2));
        }
}


int main()
{
        printf("aritmetic extended precision add testing:\n");
        uint32_t a[2]={4294967292,0};
        uint32_t b[2]={4294967292,0};
        uint32_t res[2];
        extended_add(a,b,res,2);
        extended_print(res,2);


        return 0;
}