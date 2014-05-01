#include  "device.h"
// return value is the number of printed characters.
int printDecimal(int decimal){
	int printedNum = 0,i;
	char number[20]; int index = 0;
	if(decimal < 0) {
		putchar('-');
		printedNum++;
		decimal *= -1;
	}
	do{
		number[index++] = decimal%10;
		decimal = decimal/10;
	}while(decimal);
	for(i =0; i < index; i++)
		number[i] += '0';
	
	printedNum += index;
	while(index)
		putchar(number[--index]);

	return printedNum;
}

int printHex(unsigned int x){

	int printedNum = 0,i;
	char number[15]; int index =0;
	putchar('0'); putchar('x');
	printedNum += 2;
	do{
		number[index++] = x%16;
		x = x/16;
	}while(x);
	for(i=0; i< index; i++)
	{ 
		if(number[i]<10)
			number[i] += '0';
		else
			number[i] += 'a' - 10;
	}
	printedNum += index;
	while(index)
		putchar(number[--index]);

	return printedNum;
}
int printf(const char* format, ...){
	unsigned long int paraStart = (unsigned long int)& format;
	int pos = 0, cnt_printed_chars =0;
 	int i ; char * str;// you should initialize them everytime you use them.
	int paraIndex = 1;
	const int WORD = sizeof(unsigned long int);
	while(format[pos] ){
		if(format[pos] == '%'){
			pos++;
			switch(format[pos]){
				case 'c':
	    //printf("\n %x \n", paraStart+paraIndex*WORD);
					putchar(*(char*)(paraStart+paraIndex*WORD)); paraIndex++; pos++; cnt_printed_chars++;  break;
				case 's':
	    //printf("\n %x \n", paraStart+paraIndex*WORD);
						 str = (char*)(*(unsigned long int*) (paraStart + paraIndex*WORD));
						 i   = 0;
						 while(str[i]){
							 cnt_printed_chars ++;
							 putchar(str[i]);
							 i++;
						 }
						 paraIndex++;
						 pos++;
						 break;
				case 'x':  
						 i = *(int *)(paraStart + paraIndex* WORD);
						 cnt_printed_chars += printHex((unsigned int)i);
						 paraIndex++;
						 pos++;
						 break;

				case 'd':  
						 i = *(int *)(paraStart + paraIndex* WORD);
						 cnt_printed_chars += printDecimal(i);
						 paraIndex++;
						 pos++;
						 break;

				default:putchar(format[pos]); pos++; cnt_printed_chars++;  break;// put the char directly

			}//end of switch 
		}else{// put char directly
			putchar(format[pos]);
			pos++;
			cnt_printed_chars++;

		}
	}//end of while 

	return cnt_printed_chars;
}
