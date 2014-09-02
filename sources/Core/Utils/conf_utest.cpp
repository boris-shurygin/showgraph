/**
 * @file: conf_utest.cpp 
 * Implementation of testing of configuration 
 */
/*
 * Copyright (c) 2009, Boris Shurygin
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 * 
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "utils_iface.h"

/**
 * Test configuration and options parsing
 */
bool uTestConf()
{
    Conf *conf = new Conf();
    
    /** Create some example options */
    conf->addOption( new Option( OPT_STRING, "o", "output", "output file name( string option example)"));
    conf->addOption( new Option( OPT_BOOL, "b", "boolean", "boolean option example"));
    conf->addOption( new Option( OPT_INT, "i", "integer", "integer option example"));
    conf->addOption( new Option( OPT_FLOAT, "f", "float", "float option example"));
    conf->printOpts(); // Print them to console

    /** Check that created options can be accessed */
    assert( isNullP( conf->option( "file")));
    assert( isNotNullP( conf->option("output")));
    assert( isNotNullP( conf->shortOption( "o")));
    assert( isNullP( conf->shortOption( "output")));
    assert( isNotNullP( conf->longOption("output")));

    /** Create array of arguments */
    char *args[ 8];
    args[ 0] = "string";// treated as app name
    args[ 1] = "--output";
    args[ 2] = "file";
    args[ 3] = "-a";
    args[ 4] = "--b";
    args[ 5] = "-b";
    args[ 6] = "-i";
    args[ 7] = "80";

    /** Read arguments from array and parse them */
    conf->readArgs( 8, args);

    /** Check options values */
    assert( conf->unknownOptsNum() == 2); // Check number of unknown arguments
    assert( !(conf->option( "output")->string().compare("file")));
    Option *int_opt = conf->option( "integer");
    assert( int_opt->isDefined());
    assert( int_opt->intVal()== 80);
    assert( conf->option( "b")->isSet());
    
    delete conf;
    return true;
}
