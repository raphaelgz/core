/*
 * $Id$
 */

/*
 * Copyright 2010 Viktor Szakats (harbour.01 syenar.hu)
 * www - http://harbour-project.org
 *
 * See COPYING for licensing terms.
 */

{
   var tst2 = new ActiveXObject( "MyOleTimeServer" );

   WScript.CreateObject("Wscript.Shell").Popup( ">" + tst2.TIME() + "<" );
}
