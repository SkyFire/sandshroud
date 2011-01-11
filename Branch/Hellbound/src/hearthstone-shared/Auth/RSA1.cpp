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

char* RSA1::ToBin(int x)
{
	char *ans = new char[20];
	int i = 0;
	while(x >= 1)
	{
		if(x%2 == 1)
			ans[i] = '1';
		else
			ans[i] = '0';
		x /= 2;
		i++;
	}

	if(x == 1)
		ans[i] = '1', i++;

	ans[i] = '\0';
	strrev(ans);
	return (ans);
}

uint32 RSA1::Crypt(int x, int key, int n) // (x^y)%z //Method 2
{
	char* B = new char[20];
	B = ToBin(key);
	uint32 ans = 1;
	int c = 1;
	for(uint32 i = 0; i < strlen(B); i++)
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
