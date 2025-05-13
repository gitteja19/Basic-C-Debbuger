
#include<stdio.h>//perror,printf
#include<stdio_ext.h>//__fpurge,
#include<stdlib.h>//system,exit
#include<string.h>//strdup,strlen
#include<unistd.h>//fork,
#include<signal.h>//sigemptyset,sigfillset,sigaddset,signal,sigprocmask

#define SZ 100

#define RESET   "\033[0m"
#define BLACK   "\033[30m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define PINK    "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define BBLACK  "\033[1;30m"
#define BRED    "\033[1;31m"
#define BGREEN  "\033[1;32m"
#define BYELLOW "\033[1;33m"
#define BBLUE   "\033[1;34m"
#define BPINK   "\033[1;35m"
#define BCYAN   "\033[1;36m"
#define BWHITE  "\033[1;37m"


void pfclr(int n){


}

int main(int argc,char *argv[]){
        char buf[SZ];


        if(argc!=2){
                puts("use:./my_gdb <file.c>");
                exit(1);
        }

        //.c -> temp.c
        ///adding handlers and headers
        FILE *rf=NULL,*wf=NULL;
        rf= fopen(argv[1],"r");
        wf= fopen("prog.c","w");
        if(!rf || !wf){
                perror("files");
                exit(1);
        }
        fputs(  "#include <signal.h>\n"
                "#include <stdio.h>\n"
                "#include <unistd.h>\n"
                "void usr1_isr(int s){/*puts(\"next isr\");*/}\n"
                "void usr2_isr(int s){puts(\"run  isr\");}\n"
                "void sig_init(void){"
                "sigset_t new;\n"
                "sigfillset(&new)\n;"
                "sigdelset(&new,SIGUSR1);\n"
                "sigdelset(&new,SIGUSR2);\n"
                "sigprocmask(SIG_SETMASK,&new,NULL);\n"
                "signal(SIGUSR1,usr1_isr);\n"
                "signal(SIGUSR2,usr2_isr);}\n",wf);
        while(fgets(buf,SZ,rf)){
                fputs(buf,wf);
        }
        fclose(rf);
        fclose(wf);

        //prog.c -> prog.i
        ///remove empty lines
        sprintf(buf,"gcc -E prog.c|grep . > prog.i");
        system(buf);

        //cpy prog.i -> temp.i
        ///and add pause in main after each line
        rf= fopen("prog.i","r");
        wf= fopen("temp.i","w");
        if(!rf || !wf){
                perror("files");
                exit(1);
        }
        int flag=0;
        while(fgets(buf,SZ,rf)){
                if(strstr(buf,"main(")){
                        fputs(buf,wf);
                        fputs("sig_init();\npause();\n",wf);
                        flag=1;
                        continue;
                }
                fputs(buf,wf);
                if(flag){
                        //in main
                        //printf("%ld,%c,%s",strlen(buf),buf[0],buf);
                        if((strlen(buf)==2)&&(buf[0]='}')){
                                //out of main
                                flag =0;
                                continue;
                        }
                        fputs("pause();\n",wf);
                }
        }
        fclose(rf);
        fclose(wf);

        // just implement n(^c),q(^\)
        char ch;
        puts("press (r)un to start,(q)uit");
        while(1){
                printf(BYELLOW"(my_gdb) :"RESET);
                __fpurge(stdin);
                ch=getchar();
                ch|=32;//smalling
                if(ch=='r'){
                        //run
                        puts("Runnig gbd");
                        break;
                }else if(ch=='q'){
                        //quit
                        puts("Exiting my_gdb");
                        exit(1);
                }else{
                        puts("(r)un to debug");
                }
        }

        pid_t ret;
        ret=fork();
        if(ret<0){
                perror("fork");
                exit(1);
        }else if(ret){
                //parent
                printf("gdb:%d,temp:%d\n",getpid(),ret);
                puts("***** MY GDB BASIC *****");
                puts("(n)ext line, (q)uit");
                rf =fopen("prog.i","r");
                if(!rf){
                        perror("files2");
                        exit(1);
                }
                flag=0;
                while(fgets(buf,SZ,rf)){
                        if(strstr(buf,"main(")){
                                fgets(buf,SZ,rf);
                                break;
                        }
                }
                int s=0;
                while(1){
                        printf(BGREEN"line-nn  :"RESET"%s",buf);
                        printf(BYELLOW"(my_gdb) : "RESET);
                        __fpurge(stdin);
                        ch=getchar();
                        ch|=32;//smalling
                        if(ch=='n'){
                                if(s){
                                        strcpy(buf,"Process terminated.\n");
                                        break;
                                }
                                //next
                                if(kill(ret,SIGUSR1)!=0){
                                        perror("kill-nxt");
                                }else{
                                        usleep(1000);
                                }
                                if(fgets(buf,SZ,rf)){
                                        if((strlen(buf)==2)&&(buf[0]='}')){
                                                s=1;
                                                strcpy(buf,"Reached main end\n");
                                        }
                                }
                        }else if(ch=='q'){
                                //quit
                                break;
                        }else{
                                puts("(n)ext line, (q)uit");
                        }
                }
                kill(ret,SIGKILL);
                fclose(rf);
                puts("Exiting my_gdb");
        }else{
                //child
                //mirror to ./temp.i
                system("gcc temp.i -o temp.exe");
                execl("./temp.exe","./temp.exe",NULL);
        }
