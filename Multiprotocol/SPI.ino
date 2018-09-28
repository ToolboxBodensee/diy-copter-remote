/*
 This project is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Multiprotocol is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Multiprotocol.  If not, see <http://www.gnu.org/licenses/>.
 */
/********************/
/**  SPI routines  **/
/********************/
void SPI_Write(uint8_t command)
{
	uint8_t n=8; 

	SCLK_off;//SCK start low
	SDI_off;
	do
	{
		if(command&0x80)
			SDI_on;
		else
			SDI_off;
		SCLK_on;
    //delay(1);
		command = command << 1;
		SCLK_off;
	}
	while(--n) ;
	SDI_on;
}

uint8_t SPI_Read(void)
{
	uint8_t result=0,i;
	for(i=0;i<8;i++)
	{
		result=result<<1;
		if(SDO_1)
			result |= 0x01;
		SCLK_on;
		NOP();
		SCLK_off;
	}
	return result;
}

