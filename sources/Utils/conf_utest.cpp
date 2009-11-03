/**
 * @file: conf_utest.cpp 
 * Implementation of testing of configuration 
 */
/*
 * Copyright (C) 2009  Boris Shurygin
 */
#include "utils_iface.h"
#include "conf.h"

/**
 * Test configuration and options parsing
 */
bool uTestConf()
{
    Conf *conf = new Conf();
    conf->addOption( new Option( OPT_STRING, "o", "output", "output file name"));
    conf->printOpts();

    return true;
}
