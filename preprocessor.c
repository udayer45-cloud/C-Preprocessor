#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
typedef struct macro_info
{
	char mname[150];
	char mvalue[500];
	struct macro_info *next;
}macro;
macro *head=NULL;
void comment_removal(char **,int,int);
void header_include(char **,int,FILE *);
void macro_handle(FILE *,FILE *);
int main(int argc,char **argv)
{
	if(argc!=2){
		printf("USAGE: ./myPreprocessor inputfile\n");
		return 0;
	}
	FILE *fp=fopen(argv[1],"r");
	if(fp==NULL)
	{
		printf("input file open failed\n");
		return 0;
	}
	FILE *hfp=fopen("helpfile.i","w+");
	if(hfp==NULL)
	{
		printf("temp file open failed\n");
		return 0;
	}
	FILE *rfp=fopen("inputfile.i","w");
	if(rfp==NULL)
	{
		printf("output file open failed\n");
		return 0;
	}
	int nline=0,maxline=0,i=0,k=0;
	int ch;
	while((ch=fgetc(fp))!=EOF)
	{
		k++;
		if(ch=='\n')
		{
			nline++;
			if(maxline<k)
			{
				maxline=k;
			}
			k=0;
		}
	}
	char **p;
	p=malloc(sizeof(char *)*nline);

	for(i=0;i<nline;i++)
		p[i]=malloc(maxline+2);

	rewind(fp);

	for(i=0;i<nline;i++)
	{
		fgets(p[i],maxline+2,fp);
	}
	comment_removal(p,nline,maxline);  /// cmt removal call
	header_include(p,nline,hfp);  /// include header files
	fflush(hfp);
	rewind(hfp);
	macro_handle(hfp,rfp);  /// macro replacement
	for(i=0;i<nline;i++)
	{
		free(p[i]);
	}
	free(p);
	fclose(fp);
	fclose(hfp);
	fclose(rfp);
	remove("helpfile.i");
	return 0;
}
void macro_handle(FILE *hfp,FILE *fp) // macro replacement
{
	char line[1500]={0},ff=0,out[2048]={0};
	while(fgets(line,1500,hfp))
	{
		ff=0;
		if(strstr(line,"#define")) //storing the macro into a linked list
		{
			int i=0;
			while(line[i]==' '||line[i]=='\t') //ignoring the leading spaces or tab space
			{
				i++;
			}
			if(strncmp(&line[i],"#define ",8)==0) // if #define really present
			{
				macro *new=malloc(sizeof(macro));
				int j;
				for(j=0,i+=8;line[i] && line[i]!=' ' && line[i]!='\t';j++,i++) //macro name
				{
					new->mname[j]=line[i];
				}
				new->mname[j]='\0';
				i++;
				for(j=0;line[i] && line[i]!='\n';i++,j++) //macro body
				{
					new->mvalue[j]=line[i];
				}
				new->mvalue[j]='\0';
				new->next=NULL;
				if(head==NULL)
				{
					head=new;
				}
				else
				{
					macro *last=head;
					while(last->next)
					{
						last=last->next;
					}
					last->next=new;
				}
				ff=1;
			}
		}
		if(ff==0) //macro expansion
		{
			memset(out,0,2048);
			int i,fout,j,instring=0;
			macro *temp;
			for(i=0,j=0;line[i];) //that line
			{
				if(line[i]=='"')
				{
					instring=!instring;
					out[j++]=line[i++];
					continue;
				}
				temp=head;
				fout=0;
				if(!instring) //dont expand inside string literals
				{
					while(temp)
					{
						int len=strlen(temp->mname);
						if(strncmp(&line[i],temp->mname,len)==0) //macro name is there or not at that i index
						{
							char prev= (i==0) ? ' ': line[i-1];
							char next=line[i+len];
							if((!isalnum(prev)) && (prev!='_') && (!isalnum(next)) && (next!='_')) //real macro name
							{
								strcpy(&out[j],temp->mvalue);
								int slen=strlen(temp->mvalue);
								j+=slen;
								i+=len;
								fout=1;
								break;
							}
						}
						temp=temp->next;
					}
				}
				if(fout==0) //if macro not starts at i th index in line[]
				{
					out[j++]=line[i++];
				}
			}
			out[j]='\0';
			fputs(out,fp);
		}
	}
	//free linked list here after processing is completed
	macro *temp=head;
	while(temp)
	{
		macro *next=temp->next;
		free(temp);
		temp=next;
	}
	head=NULL; //reset global to NULL
}

void header_include(char **p,int nline,FILE *fp)  //header file inclusion
{
	char headu[100]={0};
	char headp[100]="/usr/include/";
	char fname[100];
	int l=strlen(headp);
	int i,j,k;
	char *s;
	for(i=0;i<nline;i++)
	{
		if(strstr(p[i],"#include"))
		{
			memset(fname,0,100);
			for(j=0;p[i][j];j++)
			{
				if(p[i][j]=='"'||p[i][j]=='<')
					break;
			}
			if(p[i][j]=='<')
			{
				j++;
				strcpy(headp,"/usr/include/");
				for(k=l;p[i][j]!='>';k++,j++)
				{
					headp[k]=p[i][j];
				}
				headp[k]='\0';
				strcpy(fname,headp);
			}
			else
			{
				j++;
				memset(headu,0,100);
				for(k=0;p[i][j]!='"';k++,j++)
				{
					headu[k]=p[i][j];
				}
				headu[k]='\0';
				strcpy(fname,headu);
			}
			FILE *hfp=fopen(fname,"r");
			if(hfp==NULL)
			{
				printf("header files not open\n");
				exit(0);
			}
			int ch;
			while((ch=fgetc(hfp))!=EOF)
			{
				fputc(ch,fp);
			}
			fclose(hfp);
		}
		else
		{
			fputs(p[i],fp);
		}
	}
}

void comment_removal(char **p,int n,int maxline)   /// comment removal definition
{
	int i,j,r=0,k=0,dc=0,mc=0;

	for(i=0;i<n;i++)
	{
		char *s=calloc(1,maxline+2);

		for(j=0,k=0;p[i][j];j++)
		{
			if(p[i][j]=='/'&&p[i][j+1]=='/'&&mc==0&&r==0)
			{
				s[k++]=' ';
				s[k++]='\n';
				break;
			}
			else if(p[i][j]=='"'&&mc==0)
			{
				if(r==1&&p[i][j-1]==92)
					dc--;

				dc++;
				r=dc%2;
			}
			else if(p[i][j]=='/'&&p[i][j+1]=='*'&&r==0)
			{
				mc=1;
			}
			else if(p[i][j]=='*'&&p[i][j+1]=='/'&&mc==1)
			{
				mc=0;
				s[k++]=' ';
				j++;
				continue;
			}

			if(mc==0)
				s[k++]=p[i][j];
		}

		if(mc==1)
		{
			s[k++]=' ';
			s[k]='\n';
		}

		strcpy(p[i],s);
		free(s);
		dc=0;
		r=0;
	}
}
