#include "ModbusRTUSlave.h"
#include "utility/LinkedList.h"


ModbusRTUSlave::ModbusRTUSlave(byte slaveAddress, HardwareSerial* serialport) 
{
  slave = slaveAddress;
  ser = serialport;
  
  words = new LinkedList<ModbusRTUSlaveWordAddress*>(); 
  bits = new LinkedList<ModbusRTUSlaveBitAddress*>(); 
}

void ModbusRTUSlave::begin(int baudrate) 
{
	ser->begin(baudrate);
	ResCnt=0;
}

boolean ModbusRTUSlave::addWordArea(u16 Address, u16* values, int cnt)
{
	if(getWordAddress(Address)==NULL)
	{
		words->add(new ModbusRTUSlaveWordAddress(Address, values, cnt));
		return true;
	}
	return false;
}

boolean ModbusRTUSlave::addBitArea(u16 Address, u8* values, int cnt)
{
	if(getBitAddress(Address)==NULL)
	{
		bits->add(new ModbusRTUSlaveBitAddress(Address, values, cnt));
		return true;
	}
	return false;
}

ModbusRTUSlaveWordAddress* ModbusRTUSlave::getWordAddress(u16 Addr)
{
	ModbusRTUSlaveWordAddress* ret=NULL;
	for(int i = 0; i < words->size(); i++)
	{
		ModbusRTUSlaveWordAddress* a = words->get(i);
		if(a!=NULL && Addr >= a->addr && Addr < a->addr + a->len) ret=a;
	}
	return ret;
}
ModbusRTUSlaveBitAddress* ModbusRTUSlave::getBitAddress(u16 Addr)
{
	ModbusRTUSlaveBitAddress* ret=NULL;
	for(int i = 0; i < bits->size(); i++)
	{
		ModbusRTUSlaveBitAddress* a = bits->get(i);
		if(a!=NULL && Addr >= a->addr && Addr < a->addr + (a->len*8)) ret=a;
	}
	return ret;
}

ModbusRTUSlaveWordAddress* ModbusRTUSlave::getWordAddress(u16 Addr, u16 Len)
{
	ModbusRTUSlaveWordAddress* ret=NULL;
	for(int i = 0; i < words->size(); i++)
	{
		ModbusRTUSlaveWordAddress* a = words->get(i);
		if(a!=NULL && Addr >= a->addr && Addr+Len <= a->addr + a->len) ret=a;
	}
	return ret;
}
ModbusRTUSlaveBitAddress* ModbusRTUSlave::getBitAddress(u16 Addr, u16 Len)
{
	ModbusRTUSlaveBitAddress* ret=NULL;
	for(int i = 0; i < bits->size(); i++)
	{
		ModbusRTUSlaveBitAddress* a = bits->get(i);
		if(a!=NULL && Addr >= a->addr && Addr+Len <= a->addr + (a->len*8)) ret=a;
	}
	return ret;
}


void ModbusRTUSlave::process()
{
	bool bvalid = true;
	while(ser->available()) 
	{
		byte d = ser->read();
		lstResponse[ResCnt++]=d;
		if(ResCnt>=4)
		{
			byte Slave = lstResponse[0];
			if(Slave == slave)
			{
				byte Function = lstResponse[1];
	            u16 Address = (lstResponse[2] << 8) | lstResponse[3];
				switch(Function)
				{
					case 1:		//BitRead
					case 2:
						if(ResCnt >= 8)
						{
							u16 Length = (lstResponse[4] << 8) | lstResponse[5];
							byte hi = 0xFF, lo = 0xFF;
							getCRC(lstResponse,300, 0, 6, &hi, &lo);
							ModbusRTUSlaveBitAddress *a = getBitAddress(Address, Length);
							if (Length > 0 && a != NULL && lstResponse[6] == hi && lstResponse[7] == lo)
							{
								u16 stidx = (Address - a->addr) / 8;
								u16 nlen = ((Length-1) / 8)+1;

								byte dat[nlen];
								memset(dat,0,nlen);

								int ng=(Address - a->addr) % 8;
								int ns=stidx;
								for(int i=0;i<nlen;i++)
								{
									byte val=0;
									for(int j=0;j<8;j++)
									{
										if(bitRead(a->values[ns], ng++)) bitSet(val,j);
										if(ng==8){ns++;ng=0;}
									}
									dat[i]=val;
								}
								
								byte ret[3+nlen+2];
								ret[0]=Slave;	ret[1]=Function;	ret[2]=nlen;
								for(int i=0;i<nlen;i++) ret[3+i]=dat[i];
  							    byte hi = 0xFF, lo = 0xFF;
								getCRC(ret, 3+nlen+2, 0, 3+nlen, &hi, &lo);
								ret[3+nlen]=hi;
								ret[3+nlen+1]=lo;
								ser->write(ret, 3+nlen+2);

								ResCnt=0;
							}
							else bvalid = false;
						}
						break;
					case 3:		//WordRead	
					case 4:
						if(ResCnt >= 8)
						{
							u16 Length = (lstResponse[4] << 8) | lstResponse[5];
							byte hi = 0xFF, lo = 0xFF;
							getCRC(lstResponse,300, 0, 6, &hi, &lo);
							ModbusRTUSlaveWordAddress *a = getWordAddress(Address, Length);
							if (Length > 0 && a != NULL && lstResponse[6] == hi && lstResponse[7] == lo)
							{
								u16 stidx = Address - a->addr;
								u16 nlen = Length * 2;

								byte ret[3+nlen+2];
								ret[0]=Slave;	ret[1]=Function;	ret[2]=nlen;
								for(int i=stidx;i<stidx+Length;i++)
								{
									ret[3+((i-stidx)*2)+0]=((a->values[i] & 0xFF00) >> 8);
									ret[3+((i-stidx)*2)+1]=((a->values[i] & 0xFF));
								}
  							    byte hi = 0xFF, lo = 0xFF;
								getCRC(ret, 3+nlen+2, 0, 3+nlen, &hi, &lo);
								ret[3+nlen]=hi;
								ret[3+nlen+1]=lo;
								ser->write(ret, 3+nlen+2);

								ResCnt=0;
							}
							else bvalid = false;
						}
						break;
					case 5:		//BitWrite
						if(ResCnt >= 8)
						{
							u16 Data = (lstResponse[4] << 8) | lstResponse[5];
							byte hi = 0xFF, lo = 0xFF;
							getCRC(lstResponse,300, 0, 6, &hi, &lo);
							ModbusRTUSlaveBitAddress *a = getBitAddress(Address);
							if (a != NULL && lstResponse[6] == hi && lstResponse[7] == lo)
							{
								u16 stidx = (Address - a->addr) / 8;

								bitWrite(a->values[stidx], (Address - a->addr)%8, Data==0xFF00);

								byte ret[8];
								ret[0]=Slave;	
								ret[1]=Function;	
								ret[2]=((Address&0xFF00)>>8);
								ret[3]=((Address&0x00FF));
								ret[4]=((Data&0xFF00)>>8);
								ret[5]=((Data&0x00FF));
  							    byte hi = 0xFF, lo = 0xFF;
								getCRC(ret, 8, 0, 6, &hi, &lo);
								ret[6]=hi;
								ret[7]=lo;
								ser->write(ret, 8);
								
								ResCnt=0;
							}
							else bvalid = false;
						}
						break;
					case 6:		//WordWrite
						if(ResCnt >= 8)
						{
							u16 Data = (lstResponse[4] << 8) | lstResponse[5];
							byte hi = 0xFF, lo = 0xFF;
							getCRC(lstResponse,300, 0, 6, &hi, &lo);
							ModbusRTUSlaveWordAddress *a = getWordAddress(Address);
							if (a != NULL && lstResponse[6] == hi && lstResponse[7] == lo)
							{
								u16 stidx = Address - a->addr;

								a->values[stidx] = Data;

								byte ret[8];
								ret[0]=Slave;	
								ret[1]=Function;	
								ret[2]=((Address&0xFF00)>>8);
								ret[3]=((Address&0x00FF));
								ret[4]=((Data&0xFF00)>>8);
								ret[5]=((Data&0x00FF));
  							    byte hi = 0xFF, lo = 0xFF;
								getCRC(ret, 8, 0, 6, &hi, &lo);
								ret[6]=hi;
								ret[7]=lo;
								ser->write(ret, 8);

								ResCnt=0;
							}
							else bvalid = false;
						}
						break;
					case 15:	//MultiBitWrite
						if(ResCnt >= 7)
						{
							u16 Length = (lstResponse[4] << 8) | lstResponse[5];
							u8 ByteCount = lstResponse[6];
							if(ResCnt >= 9+ByteCount)
							{
								byte hi = 0xFF, lo = 0xFF;
								getCRC(lstResponse,300, 0, 7 + ByteCount, &hi, &lo);
								if(lstResponse[(9 + ByteCount - 2)] == hi && lstResponse[(9 + ByteCount - 1)] == lo)
								{
									ModbusRTUSlaveBitAddress *a = getBitAddress(Address, Length);
									if (a != NULL) 
									{
										u16 stidx = (Address - a->addr) / 8;
										int ng=(Address - a->addr) % 8;
										int ns=stidx;

										for(int i=7; i<7+ByteCount;i++)
										{
											byte val = lstResponse[i];
											for(int j=0;j<8;j++)
											{
												bitWrite(a->values[ns], ng++, bitRead(val,j));
												if(ng==8){ns++;ng=0;}
											}
										}

										if(bvalid)
										{
											byte ret[8];
											ret[0]=Slave;	
											ret[1]=Function;	
											ret[2]=((Address&0xFF00)>>8);
											ret[3]=((Address&0x00FF));
											ret[4]=((Length&0xFF00)>>8);
											ret[5]=((Length&0x00FF));
											byte hi = 0xFF, lo = 0xFF;
											getCRC(ret, 8, 0, 6, &hi, &lo);
											ret[6]=hi;
											ret[7]=lo;
											ser->write(ret, 8);

											ResCnt=0;
										}
									}
								}
								else bvalid=false;
							}
						}
						break; 
					case 16:	//MultiWordWrite
						if(ResCnt >= 7)
						{
							u16 Length = (lstResponse[4] << 8) | lstResponse[5];
							u8 ByteCount = lstResponse[6];
							if(ResCnt >= 9+ByteCount)
							{
								byte hi = 0xFF, lo = 0xFF;
								getCRC(lstResponse,300, 0, 7 + ByteCount, &hi, &lo);
								if(lstResponse[(9 + ByteCount - 2)] == hi && lstResponse[(9 + ByteCount - 1)] == lo)
								{
									for(int i=7; i<7+ByteCount;i+=2)
									{
										u16 data = lstResponse[i] << 8 | lstResponse[i+1];
										ModbusRTUSlaveWordAddress *a = getWordAddress(Address + ((i-7)/2));
										if (a != NULL) { a->values[(Address + ((i-7)/2)) - a->addr] = data;	}
										else { bvalid=false; break; }
									}
									if(bvalid)
									{
										byte ret[8];
										ret[0]=Slave;	
										ret[1]=Function;	
										ret[2]=((Address&0xFF00)>>8);
										ret[3]=((Address&0x00FF));
										ret[4]=((Length&0xFF00)>>8);
										ret[5]=((Length&0x00FF));
										byte hi = 0xFF, lo = 0xFF;
										getCRC(ret, 8, 0, 6, &hi, &lo);
										ret[6]=hi;
										ret[7]=lo;
										ser->write(ret, 8);

										ResCnt=0;
									}
								}
								else bvalid=false;
							}
						}
						break;
				}
			}
			else bvalid = false;
		}
		lastrecv = millis();
	}
	if(!bvalid && ResCnt>0) ResCnt=0;
	if(ResCnt>0 && (millis()-lastrecv > 200 || millis() < lastrecv)) ResCnt=0;
}
/*
void ModbusRTUSlave::getCRC(LinkedList<byte>* pby, int startindex, int nSize, byte* byFirstReturn, byte* bySecondReturn)
{
	int uIndex;
	byte uchCRCHi = 0xff;
	byte uchCRCLo = 0xff;
	for (int i = startindex; i < startindex + nSize && i<pby->size(); i++)
	{
		uIndex = uchCRCHi ^ pby->get(i);
		uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex];
		uchCRCLo = auchCRCLo[uIndex];
	}
	(*byFirstReturn) = uchCRCHi;
	(*bySecondReturn) = uchCRCLo;
}
*/
void ModbusRTUSlave::getCRC(byte* pby, int arsize, int startindex, int nSize, byte* byFirstReturn, byte* bySecondReturn)
{
	int uIndex;
	byte uchCRCHi = 0xff;
	byte uchCRCLo = 0xff;
	for (int i = startindex; i < startindex + nSize && i<arsize; i++)
	{
		uIndex = uchCRCHi ^ pby[i];
		uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex];
		uchCRCLo = auchCRCLo[uIndex];
	}
	(*byFirstReturn) = uchCRCHi;
	(*bySecondReturn) = uchCRCLo;
}

boolean getBit(u8* area, int index)
{
	u16 stidx = index / 8;
	return bitRead(area[stidx], index%8);
}

void setBit(u8* area, int index, bool value)
{
	u16 stidx = index / 8;
	bitWrite(area[stidx], index%8, value);
}

ModbusRTUSlaveBitAddress::ModbusRTUSlaveBitAddress(u16 Address, u8* value, int cnt)
{
	addr = Address;
	values = value;
	len = cnt;
}

ModbusRTUSlaveWordAddress::ModbusRTUSlaveWordAddress(u16 Address, u16* value, int cnt)
{
	addr = Address;
	values = value;
	len = cnt;
}
