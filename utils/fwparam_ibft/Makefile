#
#  Copyright (C) IBM Corporation. 2007
#  Author: Doug Maxey <dwm@austin.ibm.com>
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 2 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
#  Authors: Patrick Mansfield <patmans@us.ibm.com>
#	    Mike Anderson <andmike@us.ibm.com>
#	    Doug Maxey <dwm@austin.ibm.com>
#	    "Prasanna Mumbai" <mumbai.prasanna@gmail.com>
#
ifeq ($(TOPDIR),)
	TOPDIR = ../..
endif


SYSDEPS_OBJS = $(sort $(wildcard ../sysdeps/*.o))
OBJS := fw_entry.o fwparam_sysfs.o $(SYSDEPS_OBJS) \
	$(TOPDIR)/usr/iscsi_net_util.o
OBJS += prom_lex.o prom_parse.tab.o fwparam_ppc.o
CLEANFILES = $(OBJS) *.output *~

CFLAGS ?= -O2 -g
WARNFLAGS ?= -Wall -Wstrict-prototypes -Wno-format-truncation
CFLAGS += -fPIC $(WARNFLAGS) -I$(TOPDIR)/include -I$(TOPDIR)/usr -D_GNU_SOURCE \
	  -I$(TOPDIR)/libopeniscsiusr

LDFLAGS += -L$(TOPDIR)/libopeniscsiusr -liscsiusr

all: $(OBJS)

clean:
	rm -f *.o $(CLEANFILES) .depend

$(OBJS): prom_parse.tab.h prom_parse.h fwparam_ibft.h

depend:
	gcc $(CFLAGS) -M `ls *.c` > .depend

-include .depend
