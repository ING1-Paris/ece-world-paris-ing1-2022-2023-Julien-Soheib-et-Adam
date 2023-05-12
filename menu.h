


void menu(void){
    char* name = malloc(30);
    int i = 0;
    while(!key[KEY_ENTER]){
        //*(name+i) = (char) readkey() && 0xFF;
        i++;
        printf("%i\n", readkey());
    }
    printf("%s\n",name);
    free(name);
}