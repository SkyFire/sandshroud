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

#ifndef _RSA2_H
#define _RSA2_H

class RSA2
{
public:
	RSA2() {};
	~RSA2() {};

	char * ToBin(int x);
	uint32 Crypt(int key,int x,int n);

	int Bin2Num(char *s);
	char * Num2Bin(int n);

	char* KeyEnc(const char* fin, int e, int n);
	uint32* KeyEnc(uint32* fin, int e, int n);
	uint32* KeyDec(uint32* fin, int d, int n);
};

#endif
