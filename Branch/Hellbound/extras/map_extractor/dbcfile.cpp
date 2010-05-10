#include "dbcfile.h"
#include "mpq_libmpq.h"

DBCFile::DBCFile(const std::string &filename):
	filename(filename),
	data(0)
{
	
}
void DBCFile::open()
{
	FILE*pf=fopen(filename.c_str(),"rb");
	char header[4];
	unsigned int na,nb,es,ss;

	fread(header,4,1,pf); // Number of records
	assert(header[0]=='W' && header[1]=='D' && header[2]=='B' && header[3] == 'C');
	fread(&na,4,1,pf); // Number of records
	fread(&nb,4,1,pf); // Number of fields
	fread(&es,4,1,pf); // Size of a record
	fread(&ss,4,1,pf); // String size

	recordSize = es;
	recordCount = na;
	fieldCount = nb;
	stringSize = ss;
	assert(fieldCount*4 == recordSize);

	data = new unsigned char[recordSize*recordCount+stringSize];
	stringTable = data + recordSize*recordCount;
	fread( data ,recordSize*recordCount+stringSize,1,pf);
	fclose(pf);
}
DBCFile::~DBCFile()
{
	delete [] data;
}

DBCFile::Record DBCFile::getRecord(size_t id)
{
	assert(data);
	return Record(*this, data + id*recordSize);
}

DBCFile::Iterator DBCFile::begin()
{
	assert(data);
	return Iterator(*this, data);
}
DBCFile::Iterator DBCFile::end()
{
	assert(data);
	return Iterator(*this, stringTable);
}

