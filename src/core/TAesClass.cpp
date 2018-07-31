#include "TAesClass.h"

Aes::~Aes()
{

}
Aes::Aes()
{

}


Aes::Aes(int keysize,unsigned char* keyBytes)
{
    SetNbNkNr(keysize);
    memcpy(key,keyBytes,keysize);
    KeyExpansion();
}

void Aes::SetNbNkNr(int keySize)
{
    Nb=4;
    if(keySize==Bits128)
    {
        Nk=4;
        Nr=10;
    }
    else if(keySize==Bits192)
    {
        Nk=6;
        Nr=12;
    }
    else if(keySize==Bits256)
    {
        Nk=8;
        Nr=14;
    }
}

void Aes::KeyExpansion()
{

    memset(w,0,16*15);
    for(int row=0;row<Nk;row++)
    {
        w[4*row+0] =  key[4*row];
        w[4*row+1] =  key[4*row+1];
        w[4*row+2] =  key[4*row+2];
        w[4*row+3] =  key[4*row+3];
    }
    byte* temp = new byte[4];
    for(int row=Nk;row<4*(Nr+1);row++)
    {
        temp[0]=w[4*row-4];
        temp[1]=w[4*row-3];
        temp[2]=w[4*row-2];
        temp[3]=w[4*row-1];
        if(row%Nk==0)
        {
            temp=SubWord(RotWord(temp));
            temp[0] = (byte)( (int)temp[0] ^ (int) AesRcon[4*(row/Nk)+0] );
            temp[1] = (byte)( (int)temp[1] ^ (int) AesRcon[4*(row/Nk)+1] );
            temp[2] = (byte)( (int)temp[2] ^ (int) AesRcon[4*(row/Nk)+2] );
            temp[3] = (byte)( (int)temp[3] ^ (int) AesRcon[4*(row/Nk)+3] );
        }
        else if ( Nk > 6 && (row % Nk == 4) )
        {
            temp = SubWord(temp);
        }

        w[4*row+0] = (byte) ( (int) w[4*(row-Nk)+0] ^ (int)temp[0] );
        w[4*row+1] = (byte) ( (int) w[4*(row-Nk)+1] ^ (int)temp[1] );
        w[4*row+2] = (byte) ( (int) w[4*(row-Nk)+2] ^ (int)temp[2] );
        w[4*row+3] = (byte) ( (int) w[4*(row-Nk)+3] ^ (int)temp[3] );
    }

}


unsigned char* Aes::RotWord(unsigned char* word)
{
    byte* temp = new byte[4];
    temp[0] = word[1];
    temp[1] = word[2];
    temp[2] = word[3];
    temp[3] = word[0];
    return temp;
}


unsigned char* Aes::SubWord(unsigned char* word)
{
    byte* temp = new byte[4];
    for(int j=0;j<4;j++)
    {
        temp[j] = AesSbox[16*(word[j] >> 4)+(word[j] & 0x0f)];
    }
    return temp;

}






void Aes::InvCipher(unsigned char* input,unsigned char* output)
{
    memset(&State[0][0],0,16);
    for (int i = 0; i < (4 * Nb); i++)
    {
        State[i % 4][ i / 4] = input[i];
    }

    AddRoundKey(Nr);

    for (int round = Nr-1; round >= 1; round--)
    {
        InvShiftRows();
        InvSubBytes();
        AddRoundKey(round);
        InvMixColumns();
    }

    InvShiftRows();
    InvSubBytes();
    AddRoundKey(0);


    for (int i = 0; i < (4 * Nb); i++)
    {
        output[i] =  State[i % 4][ i / 4];
    }
}

void Aes::AddRoundKey(int round)
{
    int i,j;
    for(j=0;j<4;j++)
    {
        for(i=0;i<4;i++)
        {
            State[i][j]=(unsigned char)((int)State[i][j]^(int)w[4*((round*4)+j)+i]);
        }
    }
}



void Aes::InvSubBytes()
{
    int i,j;
    for(j=0;j<4;j++)
    {
        for(i=0;i<4;i++)
        {
            State[i][j]=AesiSbox[State[i][j]];
        }
    }

}

void Aes::InvShiftRows()
{
    unsigned char temp[4*4];
    int i,j;
    for(j=0;j<4;j++)
    {
        for(i=0;i<4;i++)
        {
            temp[4*i+j]=State[i][j];
        }
    }
    for(i=1;i<4;i++)
    {
        for(j=0;j<4;j++)
        {
            if(i==1)State[i][j]=temp[4*i+(j+3)%4];
            else if(i==2)State[i][j]=temp[4*i+(j+2)%4];
            else if(i==3)State[i][j]=temp[4*i+(j+1)%4];
        }
    }

}


void Aes::InvMixColumns()
{
    unsigned char temp[4*4];
    int i,j;
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            temp[4*i+j] =  State[i][j];
        }
    }

    for (j = 0; j < 4; j++)
    {
        State[0][j] = (unsigned char) ( (int)gfmultby0e(temp[j]) ^ (int)gfmultby0b(temp[4+j]) ^
                                        (int)gfmultby0d(temp[4*2+j]) ^ (int)gfmultby09(temp[4*3+j]) );
        State[1][j] = (unsigned char) ( (int)gfmultby09(temp[j]) ^ (int)gfmultby0e(temp[4+j]) ^
                                        (int)gfmultby0b(temp[4*2+j]) ^ (int)gfmultby0d(temp[4*3+j]) );
        State[2][j] = (unsigned char) ( (int)gfmultby0d(temp[j]) ^ (int)gfmultby09(temp[4+j]) ^
                                        (int)gfmultby0e(temp[4*2+j]) ^ (int)gfmultby0b(temp[4*3+j]) );
        State[3][j] = (unsigned char) ( (int)gfmultby0b(temp[j]) ^ (int)gfmultby0d(temp[4+j]) ^
                                        (int)gfmultby09(temp[4*2+j]) ^ (int)gfmultby0e(temp[4*3+j]) );
    }
}

unsigned char Aes::gfmultby01(unsigned char b)
{
    return b;
}
unsigned char Aes::gfmultby02(unsigned char b)
{
    if (b < 0x80)
        return (unsigned char)(int)(b <<1);
    else
        return (unsigned char)( (int)(b << 1) ^ (int)(0x1b) );
}

unsigned char Aes::gfmultby03(unsigned char b)
{
    return (unsigned char) ( (int)gfmultby02(b) ^ (int)b );
}

unsigned char Aes::gfmultby09(unsigned char b)
{
    return (unsigned char)( (int)gfmultby02(gfmultby02(gfmultby02(b))) ^ (int)b );
}

unsigned char Aes::gfmultby0b(unsigned char b)
{
    return (unsigned char)( (int)gfmultby02(gfmultby02(gfmultby02(b))) ^
                            (int)gfmultby02(b) ^ (int)b );
}

unsigned char Aes::gfmultby0d(unsigned char b)
{
    return (unsigned char)( (int)gfmultby02(gfmultby02(gfmultby02(b))) ^
                            (int)gfmultby02(gfmultby02(b)) ^ (int)(b) );
}

unsigned char Aes::gfmultby0e(unsigned char b)
{
    return (unsigned char)( (int)gfmultby02(gfmultby02(gfmultby02(b))) ^
                            (int)gfmultby02(gfmultby02(b)) ^(int)gfmultby02(b) );
}

TAesClass::TAesClass()
{
    m_lpAes=NULL;
    InitializePrivateKey(16,(unsigned char*)"\x79\x76\x68\x6B\x77\x66\x6E\x68\x72\x65\x73\x63\x6C\x6B\x70\x6E");
}

TAesClass::~TAesClass()
{
    if (m_lpAes!=NULL)
    {
        delete m_lpAes;
    }
}


VOID TAesClass::InitializePrivateKey(DWORD KeySize,UCHAR *KeyBytes)
{
    if (m_lpAes)
    {
        delete m_lpAes;
        m_lpAes=NULL;
    }
    m_lpAes=new Aes(KeySize,KeyBytes);

}


DWORD TAesClass::OnAesUncrypt(LPVOID InBuffer,DWORD InLength,LPVOID OutBuffer)
{
    DWORD OutLength=0;
    if (m_lpAes==NULL||OutBuffer==NULL)
    {
        return 0;
    }
    UCHAR *lpCurInBuff=(UCHAR *)InBuffer;
    UCHAR *lpCurOutBuff=(UCHAR *)OutBuffer;
    long blocknum=InLength/16;
    long leftnum=InLength%16;
    if(leftnum)
    {
        return 0;
    }
    for(long i=0;i<blocknum;i++)
    {
        m_lpAes->InvCipher(lpCurInBuff,lpCurOutBuff);
        lpCurInBuff+=16;
        lpCurOutBuff+=16;
        OutLength+=16;
    }

    UCHAR *lpExtraInBuff=lpCurOutBuff-16;
    DWORD  dwExtraBytes=*((LPDWORD)lpExtraInBuff);
    return (OutLength-dwExtraBytes);

}
