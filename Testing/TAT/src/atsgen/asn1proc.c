#include "asn1proc.h"

int ParseASN1(char* s, char* d)
{
	int level=0;
	int closed=0;
	int brlevel=0;
	int quoted=0;
	char tmp[TAT_MAX_LINE_LEN];
	char* c;
	
	while(*s)
	{
		if((s[0]=='(') && (s[1]=='.'))							/* Start STRUCT */
		{
			if(closed) return 1;								/* Separator expected */
			strcpy(d,liststart);
			d+=strlen(liststart);
			strcpy(d,structstart);
			d+=strlen(structstart);
			level++;
			s+=2;
		}
		else if((s[0]=='(') && (s[1]==':'))						/* Start ARRAY */
		{
			if(closed) return 1;								/* Separator expected */
			strcpy(d,liststart);	
			d+=strlen(liststart);
			strcpy(d,arraystart);
			d+=strlen(arraystart);
			level++;
			s+=2;
		}
		else if(((s[0]==':') || (s[0]=='.')) && (s[1]==')'))	/* End STRUCT or ARRAY */
		{
			if(level<=0) return 1;
			strcpy(d,listend);
			d+=strlen(listend);
			level--;
			closed=1;
			s+=2;
		}
		else if(s[0]==',')										/* Separator */
		{
			if(level<=0) return 1;
			strcpy(d,listsep);
			d+=strlen(listsep);
			closed=0;
			s++;
		}
		else if((s[0]==' ') || (s[0]=='\t'))					/* Blank */
		{
			s++;
		}
		else													/* Any other char */
		{
			tmp[0]=0; c=tmp; brlevel=0; quoted=0;
			while(*s)
			{
				if(*s=='\"')									/* Quote */
					quoted=!quoted;
				else if(((*s=='(') || (*s=='[')) && !quoted)	/* Open bracket */
					brlevel++;
				else if(((*s==')') || (*s==']')) && !quoted)	/* Close bracket */
					brlevel--;
				else if(*s=='\\')								/* Backslash */
					s++;
				else if((*s==',') || (((s[0]=='.') || (s[0]==':')) && (s[1]==')')) || (*s==' ') || (*s=='\t'))
					if((brlevel==0) && !quoted) break;
				if(brlevel<0) return 1;
				*c++=*s++;
			}
			*c=0;
			d=StrEmbedLineQuotes(d,tmp);
		}
	}
	if(level>0) return 1;
	return 0;
}
