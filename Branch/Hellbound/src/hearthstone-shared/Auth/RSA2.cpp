/*
 * Sandshroud Hearthstone
 * Copyright (C) 2010 - 2011 Sandshroud <http://www.sandshroud.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "SharedStdAfx.h"

char* RSA2::ToBin(int x)
{
	char *ans = new char[20];
	int i = 0;
	while(x >= 1)
	{
		if(x%2 == 1)
			ans[i] = '1';
		else
			ans[i] = '0';

		x/=2;
		i++;
	}
	if(x == 1)
		ans[i] = '1', i++;
	ans[i] = '\0';
	strrev(ans);
	return(ans);
}

uint32 RSA2::Crypt(int key, int x, int n) // (x^y)%z //Method 2
{
	char *B = new char[20];

	int c = 1;
	B = ToBin(key);
	uint32 ans = 1;
	int32 size = int32(strlen(B));
	for(int i = 0; i < size; i++)
	{
		c = 2*c;
		ans = (ans*ans)%n;
		if(B[i] == '1')
		{
			c = c+1;
			ans = (ans*x)%n;
		}
	}
	return(ans);
}

int RSA2::Bin2Num(char *s)
{
	int ans = 0;
	int i, j;
	for(j = 0, i = (int)strlen(s); i >= 0; j++, i--)
	{
		if(s[i] == '1')
			ans += expon(2,j);
	}
	return ans;
}

char * RSA2::Num2Bin(int n)
{
	char *s = new char[20];
	int i;
	for(i = 0; i < 8; i++)
		s[i] = '0';
	i = 0;
	while(n > 1)
	{
		if(n%2 == 1)
			s[i] = '1';
		else
			s[i] = '0';
		n /= 2;
		i++;
	}
	if(n%2 == 1)
		s[i] = '1';
	else
		s[i] = '0';
	s[8] = '\0';
	strrev(s);
	return s;
}

char* RSA2::KeyEnc(const char* fin, int e, int n)
{
	uint32 finsize = (uint32)strlen(fin);
	char* str = new char[finsize+1];
	char* fout = new char[3000];
	uint32 i, X, Y, j, s = 0;
	for(j = 0; j < finsize; j++)
	{
		for(i = 0; i < finsize; i++)
		{
			str[i] = fin[i];
		}

		str[finsize+1] = '\0';
		X = Bin2Num(str);
		Y = Crypt(X, e, n);
		fout[s++] = Y;
	}
	return fout;
}

uint32* RSA2::KeyEnc(uint32* fin, int e, int n)
{
	char str[10];
	uint32* fout = new uint32;
	uint32 i, X, Y, j, s = 0;
	for(j = 0; j < 8; j++)
	{
		for(i = 0; i < 8; i++)
		{
			str[i] = fin[i];
		}

		str[8] = '\0';
		X = Bin2Num(str);
		Y = Crypt(X, e, n);
		fout[s++] = Y;
	}
	return fout;
}

uint32* RSA2::KeyDec(uint32* fin, int d, int n)
{
	int Ch;
	uint32 Y, s = 0;
	char *str = new char[20];
	uint32* fout = new uint32;
	uint32 fsize = (sizeof(fin)/sizeof(uint32));
	while(s < fsize)
	{
		Ch = fin[s++];
		Y = Crypt(Ch, d, n);
		str = Num2Bin(Y);
		fout[s] = atol(str);
	}
	return fout;
}
