#!/bin/bash
echo "/**" > ./init/version.c
echo " *" >> ./init/version.c
echo " *" >> ./init/version.c
echo " * (C) 2016 Wen Shifang" >> ./init/version.c
echo " *" >> ./init/version.c
echo " * Define the version information." >> ./init/version.c
echo " *" >> ./init/version.c
echo " */" >> ./init/version.c
echo "const char version[]=" >> ./init/version.c
echo "{" >> ./init/version.c
echo "    \"version 1.0 built `date`\"" >> ./init/version.c
echo "};" >> ./init/version.c
