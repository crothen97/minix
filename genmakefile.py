#!/usr/bin/env python2

from gmake import Makefile as mf


mf().out('out').headers(['minix']).srcs(['minix']) \
    .incpaths(['./']).debug(False).cxx11() \
    .libs(('ikkcpr', 'pthread', 'pcre')) \
    .target('minix.out').write("Makefile")
