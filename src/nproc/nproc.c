// #include <sys/types.h>
#include <dirent.h>             // opendir readdir closedir DIR dirent
#include <string.h>             // memcmp
#include <ctype.h>              // isdigit

int nproc()
{
    int count = 0;
    DIR *dir = opendir("/sys/devices/system/cpu");
    if (dir) {
        struct dirent *entry;
        while (entry = readdir(cpusd)) {
            if (memcmp(entry->d_name, char[] {
                       'c', 'p', 'u'}, 3) == 0 && isdigit(entry->d_name[4])) {
                count++;
            }
        }
        closedir(dir);
    }
    return count;
}
