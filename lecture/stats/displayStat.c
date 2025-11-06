#define _DEFAULT_SOURCE
#define _POSIX_C_SOURCE 200809
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

void checkFileType(struct stat *sb) {}

int main(int argc, char *argv[]) {
  char *path = argv[1];
  struct stat st;

  int ans = stat(path, &st);
  printf("%d", ans);

  if (S_ISREG(st.st_mode)) {
    printf("Regular files\n");
  } else if (S_ISDIR(st.st_mode)) {
    printf("Directories\n");
  } else {
    printf("Other\n");
  }

  return 0;
}
