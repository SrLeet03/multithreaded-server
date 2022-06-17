#include<common.h>

void error_and_kill(const char *ch , ...){
    int error_save ;
    va_list ap;
    
    //libary call to save the error no , now we need to save it now
    error_save = errno ; 
    
    va_start(ap , ch) ;
    vfprintf(stdout , ch , ap);
    fprintf(stdout , "\n");
    fflush(stdout);

    if ( error_save != 0  ){
        fprintf(stdout , "  ( error = %d ) : %s\n" , error_save , 
        strerror(error_save)) ;

        fprintf(stdout , "\n") ; 
        fflush(stdout) ; 
    }

    va_end(ap) ; 

    exit(1) ; 

}

char *bin2hex(const unsigned *input  , size_t len){
    char *result ; 
    char *hexits = "0123456789ABCDEF" ; 
    
    if( input == NULL || len<=0 ){
        return NULL ;
    }
    int resultLen = ((len*3)) + 1 ; 
     malloc( resultLen  ) ; 
    bzero(result , resultLen) ; 

    for(int i=0 ; i<len ; i++){
        result[i*3] = hexits[input[i]>>4] ;
        result[i*3 + 1] = hexits[input[i] & 0X0F] ;
        result[i*3 + 2] = ' ' ;
    }
    return result;
}