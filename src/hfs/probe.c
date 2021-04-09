/*
    libparted - a library for manipulating disk partitions
    Copyright (C) 2004-2005, 2007, 2009-2014 Free Software Foundation, Inc.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <config.h>

#include <parted/parted.h>
#include <parted/endian.h>
#include <parted/debug.h>
#include <stdint.h>

#if ENABLE_NLS
#  include <libintl.h>
#  define _(String) dgettext (PACKAGE, String)
#else
#  define _(String) (String)
#endif /* ENABLE_NLS */

#include "hfs.h"

#include "probe.h"

int
my_hfsc_can_use_geom (PedGeometry* geom)
{
	PedDevice* dev;

	dev = geom->dev;
	PED_ASSERT (geom != NULL);
	PED_ASSERT (dev != NULL);

	if (dev->sector_size != PED_SECTOR_SIZE_DEFAULT) {
		ped_exception_throw (
			PED_EXCEPTION_ERROR,
			PED_EXCEPTION_CANCEL,
			_("Parted can't use HFS file systems on disks "
			  "with a sector size not equal to %d bytes."),
			(int)PED_SECTOR_SIZE_DEFAULT );
		return 0;
	}

	return 1;
}
