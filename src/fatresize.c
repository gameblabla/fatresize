// fatresize_hc
//
// public domain code
// by flabbergast
//
// resizes PART partition on DEVICE (hardcoded below) to maximal size
// (keeping the beginning of pinned)
// it must be the last partition!
//
// TODO: if there is an error, various object may not be deallocated properly
//

#include <parted/parted.h>

#define DEVICE "/dev/mmcblk0"
#define PART 4

#define err(f_, ...) fprintf(stderr, ("fatresize_hc error: "f_"\n"), ##__VA_ARGS__)
#define msg(f_, ...) fprintf(stdout, ("fatresize_hc: "f_"\n"), ##__VA_ARGS__)

int main(void) {
    PedDevice *dev = NULL;
    PedDisk *disk = NULL;
    PedPartition *part = NULL;
    PedFileSystem *fs = NULL;
    PedGeometry *new_geom = NULL;
    PedConstraint *constraint = NULL;

    if(!(dev = ped_device_get(DEVICE))) {
        err("Could not find device "DEVICE);
        return 1;
    }

    if(!ped_device_open(dev)) {
        err("Could not open device "DEVICE);
        return 1;
    }

    if(!(disk = ped_disk_new(dev))) {
        err("Could not read partition table of "DEVICE);
        return 1;
    }

    if(!(part = ped_disk_get_partition(disk, PART))) {
        err("Could not open partition %d of "DEVICE, PART);
        return 1;
    }

    if(!(fs = ped_file_system_open(&part->geom))) {
        err("Could not open filesystem");
        return 1;
    }

    if(!(constraint = ped_file_system_get_resize_constraint(fs))) {
        err("Could not get resize constraint");
        return 1;
    }

    new_geom = ped_geometry_new(dev, (part->geom).start, constraint->max_size - (part->geom).start);

    msg("old: start %llu, end %llu\n", (part->geom).start, (part->geom).end);
    msg("new: start %llu, end %llu\n", new_geom->start, new_geom->end);

    // resize partititon
    msg("resizing partition...");
    if(!ped_disk_set_partition_geom(disk, part, ped_constraint_exact(new_geom), new_geom->start, new_geom->end)) {
        err("Could not resize partition");
        return 1;
    }

    // resize filesystem
    msg("resizing filesystem...");
    if(!ped_file_system_resize(fs, new_geom, NULL)) {
        err("Could not resize filesystem");
        return 1;
    }

    // commit
    msg("committing changes...");
    if(!ped_disk_commit(disk)) {
        err("Could not commit changes!");
        return 1;
    }

    ped_constraint_destroy(constraint);
    ped_geometry_destroy(new_geom);
    ped_file_system_close(fs);
    ped_disk_destroy(disk);
    ped_device_close(dev);
    msg("done.");
    return 0;
}
