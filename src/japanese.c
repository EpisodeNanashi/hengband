/* File: japanese.c */


#include "angband.h"

#ifdef JP

/*���ܸ�ư����� (�Ǥġ��Ǥä�,�Ǥ� etc) */

#define CMPTAIL(y) strncmp(&in[l-strlen(y)],y,strlen(y))

/* ����,����䲥��,���� */
void jverb1( const char *in , char *out){
int l=strlen(in);
strcpy(out,in);

if( CMPTAIL("����")==0) sprintf(&out[l-4],"��");else
if( CMPTAIL("����")==0) sprintf(&out[l-4],"����");else

if( CMPTAIL("����")==0) sprintf(&out[l-4],"��");else
if( CMPTAIL("����")==0) sprintf(&out[l-4],"��");else
if( CMPTAIL("����")==0) sprintf(&out[l-4],"��");else
if( CMPTAIL("����")==0) sprintf(&out[l-4],"��");else
if( CMPTAIL("����")==0) sprintf(&out[l-4],"��");else
if( CMPTAIL("�Ƥ�")==0) sprintf(&out[l-4],"��");else
if( CMPTAIL("�Ǥ�")==0) sprintf(&out[l-4],"��");else
if( CMPTAIL("�ͤ�")==0) sprintf(&out[l-4],"��");else
if( CMPTAIL("�ؤ�")==0) sprintf(&out[l-4],"��");else
if( CMPTAIL("�٤�")==0) sprintf(&out[l-4],"��");else
if( CMPTAIL("���")==0) sprintf(&out[l-4],"��");else
if( CMPTAIL("���")==0) sprintf(&out[l-4],"��");else

if( CMPTAIL("��")==0) sprintf(&out[l-2],"��");else
if( CMPTAIL("��")==0) sprintf(&out[l-2],"��");else
if( CMPTAIL("��")==0) sprintf(&out[l-2],"��");else
if( CMPTAIL("��")==0) sprintf(&out[l-2],"��");else
if( CMPTAIL("��")==0) sprintf(&out[l-2],"��");else
if( CMPTAIL("��")==0) sprintf(&out[l-2],"��");else
if( CMPTAIL("��")==0) sprintf(&out[l-2],"��");else
if( CMPTAIL("��")==0) sprintf(&out[l-2],"��");else
if( CMPTAIL("��")==0) sprintf(&out[l-2],"��");else
if( CMPTAIL("��")==0) sprintf(&out[l-2],"��");else
if( CMPTAIL("��")==0) sprintf(&out[l-2],"��");else
if( CMPTAIL("��")==0) sprintf(&out[l-2],"��");else

  sprintf(&out[l],"������");}

/* ����,����> ���äƽ��� */
void jverb2( const char *in , char *out){
int l=strlen(in);
strcpy(out,in);

if( CMPTAIL("����")==0) sprintf(&out[l-4],"����");else
if( CMPTAIL("����")==0) sprintf(&out[l-4],"����");else

if( CMPTAIL("����")==0) sprintf(&out[l-4],"����");else
if( CMPTAIL("����")==0) sprintf(&out[l-4],"����");else
if( CMPTAIL("����")==0) sprintf(&out[l-4],"����");else
if( CMPTAIL("����")==0) sprintf(&out[l-4],"����");else
if( CMPTAIL("����")==0) sprintf(&out[l-4],"����");else
if( CMPTAIL("�Ƥ�")==0) sprintf(&out[l-4],"�Ƥä�");else
if( CMPTAIL("�Ǥ�")==0) sprintf(&out[l-4],"�Ǥ�");else
if( CMPTAIL("�ͤ�")==0) sprintf(&out[l-4],"�ͤ�");else
if( CMPTAIL("�ؤ�")==0) sprintf(&out[l-4],"�ؤ�");else
if( CMPTAIL("�٤�")==0) sprintf(&out[l-4],"�٤�");else
if( CMPTAIL("���")==0) sprintf(&out[l-4],"���");else
if( CMPTAIL("���")==0) sprintf(&out[l-4],"���");else

if( CMPTAIL("��")==0) sprintf(&out[l-2],"�ä�");else
if( CMPTAIL("��")==0) sprintf(&out[l-2],"����");else
if( CMPTAIL("��")==0) sprintf(&out[l-2],"����");else
if( CMPTAIL("��")==0) sprintf(&out[l-2],"����");else
if( CMPTAIL("��")==0) sprintf(&out[l-2],"����");else
if( CMPTAIL("��")==0) sprintf(&out[l-2],"�ä�");else
if( CMPTAIL("��")==0) sprintf(&out[l-2],"�ä�");else
if( CMPTAIL("��")==0) sprintf(&out[l-2],"�ͤ�");else
if( CMPTAIL("��")==0) sprintf(&out[l-2],"�ؤ�");else
if( CMPTAIL("��")==0) sprintf(&out[l-2],"���");else
if( CMPTAIL("��")==0) sprintf(&out[l-2],"���");else
if( CMPTAIL("��")==0) sprintf(&out[l-2],"�ä�");else
  sprintf(&out[l],"���Ȥˤ��");}

/* ����,���� > ���ä��꽳�ä��� */
void jverb3( const char *in , char *out){
int l=strlen(in);
strcpy(out,in);

if( CMPTAIL("����")==0) sprintf(&out[l-4],"����");else
if( CMPTAIL("����")==0) sprintf(&out[l-4],"����");else

if( CMPTAIL("����")==0) sprintf(&out[l-4],"����");else
if( CMPTAIL("����")==0) sprintf(&out[l-4],"����");else
if( CMPTAIL("����")==0) sprintf(&out[l-4],"����");else
if( CMPTAIL("����")==0) sprintf(&out[l-4],"����");else
if( CMPTAIL("����")==0) sprintf(&out[l-4],"����");else
if( CMPTAIL("�Ƥ�")==0) sprintf(&out[l-4],"�Ƥä�");else
if( CMPTAIL("�Ǥ�")==0) sprintf(&out[l-4],"�Ǥ�");else
if( CMPTAIL("�ͤ�")==0) sprintf(&out[l-4],"�ͤ�");else
if( CMPTAIL("�ؤ�")==0) sprintf(&out[l-4],"�ؤ�");else
if( CMPTAIL("�٤�")==0) sprintf(&out[l-4],"�٤�");else
if( CMPTAIL("���")==0) sprintf(&out[l-4],"�᤿");else
if( CMPTAIL("���")==0) sprintf(&out[l-4],"�줿");else

if( CMPTAIL("��")==0) sprintf(&out[l-2],"�ä�");else
if( CMPTAIL("��")==0) sprintf(&out[l-2],"����");else
if( CMPTAIL("��")==0) sprintf(&out[l-2],"����");else
if( CMPTAIL("��")==0) sprintf(&out[l-2],"����");else
if( CMPTAIL("��")==0) sprintf(&out[l-2],"����");else
if( CMPTAIL("��")==0) sprintf(&out[l-2],"�ä�");else
if( CMPTAIL("��")==0) sprintf(&out[l-2],"�ä�");else
if( CMPTAIL("��")==0) sprintf(&out[l-2],"�ͤ�");else
if( CMPTAIL("��")==0) sprintf(&out[l-2],"�ؤ�");else
if( CMPTAIL("��")==0) sprintf(&out[l-2],"���");else
if( CMPTAIL("��")==0) sprintf(&out[l-2],"���");else
if( CMPTAIL("��")==0) sprintf(&out[l-2],"�ä�");else
  sprintf(&out[l],"���Ȥ�");}


void jverb( const char *in , char *out , int flag){
  switch (flag){
  case JVERB_AND:jverb1(in , out);break;
  case JVERB_TO :jverb2(in , out);break;
  case JVERB_OR :jverb3(in , out);break;
  }
}

char* strstr_j(const char* s, const char* t)
{
	int i, l1, l2;

	l1 = strlen(s);
	l2 = strlen(t);
	if (l1 >= l2) {
		for(i = 0; i <= l1 - l2; i++) {
			if(!strncmp(s + i, t, l2))
				return (char *)s + i;
			if (iskanji(*(s + i)))
				i++;
		}
	}
	return NULL;
}


/*
 * Convert SJIS string to EUC string
 */
void sjis2euc(char *str)
{
	int i;
	unsigned char c1, c2;
	unsigned char *tmp;

	int len = strlen(str);

	C_MAKE(tmp, len+1, byte);

	for (i = 0; i < len; i++)
	{
		c1 = str[i];
		if (c1 & 0x80)
		{
			i++;
			c2 = str[i];
			if (c2 >= 0x9f)
			{
				c1 = c1 * 2 - (c1 >= 0xe0 ? 0xe0 : 0x60);
				c2 += 2;
			}
			else
			{
				c1 = c1 * 2 - (c1 >= 0xe0 ? 0xe1 : 0x61);
				c2 += 0x60 + (c2 < 0x7f);
			}
			tmp[i - 1] = c1;
			tmp[i] = c2;
		}
		else
			tmp[i] = c1;
	}
	tmp[len] = 0;
	strcpy(str, (char *)tmp);

	C_KILL(tmp, len+1, byte);
}  


/*
 * Convert EUC string to SJIS string
 */
void euc2sjis(char *str)
{
	int i;
	unsigned char c1, c2;
	unsigned char *tmp;
	
	int len = strlen(str);

	C_MAKE(tmp, len+1, byte);

	for (i = 0; i < len; i++)
	{
		c1 = str[i];
		if (c1 & 0x80)
		{
			i++;
			c2 = str[i];
			if (c1 % 2)
			{
				c1 = (c1 >> 1) + (c1 < 0xdf ? 0x31 : 0x71);
				c2 -= 0x60 + (c2 < 0xe0);
			}
			else
			{
				c1 = (c1 >> 1) + (c1 < 0xdf ? 0x30 : 0x70);
				c2 -= 2;
			}

			tmp[i - 1] = c1;
			tmp[i] = c2;
		}
		else
			tmp[i] = c1;
	}
	tmp[len] = 0;
	strcpy(str, (char *)tmp);

	C_KILL(tmp, len+1, byte);
}  


/*
 * str��Ķ��˹�ä�ʸ�������ɤ��Ѵ����롣
 * str��Ĺ�������¤Ϥʤ���
 */
bool codeconv(char *str)
{
	int i;
	int kanji = 0, iseuc = 1;

	/* ������¸�ߤ������δ��������ɤ�EUC���ɤ���Ĵ�٤롣*/
	for (i = 0; str[i]; i++)
	{
		unsigned char c1 = str[i];

		if (c1 & 0x80)  kanji = 1;
		if ( c1>=0x80 && (c1 < 0xa1 || c1 > 0xfe)) iseuc = 0;
	}

#ifdef EUC
	if (kanji && !iseuc)	 /* SJIS -> EUC */
	{
		sjis2euc(str);

		return TRUE;
	}
#endif

#ifdef SJIS
	if (kanji && iseuc)	/* EUC -> SJIS */
	{
		euc2sjis(str);

		return TRUE;
	}
#endif

	return FALSE;
}

/* ʸ����s��x�Х����ܤ�������1�Х����ܤ��ɤ���Ƚ�ꤹ�� */
bool iskanji2(cptr s, int x)
{
	int i;

	for (i = 0; i < x; i++)
	{
		if (iskanji(s[i])) i++;
	}
	if ((x == i) && iskanji(s[x])) return TRUE;

	return FALSE;
}

#endif /* JP */

