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
/************************************/
/************************************/
/**  Arduino replacement routines  **/
/************************************/
// replacement map()
int16_t map16b( int16_t x, int16_t in_min, int16_t in_max, int16_t out_min, int16_t out_max)
{
//  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  long y ;
  x -= in_min ;
  y = out_max - out_min ;
  y *= x ;
  x = y / (in_max - in_min) ;
  return x  + out_min ;
}
#define delayMilliseconds(x) delay(x)
