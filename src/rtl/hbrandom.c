/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * Random number generator routine
 *
 * Copyright 2011 Przemyslaw Czerpak <druzus / at / priv.onet.pl>
 * Copyright 2003 Giancarlo Niccolai <gian@niccolai.ws>
 * www - http://harbour-project.org
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307 USA (or visit the web site http://www.gnu.org/).
 *
 * As a special exception, the Harbour Project gives permission for
 * additional uses of the text contained in its release of Harbour.
 *
 * The exception is that, if you link the Harbour libraries with other
 * files to produce an executable, this does not by itself cause the
 * resulting executable to be covered by the GNU General Public License.
 * Your use of that executable is in no way restricted on account of
 * linking the Harbour library code into it.
 *
 * This exception does not however invalidate any other reasons why
 * the executable file might be covered by the GNU General Public License.
 *
 * This exception applies only to the code released by the Harbour
 * Project under the name Harbour.  If you copy code from other
 * Harbour Project or Free Software Foundation releases into a copy of
 * Harbour, as the General Public License permits, the exception does
 * not apply to the code that you add in this way.  To avoid misleading
 * anyone as to the status of such modified files, you must delete
 * this exception notice from them.
 *
 * If you write modifications of your own for Harbour, it is your choice
 * whether to permit this exception to apply to your modifications.
 * If you do not wish that, delete this exception notice.
 *
 */

#include "hbapi.h"
#include "hbdate.h"

/* NOTE: core random generator algorithm is the work of Steve Park
         http://www.cs.wm.edu/~va/software/park/
 */

#define MODULUS    2147483647 /* DON'T CHANGE THIS VALUE                  */
#define MULTIPLIER 48271      /* DON'T CHANGE THIS VALUE                  */
#define DEFAULT    123456789  /* initial seed, use 0 < DEFAULT < MODULUS  */
#define MAXIMUM    0xFFFFFFFF

static HB_BOOL s_fInit = HB_FALSE;
static HB_I32  s_seed  = DEFAULT;

/*
 * HB_RANDOM
 *
 * HB_RANDOM() --> returns a real value n so that 0 <= n < 1
 * HB_RANDOM( x ) --> returns a real number n so that 0 <= n < x
 * HB_RANDOM( x, y ) --> Returns a real number n so that x <= n < y
 */
HB_FUNC( HB_RANDOM )
{
   double dRnd = hb_random_num();

   if( ! HB_ISNUM( 1 ) )
      hb_retnd( dRnd );
   else if( ! HB_ISNUM( 2 ) )
      hb_retnd( dRnd * hb_parnd( 1 ) );
   else
   {
      double dX = hb_parnd( 2 );
      double dY = hb_parnd( 1 );
      if( dX > dY )
      {
         double dZ = dY;
         dY = dX;
         dX = dZ;
      }
      hb_retnd( dRnd * ( dY - dX ) + dX );
   }
}

/*
 * HB_RANDOMINT
 *
 * HB_RANDOMINT() --> returns 0 or 1, evenly distributed
 * HB_RANDOMINT( N ) --> returns an integer between 1 and N (inclusive)
 * HB_RANDOMINT( x, y ) --> Returns an integer number between x and y (inclusive)
 * The integer returned is of the longest type available
 */
HB_FUNC( HB_RANDOMINT )
{
   double dRnd = hb_random_num();

   if( ! HB_ISNUM( 1 ) )
      hb_retni( dRnd >= 0.5 ? 0 : 1 );
   else if( ! HB_ISNUM( 2 ) )
      hb_retnint( ( HB_MAXINT ) ( 1 + ( dRnd * hb_parnint( 1 ) ) ) );
   else
   {
      HB_MAXINT lX = hb_parnint( 1 );
      HB_MAXINT lY = hb_parnint( 2 );
      if( lX > lY )
      {
         HB_MAXINT lZ = lY;
         lY = lX;
         lX = lZ;
      }
      hb_retnint( ( HB_MAXINT ) ( lX + ( dRnd * ( lY - lX + 1 ) ) ) );
   }
}

HB_FUNC( HB_RANDOMSEED )
{
   hb_random_seed( HB_ISNUM( 1 ) ? ( HB_I32 ) hb_parni( 1 ) : ( HB_I32 ) hb_dateMilliSeconds() );

   s_fInit = HB_TRUE;
}

HB_FUNC( HB_RANDOMINTMAX )
{
#if MAXIMUM > HB_VMLONG_MAX
   hb_retnd( MAXIMUM );
#else
   hb_retnint( MAXIMUM );
#endif
}

/* Returns a double value between 0 and 1 */
double hb_random_num( void )
{
   const HB_I32 Q = MODULUS / MULTIPLIER;
   const HB_I32 R = MODULUS % MULTIPLIER;

   HB_I32 t;

   if( ! s_fInit )
   {
      hb_random_seed( ( HB_I32 ) hb_dateMilliSeconds() );
      s_fInit = HB_TRUE;
   }

   t = s_seed;
   t = MULTIPLIER * ( t % Q ) - R * ( t / Q );
   if( t <= 0 )
      t += MODULUS;
   s_seed = t;

   return ( ( double ) t / MODULUS );
}

void hb_random_seed( HB_I32 seed )
{
   seed %= MODULUS;
   s_seed = ( seed <= 0 ) ? seed + MODULUS : seed;
}
