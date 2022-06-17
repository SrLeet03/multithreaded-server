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
