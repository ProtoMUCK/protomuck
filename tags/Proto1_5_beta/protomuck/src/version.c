/*
 * Copyright (c) 1990 Chelsea Dyerman
 * University of California, Berkeley (XCF)
 *
 * Some parts of this code -- in particular the dictionary based compression
 * code is Copyright 1995 by Dragon's Eye Productions
 *
 */

/*
 * This file is generated by mkversion.sh. Any changes made will go away.
 */

#include "patchlevel.h"
#include "params.h"
#include "interface.h"
#include "externs.h"

const char *generation = "567";
const char *creation = "Mon Oct 23 2000 at 00:32:27 EDT";
const char *version = PATCHLEVEL;

const char *infotext[] =
{

    "ProtoMUCK " PROTOBASE " (" VERSION " -- " NEONVER ")",
    " ",
    "Based on the original code written by these programmers:",
    "  David Applegate    James Aspnes    Timothy Freeman    Bennet Yee",
    " ",
    "Others who have done major coding work along the way:",
    "  Lachesis, ChupChups, FireFoot, Russ 'Random' Smith, Dr. Cat,",
    "  Revar, Points, Loki, PakRat, Nodaitsu",
    " ",
    "ProtoMuck is derived from TinyMUCK, which was itself an extension",
    "of TinyMUD.  Proto is an updated version of the NeonMUCK code base,",
    "which originaly came from the FuzzBall distribution, by Foxen/Revar,",
    "of TinyMUCK.",
    " ",
    "And, here is the list of the programmers for ProtoMUCK:",
    "  Moose/Van  : ashitaka@home.com    Akari : Nakoruru08@hotmail.com",
    " ",
    "Thanks are also due towards the multiple people who had also",
    "contributed ideas for the MUCK as we worked on it, and even tried to",
    "help us out along the way.",
    "Feel free to check ProtoMUCK's webpage at the following address:",
    "http://protomuck.sourceforge.net/",
    " ",
#ifdef WINNT
    "The Cygwin-32 freeware porting tools used to create the WinNT/95",
    "distribution of ProtoMuck are created and maintained by Cygnus.",
    "http://www.cygnus.com/gnu-win32 for more information.",
    " ",
#endif
    "The Pueblo multimedia protocol is (C)Chaco Communications.",
    "http://www.chaco.com/pueblo for more information.",
    "The recommended client by us for any mu* server is BeipMU*.",
    "http://beipmu.twu.net/ for more information.",
    0,
};


void
do_credits(dbref player)
{
    int i;

    for (i = 0; infotext[i]; i++) {
        anotify_nolisten2(player, infotext[i]);
    }
}
