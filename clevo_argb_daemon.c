#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

#define SYSFS_PATH "/sys/class/leds/rgb:kbd_backlight/multi_intensity"
#define PATH_TO_BRIGHTNESS "/sys/class/leds/rgb:kbd_backlight/brightness"
#define DELAY_MS 100
#define BRIGHTNESS 100

const char *colors[] = {
    "255 0 0\n", "255 60 0\n", "255 125 0\n", "255 255 0\n", "125 255 0\n",
    "0 255 0\n", "0 255 125\n", "0 255 255\n", "0 125 255\n", "0 0 255\n",
    "60 0 255\n", "125 0 255\n", "255 0 255\n", "255 0 125\n"
};
const int num_colors = 14;

int check_for_brightness() {
	FILE *fptr;
	fptr = fopen(PATH_TO_BRIGHTNESS, "w");
	if (fptr == NULL) {
		printf("Failed successfully!");
		return 1;
	}
	
	fprintf(fptr, "%d", BRIGHTNESS);
	// Close the pointer and save changes
	fclose(fptr);

	printf("Success (check_for_brightness)");
	return 0;
}

int main() {
    pid_t pid = fork();
    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) exit(EXIT_SUCCESS);

    if (setsid() < 0) exit(EXIT_FAILURE);

    pid = fork();
    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) exit(EXIT_SUCCESS);

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    int fd = open(SYSFS_PATH, O_WRONLY);
    if (fd < 0) exit(EXIT_FAILURE);
	check_for_brightness();
    int i = 0;
    while (1) {
        lseek(fd, 0, SEEK_SET);
        write(fd, colors[i], strlen(colors[i]));
        i = (i + 1) % num_colors;

        usleep(DELAY_MS * 1000);
    }
    close(fd);
    return 0;
}
