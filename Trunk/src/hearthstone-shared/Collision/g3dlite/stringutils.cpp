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
#include "platform.h"
#include "stringutils.h"
#include <algorithm>

namespace G3D {

#ifdef _MSC_VER
	// disable: "C++ exception handler used"
#   pragma warning (push)
#   pragma warning (disable : 4530)
#endif
#ifdef G3D_WIN32
	const char* NEWLINE = "\r\n";
#else
	const char* NEWLINE = "\n";
	static bool iswspace(int ch) { return (ch==' ' || ch=='\t' || ch=='\n' || ch=='\r'); }
#endif

void parseCommaSeparated(const std::string s, Array<std::string>& array, bool stripQuotes) {
	array.fastClear();
	if (s == "") {
		return;
	}

	size_t begin = 0;
	const char delimiter = ',';
	const char quote = '\"';
	do {
		size_t end = begin;
		// Find the next comma, or the end of the string
		bool inQuotes = false;
		while ((end < s.length()) && (inQuotes || (s[end] != delimiter))) {
			if (s[end] == quote) {
				if ((end < s.length() - 2) && (s[end + 1] == quote) && (s[end + 2]) == quote) {
					// Skip over the superquote
					end += 2;
				}
				inQuotes = ! inQuotes;
			}
			++end;
		}
		array.append(s.substr(begin, end - begin));
		begin = end + 1;
	} while (begin < s.length());

	if (stripQuotes) {
		for (int i = 0; i < array.length(); ++i) {
			std::string& t = array[i];
			int L = (int)t.length();
			if ((L > 1) && (t[0] == quote) && (t[L - 1] == quote)) {
				if ((L > 6)  && (t[1] == quote) && (t[2] == quote) && (t[L - 3] == quote) && (t[L - 2] == quote)) {
					// Triple-quote
					t = t.substr(3, L - 6);
				} else {
					// Double-quote
					t = t.substr(1, L - 2);
				}
			}
		}
	}
}

bool beginsWith(
	const std::string& test,
	const std::string& pattern) {

	if (test.size() >= pattern.size()) {
		for (int i = 0; i < (int)pattern.size(); ++i) {
			if (pattern[i] != test[i]) {
				return false;
			}
		}
		return true;
	} else {
		return false;
	}
}


bool endsWith(
	const std::string& test,
	const std::string& pattern) {

	if (test.size() >= pattern.size()) {
		int te = (int)test.size() - 1;
		int pe = (int)pattern.size() - 1;
		for (int i = (int)pattern.size() - 1; i >= 0; --i) {
			if (pattern[pe - i] != test[te - i]) {
				return false;
			}
		}
		return true;
	} else {
		return false;
	}
}


std::string wordWrap(
	const std::string&	  input,
	int					 numCols) {

	std::string output;
	size_t	  c = 0;
	int		 len;

	// Don't make lines less than this length
	int		 minLength = numCols / 4;
	size_t	  inLen = input.size();

	bool first = true;
	while (c < inLen) {
		if (first) {
			first = false;
		} else {
			output += NEWLINE;
		}

		if ((int)inLen - (int)c - 1 < numCols) {
			// The end
			output += input.substr(c, inLen - c);
			break;
		}

		len = numCols;

		// Look at character c + numCols, see if it is a space.
		while ((len > minLength) &&
			   (input[c + len] != ' ')) {
			len--;
		}

		if (len == minLength) {
			// Just crop
			len = numCols;

		}

		output += input.substr(c, len);
		c += len;
		if (c < input.size()) {
			// Collapse multiple spaces.
			while ((input[c] == ' ') && (c < input.size())) {
				c++;
			}
		}
	}

	return output;
}


int stringCompare(
	const std::string&	  s1,
	const std::string&	  s2) {

	return stringPtrCompare(&s1, &s2);
}


int stringPtrCompare(
	const std::string*	  s1,
	const std::string*	  s2) {

	return s1->compare(*s2);
}


std::string toUpper(const std::string& x) {
	std::string result = x;
	std::transform(result.begin(), result.end(), result.begin(), ::toupper);
	return result;
}


std::string toLower(const std::string& x) {
	std::string result = x;
	std::transform(result.begin(), result.end(), result.begin(), ::tolower);
	return result;
}


Array<std::string> stringSplit(
	const std::string&		  x,
	char						splitChar) {

	Array<std::string> out;
	
	// Pointers to the beginning and end of the substring
	const char* start = x.c_str();
	const char* stop = start;
	
	while ((stop = strchr(start, splitChar))) {
		out.append(std::string(start, stop - start));
		start = stop + 1;
	}

	// Append the last one
	out.append(std::string(start));
	
	return out;
}


std::string stringJoin(
	const Array<std::string>&   a,
	char						joinChar) {

	std::string out;

	for (int i = 0; i < (int)a.size() - 1; ++i) {
		out += a[i] + joinChar;
	}

	if (a.size() > 0) {
		return out + a.last();
	} else {
		return out;
	}
}


std::string stringJoin(
	const Array<std::string>&   a,
	const std::string&		  joinStr) {

	std::string out;

	for (int i = 0; i < (int)a.size() - 1; ++i) {
		out += a[i] + joinStr;
	}

	if (a.size() > 0) {
		return out + a.last();
	} else {
		return out;
	}
}


std::string trimWhitespace(
	const std::string&			  s) {

	size_t left = 0;
	
	// Trim from left
	while ((left < s.length()) && iswspace(s[left])) {
		++left;
	}

	int right = (int)s.length() - 1;
	// Trim from right
	while ((right > (int)left) && iswspace(s[right])) {
		--right;
	}
	
	return s.substr(left, right - left + 1);
}

}; // namespace

#undef NEWLINE
#ifdef _MSC_VER
#   pragma warning (pop)
#endif
