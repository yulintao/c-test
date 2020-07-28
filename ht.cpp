// HashTest.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <unordered_map>
#include <time.h>

/* gcc -std=gnu++11 -DGET_TICK -o ht ht.cpp -lstdc++ */

class IpKey
{
public:
	int m_Key[3];
	size_t m_hash;
	IpKey() {};
	~IpKey() {};

	bool operator < (const IpKey & ipkey) const
	{
		int nRst = memcmp(&m_Key, &ipkey.m_Key, 12);
		return (nRst == -1) ? true : false;
	}


	static IpKey RandKey()
	{
		IpKey key;
		key.m_Key[0] = rand();
		key.m_Key[1] = rand();
		key.m_Key[2] = rand();
		key.m_hash = std::hash<int>()(key.m_Key[0]) ^ std::hash<int>()(key.m_Key[1]) ^ std::hash<int>()(key.m_Key[2]);
		return key;
	}

private:

};

struct IpKey_Compare {
	bool operator()(const IpKey & a, const IpKey & b) const {
		return (memcmp(&a.m_Key, &b.m_Key, 12) == 0);
	}
};

struct IpKey_hash {
	size_t operator()(const IpKey &x) const {
		//return std::hash<int>()(x.m_Key[0]) ^ std::hash<int>()(x.m_Key[1]) ^ std::hash<int>()(x.m_Key[2]);
		return x.m_hash;
	}
};

#define TEST_RAND_SEEK 6000*10000
#define USED_TIME(_nWaitTime,_Code) _nWaitTime=GetTickCount();_Code;_nWaitTime=GetTickCount()-_nWaitTime;

#ifdef GET_TICK
unsigned long GetTickCount()
{
	struct timespec ts;

	clock_gettime(CLOCK_MONOTONIC, &ts);

	return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}
#endif


void TestHashInsertSeek()
{
	unsigned long nWaitTime = 0;
	std::unordered_map<IpKey, int, IpKey_hash, IpKey_Compare> vec;
	srand(1234);
	USED_TIME(nWaitTime, {
		for (unsigned int i = 0; i < TEST_RAND_SEEK; i++)
		{
			IpKey::RandKey();
		}
	});



	printf("TestInsertSeek RandUseTime  nWaitTime=%d pps=%u kpps\r\n",  nWaitTime, TEST_RAND_SEEK / nWaitTime);

	unsigned int nTmpTime = nWaitTime;
	unsigned int nFind = 0;

	srand(1234);
	USED_TIME(nWaitTime, {
		for (unsigned int i = 0; i < TEST_RAND_SEEK; i++)
		{
			vec[IpKey::RandKey()] = i;
		}
	});

	printf("TestInsertSeek RandInsert UseTime  nWaitTime=%d pps=%u kpps\r\n",
		 nWaitTime, TEST_RAND_SEEK / (nWaitTime - nTmpTime));

	srand(1234);
	USED_TIME(nWaitTime, {
		for (unsigned int i = 0; i < TEST_RAND_SEEK; i++)
		{
			if (vec.find(IpKey::RandKey()) != vec.end())
			{
				nFind++;
			}
		}
	});

	printf("TestInsertSeek RandSeek UseTime  nFind=%u nWaitTime=%d pps=%u kpps \r\n", 
		nFind, nWaitTime, TEST_RAND_SEEK / (nWaitTime - nTmpTime));
}


int main()
{
	TestHashInsertSeek();
	for (;;)
	{
		sleep(100);
	}
  return 0;
}

